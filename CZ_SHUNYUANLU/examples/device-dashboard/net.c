// Copyright (c) 2023 Cesanta Software Limited
// All rights reserved

#include <syslog.h>
#include "net.h"
#include "appl.h"

// Authenticated user.
// A user can be authenticated by:
//   - a name:pass pair, passed in a header Authorization: Basic .....
//   - an access_token, passed in a header Cookie: access_token=....
// When a user is shown a login screen, she enters a user:pass. If successful,
// a server responds with a http-only access_token cookie set.
struct user {
  const char *name, *pass, *access_token;
};

// Settings
struct settings {
  bool log_enabled;
  int log_level;
  long brightness;
  char *device_name;
};

static struct settings s_settings = {true, 1, 57, NULL};

static const char *s_json_header =
    "Content-Type: application/json\r\n"
    "Cache-Control: no-cache\r\n";
static uint64_t s_boot_timestamp = 0;  // Updated by SNTP

// This is for newlib and TLS (mbedTLS)
uint64_t mg_now(void) {
  return mg_millis() + s_boot_timestamp;
}

int ui_event_next(int no, struct ui_event *e) {
  if (no < 0 || no >= MAX_EVENTS_NO) return 0;

  srand((unsigned) no);
  e->type = (uint8_t) rand() % 4;
  e->prio = (uint8_t) rand() % 3;
  e->timestamp =
      (unsigned long) ((int64_t) mg_now() - 86400 * 1000 /* one day back */ +
                       no * 300 * 1000 /* 5 mins between alerts */ +
                       1000 * (rand() % 300) /* randomize event time */) /
      1000UL;

  mg_snprintf(e->text, MAX_EVENT_TEXT_SIZE, "event#%d", no);
  return no + 1;
}

// SNTP connection event handler. When we get a response from an SNTP server,
// adjust s_boot_timestamp. We'll get a valid time from that point on
static void sfn(struct mg_connection *c, int ev, void *ev_data) {
  uint64_t *expiration_time = (uint64_t *) c->data;
  if (ev == MG_EV_OPEN) {
    *expiration_time = mg_millis() + 3000;  // Store expiration time in 3s
  } else if (ev == MG_EV_SNTP_TIME) {
    uint64_t t = *(uint64_t *) ev_data;
    s_boot_timestamp = t - mg_millis();
    c->is_closing = 1;
  } else if (ev == MG_EV_POLL) {
    if (mg_millis() > *expiration_time) c->is_closing = 1;
  }
}

static void timer_sntp_fn(void *param) {  // SNTP timer function. Sync up time
  mg_sntp_connect(param, "udp://time.google.com:123", sfn, NULL);
}

// Parse HTTP requests, return authenticated user or NULL
static struct user *authenticate(struct mg_http_message *hm) {
  // In production, make passwords strong and tokens randomly generated
  // In this example, user list is kept in RAM. In production, it can
  // be backed by file, database, or some other method.
  static struct user users[] = {
      {"admin", "admin", "admin_token"},
      {"user1", "user1", "user1_token"},
      {"user2", "user2", "user2_token"},
      {NULL, NULL, NULL},
  };
  char user[64], pass[64];
  struct user *u, *result = NULL;
  mg_http_creds(hm, user, sizeof(user), pass, sizeof(pass));
  MG_VERBOSE(("user [%s] pass [%s]", user, pass));

  if (user[0] != '\0' && pass[0] != '\0') {
    // Both user and password is set, search by user/password
    for (u = users; result == NULL && u->name != NULL; u++)
      if (strcmp(user, u->name) == 0 && strcmp(pass, u->pass) == 0) result = u;
  } else if (user[0] == '\0') {
    // Only password is set, search by token
    for (u = users; result == NULL && u->name != NULL; u++)
      if (strcmp(pass, u->access_token) == 0) result = u;
  }
  return result;
}

static void handle_login(struct mg_connection *c, struct user *u) {
  char cookie[256];
  mg_snprintf(cookie, sizeof(cookie),
              "Set-Cookie: access_token=%s; Path=/; "
              "%sHttpOnly; SameSite=Lax; Max-Age=%d\r\n",
              u->access_token, c->is_tls ? "Secure; " : "", 3600 * 24);
  mg_http_reply(c, 200, cookie, "{%m:%m}", MG_ESC("user"), MG_ESC(u->name));
}

static void handle_logout(struct mg_connection *c) {
  char cookie[256];
  mg_snprintf(cookie, sizeof(cookie),
              "Set-Cookie: access_token=; Path=/; "
              "Expires=Thu, 01 Jan 1970 00:00:00 UTC; "
              "%sHttpOnly; Max-Age=0; \r\n",
              c->is_tls ? "Secure; " : "");
  mg_http_reply(c, 200, cookie, "true\n");
}

static void handle_debug(struct mg_connection *c, struct mg_http_message *hm) {
  int level = mg_json_get_long(hm->body, "$.level", MG_LL_DEBUG);
  mg_log_set(level);
  mg_http_reply(c, 200, "", "Debug level set to %d\n", level);
}

static size_t print_int_arr(void (*out)(char, void *), void *ptr, va_list *ap) {
  size_t i, len = 0, num = va_arg(*ap, size_t);  // Number of items in the array
  int *arr = va_arg(*ap, int *);              // Array ptr
  for (i = 0; i < num; i++) {
    len += mg_xprintf(out, ptr, "%s%d", i == 0 ? "" : ",", arr[i]);
  }
  return len;
}

static void handle_stats_get(struct mg_connection *c) {
  int points[] = {21, 22, 22, 19, 18, 20, 23, 23, 22, 22, 22, 23, 22};
  mg_http_reply(c, 200, s_json_header, "{%m:%d,%m:%d,%m:[%M]}\n",
                MG_ESC("temperature"), 21,  //
                MG_ESC("humidity"), 67,     //
                MG_ESC("points"), print_int_arr,
                sizeof(points) / sizeof(points[0]), points);
}

static size_t print_events(void (*out)(char, void *), void *ptr, va_list *ap) {
  size_t len = 0;
  struct ui_event ev;
  int pageno = va_arg(*ap, int);
  int no = (pageno - 1) * EVENTS_PER_PAGE;
  int end = no + EVENTS_PER_PAGE;

  while ((no = ui_event_next(no, &ev)) != 0 && no <= end) {
    len += mg_xprintf(out, ptr, "%s{%m:%lu,%m:%d,%m:%d,%m:%m}\n",  //
                      len == 0 ? "" : ",",                         //
                      MG_ESC("time"), ev.timestamp,                //
                      MG_ESC("type"), ev.type,                     //
                      MG_ESC("prio"), ev.prio,                     //
                      MG_ESC("text"), MG_ESC(ev.text));
  }

  return len;
}

static void handle_events_get(struct mg_connection *c,
                              struct mg_http_message *hm) {
  int pageno = mg_json_get_long(hm->body, "$.page", 1);
  mg_http_reply(c, 200, s_json_header, "{%m:[%M], %m:%d}\n", MG_ESC("arr"),
                print_events, pageno, MG_ESC("totalCount"), MAX_EVENTS_NO);
}

static void handle_settings_set(struct mg_connection *c, struct mg_str body) {
  struct Settings_t* set = &APPL.Set.s;
  struct chanmqtt_t* mqtt = &APPL.chanmqtt[2];
  int cmd;
  int param;
  bool ok = true;
  int rc;
  long val;
  int i;
  char buf[128];
  char* p;

  val = mg_json_get_long(body, "$.cmd", -1);
  if(val != -1){
      cmd = val;
      syslog(LOG_INFO,"%s, cmd:%d", __func__, cmd);
      switch (cmd)
      {
      case SETTINGS_CMD_SET_CHG_CELLV:
        val = mg_json_get_long(body, "$.param", 0);
        if( val == 0 ){
          ok = false;
          syslog(LOG_INFO, "%s, SETTINGS_CMD_SET_CHG_CELLV, mg_json_get_long Fail", __func__);
        }else{
          set->ChgCellV = val/1000.0;
          if( appl_cfg_save() != 0){
            syslog(LOG_INFO, "%s, SETTINGS_CMD_SET_CHG_CELLV, appl_cfg_save Fail", __func__);
            appl_cfg_set_err();
            ok = false;
          }else{
            syslog(LOG_INFO, "%s, SETTINGS_CMD_SET_CHG_CELLV, set->ChgCellV is set to %.3f", __func__, set->ChgCellV);
          }
        }
        break;

      case SETTINGS_CMD_SET_DHG_CELLV:
        val = mg_json_get_long(body, "$.param", 0);
        if( val == 0 ){
          ok = false;
          syslog(LOG_INFO, "%s, SETTINGS_CMD_SET_DHG_CELLV, mg_json_get_long Fail", __func__);
        }else{
          set->DhgCellV = val/1000.0;
          if(appl_cfg_save() != 0){
            syslog(LOG_INFO, "%s, SETTINGS_CMD_SET_DHG_CELLV, appl_cfg_save Fail", __func__);
            appl_cfg_set_err();
            ok = false;
          }else{
            syslog(LOG_INFO, "%s, SETTINGS_CMD_SET_DHG_CELLV, set->DhgCellV is set to %.3f", __func__, set->DhgCellV);
          }
        }
        break;  

      case SETTINGS_CMD_SET_PCURV:
        for(i = 0; i < 96; i++){
          sprintf(buf,"$.%d", i+1);
          val = mg_json_get_long(body, buf, -999999);
          if( val == -999999 ){
            ok = false;
            break;
            syslog(LOG_INFO, "%s, SETTINGS_CMD_SET_PCURV, mg_json_get_long Fail", __func__);
          }else{
            set->pcurv[i] = val;
            syslog(LOG_INFO, "%s, SETTINGS_CMD_SET_PCURV, new point set, idx:%d,val:%d", __func__, i+1, val);
          }
        }
        if( ok ){
          if(appl_cfg_save() != 0){
            syslog(LOG_INFO, "%s, SETTINGS_CMD_SET_PCURV, appl_cfg_save Fail", __func__);
            appl_cfg_set_err();
            ok = false;
          }else{
            syslog(LOG_INFO, "%s, SETTINGS_CMD_SET_PCURV, Ok", __func__);
          }
        }
        break;  

      case SETTINGS_CMD_SET_CHGGATELIM:
        val = mg_json_get_long(body, "$.param", -999999);
        if( val == -999999 ){
          ok = false;
          syslog(LOG_INFO, "%s, SETTINGS_CMD_SET_CHGGATELIM, mg_json_get_long Fail", __func__);
        }else{
          set->ChgGateLim = val;
          if(appl_cfg_save() != 0){
            syslog(LOG_INFO, "%s, SETTINGS_CMD_SET_CHGGATELIM, appl_cfg_save Fail", __func__);
            appl_cfg_set_err();
            ok = false;
          }else{
            syslog(LOG_INFO, "%s, SETTINGS_CMD_SET_CHGGATELIM, set->ChgGateLim is set to %d", __func__, set->ChgGateLim);
          }
        }
        break;   

      case SETTINGS_CMD_SET_CHGTRANSLIM:
        val = mg_json_get_long(body, "$.param", -999999);
        if( val == -999999 ){
          ok = false;
          syslog(LOG_INFO, "%s, SETTINGS_CMD_SET_CHGTRANSLIM, mg_json_get_long Fail", __func__);
        }else{
          set->ChgTransLim = val;
          if(appl_cfg_save() != 0){
            syslog(LOG_INFO, "%s, SETTINGS_CMD_SET_CHGTRANSLIM, appl_cfg_save Fail", __func__);
            appl_cfg_set_err();
            ok = false;
          }else{
            syslog(LOG_INFO, "%s, SETTINGS_CMD_SET_CHGTRANSLIM, set->ChgTransLim is set to %d", __func__, set->ChgTransLim);
          }
        }
        break;       

      case SETTINGS_CMD_SET_DHGGATELIM:
        val = mg_json_get_long(body, "$.param", -999999);
        if( val == -999999 ){
          ok = false;
          syslog(LOG_INFO, "%s, SETTINGS_CMD_SET_DHGGATELIM, mg_json_get_long Fail", __func__);
        }else{
          set->DhgGateLim = val;
          if(appl_cfg_save() != 0){
            syslog(LOG_INFO, "%s, SETTINGS_CMD_SET_DHGGATELIM, appl_cfg_save Fail", __func__);
            appl_cfg_set_err();
            ok = false;
          }else{
            syslog(LOG_INFO, "%s, SETTINGS_CMD_SET_DHGGATELIM, set->DhgGateLim is set to %d", __func__, set->DhgGateLim);
          }
        }
        break;      

      case SETTINGS_CMD_SET_DHGTRANSLIM:
        val = mg_json_get_long(body, "$.param", -999999);
        if( val == -999999 ){
          ok = false;
          syslog(LOG_INFO, "%s, SETTINGS_CMD_SET_DHGTRANSLIM, mg_json_get_long Fail", __func__);
        }else{
          set->DhgTransLim = val;
          if(appl_cfg_save() != 0){
            syslog(LOG_INFO, "%s, SETTINGS_CMD_SET_DHGTRANSLIM, appl_cfg_save Fail", __func__);
            appl_cfg_set_err();
            ok = false;
          }else{
            syslog(LOG_INFO, "%s, SETTINGS_CMD_SET_DHGTRANSLIM, set->DhgTransLim is set to %d", __func__, set->DhgTransLim);
          }
        }
        break;             

      case SETTINGS_CMD_SET_SN:
        p = mg_json_get_str(body, "$.param");
        if( p != NULL){
          strcpy(set->szSN, p);
          free(p);
          if(appl_cfg_save() != 0){
            appl_cfg_set_err();
            ok = false;
            syslog(LOG_INFO, "%s, SETTINGS_CMD_SET_SN, appl_cfg_save Fail", __func__);
          }else{
            syslog(LOG_INFO, "%s, SETTINGS_CMD_SET_SN, appl_cfg_save Ok : %s", __func__, set->szSN);
          }
        }else{
          ok = false;
          syslog(LOG_INFO, "%s, SETTINGS_CMD_SET_SN, mg_json_get_str Fail", __func__);
        }
      break;

      case SETTINGS_CMD_SET_CLOUD_USERNAME:
        p = mg_json_get_str(body, "$.param");
        if( p != NULL){
          strcpy(set->szCloudUserName, p);
          free(p);
          if(appl_cfg_save() != 0){
            appl_cfg_set_err();
            ok = false;
            syslog(LOG_INFO, "%s, SETTINGS_CMD_SET_CLOUD_USERNAME, appl_cfg_save Fail", __func__);
          }else{
            syslog(LOG_INFO, "%s, SETTINGS_CMD_SET_CLOUD_USERNAME, appl_cfg_save Ok : %s", __func__, set->szCloudUserName);
          }
        }else{
          ok = false;
          syslog(LOG_INFO, "%s, SETTINGS_CMD_SET_CLOUD_USERNAME, mg_json_get_str Fail", __func__);
        }
      break;      

      case SETTINGS_CMD_SET_CLOUD_PASSWD:
        p = mg_json_get_str(body, "$.param");
        if( p != NULL){
          strcpy(set->szCloudPasswd, p);
          free(p);
          if(appl_cfg_save() != 0){
            appl_cfg_set_err();
            ok = false;
            syslog(LOG_INFO, "%s, SETTINGS_CMD_SET_CLOUD_PASSWD, appl_cfg_save Fail", __func__);
          }else{
            syslog(LOG_INFO, "%s, SETTINGS_CMD_SET_CLOUD_PASSWD, appl_cfg_save Ok : %s", __func__, set->szCloudPasswd);
          }
        }else{
          ok = false;
          syslog(LOG_INFO, "%s, SETTINGS_CMD_SET_CLOUD_PASSWD, mg_json_get_str Fail", __func__);
        }
      break;           

      case SETTINGS_CMD_SET_CLOUD_URL:
        p = mg_json_get_str(body, "$.param");
        if( p != NULL){
          strcpy(set->szCloudUrl, p);
          free(p);
          if(appl_cfg_save() != 0){
            appl_cfg_set_err();
            ok = false;
            syslog(LOG_INFO, "%s, SETTINGS_CMD_SET_CLOUD_URL, appl_cfg_save Fail", __func__);
          }else{
            syslog(LOG_INFO, "%s, SETTINGS_CMD_SET_CLOUD_URL, appl_cfg_save Ok : %s", __func__, set->szCloudUrl);
          }
        }else{
          ok = false;
          syslog(LOG_INFO, "%s, SETTINGS_CMD_SET_CLOUD_URL, mg_json_get_str Fail", __func__);
        }
      break;     

      case SETTINGS_CMD_SET_CLOUD_CLIENTID:
        p = mg_json_get_str(body, "$.param");
        if( p != NULL){
          strcpy(set->szClientId, p);
          free(p);
          if(appl_cfg_save() != 0){
            appl_cfg_set_err();
            ok = false;
            syslog(LOG_INFO, "%s, SETTINGS_CMD_SET_CLOUD_CLIENTID, appl_cfg_save Fail", __func__);
          }else{
            syslog(LOG_INFO, "%s, SETTINGS_CMD_SET_CLOUD_CLIENTID, appl_cfg_save Ok : %s", __func__, set->szClientId);
          }
        }else{
          ok = false;
          syslog(LOG_INFO, "%s, SETTINGS_CMD_SET_CLOUD_CLIENTID, mg_json_get_str Fail", __func__);
        }
      break;             

      case SETTINGS_CMD_REGISTER:
        mqtt->Cmd = CMD_MQTT_REGISTER;
        break;     

      case SETTINGS_CMD_SET_DATAKEEPDAY:
        val = mg_json_get_long(body, "$.param", -1);
        if( val == -1 ){
          ok = false;
          syslog(LOG_INFO, "%s, SETTINGS_CMD_SET_DATAKEEPDAY, mg_json_get_long Fail", __func__);
        }else{
          set->DataKeepDay = val;
          if(appl_cfg_save() != 0){
            syslog(LOG_INFO, "%s, SETTINGS_CMD_SET_DATAKEEPDAY, appl_cfg_save Fail", __func__);
            appl_cfg_set_err();
            ok = false;
          }else{
            syslog(LOG_INFO, "%s, SETTINGS_CMD_SET_DATAKEEPDAY, set->DataKeepDay is set to %d", __func__, set->DataKeepDay);
          }
        }
        break;   

      case SETTINGS_CMD_SET_UPLOADHIHGSPEED:
        val = mg_json_get_long(body, "$.param", -1);
        if( val == -1 ){
          ok = false;
          syslog(LOG_INFO, "%s, SETTINGS_CMD_SET_UPLOADHIHGSPEED, mg_json_get_long Fail", __func__);
        }else{
          set->UploadHighSpeed = val;
          if(appl_cfg_save() != 0){
            syslog(LOG_INFO, "%s, SETTINGS_CMD_SET_UPLOADHIHGSPEED, appl_cfg_save Fail", __func__);
            appl_cfg_set_err();
            ok = false;
          }else{
            syslog(LOG_INFO, "%s, SETTINGS_CMD_SET_UPLOADHIHGSPEED, set->UploadHighSpeed is set to %d", __func__, set->UploadHighSpeed);
          }
        }
        break;           

      case SETTINGS_CMD_SET_UPLOADMEDIUMSPEED:
        val = mg_json_get_long(body, "$.param", -1);
        if( val == -1 ){
          ok = false;
          syslog(LOG_INFO, "%s, SETTINGS_CMD_SET_UPLOADMEDIUMSPEED, mg_json_get_long Fail", __func__);
        }else{
          set->UploadMediumSpeed = val;
          if(appl_cfg_save() != 0){
            syslog(LOG_INFO, "%s, SETTINGS_CMD_SET_UPLOADMEDIUMSPEED, appl_cfg_save Fail", __func__);
            appl_cfg_set_err();
            ok = false;
          }else{
            syslog(LOG_INFO, "%s, SETTINGS_CMD_SET_UPLOADMEDIUMSPEED, set->UploadMediumSpeed is set to %d", __func__, set->UploadMediumSpeed);
          }
        }
        break;            

      case SETTINGS_CMD_SET_UPLOADSLOWSPEED:
        val = mg_json_get_long(body, "$.param", -1);
        if( val == -1 ){
          ok = false;
          syslog(LOG_INFO, "%s, SETTINGS_CMD_SET_UPLOADSLOWSPEED, mg_json_get_long Fail", __func__);
        }else{
          set->UploadSlowSpeed = val;
          if(appl_cfg_save() != 0){
            syslog(LOG_INFO, "%s, SETTINGS_CMD_SET_UPLOADSLOWSPEED, appl_cfg_save Fail", __func__);
            appl_cfg_set_err();
            ok = false;
          }else{
            syslog(LOG_INFO, "%s, SETTINGS_CMD_SET_UPLOADSLOWSPEED, set->UploadSlowSpeed is set to %d", __func__, set->UploadSlowSpeed);
          }
        }
        break;         

      case SETTINGS_CMD_SET_GATE_COUPLE_NBR:
        val = mg_json_get_long(body, "$.param", -1);
        if( val == -1 ){
          ok = false;
          syslog(LOG_INFO, "%s, SETTINGS_CMD_SET_GATE_COUPLE_NBR, mg_json_get_long Fail", __func__);
        }else{
          set->GateCoupleNbr = val;
          if(appl_cfg_save() != 0){
            syslog(LOG_INFO, "%s, SETTINGS_CMD_SET_GATE_COUPLE_NBR, appl_cfg_save Fail", __func__);
            appl_cfg_set_err();
            ok = false;
          }else{
            syslog(LOG_INFO, "%s, SETTINGS_CMD_SET_GATE_COUPLE_NBR, set->GateCoupleNbr is set to %d", __func__, set->GateCoupleNbr);
          }
        }
        break;            

      case SETTINGS_CMD_SET_TRANS_COUPLE_NBR:
        val = mg_json_get_long(body, "$.param", -1);
        if( val == -1 ){
          ok = false;
          syslog(LOG_INFO, "%s, SETTINGS_CMD_SET_TRANS_COUPLE_NBR, mg_json_get_long Fail", __func__);
        }else{
          set->TransCoupleNbr = val;
          if(appl_cfg_save() != 0){
            syslog(LOG_INFO, "%s, SETTINGS_CMD_SET_TRANS_COUPLE_NBR, appl_cfg_save Fail", __func__);
            appl_cfg_set_err();
            ok = false;
          }else{
            syslog(LOG_INFO, "%s, SETTINGS_CMD_SET_TRANS_COUPLE_NBR, set->TransCoupleNbr is set to %d", __func__, set->TransCoupleNbr);
          }
        }
        break;                     

      case SETTINGS_CMD_SET_CTNMETER_NBR:
        val = mg_json_get_long(body, "$.param", -1);
        if( val == -1 ){
          ok = false;
          syslog(LOG_INFO, "%s, SETTINGS_CMD_SET_CTNMETER_NBR, mg_json_get_long Fail", __func__);
        }else{
          set->CtnMeterNbr = val;
          if(appl_cfg_save() != 0){
            syslog(LOG_INFO, "%s, SETTINGS_CMD_SET_CTNMETER_NBR, appl_cfg_save Fail", __func__);
            appl_cfg_set_err();
            ok = false;
          }else{
            syslog(LOG_INFO, "%s, SETTINGS_CMD_SET_CTNMETER_NBR, set->TransCoupleNbr is set to %d", __func__, set->TransCoupleNbr);
          }
        }
      break;     

      case SETTINGS_CMD_SET_TRANS_ID:
        val = mg_json_get_long(body, "$.param", -1);
        if( val == -1 ){
          ok = false;
          syslog(LOG_INFO, "%s, SETTINGS_CMD_SET_TRANS_ID, mg_json_get_long Fail", __func__);
        }else{
          set->TransId = val;
          if(appl_cfg_save() != 0){
            syslog(LOG_INFO, "%s, SETTINGS_CMD_SET_TRANS_ID, appl_cfg_save Fail", __func__);
            appl_cfg_set_err();
            ok = false;
          }else{
            syslog(LOG_INFO, "%s, SETTINGS_CMD_SET_TRANS_ID, set->TransCoupleNbr is set to %d", __func__, set->TransCoupleNbr);
          }
        }
      break;      

      case SETTINGS_CMD_SET_CTNMETER_ID:
        val = mg_json_get_long(body, "$.param", -1);
        if( val == -1 ){
          ok = false;
          syslog(LOG_INFO, "%s, SETTINGS_CMD_SET_CTNMETER_ID, mg_json_get_long Fail", __func__);
        }else{
          set->CtnMeterId = val;
          if(appl_cfg_save() != 0){
            syslog(LOG_INFO, "%s, SETTINGS_CMD_SET_CTNMETER_ID, appl_cfg_save Fail", __func__);
            appl_cfg_set_err();
            ok = false;
          }else{
            syslog(LOG_INFO, "%s, SETTINGS_CMD_SET_CTNMETER_ID, set->TransCoupleNbr is set to %d", __func__, set->CtnMeterId);
          }
        }
      break;               

      case SETTINGS_CMD_TEST:
        syslog(LOG_INFO,"%s, Get Test Cmd", __func__);
        rc = appl_snap_rmdir("./snap/test");
        syslog(LOG_INFO,"%s, rc:%d", __func__, rc);
        break;  
      
      default:
        break;
      }    
  }else{
    ok = false;
    syslog(LOG_INFO,"%s, Get Cmd Fail", __func__);
  }
  mg_http_reply(c, 200, s_json_header,
                "{%m:%s,%m:%m}",                          //
                MG_ESC("status"), ok ? "true" : "false",  //
                MG_ESC("message"), MG_ESC(ok ? "Success" : "Failed"));
}

static void handle_pcs_set(struct mg_connection *c, struct mg_str body) {
  long cmd;
  long param;
  bool ok = true;
  struct chan485_t* ch = &APPL.chan485[1];
  cmd = mg_json_get_long(body, "$.cmd", -1);
  param = mg_json_get_long(body, "$.param", -1);
  if( cmd == -1 || param == -1){
    ok = false;
  }else{
    switch (cmd){
    case PCS_CMD_START:
      ch->Cmd = CMD_485_PCS_START;
      break;

    case PCS_CMD_STOP:
      ch->Cmd = CMD_485_PCS_STOP;
      break;      
    
    case PCS_CMD_SET_APS:
      ch->Cmd = CMD_485_PCS_SET_APS;
      ch->CmdParam = param;
      break;   

    default:
      syslog(LOG_INFO,"%s, unknown cmd : %d", __func__, cmd);
      break;
    }
  }
  if(ok){
    mg_http_reply(c, 200, s_json_header, "true\n");
  }else{
    mg_http_reply(c, 200, s_json_header, "false\n");
  } 
}

static void handle_ac_set(struct mg_connection *c, struct mg_str body) {
  long cmd;
  long param;
  bool ok = true;
  struct chan485_t* ch = &APPL.chan485[3];
  struct Envicool5kW_t* ac = &APPL.Envicool5kW;
  struct Settings_t* set = &APPL.Set.s;
  cmd = mg_json_get_long(body, "$.cmd", -1);
  param = mg_json_get_long(body, "$.param", -1);
  syslog(LOG_INFO,"%s, Get Cmd:%d, Param:%d", __func__, cmd, param);
  if( cmd == -1 || param == -1){
    ok = false;
  }else{
    switch (cmd){
    // 同飞5kW LCI-50C-01SZ-1227 开关机命令
    // 开关机命令 0x300 0 0x05/0x06/0x10  B0:开机命令 B1:关机命令 B2:复位
    case AC_CMD_ONOFF:
      ch->Cmd = CMD_485_AC_SET_ONOFF;
      ch->CmdParam = param;
      break;

    // EMS or Non-EMS Control
    case AC_CMD_SET_CTLMOD:
      appl_ac_set_ctlmod(param);
      break;   

    // // 同飞5kW LCI-50C-01SZ-1227 制冷方式
    // // 1 制冷 2加热 3自循环 4自动
    // case AC_CMD_MODESET:
    //   ch->Cmd = CMD_485_AC_SET_MODESET;
    //   ch->CmdParam = param;
    //   break;      
    // 英维克5kW立式 请求设定模式
    // 0：停机模式；1；制冷：2：加热；3：自循环；
    case AC_CMD_MODESET:
      ac->SetMode = param;
      break;     

    // // 同飞5kW LCI-50C-01SZ-1227 恒温方式预设温度
    // case AC_CMD_SET_TEMP:
    //   ch->Cmd = CMD_485_AC_SET_TEMP;
    //   ch->CmdParam = param;
    //   break;     
    // 英维克5kW立式 设定温度
    case AC_CMD_SET_TEMP:
      ac->SetTemp = param;
      break;               
    
    // // 同飞5kW LCI-50C-01SZ-1227 水温目标控制温度
    // // 0 出水水温控制 1 回水水温控制
    // case AC_CMD_CTLTEMPSEL:
    //   ch->Cmd = CMD_485_AC_SET_CTLTEMPSEL;
    //   ch->CmdParam = param;
    //   break;        

    case AC_CMD_SET_COOLTEMP:
      set->CoolTemp = param;
      break;   

    case AC_CMD_SET_COOLGAP:
      set->CoolGap = param;
      break;   

    case AC_CMD_SET_HEATTEMP:
      set->HeatTemp = param;
      break;   

    case AC_CMD_SET_HEATGAP:
      set->HeatGap = param;
      break;   

    case AC_CMD_SET_HEATTEMPSET:
      set->HeatTempSet = param;
    break;  

    case AC_CMD_SET_COOLTEMPSET:
      set->CoolTempSet = param;
    break;

    default:
      syslog(LOG_INFO,"%s, unknown cmd : %d", __func__, cmd);
      break;
    }
  }
  if(ok){
    mg_http_reply(c, 200, s_json_header, "true\n");
  }else{
    mg_http_reply(c, 200, s_json_header, "false\n");
  } 
}

static void handle_env_set_ledmod(struct mg_connection *c, struct mg_str body) {
  struct Dido_t* dido = &APPL.Dido;
  int mod = mg_json_get_long(body, "$.mode", 0);
  //appl_dido_set_led(mod);
  dido->Cmd = CMD_DIDO_SET_LEDMOD;
  dido->CmdParam = mod;
  mg_http_reply(c, 200, s_json_header, "true\n");
}

static void handle_settings_get(struct mg_connection *c) {
  struct Settings_t* set = &APPL.Set.s;
  char buf[8192] = {0};
  char tmpbuf[64] = {0};
  int i;
  if(set->bErr){
    for( i = 0; i < 96; i++){
      sprintf(tmpbuf, "\"%d\":%d,", i+1, 0);
      strcat(buf,tmpbuf);
    }
  }else{
    for( i = 0; i < 95; i++){
      sprintf(tmpbuf, "\"%d\":%d,", i+1, set->pcurv[i]);
      strcat(buf,tmpbuf);
    }
    i = 95;
    sprintf(tmpbuf, "\"%d\":%d", i+1, set->pcurv[i]);
    strcat(buf,tmpbuf);
  }
  mg_http_reply(c, 200, s_json_header, "{\
%m:%m,%m:%m,%m:%m,%m:%m,%m:%m,\
%m:%d,%m:%d,%m:%d,%m:%d,%m:%d,\
%m:%.4f,%m:%.4f,\
%m:%d,%m:%d,%m:%d,%m:%d,\
%m:%d,%m:%d,%m:%d,%m:%d,\
%s}\n",  
/*1*/MG_ESC("序列号"),MG_ESC(set->szSN),MG_ESC("Cloud用户名"),MG_ESC(set->szCloudUserName),MG_ESC("Cloud密码"),MG_ESC(set->szCloudPasswd),MG_ESC("Cloud Url"),MG_ESC(set->szCloudUrl),MG_ESC("Cloud ID"),MG_ESC(set->szClientId),
/*2*/MG_ESC("关口并柜数量"), set->GateCoupleNbr, MG_ESC("变压器并柜数量"), set->TransCoupleNbr, MG_ESC("变压器ID"),(set->TransId),MG_ESC("储能柜表ID"),(set->CtnMeterId),MG_ESC("储能柜表数量"),(set->CtnMeterNbr),
/*3*/MG_ESC("充电截止电芯电压"), set->ChgCellV,MG_ESC("放电截止电芯电压"), set->DhgCellV,
/*4*/MG_ESC("充电关口限制"), set->ChgGateLim,MG_ESC("充电变压器限制"),set->ChgTransLim,MG_ESC("放电关口限制"), set->DhgGateLim,MG_ESC("放电变压器限制"),set->DhgTransLim,
/*5*/MG_ESC("数据保留天数"),set->DataKeepDay,MG_ESC("上传高速"),set->UploadHighSpeed,MG_ESC("上传中速"),set->UploadMediumSpeed,MG_ESC("上传低速"),set->UploadSlowSpeed,
  buf);
}

static void handle_ctl_get(struct mg_connection *c) {
  struct Ctl_t* ctl = &APPL.Ctl;
  struct Settings_t* set = &APPL.Set.s;
  struct Snap_t* snap = &APPL.Snap;
  struct Enjoy100kW_t* pcs = &APPL.Enjoy100kW;
  struct GaoteBms_t* bms = &APPL.GaoteBms;
  struct Envicool5kW_t* ac = &APPL.Envicool5kW;
  struct Dtsd1352_t* gm = &APPL.GateMeter;
  struct Dtsd1352_t* tm = &APPL.TransMeter;
  char buf[128];
  if(ctl->bChgAble==1 && ctl->bDhgAble==1){
    strcpy(buf,"可充可放");
  }else if(ctl->bChgAble==0 && ctl->bDhgAble==1){
    strcpy(buf,"不可充可放");
  }else if(ctl->bChgAble==1 && ctl->bDhgAble==0){
    strcpy(buf,"可充不可放");
  }else if(ctl->bChgAble==0 && ctl->bDhgAble==0){
    strcpy(buf,"不可充不可放");
  }
  mg_http_reply(c, 200, s_json_header, "{\
%m:%m,%m:%m,%m:%m,%m:%m,%m:%m,\
%m:%d,%m:%d,%m:%d,%m:%d,\
%m:%m,%m:%.3f,%m:%.3f,%m:%.3f,%m:%.3f,\
%m:%.3f,%m:%.4f,%m:%.4f,\
%m:%.4f,%m:%.4f,%m:%.4f,%m:%.3f,%m:%.3f,%m:%.3f,\
%m:%m,%m:%d,%m:%d,%m:%.2f,%m:%.2f,\
%m:%m,%m:%m,%m:%m,\
%m:%m,%m:%m,\
%m:%m,%m:%m}\n",
/*1*/MG_ESC("序列号"),MG_ESC(set->szSN),MG_ESC("系统时间"), MG_ESC(appl_get_datetime_long()),MG_ESC("储能状态"),MG_ESC(ctl->szState),MG_ESC("储能工作模式"),MG_ESC(ctl->szWorkMode), MG_ESC("故障信息"),MG_ESC(ctl->szErr),
/*2*/MG_ESC("储能功率"),ctl->Ap,MG_ESC("关口功率"), (int)gm->com_active_p, MG_ESC("变压器功率"), (int)tm->com_active_p,MG_ESC("储能表功率"), (int)APPL.CtnMeter[set->CtnMeterId].com_active_p,
/*3*/MG_ESC("PCS状态"),MG_ESC(pcs->szWorkState), MG_ESC("PCS功率"),pcs->Ap,MG_ESC("AB电压"),pcs->Uab,MG_ESC("BC电压"),pcs->Ubc,MG_ESC("CA电压"),pcs->Uca,
/*4*/MG_ESC("BMS SOC"), bms->Soc, MG_ESC("BMS总电压"),bms->BatV,MG_ESC("BMS总电流"),bms->BatI,
/*5*/MG_ESC("最高电芯电压"),bms->MaxCellV,MG_ESC("平均电芯电压"),bms->AvgCellV,MG_ESC("最低电芯电压"),bms->MinCellV,MG_ESC("最高电芯温度"),bms->MaxCellT,MG_ESC("平均电芯温度"),bms->AvgCellT,MG_ESC("最低电芯温度"),bms->MinCellT,
/*6*/MG_ESC("空调模式"),MG_ESC(ac->szWorkMode),MG_ESC("出水温度"),ac->OutWaterTemp,MG_ESC("回水温度"),ac->InWaterTemp,MG_ESC("出水压力"),ac->OutWaterPre,MG_ESC("回水压力"),ac->InWaterPre,
/*7*/MG_ESC("关口表通信状态"),MG_ESC(gm->szCommState),MG_ESC("变压器表通信状态"),MG_ESC(tm->szCommState),MG_ESC("储能表通信状态"),MG_ESC(APPL.CtnMeter[set->CtnMeterId].szCommState),
/*8*/MG_ESC("数据记录状态"), MG_ESC(snap->szState),MG_ESC("参数配置状态"), MG_ESC(set->szState),
/*9*/MG_ESC("软件版本"),MG_ESC(VERSION),MG_ESC("可充放状态"),MG_ESC(buf));
}

static void handle_ctl_set(struct mg_connection *c, struct mg_str body) {
  struct Ctl_t* ctl = &APPL.Ctl;
  int cmd, param;
  cmd = mg_json_get_long(body, "$.cmd", 0);
  param = mg_json_get_long(body, "$.param", 0);
  syslog(LOG_INFO, "%s, Cmd:%d, Param:%d", __func__, cmd, param);
  ctl->Cmd = cmd;
  ctl->CmdPara = param;
  mg_http_reply(c, 200, s_json_header, "true\n");
}

static void handle_comm_get(struct mg_connection *c) {
  struct chan485_t* c1 = &APPL.chan485[1];
  struct chan485_t* c2 = &APPL.chan485[2];
  struct chan485_t* c3 = &APPL.chan485[3];
  struct chan485_t* c4 = &APPL.chan485[4];
  struct chancan_t* c5 = &APPL.chancan[1];
  struct chancan_t* c6 = &APPL.chancan[2];
  struct chanmqtt_t* c7 = &APPL.chanmqtt[2];
  mg_http_reply(c, 200, s_json_header, "{\
%m:{%m:%m,%m:%m,%m:%d,%m:%m,%m:%m,%m:%lld,%m:%lld,%m:%lld,%m:%lld},\
%m:{%m:%m,%m:%m,%m:%d,%m:%m,%m:%m,%m:%lld,%m:%lld,%m:%lld,%m:%lld},\
%m:{%m:%m,%m:%m,%m:%d,%m:%m,%m:%m,%m:%lld,%m:%lld,%m:%lld,%m:%lld},\
%m:{%m:%m,%m:%m,%m:%d,%m:%m,%m:%m,%m:%lld,%m:%lld,%m:%lld,%m:%lld},\
%m:{%m:%m,%m:%m,%m:%m,%m:%m,%m:%lld,%m:%lld,%m:%lld,%m:%lld},\
%m:{%m:%m,%m:%m,%m:%m,%m:%m,%m:%lld,%m:%lld,%m:%lld,%m:%lld},\
%m:{%m:%m,%m:%lld,%m:%lld,%m:%d,%m:%d,%m:%lld}}\n",
    MG_ESC("1#485"),MG_ESC("状态"), MG_ESC(c1->szstate),MG_ESC("故障码"),MG_ESC(c1->szerr),MG_ESC("波特率"),c1->baud,MG_ESC("设备名"), MG_ESC(c1->szdev),
                MG_ESC("备注"), MG_ESC(c1->szinfo), MG_ESC("循环次数"),c1->loopcnt, MG_ESC("循环时间"),c1->looptime, MG_ESC("请求次数"),c1->reqcnt, MG_ESC("失败次数"),c1->failcnt,
    MG_ESC("2#485"),MG_ESC("状态"), MG_ESC(c2->szstate),MG_ESC("故障码"),MG_ESC(c2->szerr),MG_ESC("波特率"),c2->baud,MG_ESC("设备名"),MG_ESC(c2->szdev),
                MG_ESC("备注"),MG_ESC(c2->szinfo),MG_ESC("循环次数"),c2->loopcnt,MG_ESC("循环时间"),c2->looptime,MG_ESC("请求次数"),c2->reqcnt,MG_ESC("失败次数"),c2->failcnt,
    MG_ESC("3#485"),MG_ESC("状态"), MG_ESC(c3->szstate),MG_ESC("故障码"),MG_ESC(c3->szerr),MG_ESC("波特率"),c3->baud,MG_ESC("设备名"),MG_ESC(c3->szdev),
                MG_ESC("备注"),MG_ESC(c3->szinfo),MG_ESC("循环次数"),c3->loopcnt,MG_ESC("循环时间"),c3->looptime,MG_ESC("请求次数"),c3->reqcnt,MG_ESC("失败次数"),c3->failcnt,
    MG_ESC("4#485"),MG_ESC("状态"), MG_ESC(c4->szstate),MG_ESC("故障码"),MG_ESC(c4->szerr),MG_ESC("波特率"),c4->baud,MG_ESC("设备名"),MG_ESC(c4->szdev),
                MG_ESC("备注"),MG_ESC(c4->szinfo),MG_ESC("循环次数"),c4->loopcnt,MG_ESC("循环时间"),c4->looptime,MG_ESC("请求次数"),c3->reqcnt,MG_ESC("失败次数"),c4->failcnt,
    MG_ESC("1#CAN"),MG_ESC("状态"), MG_ESC(c5->szState),MG_ESC("故障码"),MG_ESC(c5->szErr),MG_ESC("设备名"),MG_ESC(c5->szdev),
                MG_ESC("备注"),MG_ESC(c5->szinfo),MG_ESC("循环次数"),c5->Loopcnt,MG_ESC("循环时间"),c5->LoopTime,MG_ESC("发送帧数"),c5->WrCnt,MG_ESC("接收帧数"),c5->RdCnt,   
    MG_ESC("2#CAN"),MG_ESC("状态"), MG_ESC(c6->szState),MG_ESC("故障码"),MG_ESC(c6->szErr),MG_ESC("设备名"),MG_ESC(c6->szdev),
                MG_ESC("备注"),MG_ESC(c6->szinfo),MG_ESC("循环次数"),c6->Loopcnt,MG_ESC("循环时间"),c6->LoopTime,MG_ESC("发送帧数"),c6->WrCnt,MG_ESC("接收帧数"),c6->RdCnt,
    MG_ESC("2#MQTT"),MG_ESC("状态"), MG_ESC(c7->szState),MG_ESC("接收计数"), c7->TotalRecvCnt,MG_ESC("发送计数"), c7->TotalSendCnt,MG_ESC("平均接收间隔"),c7->AvgRecvIntv,MG_ESC("最大接收间隔"), c7->MaxRecvIntv,MG_ESC("重连次数"), c7->TotalReconnCnt);
}

static void handle_comm_set(struct mg_connection *c, struct mg_str body) {
  struct chan485_t* ch = NULL;
  int idx = mg_json_get_long(body, "$.index", 0);
  int cmd = mg_json_get_long(body, "$.cmd", 0);
  printf("%s idx:%d cmd:%d\n", __func__, idx, cmd);
  if( idx >= 1 && idx <= 4 ){
    ch = &APPL.chan485[idx];
    ch->Cmd = cmd;
    mg_http_reply(c, 200, s_json_header, "true\n");
  }else{
    mg_http_reply(c, 403, s_json_header, "false\n");
  }
}

static void handle_pcs_get(struct mg_connection *c) {
    struct Enjoy100kW_t* p = &APPL.Enjoy100kW;
  mg_http_reply(c, 200, s_json_header, "{\
%m:%m,%m:%m,%m:%lld,\
%m:%.4f,%m:%.4f,%m:%.4f,\
%m:%.4f,%m:%.4f,%m:%.4f,\
%m:%.4f,%m:%.4f,%m:%.4f,%m:%.4f,\
%m:%.3f,%m:%.3f,%m:%.3f,\
%m:%m,%m:%m,%m:%.4f,%m:%.4f,\
%m:%m,%m:%m,%m:%m,%m:%m,\
%m:%m,%m:%m,%m:%m,%m:%m}\n",
/*1*/MG_ESC("通信状态"), MG_ESC(p->szCommState), MG_ESC("数据更新时间"),MG_ESC(p->szLastUpdate),MG_ESC("通信失败次数"),p->CommFailTotalCnt,
/*2*/MG_ESC("AB线电压"),(p->Uab), MG_ESC("BC线电压"),(p->Ubc), MG_ESC("CA线电压"),(p->Uca),
/*3*/MG_ESC("A相电流"),(p->Ia), MG_ESC("B相电流"),(p->Ib), MG_ESC("C相电流"),(p->Ic),
/*4*/MG_ESC("有功功率"),(p->Ap), MG_ESC("无功功率"),(p->Rap), MG_ESC("电池电压"),(p->BatV), MG_ESC("电池电流"),(p->BatC),
/*5*/MG_ESC("IGBT温度"),(p->Tigbt), MG_ESC("环境温度"),(p->Tenv), MG_ESC("电感温度"),(p->Tind),
/*6*/MG_ESC("工作状态"),MG_ESC(p->szWorkState), MG_ESC("故障状态"), MG_ESC(p->szErrState), MG_ESC("有功功率设定值"), p->Aps, MG_ESC("总母线电压"),p->TotalBusVolt,
/*7*/MG_ESC("硬件故障字1"),MG_ESC(p->szHwFault1),MG_ESC("硬件故障字2"),MG_ESC(p->szHwFault2),MG_ESC("电网故障字"),MG_ESC(p->szGridFault),MG_ESC("母线故障字"),MG_ESC(p->szBusFault),
/*8*/MG_ESC("交流电容故障字"),MG_ESC(p->szAcCapFault),MG_ESC("系统故障字"),MG_ESC(p->szSysFault),MG_ESC("开关故障字"),MG_ESC(p->szOnOffFault),MG_ESC("其他故障字"),MG_ESC(p->szOtherFault));
}

static void handle_bms_get(struct mg_connection *c) {
  struct GaoteBms_t* bms = &APPL.GaoteBms;
  mg_http_reply(c, 200, s_json_header, "{\
%m:%m,%m:%.4f,%m:%.4f,%m:%.4f,%m:%.4f,%m:%m,\
%m:%f,%m:%f,\
%m:%.4f,%m:%.4f,%m:%.4f,\
%m:%.3f,%m:%.3f,%m:%.3f,\
%m:%.3f,%m:%.3f,\
%m:%m,\
%m:%d,%m:%d,%m:%d,%m:%d,\
%m:%.4f,%m:%.4f,\
%m:%m,%m:%lld,%m:%m}\n",
/*1*/MG_ESC("通信状态"), MG_ESC(bms->szCommState), MG_ESC("总电压"),bms->BatV,MG_ESC("总电流"),bms->BatI, MG_ESC("SOC"),bms->Soc, MG_ESC("SOH"),bms->Soh,MG_ESC("电池状态"),MG_ESC(bms->szBatState),
/*2*/MG_ESC("绝缘电阻R+"), bms->PosRes, MG_ESC("绝缘电阻R-"), bms->NegRes,
/*3*/MG_ESC("最高电芯电压"),bms->MaxCellV, MG_ESC("平均电芯电压"),bms->AvgCellV, MG_ESC("最低电芯电压"),bms->MinCellV,
/*4*/MG_ESC("最高电芯温度"),bms->MaxCellT, MG_ESC("平均电芯温度"),bms->AvgCellT, MG_ESC("最低电芯温度"),bms->MinCellT,
/*5*/MG_ESC("压差"),bms->CellVDiff, MG_ESC("温差"),bms->CellTDiff,
/*6*/MG_ESC("高压状态"),MG_ESC(bms->szHvState), 
/*7*/MG_ESC("最高温度位置"),bms->MaxCellTIdx,MG_ESC("最低温度位置"),bms->MinCellTIdx,MG_ESC("最高电压位置"),bms->MaxCellVIdx,MG_ESC("最低电压位置"),bms->MinCellVIdx,
/*8*/MG_ESC("最大可充电流"),bms->MaxChgCurr,MG_ESC("最大可放电流"),bms->MaxDhgCurr,
/*9*/MG_ESC("数据更新时间"),MG_ESC(bms->szLastUpdate),MG_ESC("通信失败次数"),bms->CommFailTotalCnt,MG_ESC("故障信息"), MG_ESC(bms->szErrMsg));
}

static void handle_ac_get(struct mg_connection *c) {
  struct Envicool5kW_t* ac = &APPL.Envicool5kW;
  struct Settings_t* set = &APPL.Set.s;
  mg_http_reply(c, 200, s_json_header, "{\
%m:%m,%m:%m,%m:%m,\
%m:%d,%m:%d,%m:%d,\
%m:%f,%m:%f,\
%m:%d,%m:%d,\
%m:%m,%m:%m,%m:%m,\
%m:%m,%m:%m,%m:%m,\
%m:%d,%m:%d,\
%m:%d,%m:%d,%m:%d,%m:%d,\
%m:%m,%m:%d,%m:%d,%m:%d,\
%m:%m,%m:%m,%m:%m}\n",
/*1*/MG_ESC("通信状态"), MG_ESC(ac->szCommState),MG_ESC("数据更新时间"), MG_ESC(ac->szLastUpdate1), MG_ESC("工作模式"),MG_ESC(ac->szWorkMode), 
/*2*/MG_ESC("出水温度"),ac->OutWaterTemp, MG_ESC("回水温度"),ac->InWaterTemp, MG_ESC("环境温度"),ac->EnvTemp,
/*3*/MG_ESC("回水压力"),ac->InWaterPre, MG_ESC("出水压力"),ac->OutWaterPre, 
/*4*/MG_ESC("故障码"),ac->ErrCode,MG_ESC("故障等级"),ac->ErrLevel,
/*5*/MG_ESC("压缩机状态"),MG_ESC(ac->szCompState),MG_ESC("电加热状态"),MG_ESC(ac->szElecHeatState),MG_ESC("水泵状态"),MG_ESC(ac->szPumpState), 
/*6*/MG_ESC("风机1运行状态"),MG_ESC(ac->szFan1State),MG_ESC("风机2运行状态"),MG_ESC(ac->szFan2State),MG_ESC("风机3运行状态"),MG_ESC(ac->szFan3State),
/*7*/MG_ESC("压缩机转数"),ac->CompRpm, MG_ESC("水泵转数"),ac->PumpRpm,
/*8*/MG_ESC("加热温度"),set->HeatTemp, MG_ESC("加热回差"),set->HeatGap,MG_ESC("制冷温度"),set->CoolTemp, MG_ESC("制冷回差"),set->CoolGap,
/*9*/MG_ESC("控制模式"),MG_ESC(ac->szCtlMode),MG_ESC("温度设定"),ac->SetTemp,MG_ESC("加热温度设定"),set->HeatTempSet,MG_ESC("制冷温度设定"),set->CoolTempSet,
/*10*/MG_ESC("故障信息1"),MG_ESC(ac->szErrMsg1),MG_ESC("故障信息2"),MG_ESC(ac->szErrMsg2),MG_ESC("故障信息3"),MG_ESC(ac->szErrMsg3));
}

static void handle_fe_get(struct mg_connection *c) {
  struct FireAlarm_t* f1 = &APPL.Fa[1];
  struct FireAlarm_t* f2 = &APPL.Fa[2];
  struct FireAlarm_t* f3 = &APPL.Fa[3];
  struct FireAlarm_t* f4 = &APPL.Fa[4];
  struct FireAlarm_t* f5 = &APPL.Fa[5];
  mg_http_reply(c, 200, s_json_header, "{\
%m:{%m:%m,%m:%d,%m:%d,%m:%d,%m:%d,%m:%m,%m:%m,%m:%m},\
%m:{%m:%m,%m:%d,%m:%d,%m:%d,%m:%d,%m:%m,%m:%m,%m:%m},\
%m:{%m:%m,%m:%d,%m:%d,%m:%d,%m:%d,%m:%m,%m:%m,%m:%m},\
%m:{%m:%m,%m:%d,%m:%d,%m:%d,%m:%d,%m:%m,%m:%m,%m:%m},\
%m:{%m:%m,%m:%d,%m:%d,%m:%d,%m:%d,%m:%m,%m:%m,%m:%m}}\n",
    MG_ESC("1#PACK"),MG_ESC("通信状态"), MG_ESC(f1->szCommState),MG_ESC("1#温度"),f1->T1,MG_ESC("2#温度"),f1->T2,MG_ESC("CO"), f1->Co,MG_ESC("VOC"), f1->Voc,
                MG_ESC("故障码"), MG_ESC(f1->szErrCode), MG_ESC("烟雾标志"), MG_ESC(f1->szSmokeFlag),MG_ESC("火情级别"), MG_ESC(f1->szLevel),
    MG_ESC("2#PACK"),MG_ESC("通信状态"), MG_ESC(f2->szCommState),MG_ESC("1#温度"),f2->T1,MG_ESC("2#温度"),f2->T2,MG_ESC("CO"), f2->Co,MG_ESC("VOC"), f2->Voc,
                MG_ESC("故障码"), MG_ESC(f2->szErrCode), MG_ESC("烟雾标志"), MG_ESC(f2->szSmokeFlag),MG_ESC("火情级别"), MG_ESC(f2->szLevel),
    MG_ESC("3#PACK"),MG_ESC("通信状态"), MG_ESC(f3->szCommState),MG_ESC("1#温度"),f3->T1,MG_ESC("2#温度"),f3->T2,MG_ESC("CO"), f3->Co,MG_ESC("VOC"), f3->Voc,
                MG_ESC("故障码"), MG_ESC(f3->szErrCode), MG_ESC("烟雾标志"), MG_ESC(f3->szSmokeFlag),MG_ESC("火情级别"), MG_ESC(f3->szLevel),
    MG_ESC("4#PACK"),MG_ESC("通信状态"), MG_ESC(f4->szCommState),MG_ESC("1#温度"),f4->T1,MG_ESC("2#温度"),f4->T2,MG_ESC("CO"), f4->Co,MG_ESC("VOC"), f4->Voc,
                MG_ESC("故障码"), MG_ESC(f4->szErrCode), MG_ESC("烟雾标志"), MG_ESC(f4->szSmokeFlag),MG_ESC("火情级别"), MG_ESC(f4->szLevel),
    MG_ESC("5#PACK"),MG_ESC("通信状态"), MG_ESC(f5->szCommState),MG_ESC("1#温度"),f5->T1,MG_ESC("2#温度"),f5->T2,MG_ESC("CO"), f5->Co,MG_ESC("VOC"), f5->Voc,
                MG_ESC("故障码"), MG_ESC(f5->szErrCode), MG_ESC("烟雾标志"), MG_ESC(f5->szSmokeFlag),MG_ESC("火情级别"), MG_ESC(f5->szLevel));
}

static void handle_env_get(struct mg_connection *c) {
  struct Adl200_t* adl = &APPL.Adl200;
  struct Dido_t* dido = &APPL.Dido;
  struct Dehumi_t* dh = &APPL.Dehumi;
  struct Co_t* co = &APPL.Co;
  mg_http_reply(c, 200, s_json_header, "{\
%m:{%m:%m,%m:%f,%m:%f,%m:%.4f,%m:%.4f,%m:%.3f,%m:%m},\
%m:{%m:%m,%m:%m,%m:%m,%m:%m,%m:%m,%m:%m,%m:%m,%m:%m,%m:%m},\
%m:{%m:%m,%m:%m,%m:%.3f,%m:%.3f},\
%m:{%m:%m,%m:%m,%m:%d,%m:%m}}\n",
/*1*/MG_ESC("辅助电表"),MG_ESC("通信状态"),MG_ESC(adl->szCommState),MG_ESC("正向有功总电能"), adl->PosAe,MG_ESC("反向有功总电能"),adl->NegAe, MG_ESC("有功功率"),adl->Ap,MG_ESC("无功功率"),adl->Rap,MG_ESC("功率因数"),adl->Pf,MG_ESC("数据更新时间"),MG_ESC(adl->szLastUpdate),
/*2*/MG_ESC("DIDO"),MG_ESC("状态"),MG_ESC(dido->szState),MG_ESC("数据更新时间"),MG_ESC(dido->szLastUpdate), MG_ESC("1#水浸"),MG_ESC(dido->szWaterDec1),MG_ESC("2#水浸"),MG_ESC(dido->szWaterDec2),MG_ESC("前门"),MG_ESC(dido->szFrontDoor),MG_ESC("后门"),MG_ESC(dido->szBackDoor),MG_ESC("急停"),MG_ESC(dido->szEmgStop),MG_ESC("状态灯"),MG_ESC(dido->szLedMode),MG_ESC("消防喷发反馈"),MG_ESC(dido->szFeEruptFb),
/*3*/MG_ESC("除湿机"),MG_ESC("通信状态"),MG_ESC(dh->szCommState),MG_ESC("数据更新时间"),MG_ESC(dh->szLastUpdate),MG_ESC("温度"),dh->Temp,MG_ESC("湿度"),dh->Humi,
/*4*/MG_ESC("一氧化碳"),MG_ESC("通信状态"),MG_ESC(co->szCommState),MG_ESC("数据更新时间"),MG_ESC(co->szLastUpdate),MG_ESC("浓度"),co->Density,MG_ESC("标志"),MG_ESC(co->szFlag));  
}

static void handle_firmware_upload(struct mg_connection *c,
                                   struct mg_http_message *hm) {
  char name[64], offset[20], total[20];
  struct mg_str data = hm->body;
  long ofs = -1, tot = -1;
  name[0] = offset[0] = '\0';
  mg_http_get_var(&hm->query, "name", name, sizeof(name));
  mg_http_get_var(&hm->query, "offset", offset, sizeof(offset));
  mg_http_get_var(&hm->query, "total", total, sizeof(total));
  MG_INFO(("File %s, offset %s, len %lu", name, offset, data.len));
  if ((ofs = mg_json_get_long(mg_str(offset), "$", -1)) < 0 ||
      (tot = mg_json_get_long(mg_str(total), "$", -1)) < 0) {
    mg_http_reply(c, 500, "", "offset and total not set\n");
  } else if (ofs == 0 && mg_ota_begin((size_t) tot) == false) {
    mg_http_reply(c, 500, "", "mg_ota_begin(%ld) failed\n", tot);
  } else if (data.len > 0 && mg_ota_write(data.ptr, data.len) == false) {
    mg_http_reply(c, 500, "", "mg_ota_write(%lu) @%ld failed\n", data.len, ofs);
    mg_ota_end();
  } else if (data.len == 0 && mg_ota_end() == false) {
    mg_http_reply(c, 500, "", "mg_ota_end() failed\n", tot);
  } else {
    mg_http_reply(c, 200, s_json_header, "true\n");
    if (data.len == 0) {
      // Successful mg_ota_end() called, schedule device reboot
      mg_timer_add(c->mgr, 500, 0, (void (*)(void *)) mg_device_reset, NULL);
    }
  }
}

static void handle_firmware_commit(struct mg_connection *c) {
  mg_http_reply(c, 200, s_json_header, "%s\n",
                mg_ota_commit() ? "true" : "false");
}

static void handle_firmware_rollback(struct mg_connection *c) {
  mg_http_reply(c, 200, s_json_header, "%s\n",
                mg_ota_rollback() ? "true" : "false");
}

static size_t print_status(void (*out)(char, void *), void *ptr, va_list *ap) {
  int fw = va_arg(*ap, int);
  return mg_xprintf(out, ptr, "{%m:%d,%m:%c%lx%c,%m:%u,%m:%u}\n",
                    MG_ESC("status"), mg_ota_status(fw), MG_ESC("crc32"), '"',
                    mg_ota_crc32(fw), '"', MG_ESC("size"), mg_ota_size(fw),
                    MG_ESC("timestamp"), mg_ota_timestamp(fw));
}

static void handle_firmware_status(struct mg_connection *c) {
  mg_http_reply(c, 200, s_json_header, "[%M,%M]\n", print_status,
                MG_FIRMWARE_CURRENT, print_status, MG_FIRMWARE_PREVIOUS);
}

static void handle_device_reset(struct mg_connection *c) {
  mg_http_reply(c, 200, s_json_header, "true\n");
  mg_timer_add(c->mgr, 500, 0, (void (*)(void *)) mg_device_reset, NULL);
}

static void handle_device_eraselast(struct mg_connection *c) {
  size_t ss = mg_flash_sector_size(), size = mg_flash_size();
  char *base = (char *) mg_flash_start(), *last = base + size - ss;
  if (mg_flash_bank() == 2) last -= size / 2;
  mg_flash_erase(last);
  mg_http_reply(c, 200, s_json_header, "true\n");
}

// HTTP request handler function
static void fn(struct mg_connection *c, int ev, void *ev_data) {
  if (ev == MG_EV_ACCEPT) {
    if (c->fn_data != NULL) {  // TLS listener!
      struct mg_tls_opts opts = {0};
      opts.cert = mg_unpacked("/certs/server_cert.pem");
      opts.key = mg_unpacked("/certs/server_key.pem");
      mg_tls_init(c, &opts);
    }
  } else if (ev == MG_EV_HTTP_MSG) {
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    struct user *u = authenticate(hm);

    if ( 0 ) { // mg_http_match_uri(hm, "/api/#") && u == NULL
      mg_http_reply(c, 403, "", "Not Authorised\n");
    } else if (mg_http_match_uri(hm, "/api/login")) {
      handle_login(c, u);
    } else if (mg_http_match_uri(hm, "/api/logout")) {
      handle_logout(c);
    } else if (mg_http_match_uri(hm, "/api/debug")) {
      handle_debug(c, hm);
    } else if (mg_http_match_uri(hm, "/api/ctl/get")) {
      handle_ctl_get(c);
    } else if (mg_http_match_uri(hm, "/api/ctl/set")) {
      handle_ctl_set(c, hm->body);
    }else if (mg_http_match_uri(hm, "/api/settings/get")) {
      handle_settings_get(c);
    } else if (mg_http_match_uri(hm, "/api/settings/set")) {
      handle_settings_set(c, hm->body);
    }else if (mg_http_match_uri(hm, "/api/stats/get")) {
      handle_stats_get(c);
    } else if (mg_http_match_uri(hm, "/api/events/get")) {
      handle_events_get(c, hm);
    } else if (mg_http_match_uri(hm, "/api/comm/get")) {
      handle_comm_get(c);
    } else if (mg_http_match_uri(hm, "/api/comm/set")) {
      handle_comm_set(c, hm->body);
    } else if (mg_http_match_uri(hm, "/api/pcs/get")) {
      handle_pcs_get(c);
    } else if (mg_http_match_uri(hm, "/api/pcs/set")) {
      handle_pcs_set(c, hm->body);
    }else if (mg_http_match_uri(hm, "/api/bms/get")) {
      handle_bms_get(c);
    }else if (mg_http_match_uri(hm, "/api/ac/get")) {
      handle_ac_get(c);
    }else if (mg_http_match_uri(hm, "/api/ac/set")) {
      handle_ac_set(c, hm->body);
    }else if (mg_http_match_uri(hm, "/api/env/get")) {
      handle_env_get(c);
    }else if (mg_http_match_uri(hm, "/api/fe/get")) {
      handle_fe_get(c);
    }else if (mg_http_match_uri(hm, "/api/env/set_ledmod")) {
      handle_env_set_ledmod(c, hm->body);
    } else if (mg_http_match_uri(hm, "/api/firmware/upload")) {
      handle_firmware_upload(c, hm);
    } else if (mg_http_match_uri(hm, "/api/firmware/commit")) {
      handle_firmware_commit(c);
    } else if (mg_http_match_uri(hm, "/api/firmware/rollback")) {
      handle_firmware_rollback(c);
    } else if (mg_http_match_uri(hm, "/api/firmware/status")) {
      handle_firmware_status(c);
    } else if (mg_http_match_uri(hm, "/api/device/reset")) {
      handle_device_reset(c);
    } else if (mg_http_match_uri(hm, "/api/device/eraselast")) {
      handle_device_eraselast(c);
    } else {
      struct mg_http_serve_opts opts;
      memset(&opts, 0, sizeof(opts));
#if MG_ARCH == MG_ARCH_UNIX || MG_ARCH == MG_ARCH_WIN32
      opts.root_dir = "web_root";  // On workstations, use filesystem
#else
      opts.root_dir = "/web_root";  // On embedded, use packed files
      opts.fs = &mg_fs_packed;
#endif
      mg_http_serve_dir(c, ev_data, &opts);
    }
    MG_DEBUG(("%lu %.*s %.*s -> %.*s", c->id, (int) hm->method.len,
              hm->method.ptr, (int) hm->uri.len, hm->uri.ptr, (int) 3,
              &c->send.buf[9]));
  }
}

void web_init(struct mg_mgr *mgr) {
  s_settings.device_name = strdup("My Device");
  mg_http_listen(mgr, HTTP_URL, fn, NULL);
  mg_http_listen(mgr, HTTPS_URL, fn, (void *) 1);
  mg_timer_add(mgr, 3600 * 1000, MG_TIMER_RUN_NOW | MG_TIMER_REPEAT,
               timer_sntp_fn, mgr);
}
