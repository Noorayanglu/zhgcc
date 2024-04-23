#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <sys/ioctl.h>
#include <linux/if.h>
#include <linux/serial.h>
#include <syslog.h>
#include <unistd.h>
#include "mongoose.h"
#include "can_frame.h"
#include "appl.h"
#include "modbus.h"
#include "mb.h"
#include "MQTTClient.h"
#include "MQTTClientPersistence.h"

#define EXPORT_PATH "/sys/class/gpio/export"            // GPIO设备导出设备
#define DIR_OUT "out"
#define DIR_IN "in"

char* VERSION = "5.3.11";

struct appl_t APPL;
struct mg_mgr mgr_mqtt1; // thingsboard
struct mg_mgr mgr_mqtt2; // cloud
struct mg_mgr mgr_mqtt3; // gate and trans meter

void appl_ac_set_ctlmod( int m )
{
    struct Envicool5kW_t* ac = &APPL.Envicool5kW;
    ac->Step = 0;
    switch (m)
    {
    case AC_CTLMOD_EMS:
        strcpy(ac->szCtlMode,"EMS控制");
        break;

    case AC_CTLMOD_NON_EMS:
        strcpy(ac->szCtlMode,"非EMS控制");
        break;        
    
    default:
        strcpy(ac->szCtlMode,"未知");
        break;
    }
    ac->CtlMode = m;
    syslog(LOG_INFO,"%s, CtlMode is Set To %s", __func__, ac->szCtlMode);
}

#if 0
int appl_cfg_save( void )
{
    long long chksum = 0;
    FILE* fp = NULL;
    int rc, i;
    fp = fopen("./cfg.bin","wb");
    if( fp == NULL){
        syslog(LOG_INFO,"%s, fopen ./cfg.bin Fail", __func__);
        return -1;
    }else{
        for( i = 0; i < sizeof(APPL.Set.buf - 8); i++ ){
            chksum += APPL.Set.buf[8 + i];
        }
        APPL.Set.s.chksum = chksum;
        rc = fwrite(APPL.Set.buf, sizeof(char), sizeof(APPL.Set.buf), fp);
        if( rc != sizeof(APPL.Set.buf)){
            syslog(LOG_INFO,"%s, fwrite ./cfg.bin Fail, rc:%d", __func__, rc);
            return -1;
        }else{
            return 0;
        }
    }
}

int appl_cfg_read( void )
{
    long long chksum = 0;
    FILE* fp = NULL;
    int rc, i;
    fp = fopen("./cfg.bin","rb");
    if( fp == NULL){
        syslog(LOG_INFO,"%s, fopen ./cfg.bin Fail", __func__);
        return -1;
    }else{
        rc = fread(APPL.Set.buf, sizeof(char), sizeof(APPL.Set.buf), fp);
        if( rc != sizeof(APPL.Set.buf)){
            syslog(LOG_INFO,"%s, fread ./cfg.bin Fail, rc:%d", __func__, rc);
            return -1;
        }else{
            for( i = 0; i < sizeof(APPL.Set.buf - 8); i++ ){
                chksum += APPL.Set.buf[8 + i];
            }
            if( chksum == APPL.Set.s.chksum ){
                return 0;
            }else{
                syslog(LOG_INFO,"%s, Chksum Fail, rc:%d", __func__, rc);
                return -1;
            }
        }
    }
}
#else
int appl_cfg_save( void )
{
    FILE* fp = NULL;
    int rc, i;
    fp = fopen("./cfg.bin","wb");
    if( fp == NULL){
        syslog(LOG_INFO,"%s, fopen ./cfg.bin Fail", __func__);
        return -1;
    }else{
        rc = fwrite(APPL.Set.buf, sizeof(char), sizeof(APPL.Set.buf), fp);
        if( rc != sizeof(APPL.Set.buf)){
            syslog(LOG_INFO,"%s, fwrite ./cfg.bin Fail, rc:%d", __func__, rc);
            return -1;
        }else{
            return 0;
        }
    }
}

int appl_cfg_read( void )
{
    FILE* fp = NULL;
    int rc, i;
    fp = fopen("./cfg.bin","rb");
    if( fp == NULL){
        syslog(LOG_INFO,"%s, fopen ./cfg.bin Fail", __func__);
        return -1;
    }else{
        rc = fread(APPL.Set.buf, sizeof(char), sizeof(APPL.Set.buf), fp);
        if( rc != sizeof(APPL.Set.buf)){
            syslog(LOG_INFO,"%s, fread ./cfg.bin Fail, rc:%d", __func__, rc);
            return -1;
        }else{
            return 0;
        }
    }
}
#endif

void appl_cfg_set_err( void )
{
    APPL.Set.s.bErr = 1;
    strcpy(APPL.Set.s.szState,"故障");
}

void appl_cfg_reset_err( void )
{
    APPL.Set.s.bErr = 0;
    strcpy(APPL.Set.s.szState,"正常");
}

void appl_dido_set_state(int s, int e)
{
    struct Dido_t* dido = &APPL.Dido;
    dido->State = s;
    switch(s){
    case ST_DIDO_INIT:
        strcpy(dido->szState,"初始化");
    break;

    case ST_DIDO_RUN:
        strcpy(dido->szState,"运行");
    break;

    case ST_DIDO_ERR:
        strcpy(dido->szState,"故障");
    break;

    default:
        strcpy(dido->szState,"未知");
    break;
    }

    dido->ErrCode = e;
    switch (e)
    {
    case ERR_DIDO_NONE:
        strcpy(dido->szErrCode,"无");
        break;
    case ERR_DIDO_INIT_FAIL:
        strcpy(dido->szErrCode,"初始化失败");
        break;
    
    default:
        strcpy(dido->szErrCode,"未知");
        break;
    }
}

char* appl_get_datetime_long( void )
{
    time_t timep;
    struct tm *tsp;
    static char buf[128];

    time(&timep);
    // tsp = gmtime(&timep);
    tsp = localtime(&timep);

    sprintf(buf, "%04d-%02d-%02d %02d:%02d:%02d", tsp->tm_year + 1900,
            tsp->tm_mon + 1,
            tsp->tm_mday,
            tsp->tm_hour,
            tsp->tm_min,
            (short)tsp->tm_sec);
    return buf;
}

static char* appl_get_datetime_short( void )
{
    static char buf[128];    
    time_t timep;
    struct tm *tsp;

    time(&timep);
    // tsp = gmtime(&timep);
    tsp = localtime(&timep);

    sprintf(buf, "%02d:%02d:%02d",
            tsp->tm_hour,
            tsp->tm_min,
            (short)tsp->tm_sec);
    return buf;
}

static void appl_get_datetime_num(int *y, int *m, int *d, int *h, int *min, int *s)
{
    time_t timep;
    struct tm *tsp;

    time(&timep);
    // tsp = gmtime(&timep);
    tsp = localtime(&timep);

    *y = 1900 + tsp->tm_year;
    *m = 1 + tsp->tm_mon;
    *d = tsp->tm_mday;
    *h = tsp->tm_hour;
    *min = tsp->tm_min;
    *s = tsp->tm_sec;
}

static void* thrd_485_1(void *param)
{
    char buf[128];
    modbus_t* ctx = NULL;
    struct timeval t;
    int rc;
    unsigned short data[256];
    unsigned short start;
    unsigned short nbr;
    int chidx = 1;
    struct chan485_t* ch = &APPL.chan485[chidx];
    struct Enjoy100kW_t* pcs = &APPL.Enjoy100kW;
    struct Adl200_t* auxm = &APPL.Adl200;
    int64_t startts;
    int trycnt;

    MG_INFO(("%s ENTER", __func__));

    appl_485_set_state(chidx, ST_485_INIT, ERR_485_NONE);
    while(1){
        startts = mg_millis();
        switch( ch->state){
        case ST_485_INIT:
            ctx = modbus_new_rtu(ch->szdev, ch->baud, 'N', 8, 1);
            if (ctx == NULL){
                MG_INFO(("%s, modbus rtu new fail", __func__));
                appl_485_set_state(chidx, ST_485_ERR, ERR_485_INIT_FAIL);
            }else if (modbus_connect(ctx) == -1){
                MG_INFO(("%s, modbus rtu connect fail", __func__));
                modbus_free(ctx);
                ctx = NULL;
                appl_485_set_state(chidx, ST_485_ERR, ERR_485_INIT_FAIL);
            }else{
                // t.tv_sec = 0;
                // t.tv_usec = 500000;  // 500ms
                // modbus_set_response_timeout(ctx, 0, 500000);
                ch->reqcnt = 0;
                ch->failcnt = 0;
                appl_485_set_state(chidx, ST_485_RUN, ERR_485_NONE);
            }
        break;

        case ST_485_RUN:
            // Process Cmd
            if(ch->Cmd == CMD_485_PCS_START){
                ch->Cmd = CMD_485_DONE;
                trycnt = 3;
                while(trycnt-- > 0){
                    appl_chan485_lock(chidx);
                    usleep(30000);
                    modbus_set_slave(ctx, pcs->Adr);
                    rc = modbus_write_register( ctx, 0x0291, 1);
                    appl_chan485_unlock(chidx);
                    if(rc >= 0){
                        break;
                    }else{
                        modbus_flush(ctx);
                    }
                }
            }else if(ch->Cmd == CMD_485_PCS_STOP){
                ch->Cmd = CMD_485_DONE;
                trycnt = 3;
                while(trycnt-- > 0){
                    appl_chan485_lock(chidx);
                    usleep(30000);
                    modbus_set_slave(ctx, pcs->Adr);
                    rc = modbus_write_register( ctx, 0x0291, 0);
                    appl_chan485_unlock(chidx);
                    if(rc >= 0){
                        break;
                    }else{
                        modbus_flush(ctx);
                    }
                }
            }else if(ch->Cmd == CMD_485_PCS_SET_APS){
                ch->Cmd = CMD_485_DONE;
                trycnt = 3;
                while(trycnt-- > 0){
                    appl_chan485_lock(chidx);
                    usleep(30000);
                    modbus_set_slave(ctx, pcs->Adr);
                    rc = modbus_write_register( ctx, 0x0D57, (0 - ch->CmdParam)*10);
                    appl_chan485_unlock(chidx);
                    if(rc >= 0){
                        break;
                    }else{
                        modbus_flush(ctx);
                    }
                }
            }else if( ch->Cmd == CMD_485_RESET ){
                ch->Cmd = CMD_485_DONE;
                if (ctx != NULL){
                    modbus_close(ctx);
                    modbus_free(ctx);
                    ctx = NULL;
                }
                appl_485_set_state(chidx, ST_485_INIT, ERR_485_NONE);
                break;
            }

            // ****************************************
            // Comm With PCS
            // ****************************************
            if( mg_millis() - pcs->LastUpdate > 5000 ){
                pcs->CommState = ST_COMM_ERR;
                strcpy(pcs->szCommState,"故障");
            }else{
                pcs->CommState = ST_COMM_NORM;
                strcpy(pcs->szCommState,"正常");
            }
            // Part1
            appl_chan485_lock(chidx);
            usleep(30000);
            modbus_set_slave(ctx, pcs->Adr);
            start = 0x6020;
            nbr = 60;
            rc = modbus_read_registers( ctx, start, nbr, data);
            appl_chan485_unlock(chidx);
            ch->reqcnt += 1;
            if(rc != nbr){
                ch->failcnt += 1;
                pcs->CommFailTotalCnt += 1;
                modbus_flush(ctx);
            }else{
                pcs->Uab = (short)data[0x6020 - start] / 10.0;
                pcs->Ubc = (short)data[0x6021 - start] / 10.0;
                pcs->Uca = (short)data[0x6022 - start] / 10.0;
                pcs->Ia = (short)data[0x6026 - start] / 10.0;
                pcs->Ib = (short)data[0x6027 - start] / 10.0;
                pcs->Ic = (short)data[0x6028 - start] / 10.0;

                pcs->Ap = 0 - (short)data[0x6039 - start] / 10.0;
                pcs->Rap = (short)data[0x603A - start] / 10.0;

                pcs->TotalBusVolt = (short)data[0x6050 - start] / 10.0;
                pcs->BatV = (short)data[0x6053 - start] / 10.0;
                pcs->BatC = (short)data[0x6054 - start] / 10.0;
                pcs->WorkState = data[0x6057 - start] & 0x0001;
                if( pcs->WorkState == 0 ){
                    strcpy(pcs->szWorkState,"待机");
                }else{
                    strcpy(pcs->szWorkState,"运行");
                }
                pcs->ErrState = data[0x6057 - start] >> 6 & 0x0001;
                if( pcs->ErrState == 0 ){
                    strcpy(pcs->szErrState,"否");
                }else{
                    strcpy(pcs->szErrState,"是");
                }
                pcs->Tigbt = (short)data[0x6058 - start] / 10.0;
                pcs->Tenv = (short)data[0x6059 - start] / 10.0;
                pcs->Tind = (short)data[0x605A - start] / 10.0;

                // Part2
                appl_chan485_lock(chidx);
                usleep(30000);
                start = 0x0D57;
                nbr = 1;
                rc = modbus_read_registers( ctx, start, nbr, data);
                appl_chan485_unlock(chidx);
                ch->reqcnt += 1;
                if(rc != nbr){
                    ch->failcnt += 1;
                    pcs->CommFailTotalCnt += 1;
                    modbus_flush(ctx);
                }else{
                    pcs->Aps = (short)data[0x0D57 - start]/10.0;
                    
                    // Part3
                    appl_chan485_lock(chidx);
                    usleep(30000);
                    start = 0x1700;
                    nbr = 8;
                    rc = modbus_read_registers( ctx, start, nbr, data);
                    appl_chan485_unlock(chidx);
                    ch->reqcnt += 1;
                    if(rc != nbr){
                        ch->failcnt += 1;
                        pcs->CommFailTotalCnt += 1;
                        modbus_flush(ctx);
                    }else{
                        // 硬件故障字 1 0x1700
                        pcs->szHwFault1[0] = 0;
                        pcs->HwFault1 = data[0x1700 - start];
                        if( (pcs->HwFault1 >> 0) & 0x0001){
                            strcat(pcs->szHwFault1,"EPO 故障");
                        }
                        if( (pcs->HwFault1 >> 1) & 0x0001){
                            strcat(pcs->szHwFault1,"IGBT 硬件过流");
                        }
                        if( (pcs->HwFault1 >> 2) & 0x0001){
                            strcat(pcs->szHwFault1,"母线硬件过压");
                        }
                        if( (pcs->HwFault1 >> 4) & 0x0001){
                            strcat(pcs->szHwFault1,"功率模块逐波限流");
                        }         

                        // 硬件故障字 2 0x1701
                        pcs->szHwFault2[0] = 0;
                        pcs->HwFault2 = data[0x1701 - start];
                        if( (pcs->HwFault2 >> 0) & 0x0001){
                            strcat(pcs->szHwFault2,"24V 电源故障");
                        }
                        if( (pcs->HwFault2 >> 1) & 0x0001){
                            strcat(pcs->szHwFault2,"风扇故障");
                        }
                        if( (pcs->HwFault2 >> 2) & 0x0001){
                            strcat(pcs->szHwFault2,"连接故障");
                        }
                        if( (pcs->HwFault2 >> 6) & 0x0001){
                            strcat(pcs->szHwFault2,"防雷器故障");
                        }    
                        if( (pcs->HwFault2 >> 8) & 0x0001){
                            strcat(pcs->szHwFault2,"功率模块过温");
                        }      
                        if( (pcs->HwFault2 >> 10) & 0x0001){
                            strcat(pcs->szHwFault2,"15V 电源故障");
                        }                                 

                        // 电网故障字 0x1702
                        pcs->szGridFault[0] = 0;
                        pcs->GridFault = data[0x1702 - start];
                        if( (pcs->GridFault >> 0) & 0x0001){
                            strcat(pcs->szGridFault,"A 相过压故障");
                        }
                        if( (pcs->GridFault >> 1) & 0x0001){
                            strcat(pcs->szGridFault,"B 相过压故障");
                        }
                        if( (pcs->GridFault >> 2) & 0x0001){
                            strcat(pcs->szGridFault,"C 相过压故障");
                        }
                        if( (pcs->GridFault >> 3) & 0x0001){
                            strcat(pcs->szGridFault,"A 相欠压故障");
                        }    
                        if( (pcs->GridFault >> 4) & 0x0001){
                            strcat(pcs->szGridFault,"B 相欠压故障");
                        }      
                        if( (pcs->GridFault >> 5) & 0x0001){
                            strcat(pcs->szGridFault,"C 相欠压故障");
                        }   
                       if( (pcs->GridFault >> 6) & 0x0001){
                            strcat(pcs->szGridFault,"电网过频");
                        }
                        if( (pcs->GridFault >> 7) & 0x0001){
                            strcat(pcs->szGridFault,"电网欠频");
                        }
                        if( (pcs->GridFault >> 8) & 0x0001){
                            strcat(pcs->szGridFault,"电网相序错误");
                        }
                        if( (pcs->GridFault >> 9) & 0x0001){
                            strcat(pcs->szGridFault,"A 相软件过流");
                        }    
                        if( (pcs->GridFault >> 10) & 0x0001){
                            strcat(pcs->szGridFault,"B 相软件过流");
                        }      
                        if( (pcs->GridFault >> 11) & 0x0001){
                            strcat(pcs->szGridFault,"C 相软件过流");
                        }  						
                        if( (pcs->GridFault >> 12) & 0x0001){
                            strcat(pcs->szGridFault,"电网电压不平衡");
                        }
                        if( (pcs->GridFault >> 13) & 0x0001){
                            strcat(pcs->szGridFault,"电网电流不平衡");
                        }    
                        if( (pcs->GridFault >> 14) & 0x0001){
                            strcat(pcs->szGridFault,"电网缺相");
                        }      
                        if( (pcs->GridFault >> 15) & 0x0001){
                            strcat(pcs->szGridFault,"N 线过流");
                        }  			

                        // 母线故障字 0x1703
                        pcs->szBusFault[0] = 0;
                        pcs->BusFault = data[0x1703 - start];
                        if( (pcs->BusFault >> 0) & 0x0001){
                            strcat(pcs->szBusFault,"预充母线过压");
                        }
                        if( (pcs->BusFault >> 1) & 0x0001){
                            strcat(pcs->szBusFault,"预充母线欠压");
                        }
                        if( (pcs->BusFault >> 2) & 0x0001){
                            strcat(pcs->szBusFault,"不控整流母线过压");
                        }
                        if( (pcs->BusFault >> 3) & 0x0001){
                            strcat(pcs->szBusFault,"不控整流母线欠压");
                        }    
                        if( (pcs->BusFault >> 4) & 0x0001){
                            strcat(pcs->szBusFault,"运行母线过压");
                        }      
                        if( (pcs->BusFault >> 5) & 0x0001){
                            strcat(pcs->szBusFault,"运行母线欠压");
                        }   
                       if( (pcs->BusFault >> 6) & 0x0001){
                            strcat(pcs->szBusFault,"正负母线不平衡");
                        }
                        if( (pcs->BusFault >> 7) & 0x0001){
                            strcat(pcs->szBusFault,"电池欠压");
                        }
                        if( (pcs->BusFault >> 8) & 0x0001){
                            strcat(pcs->szBusFault,"电流模式母线欠压");
                        }
                        if( (pcs->BusFault >> 9) & 0x0001){
                            strcat(pcs->szBusFault,"电池过压");
                        }    
                        if( (pcs->BusFault >> 10) & 0x0001){
                            strcat(pcs->szBusFault,"直流预充电过流");
                        }      
                        if( (pcs->BusFault >> 11) & 0x0001){
                            strcat(pcs->szBusFault,"直流过流");
                        }  						
		                        					
                        // 交流电容故障字 0x1704
                        pcs->szAcCapFault[0] = 0;
                        pcs->AcCapFault = data[0x1704 - start];
                        if( (pcs->AcCapFault >> 0) & 0x0001){
                            strcat(pcs->szAcCapFault,"预充电超时");
                        }
                        if( (pcs->AcCapFault >> 1) & 0x0001){
                            strcat(pcs->szAcCapFault,"预充电 A 相过流");
                        }
                        if( (pcs->AcCapFault >> 2) & 0x0001){
                            strcat(pcs->szAcCapFault,"预充电 B 相过流");
                        }
                        if( (pcs->AcCapFault >> 3) & 0x0001){
                            strcat(pcs->szAcCapFault,"预充电 C 相过流");
                        }    

                        // 系统故障字 0x1705
                        pcs->szSysFault[0] = 0;
                        pcs->SysFault = data[0x1705 - start];
                        if( (pcs->SysFault >> 2) & 0x0001){
                            strcat(pcs->szSysFault,"AD 采样零漂");
                        }
                        if( (pcs->SysFault >> 11) & 0x0001){
                            strcat(pcs->szSysFault,"STS 通信故障");
                        }
                        if( (pcs->SysFault >> 12) & 0x0001){
                            strcat(pcs->szSysFault,"电池系统告警或故障");
                        }
                        if( (pcs->SysFault >> 13) & 0x0001){
                            strcat(pcs->szSysFault,"BMS 通讯故障");
                        }   
                        if( (pcs->SysFault >> 14) & 0x0001){
                            strcat(pcs->szSysFault,"从模块 CAN 通信故障");
                        }
                        if( (pcs->SysFault >> 15) & 0x0001){
                            strcat(pcs->szSysFault,"EMS 通信故障");
                        }   

                        // 开关故障字 0x1706
                        pcs->szOnOffFault[0] = 0;
                        pcs->OnOffFault = data[0x1706 - start];
                        if( (pcs->OnOffFault >> 0) & 0x0001){
                            strcat(pcs->szOnOffFault,"预充电继电器闭合失败");
                        }
                        if( (pcs->OnOffFault >> 1) & 0x0001){
                            strcat(pcs->szOnOffFault,"预充电继电器断开失败");
                        }
                        if( (pcs->OnOffFault >> 2) & 0x0001){
                            strcat(pcs->szOnOffFault,"预充电继电器闭合状态错误");
                        }
                        if( (pcs->OnOffFault >> 3) & 0x0001){
                            strcat(pcs->szOnOffFault,"预充电继电器断开状态错误");
                        }   
                        if( (pcs->OnOffFault >> 4) & 0x0001){
                            strcat(pcs->szOnOffFault,"主继电器闭合失败");
                        }
                        if( (pcs->OnOffFault >> 5) & 0x0001){
                            strcat(pcs->szOnOffFault,"主继电器断开失败");
                        }   
                        if( (pcs->OnOffFault >> 6) & 0x0001){
                            strcat(pcs->szOnOffFault,"主继电器闭合状态错误");
                        }
                        if( (pcs->OnOffFault >> 7) & 0x0001){
                            strcat(pcs->szOnOffFault,"主继电器断开状态错误");
                        }                           

                        // 其他故障字 0x1707
                        pcs->szOtherFault[0] = 0;
                        pcs->OtherFault = data[0x1707 - start];
                        if( (pcs->OtherFault >> 0) & 0x0001){
                            strcat(pcs->szOtherFault,"逆变电压 A 相过压故障");
                        }
                        if( (pcs->OtherFault >> 1) & 0x0001){
                            strcat(pcs->szOtherFault,"逆变电压 B 相过压故障");
                        }
                        if( (pcs->OtherFault >> 2) & 0x0001){
                            strcat(pcs->szOtherFault,"逆变电压 C 相过压故障");
                        }
                        if( (pcs->OtherFault >> 3) & 0x0001){
                            strcat(pcs->szOtherFault,"电网孤岛故障");
                        }   
                        if( (pcs->OtherFault >> 5) & 0x0001){
                            strcat(pcs->szOtherFault,"系统谐振故障");
                        }   
                        if( (pcs->OtherFault >> 6) & 0x0001){
                            strcat(pcs->szOtherFault,"软件过压过流");
                        }
                        if( (pcs->OtherFault >> 8) & 0x0001){
                            strcat(pcs->szOtherFault,"模块拨码地址错误");
                        }
                        if( (pcs->OtherFault >> 9) & 0x0001){
                            strcat(pcs->szOtherFault,"逆变电压 A 相欠压故障");
                        }
                        if( (pcs->OtherFault >> 10) & 0x0001){
                            strcat(pcs->szOtherFault,"逆变电压 B 相欠压故障");
                        }
                        if( (pcs->OtherFault >> 11) & 0x0001){
                            strcat(pcs->szOtherFault,"逆变电压 C 相欠压故障");
                        }   
                        if( (pcs->OtherFault >> 12) & 0x0001){
                            strcat(pcs->szOtherFault,"离网无同步信号故障");
                        }
                        if( (pcs->OtherFault >> 14) & 0x0001){
                            strcat(pcs->szOtherFault,"离网短路故障");
                        }                         

                        pcs->LastUpdate = mg_millis();
                        strcpy(pcs->szLastUpdate, appl_get_datetime_long());
                    }
                }
            }

            // comm with AUXM
            if( mg_millis() - auxm->LastUpdate > 5000 ){
                auxm->CommState = ST_COMM_ERR;
                strcpy(auxm->szCommState,"故障");
            }else{
                auxm->CommState = ST_COMM_NORM;
                strcpy(auxm->szCommState,"正常");
            }
            appl_chan485_lock(chidx);
            usleep(30000);
            modbus_set_slave(ctx, auxm->Adr);
            start = 0x000B;
            nbr = 7;
            rc = modbus_read_registers( ctx, start, nbr, data);
            appl_chan485_unlock(chidx);
            ch->reqcnt += 1;
            if(rc != nbr){
                ch->failcnt += 1;
                auxm->CommFailTotalCnt += 1;
                modbus_flush(ctx);
            }else{
                auxm->Volt = data[0x000B - start] * 0.1;
                auxm->Curr = data[0x000C - start] * 0.01;
                auxm->Ap = data[0x000D - start] * 0.001;
                auxm->Rap = data[0x000E - start] * 0.001;
                auxm->Pf = data[0x0010 - start] * 0.001;
                auxm->Gf = data[0x0011 - start] * 0.01;

                appl_chan485_lock(chidx);
                usleep(30000);
                start = 0x0068;
                nbr = 24;
                rc = modbus_read_registers( ctx, start, nbr, data);
                appl_chan485_unlock(chidx);
                ch->reqcnt += 1;
                if(rc != nbr){
                    ch->failcnt += 1;
                    auxm->CommFailTotalCnt += 1;
                    modbus_flush(ctx);
                }else{
                    auxm->PosAe = (data[0x0069 - start] | data[0x0068 - start] << 16) * 0.01;
                    auxm->NegAe = (data[0x0073 - start] | data[0x0072 - start]<<16) * 0.01;

                    auxm->LastUpdate = mg_millis();
                    strcpy(auxm->szLastUpdate, appl_get_datetime_long());
                }
            }
        break;

        case ST_485_ERR:
            if(ch->Cmd == CMD_485_RESET){
                ch->Cmd = CMD_485_DONE;
                if (ctx != NULL){
                    modbus_close(ctx);
                    modbus_free(ctx);
                    ctx = NULL;
                }
                appl_485_set_state(chidx, ST_485_INIT, ERR_485_NONE);
            }else{
                usleep(300000);
            }
        break;

        default:
            // never reach here
        break;
        }
        usleep(100000);
        ch->loopcnt += 1;
        ch->looptime = mg_millis() - startts;
    }

    MG_INFO(("%s EXIT", __func__));
}

static void* thrd_485_2(void *param)
{
    char buf[128];
    modbus_t* ctx = NULL;
    struct timeval t;
    int rc;
    unsigned short data[256];
    unsigned char bits[256];
    unsigned short start;
    unsigned short nbr;
    int chidx = 2;
    struct chan485_t* ch = &APPL.chan485[chidx];
    struct GaoteBms_t* bms = &APPL.GaoteBms;
    int step = 1;
    int i;
    int64_t startts;

    MG_INFO(("%s ENTER", __func__));

    appl_485_set_state(chidx, ST_485_INIT, ERR_485_NONE);
    while(1){
        startts = mg_millis();
        switch( ch->state){
        case ST_485_INIT:
            ctx = modbus_new_rtu(ch->szdev, ch->baud, 'N', 8, 1);
            if (ctx == NULL){
                MG_INFO(("%s, modbus rtu new fail", __func__));
                appl_485_set_state(chidx, ST_485_ERR, ERR_485_INIT_FAIL);
            }else if (modbus_connect(ctx) == -1){
                MG_INFO(("%s, modbus rtu connect fail", __func__));
                modbus_free(ctx);
                ctx = NULL;
                appl_485_set_state(chidx, ST_485_ERR, ERR_485_INIT_FAIL);
            }else{
                t.tv_sec = 0;
                t.tv_usec = 500000;
                modbus_set_response_timeout(ctx, 0, 500000);
                ch->reqcnt = 0;
                ch->failcnt = 0;
                appl_485_set_state(chidx, ST_485_RUN, ERR_485_NONE);
            }
        break;

        case ST_485_RUN:
            // Process Cmd
            if( ch->Cmd == CMD_485_RESET ){
                ch->Cmd = CMD_485_DONE;
                if (ctx != NULL){
                    modbus_close(ctx);
                    modbus_free(ctx);
                    ctx = NULL;
                }
                appl_485_set_state(chidx, ST_485_INIT, ERR_485_NONE);
                break;
            }

            // comm with BMS
            if( mg_millis() - bms->LastUpdate > 5000 ){
                bms->CommState = ST_COMM_ERR;
                strcpy(bms->szCommState,"故障");
            }else{
                bms->CommState = ST_COMM_NORM;
                strcpy(bms->szCommState,"正常");
            }
            // ****** PART 1 ******************
            appl_chan485_lock(chidx);
            usleep(30000);
            modbus_set_slave(ctx, bms->Adr);
            start = 1;
            nbr = 64;
            rc = modbus_read_input_registers( ctx, start, nbr, data);
            appl_chan485_unlock(chidx);
            ch->reqcnt += 1;
            if(rc != nbr){
                ch->failcnt += 1;
                bms->CommFailTotalCnt += 1;
                modbus_flush(ctx);
            }else{
                bms->BatV = data[1 - start] / 10.0;
                bms->BatI = data[2 - start] / 10.0 -1600;
                bms->Soc = data[3 - start];
                bms->Soh = data[4 - start];
                bms->PosRes = data[5 - start];
                bms->NegRes = data[6 - start];
                bms->BatState = data[7 - start];
                if(bms->BatState == 1){
                    strcpy(bms->szBatState,"充电");
                }else if(bms->BatState == 2){
                    strcpy(bms->szBatState,"放电");
                }else if(bms->BatState == 3){
                    strcpy(bms->szBatState,"开路");
                }else{
                    strcpy(bms->szBatState,"未知");
                }
                bms->DI = data[8 - start];
                if( (bms->DI & 0x00000025) == 0x00000025){
                    bms->HvState = 1;
                    strcpy(bms->szHvState,"ON");
                }else{
                    bms->HvState = 0;
                    strcpy(bms->szHvState,"OFF");
                }
                bms->MaxCellT = data[11 - start] - 40;
                bms->MaxCellTModIdx = data[12 - start];
                bms->MaxCellTIdx = data[13 - start];
                bms->MinCellT = data[14 - start] - 40;
                bms->MinCellTModIdx = data[15 - start];
                bms->MinCellTIdx = data[16 - start];
                bms->AvgCellT = data[17 - start] - 40;
                bms->MaxCellV = data[20 - start] / 1000.0;
                bms->MaxCellVModIdx = data[21 - start];
                bms->MaxCellVIdx = data[22 - start];
                bms->AvgCellV = data[19 - start] / 1000.0;
                bms->MinCellV = data[23 - start] / 1000.0;
                bms->MinCellVModIdx = data[24 - start];
                bms->MinCellVIdx = data[25 - start];
                bms->MaxChgCurr = data[59 - start]/10.0;
                bms->MaxDhgCurr = data[60 - start]/10.0;
                if( data[61 - start] & 0x0001 ){
                    bms->bChgNotAllowed = 1;
                }else{
                    bms->bChgNotAllowed = 0;
                }
                if( (data[61 - start] >> 1) & 0x0001 ){
                    bms->bDhgNotAllowed = 1;
                }else{
                    bms->bDhgNotAllowed = 0;
                }
                if( (data[61 - start] >> 2) & 0x0001 ){
                    bms->bTotalFatalErr = 1;
                }else{
                    bms->bTotalFatalErr = 0;
                }
                if( (data[61 - start] >> 2) & 0x0001 ){
                    bms->bWarning = 1;
                }else{
                    bms->bWarning = 0;
                }
                bms->CellVDiff = data[62 - start];
                bms->CellTDiff = data[63 - start] - 40;

                // ****** PART 2 ******************
                appl_chan485_lock(chidx);
                usleep(30000);
                modbus_set_slave(ctx, bms->Adr);
                start = 1;
                nbr = 58;
                rc = modbus_read_input_bits( ctx, start, nbr, bits);
                appl_chan485_unlock(chidx);
                ch->reqcnt += 1;
                if(rc != nbr){
                    ch->failcnt += 1;
                    bms->CommFailTotalCnt += 1;
                }else{
                    bms->szErrMsg[0] = 0;
                    if( bits[1 - start] ){
                        bms->bTotalOv = 1;
                        strcpy(bms->szErrMsg, "组端过压3级报警");
                    }else{
                        bms->bTotalOv = 0;
                    }
                    if( bits[6 - start] ){
                        bms->bTotalUv = 1;
                        strcpy(bms->szErrMsg, "组端欠压3级报警");
                    }else{
                        bms->bTotalUv = 0;
                    }
                    if( bits[9 - start] ){
                        bms->bTotalDhgOc = 1;
                        strcpy(bms->szErrMsg, "组端放电过流 3 级报警");
                    }else{
                        bms->bTotalDhgOc = 0;
                    }
                    if( bits[12 - start] ){
                        bms->bTotalChgOc = 1;
                        strcpy(bms->szErrMsg, "组端充电过流 3 级报警");
                    }else{
                        bms->bTotalChgOc = 0;
                    }
                    if( bits[15 - start] ){
                        bms->bTotalResWarn = 1;
                        strcpy(bms->szErrMsg, "组端绝缘 3 级报警");
                    }else{
                        bms->bTotalResWarn = 0;
                    }
                    if( bits[18 - start] ){
                        bms->bCellOt = 1;
                        strcpy(bms->szErrMsg, "单体电池过温 3 级报警");
                    }else{
                        bms->bCellOt = 0;
                    }
                    if( bits[21 - start] ){
                        bms->bCellUt = 1;
                        strcpy(bms->szErrMsg, "单体电池欠温 3 级报警");
                    }else{
                        bms->bCellUt = 0;
                    }
                    if( bits[24 - start] ){
                        bms->bCellOv = 1;
                        strcpy(bms->szErrMsg, "单体电压过压 3 级报警");
                    }else{
                        bms->bCellOv = 0;
                    }
                    if( bits[27 - start] ){
                        bms->bCellUv = 1;
                        strcpy(bms->szErrMsg, "单体电压欠压 3 级报警");
                    }else{
                        bms->bCellUv = 0;
                    }
                    if( bits[30 - start] ){
                        bms->bCellVDiffErr = 1;
                        strcpy(bms->szErrMsg, "单体压差过高 3 级报警");
                    }else{
                        bms->bCellVDiffErr = 0;
                    }
                    if( bits[33 - start] ){
                        bms->bCellTDiffErr = 1;
                        strcpy(bms->szErrMsg, "单体温差过高 3 级报警");
                    }else{
                        bms->bCellTDiffErr = 0;
                    }            
                    if( bits[36 - start] ){
                        bms->bSocLowErr = 1;
                        strcpy(bms->szErrMsg, "SOC 过低 3 级告警");
                    }else{
                        bms->bSocLowErr = 0;
                    }               
                    if( bits[39 - start] ){
                        bms->bPowerPackOtErr = 1;
                        strcpy(bms->szErrMsg, "动力插箱温度过高 3 级报警");
                    }else{
                        bms->bPowerPackOtErr = 0;
                    }             
                    if( bits[42 - start] ){
                        bms->bPackOv = 1;
                        strcpy(bms->szErrMsg, "电池模组过压 3 级报警");
                    }else{
                        bms->bPackOv = 0;
                    }      
                    if( bits[45 - start] ){
                        bms->bPackUv = 1;
                        strcpy(bms->szErrMsg, "电池模组欠压 3 级报警");
                    }else{
                        bms->bPackUv = 0;
                    }          
                    if( bits[46 - start] ){
                        bms->bDI1Err = 1;
                        strcpy(bms->szErrMsg, "DI1故障");
                    }else{
                        bms->bDI1Err = 0;
                    }          
                    if( bits[47 - start] ){
                        bms->bDI2Err = 1;
                        strcpy(bms->szErrMsg, "DI2故障");
                    }else{
                        bms->bDI2Err = 0;
                    }         
                    if( bits[48 - start] ){
                        bms->bDI3Err = 1;
                        strcpy(bms->szErrMsg, "DI3故障");
                    }else{
                        bms->bDI3Err = 0;
                    }        
                    if( bits[49 - start] ){
                        bms->bDI4Err = 1;
                        strcpy(bms->szErrMsg, "DI4故障");
                    }else{
                        bms->bDI4Err = 0;
                    }             
                    if( bits[50 - start] ){
                        bms->bDI5Err = 1;
                        strcpy(bms->szErrMsg, "DI5故障");
                    }else{
                        bms->bDI5Err = 0;
                    }                    
                    if( bits[51 - start] ){
                        bms->bDI6Err = 1;
                        strcpy(bms->szErrMsg, "DI6故障");
                    }else{
                        bms->bDI6Err = 0;
                    }     
                    if( bits[52 - start] ){
                        bms->bDI7Err = 1;
                        strcpy(bms->szErrMsg, "DI7故障");
                    }else{
                        bms->bDI7Err = 0;
                    }           
                    if( bits[53 - start] ){
                        bms->bDI8Err = 1;
                        strcpy(bms->szErrMsg, "DI8故障");
                    }else{
                        bms->bDI8Err = 0;
                    }             
                    if( bits[54 - start] ){
                        bms->bMasterSlaveCommErr = 1;
                        strcpy(bms->szErrMsg, "主从内网通讯失联");
                    }else{
                        bms->bMasterSlaveCommErr = 0;
                    }               
                    if( bits[55 - start] ){
                        bms->bCellVoltDacErr = 1;
                        strcpy(bms->szErrMsg, "单体电压采集故障");
                    }else{
                        bms->bCellVoltDacErr = 0;
                    }        
                    if( bits[56 - start] ){
                        bms->bCellTempDacErr = 1;
                        strcpy(bms->szErrMsg, "单体温度采集故障");
                    }else{
                        bms->bCellTempDacErr = 0;
                    }     
                    if( bits[57 - start] ){
                        bms->bJumpErr = 1;
                        strcpy(bms->szErrMsg, "跳机故障");
                    }else{
                        bms->bJumpErr = 0;
                    }                                                                                                                                                   
                    if( bits[58 - start] ){
                        bms->bBatLimErr = 1;
                        strcpy(bms->szErrMsg, "电池极限故障");
                    }else{
                        bms->bBatLimErr = 0;
                    }    
 
                    // ****** PART 3 ******************
                    if( step == 1 ){ 
                        appl_chan485_lock(chidx);
                        usleep(30000);
                        modbus_set_slave(ctx, bms->Adr);
                        start = 100;
                        nbr = 52*2;
                        rc = modbus_read_input_registers( ctx, start, nbr, data);
                        appl_chan485_unlock(chidx);
                        ch->reqcnt += 1;
                        if(rc != nbr){
                            ch->failcnt += 1;
                            bms->CommFailTotalCnt += 1;
                            modbus_flush(ctx);
                        }else{
                            for( i = 1; i <= 52; i++){ // pack1 cell volt
                                bms->CellVolt[1][i] = data[100 + i - 1 - start] / 1000.0;
                            }
                            for( i = 1; i <= 52; i++){ // pack2 cell volt
                                bms->CellVolt[2][i] = data[100 + i - 1 + 52 - start] / 1000.0;
                            }
                            // ****** PART 4 ******************
                            appl_chan485_lock(chidx);
                            usleep(30000);
                            modbus_set_slave(ctx, bms->Adr);
                            start = 204;
                            nbr = 52*2;
                            rc = modbus_read_input_registers( ctx, start, nbr, data);
                            appl_chan485_unlock(chidx);
                            ch->reqcnt += 1;
                            if(rc != nbr){
                                ch->failcnt += 1;
                                bms->CommFailTotalCnt += 1;
                                modbus_flush(ctx);
                            }else{
                                for( i = 1; i <= 52; i++){ // pack3 cell volt
                                    bms->CellVolt[3][i] = data[i - 1] / 1000.0;
                                }
                                for( i = 1; i <= 52; i++){ // pack4 cell volt
                                    bms->CellVolt[4][i] = data[i - 1 + 52] / 1000.0;
                                }

                                step = 2;
                                bms->LastUpdate = mg_millis();
                                strcpy(bms->szLastUpdate, appl_get_datetime_long());
                            }
                        }
                    }else if( step == 2 ){
                        appl_chan485_lock(chidx);
                        usleep(30000);
                        modbus_set_slave(ctx, bms->Adr);
                        start = 308;
                        nbr = 52;
                        rc = modbus_read_input_registers( ctx, start, nbr, data);
                        appl_chan485_unlock(chidx);
                        ch->reqcnt += 1;
                        if(rc != nbr){
                            ch->failcnt += 1;
                            bms->CommFailTotalCnt += 1;
                            modbus_flush(ctx);
                        }else{
                            for( i = 1; i <= 52; i++){
                                bms->CellVolt[5][i] = data[i - 1] / 1000.0;
                            }
                            appl_chan485_lock(chidx);
                            usleep(30000);
                            modbus_set_slave(ctx, bms->Adr);
                            start = 600;
                            nbr = 52*2;
                            rc = modbus_read_input_registers( ctx, start, nbr, data);
                            appl_chan485_unlock(chidx);
                            ch->reqcnt += 1;
                            if(rc != nbr){
                                ch->failcnt += 1;
                                bms->CommFailTotalCnt += 1;
                                modbus_flush(ctx);
                            }else{
                                for( i = 1; i <= 52; i++){ // pack1 temp
                                    bms->CellTemp[1][i] = data[i - 1] - 40;
                                }
                                for( i = 1; i <= 52; i++){ // pack2 temp
                                    bms->CellTemp[2][i] = data[i - 1 + 52] - 40;
                                }
                                
                                step = 3;
                                bms->LastUpdate = mg_millis();
                                strcpy(bms->szLastUpdate, appl_get_datetime_long());
                            }
                        }
                    }else if( step == 3 ){
                        appl_chan485_lock(chidx);
                        usleep(30000);
                        modbus_set_slave(ctx, bms->Adr);
                        start = 704;
                        nbr = 52*2;
                        rc = modbus_read_input_registers( ctx, start, nbr, data);
                        appl_chan485_unlock(chidx);
                        ch->reqcnt += 1;
                        if(rc != nbr){
                            ch->failcnt += 1;
                            bms->CommFailTotalCnt += 1;
                            modbus_flush(ctx);
                        }else{
                            for( i = 1; i <= 52; i++){ // pack3 temp
                                bms->CellTemp[3][i] = data[i - 1] - 40;
                            }
                            for( i = 1; i <= 52; i++){ // pack4 temp
                                bms->CellTemp[4][i] = data[i - 1 + 52] - 40;
                            }
                            appl_chan485_lock(chidx);
                            usleep(30000);
                            modbus_set_slave(ctx, bms->Adr);
                            start = 808;
                            nbr = 52;
                            rc = modbus_read_input_registers( ctx, start, nbr, data);
                            appl_chan485_unlock(chidx);
                            ch->reqcnt += 1;
                            if(rc != nbr){
                                ch->failcnt += 1;
                                bms->CommFailTotalCnt += 1;
                                modbus_flush(ctx);
                            }else{
                                for( i = 1; i <= 52; i++){ // pack5 temp
                                    bms->CellTemp[5][i] = data[i - 1] - 40;
                                }
                                 
                                step = 1;
                                bms->LastUpdate = mg_millis();
                                strcpy(bms->szLastUpdate, appl_get_datetime_long());
                            }
                        }
                    }
                }
            }

        break;

        case ST_485_ERR:
            if(ch->Cmd == CMD_485_RESET){
                ch->Cmd = CMD_485_DONE;
                if (ctx != NULL){
                    modbus_close(ctx);
                    modbus_free(ctx);
                    ctx = NULL;
                }
                appl_485_set_state(chidx, ST_485_INIT, ERR_485_NONE);
            }else{
                usleep(300000);
            }
        break;

        default:
            // never reach here
        break;
        }
        usleep(100000);
        ch->loopcnt += 1;
        ch->looptime = mg_millis() - startts;
    }
    MG_INFO(("%s EXIT", __func__));
}

static void* thrd_485_3(void *param)
{
    char buf[128];
    modbus_t* ctx = NULL;
    struct timeval t;
    int rc;
    unsigned short data[256];
    unsigned short start;
    unsigned short nbr;
    int chidx = 3;
    struct chan485_t* ch = &APPL.chan485[chidx];
    struct Envicool5kW_t* ac = &APPL.Envicool5kW;
    struct GaoteBms_t* bms = &APPL.GaoteBms;
    int64_t startts;
    int trycnt;

    syslog(LOG_INFO, "%s ENTER", __func__);

    appl_485_set_state(chidx, ST_485_INIT, ERR_485_NONE);
    while(1){
        startts = mg_millis();
        switch( ch->state){
        case ST_485_INIT:
            ctx = modbus_new_rtu(ch->szdev, ch->baud, 'N', 8, 1);
            if (ctx == NULL){
                MG_INFO(("%s, modbus rtu new fail", __func__));
                appl_485_set_state(chidx, ST_485_ERR, ERR_485_INIT_FAIL);
            }else if (modbus_connect(ctx) == -1){
                MG_INFO(("%s, modbus rtu connect fail", __func__));
                modbus_free(ctx);
                ctx = NULL;
                appl_485_set_state(chidx, ST_485_ERR, ERR_485_INIT_FAIL);
            }else{
                t.tv_sec = 0;
                t.tv_usec = 500000;
                modbus_set_response_timeout(ctx, 0, 500000);
                ch->reqcnt = 0;
                ch->failcnt = 0;
                appl_485_set_state(chidx, ST_485_RUN, ERR_485_NONE);
            }
        break;

        case ST_485_RUN:
            // ****************************
            // Process Cmd
            // ****************************
            if( ch->Cmd == CMD_485_RESET ){
                ch->Cmd = CMD_485_DONE;
                if (ctx != NULL){
                    modbus_close(ctx);
                    modbus_free(ctx);
                    ctx = NULL;
                }
                appl_485_set_state(chidx, ST_485_INIT, ERR_485_NONE);
                break;
            }
        break;

        case ST_485_ERR:
            if(ch->Cmd == CMD_485_RESET){
                ch->Cmd = CMD_485_DONE;
                if (ctx != NULL){
                    modbus_close(ctx);
                    modbus_free(ctx);
                    ctx = NULL;
                }
                appl_485_set_state(chidx, ST_485_INIT, ERR_485_NONE);
            }else{
                usleep(300000);
            }
        break;

        default:
            // never reach here
        break;
        }

        usleep(100000);
        ch->loopcnt += 1;
        ch->looptime = mg_millis() - startts;
    }
    MG_INFO(("%s EXIT", __func__));
}

static void* thrd_485_4(void *param)
{
    char buf[128];
    modbus_t* ctx = NULL;
    struct timeval t;
    int rc;
    unsigned short data[256];
    unsigned char uc[128];
    unsigned short start;
    unsigned short nbr;
    int chidx = 4;
    struct chan485_t* ch = &APPL.chan485[chidx];
    struct Dehumi_t* dh = &APPL.Dehumi;
    struct Co_t* co = &APPL.Co;
    int64_t startts;
    int trycnt;

    MG_INFO(("%s ENTER", __func__));

    appl_485_set_state(chidx, ST_485_INIT, ERR_485_NONE);
    while(1){
        startts = mg_millis();
        switch( ch->state){
        case ST_485_INIT:
            ctx = modbus_new_rtu(ch->szdev, ch->baud, 'N', 8, 1);
            if (ctx == NULL){
                MG_INFO(("%s, modbus rtu new fail", __func__));
                appl_485_set_state(chidx, ST_485_ERR, ERR_485_INIT_FAIL);
            }else if (modbus_connect(ctx) == -1){
                MG_INFO(("%s, modbus rtu connect fail", __func__));
                modbus_free(ctx);
                ctx = NULL;
                appl_485_set_state(chidx, ST_485_ERR, ERR_485_INIT_FAIL);
            }else{
                t.tv_sec = 0;
                t.tv_usec = 500000;
                modbus_set_response_timeout(ctx, 0, 500000);
                ch->reqcnt = 0;
                ch->failcnt = 0;
                appl_485_set_state(chidx, ST_485_RUN, ERR_485_NONE);
            }
        break;

        case ST_485_RUN:
            // Process Cmd
            if( ch->Cmd == CMD_485_RESET ){
                ch->Cmd = CMD_485_DONE;
                if (ctx != NULL){
                    modbus_close(ctx);
                    modbus_free(ctx);
                    ctx = NULL;
                }
                appl_485_set_state(chidx, ST_485_INIT, ERR_485_NONE);
                break;
            }

            // comm with DH
            if( mg_millis() - dh->LastUpdate > 5000 ){
                dh->CommState = ST_COMM_ERR;
                strcpy(dh->szCommState,"故障");
            }else{
                dh->CommState = ST_COMM_NORM;
                strcpy(dh->szCommState,"正常");
            }
            appl_chan485_lock(chidx);
            usleep(30000);
            modbus_set_slave(ctx, dh->Adr);
            start = 0;
            nbr = 2;
            rc = modbus_read_registers( ctx, start, nbr, data);
            appl_chan485_unlock(chidx);
            ch->reqcnt += 1;
            if(rc != nbr){
                ch->failcnt += 1;
                dh->CommFailTotalCnt += 1;
            }else{
                dh->Temp = (short)data[0 - start] / 10.0;
                dh->Humi = (short)data[1 - start] / 10.0;
                dh->LastUpdate = mg_millis();
                strcpy(dh->szLastUpdate, appl_get_datetime_long());
            }

            // comm with CO
            if( mg_millis() - co->LastUpdate > 5000 ){
                co->CommState = ST_COMM_ERR;
                strcpy(co->szCommState,"故障");
            }else{
                co->CommState = ST_COMM_NORM;
                strcpy(co->szCommState,"正常");
            }
            appl_chan485_lock(chidx);
            usleep(30000);
            modbus_set_slave(ctx, co->Adr);
            start = 1;
            nbr = 2;
            rc = modbus_read_registers( ctx, start, nbr, data);
            appl_chan485_unlock(chidx);
            ch->reqcnt += 1;
            if(rc != nbr){
                ch->failcnt += 1;
                co->CommFailTotalCnt += 1;
            }else{
                co->Flag = data[1 - start]&0x000F;
                if(co->Flag == 0x00){
                    strcpy(co->szFlag,"无报警");
                }else if(co->Flag == 0xFF){
                    strcpy(co->szFlag,"气体告警");
                }else if(co->Flag == 0xFE){
                    strcpy(co->szFlag,"传感器故障");
                }else if(co->Flag == 0xFD){
                    strcpy(co->szFlag,"传感器预热");
                }else{
                    strcpy(co->szFlag,"未知");
                }
                co->Density = data[2 - start];
                co->LastUpdate = mg_millis();
                strcpy(co->szLastUpdate, appl_get_datetime_long());
            }    
                     
        break;

        case ST_485_ERR:
            if(ch->Cmd == CMD_485_RESET){
                ch->Cmd = CMD_485_DONE;
                if (ctx != NULL){
                    modbus_close(ctx);
                    modbus_free(ctx);
                    ctx = NULL;
                }
                appl_485_set_state(chidx, ST_485_INIT, ERR_485_NONE);
            }else{
                usleep(300000);
            }
        break;

        default:
            // never reach here
        break;
        }

        usleep(200000);
        ch->loopcnt += 1;
        ch->looptime = mg_millis() - startts;
    }
    MG_INFO(("%s EXIT", __func__));
}

#define PF_CAN 29
#define AF_CAN PF_CAN
#define SIOCSCANBAUDRATE (SIOCDEVPRIVATE + 0)
#define SIOCGCANBAUDRATE (SIOCDEVPRIVATE + 1)
#define SOL_CAN_RAW (SOL_CAN_BASE + CAN_RAW)
#define CAN_RAW_FILTER 1
#define CAN_RAW_RECV_OWN_MSGS 0x4
typedef __u32 can_baudrate_t;
struct ifreq ifr[CHANCAN_NBR + 1];

static void* thrd_can1_rx(void *thrdparam)
{
    int i = 0;
    int rc = 0;
    struct can_frame frame;
    int chidx = 1;
    struct chancan_t* ch = &APPL.chancan[chidx];
    int is_ext_frm = 0;
    unsigned int id = 0;
    int pidx;
    struct FireAlarm_t* fa = NULL;

    syslog(LOG_INFO,"%s ENTER", __func__);
    while (1){
        if( ch->sock <= 0 ){
            usleep(100000);
            continue;
        }
        memset(&frame, 0, sizeof(struct can_frame));
        rc = read(ch->sock, &frame, sizeof(struct can_frame));
        if (rc > 0){
            ch->RdCnt += 1;
            if (frame.can_dlc){
                is_ext_frm = (frame.can_id & CAN_EFF_FLAG) ? 1 : 0;
                if (is_ext_frm){
                    id = frame.can_id & CAN_EFF_MASK;
                }else{
                    id = frame.can_id & CAN_SFF_MASK;
                }
                if ( ((id >> 16) & 0xFF) == 0xB0){
                    pidx = id & 0xFF;
                    fa = &APPL.Fa[pidx];
                    fa->T1 = frame.data[0] - 40;
                    fa->T2 = frame.data[1] - 40;
                    fa->Co = frame.data[2] * 256 + frame.data[3];
                    fa->Voc = frame.data[4] * 256 + frame.data[5];
                    if (frame.data[6] == 0xAA){
                        strcpy(fa->szSmokeFlag, "正常");
                        fa->SmokeFlagVal = 0;
                    }else if (frame.data[6] == 0x55){
                        strcpy(fa->szSmokeFlag, "预警");
                        fa->SmokeFlagVal = 1;
                    }else{
                        strcpy(fa->szSmokeFlag, "未知");
                        fa->SmokeFlagVal = -1;
                    }
                    fa->LevelVal = ((frame.data[7] >> 4) & 0x0F);
                    if (fa->LevelVal == 0x00){
                        strcpy(fa->szLevel, "正常");
                    }else if (fa->LevelVal == 0x01){
                        strcpy(fa->szLevel, "一级");
                    }else if (fa->LevelVal == 0x02){
                        strcpy(fa->szLevel, "二级");
                    }else if (fa->LevelVal == 0x03){
                        strcpy(fa->szLevel, "三级");
                    }else if (fa->LevelVal == 0x04){
                        strcpy(fa->szLevel, "四级");
                    }else{
                        strcpy(fa->szLevel, "未知");
                    }
                    fa->ErrCodeVal = (frame.data[7] & 0x0F);
                    if (fa->ErrCodeVal == 0x00){
                        strcpy(fa->szErrCode,"正常");
                    }else if (fa->ErrCodeVal == 0x01){
                        strcpy(fa->szErrCode,"灭火器已启动");
                    }else if (fa->ErrCodeVal == 0x02){
                        strcpy(fa->szErrCode,"传感器故障");
                    }else if (fa->ErrCodeVal == 0x03){
                        strcpy(fa->szErrCode,"硬件故障");
                    }else{
                        strcpy(fa->szErrCode,"未知");
                    }

                    fa->LastUpdate = mg_millis();
                }
            }
        }else{
            APPL.chancan[chidx].RdFailcnt += 1;
        }
    }
    return NULL;
}

static void* thrd_can1(void* param)
{
    int chidx = 1;
    struct chancan_t* ch = &APPL.chancan[chidx];
    struct sockaddr_can addr;
    int recv_own_msgs = 0; // set loop back:  1 enable 0 disable
    int rc = 0;
    pthread_t thrd_rx;
    struct can_frame frame;
    int64_t startts;
    unsigned char FaAdr = 1;
    int i;
    struct FireAlarm_t* fa = NULL;

    MG_INFO(("%s ENTER", __func__));

    appl_can_set_state(chidx, ST_CAN_INIT, ERR_CAN_NONE);
    while(1){
        startts = mg_millis();
        for( i = 1; i <= 5; i++ ){
            fa = &APPL.Fa[i];
            if( mg_millis() - fa->LastUpdate > 5000 ){
                fa->CommState = ST_COMM_ERR;
                strcpy(fa->szCommState,"故障");
            }else{
                fa->CommState = ST_COMM_NORM;
                strcpy(fa->szCommState,"正常");
            }
        }

        switch(appl_can_get_state(chidx)){
            case ST_CAN_INIT:
                ch->sock = socket(PF_CAN, SOCK_RAW, CAN_RAW);
                if (ch->sock < 0){
                    appl_can_set_state(chidx, ST_CHANCAN_ERR, ERR_CAN_INIT_FAIL);
                }else{
                    addr.can_family = AF_CAN;
                    strcpy(ifr[chidx].ifr_name, ch->szdev);
                    rc = ioctl(ch->sock, SIOCGIFINDEX, &ifr[chidx]);
                    if (rc && ifr[chidx].ifr_ifindex == 0){
                        close(ch->sock);
                        ch->sock = 0;
                        appl_can_set_state(chidx, ST_CHANCAN_ERR, ERR_CAN_INIT_FAIL);
                    }else{
                        addr.can_ifindex = ifr[chidx].ifr_ifindex;
                        setsockopt(ch->sock, SOL_CAN_RAW, CAN_RAW_RECV_OWN_MSGS, &recv_own_msgs, sizeof(recv_own_msgs));
                        if (bind(ch->sock, (struct sockaddr *)&addr, sizeof(addr)) < 0){
                            close(ch->sock);
                            ch->sock = 0;
                            appl_can_set_state(chidx, ST_CHANCAN_ERR, ERR_CAN_INIT_FAIL);
                        }else{
                            if (pthread_create(&thrd_rx, NULL, thrd_can1_rx, NULL) != 0){
                                close(ch->sock);
                                ch->sock = 0;
                                appl_can_set_state(chidx, ST_CHANCAN_ERR, ERR_CAN_INIT_FAIL);
                            }else{
                                appl_can_set_state(chidx, ST_CHANCAN_RUN,ERR_CAN_NONE);
                            }
                        }
                    }
                }
            break;

            case ST_CHANCAN_RUN:
                usleep(200000); // 200ms
                frame.can_id = 0x18A200F6 | (FaAdr << 8);
                FaAdr++;
                if(FaAdr > 5){
                    FaAdr = 1;
                }
                // e.frame.can_id = 0x18A200F6 | (0xFF << 8);  //发送广播消息
                frame.can_id |= CAN_EFF_FLAG;
                frame.can_dlc = 2;
                frame.data[0] = 0x5A;
                frame.data[1] = 0x4E;
                frame.data[2] = 0;
                frame.data[3] = 0;
                frame.data[4] = 0;
                frame.data[5] = 0;
                frame.data[6] = 0;
                frame.data[7] = 0;
                write(ch->sock, (char*)&frame, sizeof(frame));
                ch->WrCnt += 1;
            break;

            case ST_CHANCAN_ERR:
                usleep(100000);
            break;

            default:
                // NEVER REACH HERE
                usleep(100000);
            break;
        }
        ch->Loopcnt += 1;
        ch->LoopTime = mg_millis() - startts;
    }

    MG_INFO(("%s EXIT", __func__));
}

static void* thrd_can2_rx(void *thrdparam)
{
    int i = 0;
    int rc = 0;
    struct can_frame frame;
    int chidx = 2;
    struct chancan_t* ch = &APPL.chancan[chidx];
    int is_ext_frm = 0;
    unsigned int id = 0;
    struct Envicool5kW_t* dev = &APPL.Envicool5kW;

    syslog(LOG_INFO, "%s ENTER", __func__);

    while (1){
        if( ch->sock <= 0 ){
            usleep(100000); // 100ms
            continue;
        }
        memset(&frame, 0, sizeof(struct can_frame));
        rc = read(ch->sock, &frame, sizeof(struct can_frame));
        if (rc > 0){
            ch->RdCnt += 1;
            if (frame.can_dlc){
                is_ext_frm = (frame.can_id & CAN_EFF_FLAG) ? 1 : 0;
                if (is_ext_frm){
                    id = frame.can_id & CAN_EFF_MASK;
                }else{
                    id = frame.can_id & CAN_SFF_MASK;
                }
                if (id == 0x18008040){
                    dev->LastUpdate1 = mg_millis();
                    strcpy(dev->szLastUpdate1, appl_get_datetime_long());
                    dev->WorkMode = frame.data[0] & 0x03;  // byte1
                    if(dev->WorkMode == 0){
                        strcpy(dev->szWorkMode,"停机");
                    }else if(dev->WorkMode == 1){
                        strcpy(dev->szWorkMode,"制冷");
                    }else if(dev->WorkMode == 2){
                        strcpy(dev->szWorkMode,"加热");
                    }else if(dev->WorkMode == 3){
                        strcpy(dev->szWorkMode,"自循环");
                    }else{
                        strcpy(dev->szWorkMode,"未知");
                    }
                    dev->OutWaterTemp = frame.data[1] - 40;
                    dev->InWaterTemp = frame.data[2] - 40;
                    dev->EnvTemp = frame.data[3] - 40;
                    dev->InWaterPre = frame.data[4] / 10.0;
                    dev->OutWaterPre = frame.data[5] / 10.0;
                    dev->ErrCode = frame.data[7] & 0x1f;
                    dev->ErrLevel = (frame.data[7] >> 6) & 0x03;
                }else if (id == 0x18018040){
                    dev->LastUpdate2 = mg_millis();
                    strcpy(dev->szLastUpdate2, appl_get_datetime_long());
                    if ((frame.data[0] & 0x01) == 0x01){
                        dev->CompState = 1;
                        strcpy(dev->szCompState,"打开");
                    }else{
                        dev->CompState = 0;
                        strcpy(dev->szCompState,"关闭");
                    }
                    if (((frame.data[0] >> 1) & 0x01) == 0x01){
                        dev->CompHeatStripState = 1;
                    }else{
                        dev->CompHeatStripState = 0;
                    }
                    if (((frame.data[0] >> 2) & 0x01) == 0x01){
                        dev->ElecHeatState = 1;
                        strcpy(dev->szElecHeatState,"打开");
                    }else{
                        dev->ElecHeatState = 0;
                        strcpy(dev->szElecHeatState,"关闭");
                    }
                    if (((frame.data[0] >> 3) & 0x01) == 0x01){
                        dev->PumpState = 1;
                        strcpy(dev->szPumpState,"打开");
                    }else{
                        dev->PumpState = 0;
                        strcpy(dev->szPumpState,"关闭");
                    }
                    if (((frame.data[0] >> 4) & 0x01) == 0x01){
                        dev->Fan1State = 1;
                        strcpy(dev->szFan1State,"打开");
                    }else{
                        dev->Fan1State = 0;
                        strcpy(dev->szFan1State,"关闭");
                    }
                    if (((frame.data[0] >> 5) & 0x01) == 0x01){
                        dev->Fan2State = 1;
                        strcpy(dev->szFan2State,"打开");
                    }else{
                        dev->Fan2State = 0;
                        strcpy(dev->szFan2State,"关闭");
                    }
                    if (((frame.data[0] >> 6) & 0x01) == 0x01){
                        dev->Fan3State = 1;
                        strcpy(dev->szFan3State,"打开");
                    }else{
                        dev->Fan3State = 0;
                        strcpy(dev->szFan3State,"关闭");
                    }
                    dev->CompRpm = frame.data[1] * 100;
                    dev->PumpRpm = frame.data[2] * 100;
                }else if (id == 0x18068040){
                    dev->szErrMsg1[0] = 0;
                    if( (frame.data[0]>>0) & 0x01 ){
                        strcat(dev->szErrMsg1,"出水压力传感器故障(进、出均故障)");
                    }
                    if( (frame.data[0]>>1) & 0x01 ){
                        strcat(dev->szErrMsg1,"交流过压告警");
                    }
                    if( (frame.data[0]>>2) & 0x01 ){
                        strcat(dev->szErrMsg1,"交流欠压告警");
                    }
                    if( (frame.data[0]>>3) & 0x01 ){
                        strcat(dev->szErrMsg1,"水箱缺水告警");
                    }           
                    if( (frame.data[0]>>4) & 0x01 ){
                        strcat(dev->szErrMsg1,"水泵故障");
                    }       
                    if( (frame.data[0]>>5) & 0x01 ){
                        strcat(dev->szErrMsg1,"水泵故障锁定");
                    }                                                  
                }else if (id == 0x18078040){
                    dev->szErrMsg2[0] = 0;
                    // byte1
                    if( (frame.data[0]>>0) & 0x01 ){
                        strcat(dev->szErrMsg2,"出水压力传感器故障");
                    }
                    if( (frame.data[0]>>1) & 0x01 ){
                        strcat(dev->szErrMsg2,"回水压力传感器故障");
                    }
                    if( (frame.data[0]>>2) & 0x01 ){
                        strcat(dev->szErrMsg2,"低压传感器故障");
                    }
                    if( (frame.data[0]>>3) & 0x01 ){
                        strcat(dev->szErrMsg2,"出水温感故障");
                    }           
                    if( (frame.data[0]>>4) & 0x01 ){
                        strcat(dev->szErrMsg2,"系统低压告警");
                    }       
                    if( (frame.data[0]>>5) & 0x01 ){
                        strcat(dev->szErrMsg2,"系统高压开关告警");
                    }      
                    if( (frame.data[0]>>6) & 0x01 ){
                        strcat(dev->szErrMsg2,"排气温度过高告警");
                    }  
                    if( (frame.data[0]>>7) & 0x01 ){
                        strcat(dev->szErrMsg2,"CAN 通讯故障告警");
                    }      
                    // byte2
                    if( (frame.data[1]>>0) & 0x01 ){
                        strcat(dev->szErrMsg2,"吸气温感故障");
                    }
                    if( (frame.data[1]>>1) & 0x01 ){
                        strcat(dev->szErrMsg2,"制冷系统异常");
                    }
                    if( (frame.data[1]>>2) & 0x01 ){
                        strcat(dev->szErrMsg2,"低吸气过热度告警");
                    }
                    if( (frame.data[1]>>3) & 0x01 ){
                        strcat(dev->szErrMsg2,"压缩机变频器过压告警");
                    }           
                    if( (frame.data[1]>>4) & 0x01 ){
                        strcat(dev->szErrMsg2,"压缩机变频器欠压告警");
                    }       
                    if( (frame.data[1]>>5) & 0x01 ){
                        strcat(dev->szErrMsg2,"压缩机变频器过流告警");
                    }      
                    if( (frame.data[1]>>6) & 0x01 ){
                        strcat(dev->szErrMsg2,"压缩机变频器过温告警");
                    }  
                    if( (frame.data[1]>>7) & 0x01 ){
                        strcat(dev->szErrMsg2,"压缩机变频器通讯故障告警");
                    }       
                    // byte3
                    if( (frame.data[2]>>0) & 0x01 ){
                        strcat(dev->szErrMsg2,"压缩机变频器缺相告警");
                    }
                    if( (frame.data[2]>>1) & 0x01 ){
                        strcat(dev->szErrMsg2,"压缩机变频器其他故障告警");
                    }
                    if( (frame.data[2]>>2) & 0x01 ){
                        strcat(dev->szErrMsg2,"水泵压差低告警");
                    }
                    if( (frame.data[2]>>3) & 0x01 ){
                        strcat(dev->szErrMsg2,"排气温度过高锁定");
                    }           
                    if( (frame.data[2]>>4) & 0x01 ){
                        strcat(dev->szErrMsg2,"系统高压锁定");
                    }       
                    if( (frame.data[2]>>5) & 0x01 ){
                        strcat(dev->szErrMsg2,"系统低压锁定");
                    }      
                    if( (frame.data[2]>>6) & 0x01 ){
                        strcat(dev->szErrMsg2,"压缩机变频器过流锁定");
                    }  
                    if( (frame.data[2]>>7) & 0x01 ){
                        strcat(dev->szErrMsg2,"压缩机变频器过压锁定");
                    }         
                    // byte4
                    if( (frame.data[3]>>0) & 0x01 ){
                        strcat(dev->szErrMsg2,"压缩机变频器欠压锁定");
                    }
                    if( (frame.data[3]>>1) & 0x01 ){
                        strcat(dev->szErrMsg2,"压缩机变频器过温锁定");
                    }
                    if( (frame.data[3]>>2) & 0x01 ){
                        strcat(dev->szErrMsg2,"压缩机变频器缺相锁定");
                    }
                    if( (frame.data[3]>>3) & 0x01 ){
                        strcat(dev->szErrMsg2,"压缩机变频器其他故障锁定");
                    }                                                                                                                                                
                }else if (id == 0x18088040){
                    dev->szErrMsg3[0] = 0;
                    // byte1
                    if( (frame.data[0]>>0) & 0x01 ){
                        strcat(dev->szErrMsg3,"高压压力传感器故障");
                    }
                    if( (frame.data[0]>>1) & 0x01 ){
                        strcat(dev->szErrMsg3,"环境温感故障");
                    }
                    if( (frame.data[0]>>2) & 0x01 ){
                        strcat(dev->szErrMsg3,"回水温感故障");
                    }
                    if( (frame.data[0]>>3) & 0x01 ){
                        strcat(dev->szErrMsg3,"排气温感故障");
                    }           
                    if( (frame.data[0]>>4) & 0x01 ){
                        strcat(dev->szErrMsg3,"冷凝温感故障");
                    }       
                    if( (frame.data[0]>>5) & 0x01 ){
                        strcat(dev->szErrMsg3,"异常掉电告警");
                    }      
                    if( (frame.data[0]>>6) & 0x01 ){
                        strcat(dev->szErrMsg3,"出水低温告警");
                    }  
                    if( (frame.data[0]>>7) & 0x01 ){
                        strcat(dev->szErrMsg3,"出水高温告警");
                    }    
                    // byte2
                    if( (frame.data[1]>>0) & 0x01 ){
                        strcat(dev->szErrMsg3,"出水压力过高告警");
                    }                                                                                  
                }
            }
        }else{
            APPL.chancan[chidx].RdFailcnt += 1;
        }
    }
    return NULL;
}

static void* thrd_can2(void* param)
{
    int chidx = 2;
    struct Settings_t* set = &APPL.Set.s;
    struct chancan_t* ch = &APPL.chancan[chidx];
    struct Envicool5kW_t* ac = &APPL.Envicool5kW;
    int* step = &ac->Step;
    struct GaoteBms_t* bms = &APPL.GaoteBms;
    struct sockaddr_can addr;
    int recv_own_msgs = 0; // set loop back:  1 enable 0 disable
    int rc = 0;
    pthread_t thrd_rx;
    struct can_frame frame;
    int64_t startts;
    int64_t Update1Intv;
    int64_t Update2Intv;

    syslog(LOG_INFO, "%s ENTER", __func__);

    ac->MaxUpdate1Intv = 0;
    ac->MaxUpdate2Intv = 0;

    appl_can_set_state(chidx, ST_CAN_INIT, ERR_CAN_NONE);
    while(1){
        startts = mg_millis();
        Update1Intv = mg_millis() - ac->LastUpdate1;
        if( Update1Intv > ac->MaxUpdate1Intv ){
            ac->MaxUpdate1Intv = Update1Intv;
        }
        Update2Intv = mg_millis() - ac->LastUpdate2;
        if( Update2Intv > ac->MaxUpdate2Intv ){
            ac->MaxUpdate2Intv = Update2Intv;
        }        
        if( Update1Intv < 900000 && Update2Intv < 900000){
            ac->CommState = ST_COMM_NORM;
            strcpy(ac->szCommState,"正常");
        }else{
            ac->CommState = ST_COMM_ERR;
            strcpy(ac->szCommState,"故障");
        }

        switch(appl_can_get_state(chidx)){
            case ST_CAN_INIT:
                ch->sock = socket(PF_CAN, SOCK_RAW, CAN_RAW);
                if (ch->sock < 0){
                    appl_can_set_state(chidx, ST_CHANCAN_ERR, ERR_CAN_INIT_FAIL);
                }else{
                    addr.can_family = AF_CAN;
                    strcpy(ifr[chidx].ifr_name, ch->szdev);
                    rc = ioctl(ch->sock, SIOCGIFINDEX, &ifr[chidx]);
                    if (rc && ifr[chidx].ifr_ifindex == 0){
                        close(ch->sock);
                        ch->sock = 0;
                        appl_can_set_state(chidx, ST_CHANCAN_ERR, ERR_CAN_INIT_FAIL);
                    }else{
                        addr.can_ifindex = ifr[chidx].ifr_ifindex;
                        setsockopt(ch->sock, SOL_CAN_RAW, CAN_RAW_RECV_OWN_MSGS, &recv_own_msgs, sizeof(recv_own_msgs));
                        if (bind(ch->sock, (struct sockaddr *)&addr, sizeof(addr)) < 0){
                            close(ch->sock);
                            ch->sock = 0;
                            appl_can_set_state(chidx, ST_CHANCAN_ERR, ERR_CAN_INIT_FAIL);
                        }else{
                            if (pthread_create(&thrd_rx, NULL, thrd_can2_rx, NULL) != 0){
                                close(ch->sock);
                                ch->sock = 0;
                                appl_can_set_state(chidx, ST_CHANCAN_ERR, ERR_CAN_INIT_FAIL);
                            }else{
                                appl_can_set_state(chidx, ST_CHANCAN_RUN,ERR_CAN_NONE);
                            }
                        }
                    }
                }
            break;

            case ST_CHANCAN_RUN:
                usleep(200000);
                frame.can_id = 0x04904000;
                frame.can_id |= CAN_EFF_FLAG;
                frame.can_dlc = 8;
                frame.data[1] = 0;
                frame.data[2] = 0;
                frame.data[3] = 0;
                frame.data[5] = 0;
                frame.data[6] = 0;
                frame.data[7] = 0;
                if(ac->CtlMode == AC_CTLMOD_NON_EMS){
                    frame.data[0] = ac->SetMode;
                    frame.data[4] = ac->SetTemp + 40;
                    write(ch->sock, (char*)&frame, sizeof(frame));
                    ch->WrCnt += 1;
                }else if(ac->CtlMode == AC_CTLMOD_EMS ){
                    if(bms->CommState == ST_COMM_NORM){
                        switch(*step){
                            case 0:
                                if(bms->AvgCellT < set->HeatTemp ){
                                    frame.data[0] = 2; // Heat
                                    frame.data[4] = set->HeatTempSet + 40;
                                    *step = 1;
                                }else if(bms->AvgCellT > set->CoolTemp){
                                    frame.data[0] = 1; // cool
                                    frame.data[4] = set->CoolTempSet + 40;
                                    *step = 2;
                                }else{
                                    frame.data[0] = 3; // circulate
                                }
                                write(ch->sock, (char*)&frame, sizeof(frame));
                                ch->WrCnt += 1;
                            break;

                            case 1: // heat
                                if(bms->AvgCellT > set->HeatTemp + set->HeatGap){
                                    frame.data[0] = 3;
                                    *step = 0;
                                }
                                write(ch->sock, (char*)&frame, sizeof(frame));
                                ch->WrCnt += 1;
                            break;

                            case 2: // cool
                                if(bms->AvgCellT < set->CoolTemp - set->CoolGap){
                                    frame.data[0] = 3;
                                    *step = 0;
                                }
                                write(ch->sock, (char*)&frame, sizeof(frame));
                                ch->WrCnt += 1;
                            break;

                            default:
                            // NEVER REACH HERE
                            break;
                        }
                    }
                }
            break;

            case ST_CHANCAN_ERR:
                usleep(100000);
            break;

            default:
                // NEVER REACH HERE
                usleep(100000);
            break;
        }
        ch->Loopcnt += 1;
        ch->LoopTime = mg_millis() - startts;
    }

    MG_INFO(("%s EXIT", __func__));
}

static void* thrd_dido(void* param)
{
    struct stat file_info;
    char buf[128] = {0};
    int rdsize;
    int exported;
    int fd_value;
    int fd_export;
    int fd_direction;	
    struct Dido_t* dido = &APPL.Dido;
    struct Ctl_t* ctl = &APPL.Ctl;

    syslog(LOG_INFO, "%s ENTER",__func__);
    appl_dido_set_state(ST_DIDO_INIT, ERR_DIDO_NONE);
    while(1){
        usleep(1000000); // 1s
        switch (dido->State)
        {
        case ST_DIDO_INIT:
            // *************************************
            // set gpio488 1#水浸
            // *************************************
            exported = 0;
            sprintf(buf, "/sys/class/gpio/gpio488");
            if (stat(buf, &file_info) != -1){
                printf("gpio480 already exported\n");
                exported = 1;
            }else{
                fd_export = open(EXPORT_PATH, O_WRONLY);
                if (fd_export == -1){
                    printf("open gpio488 fail when export\n");
                    appl_dido_set_state(ST_DIDO_ERR, ERR_DIDO_INIT_FAIL);
                    continue;
                }else{
                    if (write(fd_export, "488", 3) == -1){
                        printf("write path %s fail:%s\n", buf, strerror(errno));
                        close(fd_export);
                        appl_dido_set_state(ST_DIDO_ERR, ERR_DIDO_INIT_FAIL);
                        continue;
                    }else{
                        close(fd_export);
                        exported = 1;
                    }
                }
            }
            if(exported == 1){
                // 设置gpio方向
                sprintf(buf, "/sys/class/gpio/gpio488/direction");
                fd_direction = open(buf, O_RDWR);
                if (fd_direction == -1){
                    printf("open direction %s fail: %s\n", buf, strerror(errno));
                    appl_dido_set_state(ST_DIDO_ERR, ERR_DIDO_INIT_FAIL);
                    continue;
                }else{
                    sprintf(buf, "%s", DIR_IN);
                    if (write(fd_direction, buf, strlen(buf)) == -1){
                        printf("write direction %s fail:%s\n", buf, strerror(errno));
                        close(fd_direction);
                        appl_dido_set_state(ST_DIDO_ERR, ERR_DIDO_INIT_FAIL);
                        continue;
                    }else{
                        close(fd_direction);
                        printf("gpio480 ok\n");
                    }
                }
            }

            // ***********************************************
            // set gpio489 2#水浸
            // ***********************************************
            exported = 0;
            sprintf(buf, "/sys/class/gpio/gpio489");
            if (stat(buf, &file_info) != -1){
                printf("stat : gpio489 path exported: %s\n", buf);
                exported = 1;
            }else{
                fd_export = open(EXPORT_PATH, O_WRONLY);
                if (fd_export == -1){
                    printf("open path %s failed : %s\n", EXPORT_PATH, strerror(errno));
                    appl_dido_set_state(ST_DIDO_ERR, ERR_DIDO_INIT_FAIL);
                    continue;
                }else{
                    if (write(fd_export, "489", 3) == -1){
                        printf("write path fail:%s\n", strerror(errno));
                        close(fd_export);
                        appl_dido_set_state(ST_DIDO_ERR, ERR_DIDO_INIT_FAIL);
                        continue;
                    }else{
                        close(fd_export);
                        exported = 1;
                    }
                }
            }
            if(exported == 1){
                // 设置gpio489方向
                sprintf(buf, "/sys/class/gpio/gpio489/direction");
                fd_direction = open(buf, O_RDWR);
                if (fd_direction == -1){
                    printf("open direction fail: %s\n", strerror(errno));
                    appl_dido_set_state(ST_DIDO_ERR, ERR_DIDO_INIT_FAIL);
                    continue;
                }else{
                    sprintf(buf, "%s", DIR_IN);
                    if (write(fd_direction, buf, strlen(buf)) == -1){
                        printf("write direction fail:%s\n", strerror(errno));
                        close(fd_direction);
                        appl_dido_set_state(ST_DIDO_ERR, ERR_DIDO_INIT_FAIL);
                        continue;
                    }else{
                        close(fd_direction);
                    }
                }
            }

            // ***************************************
            // set gpio491  及安盾消防触发反馈
            // ***************************************
            exported = 0;
            sprintf(buf, "/sys/class/gpio/gpio491");
            if (stat(buf, &file_info) != -1){
                printf("stat : gpio491 path exported: %s\n", buf);
                exported = 1;
            }else{
                fd_export = open(EXPORT_PATH, O_WRONLY);
                if (fd_export == -1){
                    printf("open path %s failed : %s\n", EXPORT_PATH, strerror(errno));
                    appl_dido_set_state(ST_DIDO_ERR, ERR_DIDO_INIT_FAIL);
                    continue;
                }else{
                    if (write(fd_export, "491", 3) == -1){
                        printf("write path %s fail\n", strerror(errno));
                        close(fd_export);
                        appl_dido_set_state(ST_DIDO_ERR, ERR_DIDO_INIT_FAIL);
                        continue;
                    }else{
                        close(fd_export);
                        exported = 1;
                    }
                }
            }
            if(exported == 1){
                // 设置gpio方向
                sprintf(buf, "/sys/class/gpio/gpio491/direction");
                fd_direction = open(buf, O_RDWR);
                if (fd_direction == -1){
                    printf("open direction fail: %s\n", strerror(errno));
                    appl_dido_set_state(ST_DIDO_ERR, ERR_DIDO_INIT_FAIL);
                    continue;
                }else{
                    sprintf(buf, "%s", DIR_IN);
                    if (write(fd_direction, buf, strlen(buf)) == -1){
                        printf("write direction %s fail\n", strerror(errno));
                        close(fd_direction);
                        appl_dido_set_state(ST_DIDO_ERR, ERR_DIDO_INIT_FAIL);
                        continue;
                    }else{
                        close(fd_direction);
                    }
                }
            }

            // ********************************************
            // set gpio134 前门磁
            // ********************************************
            exported = 0;
            sprintf(buf, "/sys/class/gpio/gpio134");
            if (stat(buf, &file_info) != -1){
                printf("gpio134 already exported\n");
                exported = 1;
            }else{
                fd_export = open(EXPORT_PATH, O_WRONLY);
                if (fd_export == -1){
                    printf("open path %s failed : %s\n", EXPORT_PATH, strerror(errno));
                    appl_dido_set_state(ST_DIDO_ERR, ERR_DIDO_INIT_FAIL);
                    continue;
                }
                if (write(fd_export, "134", 3) == -1){
                    printf("write path %s fail\n", strerror(errno));
                    close(fd_export);
                    appl_dido_set_state(ST_DIDO_ERR, ERR_DIDO_INIT_FAIL);
                    continue;
                }else{
                    close(fd_export);
                    exported = 1;
                }
            }
            if(exported == 1){
                // 设置gpio方向
                sprintf(buf, "/sys/class/gpio/gpio134/direction");
                fd_direction = open(buf, O_RDWR);
                if (fd_direction == -1){
                    printf("open direction fail: %s\n", strerror(errno));
                    appl_dido_set_state(ST_DIDO_ERR, ERR_DIDO_INIT_FAIL);
                    continue;
                }else{
                    sprintf(buf, "%s", DIR_IN);
                    if (write(fd_direction, buf, strlen(buf)) == -1){
                        printf("write direction %s fail\n", strerror(errno));
                        close(fd_direction);
                        appl_dido_set_state(ST_DIDO_ERR, ERR_DIDO_INIT_FAIL);
                        continue;
                    }else{
                        close(fd_direction);
                    }
                }
            }

            // ***************************************
            // set gpio135 后门磁
            // ***************************************
            exported = 0;
            sprintf(buf, "/sys/class/gpio/gpio135");
            if (stat(buf, &file_info) != -1){
                printf("stat : gpio135 path exported: %s\n", buf);
                exported = 1;
            }else{
                fd_export = open(EXPORT_PATH, O_WRONLY);
                if (fd_export == -1){
                    printf("open path %s failed : %s\n", EXPORT_PATH, strerror(errno));
                    appl_dido_set_state(ST_DIDO_ERR, ERR_DIDO_INIT_FAIL);
                    continue;
                }else{
                    if (write(fd_export, "135", 3) == -1){
                        printf("write path %s fail\n", strerror(errno));
                        close(fd_export);
                        appl_dido_set_state(ST_DIDO_ERR, ERR_DIDO_INIT_FAIL);
                        continue;
                    }else{
                        close(fd_export);
                        exported = 1;
                    }
                }
            }
            if(exported == 1){
                // 设置gpio方向
                sprintf(buf, "/sys/class/gpio/gpio135/direction");
                fd_direction = open(buf, O_RDWR);
                if (fd_direction == -1){
                    printf("open direction fail: %s\n", strerror(errno));
                    appl_dido_set_state(ST_DIDO_ERR, ERR_DIDO_INIT_FAIL);
                    continue;
                }else{
                    sprintf(buf, "%s", DIR_IN);
                    if (write(fd_direction, buf, strlen(buf)) == -1){
                        printf("write direction %s fail\n", strerror(errno));
                        close(fd_direction);
                        appl_dido_set_state(ST_DIDO_ERR, ERR_DIDO_INIT_FAIL);
                        continue;
                    }else{
                        close(fd_direction);
                    }
                }
            }

            // **************************************************
            // set gpio137 急停
            // **************************************************
            exported = 0;
            sprintf(buf, "/sys/class/gpio/gpio137");
            if (stat(buf, &file_info) != -1){
                printf("stat : gpio137 path exported: %s\n", buf);
                exported = 1;
            }else{
                fd_export = open(EXPORT_PATH, O_WRONLY);
                if (fd_export == -1){
                    printf("open path %s failed : %s\n", EXPORT_PATH, strerror(errno));
                    appl_dido_set_state(ST_DIDO_ERR, ERR_DIDO_INIT_FAIL);
                    continue;
                }else{
                    if (write(fd_export, "137", 3) == -1){
                        printf("write path %s fail\n", strerror(errno));
                        close(fd_export);
                        appl_dido_set_state(ST_DIDO_ERR, ERR_DIDO_INIT_FAIL);
                        continue;
                    }else{
                        close(fd_export);
                        exported = 1;
                    }
                }
            }
            if(exported == 1){
                // 设置gpio方向
                sprintf(buf, "/sys/class/gpio/gpio137/direction");
                fd_direction = open(buf, O_RDWR);
                if (fd_direction == -1){
                    printf("open direction fail: %s\n", strerror(errno));
                    appl_dido_set_state(ST_DIDO_ERR, ERR_DIDO_INIT_FAIL);
                    continue;
                }else{
                    sprintf(buf, "%s", DIR_IN);
                    if (write(fd_direction, buf, strlen(buf)) == -1){
                        printf("write direction %s fail\n", strerror(errno));
                        close(fd_direction);
                        appl_dido_set_state(ST_DIDO_ERR, ERR_DIDO_INIT_FAIL);
                        continue;
                    }else{
                        close(fd_direction);
                    }   
                }
            }

            // ************************************************
            // set gpio480 红灯
            // ************************************************
            exported = 0;
            sprintf(buf, "/sys/class/gpio/gpio480");
            if (stat(buf, &file_info) != -1){
                printf("gpio480 already exported\n");
                exported = 1;
            }else{
                fd_export = open(EXPORT_PATH, O_WRONLY);
                if (fd_export == -1){
                    printf("open path %s failed : %s\n", EXPORT_PATH, strerror(errno));
                    appl_dido_set_state(ST_DIDO_ERR, ERR_DIDO_INIT_FAIL);
                    continue;
                }else{
                    if (write(fd_export, "480", 3) == -1){
                        printf("write path %s fail\n", strerror(errno));
                        close(fd_export);
                        appl_dido_set_state(ST_DIDO_ERR, ERR_DIDO_INIT_FAIL);
                        continue;
                    }else{
                        close(fd_export);
                        exported = 1;
                    }
                }
            }
            if(exported == 1){
                // 设置gpio方向
                sprintf(buf, "/sys/class/gpio/gpio480/direction");
                fd_direction = open(buf, O_RDWR);
                if (fd_direction == -1){
                    printf("open direction fail: %s\n", strerror(errno));
                    appl_dido_set_state(ST_DIDO_ERR, ERR_DIDO_INIT_FAIL);
                    continue;
                }else{
                    sprintf(buf, "%s", DIR_OUT);
                    if (write(fd_direction, buf, strlen(buf)) == -1){
                        printf("write direction %s fail\n", strerror(errno));
                        close(fd_direction);
                        appl_dido_set_state(ST_DIDO_ERR, ERR_DIDO_INIT_FAIL);
                        continue;
                    }else{
                        printf("gpio480 set ok");
                        close(fd_direction);
                        appl_dido_set_state(ST_DIDO_RUN, ERR_DIDO_NONE);
                    }
                }
            }

            // ************************************************
            // set gpio481 绿灯
            // ************************************************
            exported = 0;
            sprintf(buf, "/sys/class/gpio/gpio481");
            if (stat(buf, &file_info) != -1){
                printf("stat : gpio481 path exported: %s\n", buf);
                exported = 1;
            }else{
                printf("%s, exporting gpio481\n", __func__);
                fd_export = open(EXPORT_PATH, O_WRONLY);
                if (fd_export == -1){
                    printf("open path %s failed : %s\n", EXPORT_PATH, strerror(errno));
                    appl_dido_set_state(ST_DIDO_ERR, ERR_DIDO_INIT_FAIL);
                    continue;
                }else{
                    if (write(fd_export, "481", 3) == -1){
                        printf("write path %s fail\n", strerror(errno));
                        close(fd_export);
                        appl_dido_set_state(ST_DIDO_ERR, ERR_DIDO_INIT_FAIL);
                        continue;
                    }else{
                        close(fd_export);
                        exported = 1;
                    }
                }
            }
            if(exported == 1){
                // 设置gpio方向
                sprintf(buf, "/sys/class/gpio/gpio481/direction");
                fd_direction = open(buf, O_RDWR);
                if (fd_direction == -1){
                    printf("open direction fail: %s\n", strerror(errno));
                    appl_dido_set_state(ST_DIDO_ERR, ERR_DIDO_INIT_FAIL);
                    continue;
                }else{
                    sprintf(buf, "%s", DIR_OUT);
                    if (write(fd_direction, buf, strlen(buf)) == -1){
                        printf("write direction %s fail\n", strerror(errno));
                        close(fd_direction);
                        appl_dido_set_state(ST_DIDO_ERR, ERR_DIDO_INIT_FAIL);
                        continue;
                    }else{
                        close(fd_direction);
                    }
                }
            }

            // ************************************************
            // set gpio482 蓝灯
            // ************************************************
            exported = 0;
            sprintf(buf, "/sys/class/gpio/gpio482");
            if (stat(buf, &file_info) != -1){
                printf("stat : gpio482 path exported: %s\n", buf);
                exported = 1;
            }else{
                fd_export = open(EXPORT_PATH, O_WRONLY);
                if (fd_export == -1){
                    printf("open path %s failed : %s\n", EXPORT_PATH, strerror(errno));
                    appl_dido_set_state(ST_DIDO_ERR, ERR_DIDO_INIT_FAIL);
                    continue;
                }else{
                    if (write(fd_export, "482", 3) == -1){
                        printf("write path %s fail\n", strerror(errno));
                        close(fd_export);
                        appl_dido_set_state(ST_DIDO_ERR, ERR_DIDO_INIT_FAIL);
                        continue;
                    }else{
                        close(fd_export);
                        exported = 1;
                    }
                }
            }
            if(exported == 1){
                // 设置gpio方向
                sprintf(buf, "/sys/class/gpio/gpio482/direction");
                fd_direction = open(buf, O_RDWR);
                if (fd_direction == -1){
                    printf("open direction fail: %s\n", strerror(errno));
                    appl_dido_set_state(ST_DIDO_ERR, ERR_DIDO_INIT_FAIL);
                    continue;
                }else{
                    sprintf(buf, "%s", DIR_OUT);
                    if (write(fd_direction, buf, strlen(buf)) == -1){
                        printf("write direction %s fail\n", strerror(errno));
                        close(fd_direction);
                        appl_dido_set_state(ST_DIDO_ERR, ERR_DIDO_INIT_FAIL);
                        continue;
                    }else{
                        close(fd_direction);	
                    }
                }
            }

            // *************************************
            // set gpio483 蜂鸣器
            // *************************************
            // exported = 0;
            // sprintf(buf, "/sys/class/gpio/gpio483");
            // if (stat(buf, &file_info) != -1){
            //     printf("stat : gpio483 path exported: %s\n", buf);
            //     exported = 1;
            // }else{
            //     fd_export = open(EXPORT_PATH, O_WRONLY);
            //     if (fd_export == -1){
            //         printf("open path %s failed : %s\n", EXPORT_PATH, strerror(errno));
            //         appl_dido_set_state(ST_DIDO_ERR, ERR_DIDO_INIT_FAIL);
            //         continue;
            //     }else{
            //         if (write(fd_export, "483", 3) == -1){
            //             printf("write path %s fail\n", strerror(errno));
            //             close(fd_export);
            //             appl_dido_set_state(ST_DIDO_ERR, ERR_DIDO_INIT_FAIL);
            //             continue;
            //         }else{
            //             close(fd_export);
            //             exported = 1;
            //         }
            //     }
            // }
            // if(exported == 1){
            //     // 设置gpio方向
            //     sprintf(buf, "/sys/class/gpio/gpio483/direction");
            //     fd_direction = open(buf, O_RDWR);
            //     if (fd_direction == -1){
            //         printf("open direction fail: %s\n", strerror(errno));
            //         appl_dido_set_state(ST_DIDO_ERR, ERR_DIDO_INIT_FAIL);
            //         continue;
            //     }else{
            //         sprintf(buf, "%s", DIR_OUT);
            //         if (write(fd_direction, buf, strlen(buf)) == -1){
            //             printf("write direction %s fail\n", strerror(errno));
            //             close(fd_direction);
            //             appl_dido_set_state(ST_DIDO_ERR, ERR_DIDO_INIT_FAIL);
            //             continue;
            //         }else{
            //             close(fd_direction);
            //             appl_dido_set_state(ST_DIDO_RUN, ERR_DIDO_NONE);
            //             MG_INFO(("%s Goto Run State",__func__));
            //         }
            //     }
            // }        
            break;
        
        case ST_DIDO_RUN:
            if(dido->Cmd == CMD_DIDO_SET_LEDMOD){
                dido->Cmd = CMD_DIDO_DONE;
                appl_dido_set_led(dido->CmdParam);
            }
            if( ctl->State == CTL_ST_RUN ){
                if( ctl->Ap > 0){ // CHG
                    appl_dido_set_led(LEDMODE_BLUE);
                }else if( ctl->Ap < 0 ){ // DHG
                    appl_dido_set_led(LEDMODE_GREEN);
                }else{
                    appl_dido_set_led(LEDMODE_WHITE); 
                }
            }else if( ctl->State == CTL_ST_ERR ){
                appl_dido_set_led(LEDMODE_RED);
            }else if( ctl->State == CTL_ST_STOP ){
                appl_dido_set_led(LEDMODE_OFF);
            }

            // read 488 1#水浸
            sprintf(buf, "/sys/class/gpio/gpio488/value");
            fd_value = open(buf, O_RDONLY);
            if (fd_value == -1){
                MG_INFO(("%s 488 open fail, Goto Err State",__func__));
                appl_dido_set_state(ST_DIDO_ERR,ERR_DIDO_READ_FAIL);
                continue;
            }else{
                memset(buf, 0, sizeof(buf));
                lseek(fd_value, 0, SEEK_SET);
                rdsize = read(fd_value, buf, sizeof(buf));
                if (rdsize < 0){
                    MG_INFO(("%s 488 read fail, Goto Err State",__func__));
                    appl_dido_set_state(ST_DIDO_ERR,ERR_DIDO_READ_FAIL);
                    continue;
                }else{
                    dido->WaterDec1 = 1 - atoi(buf);
                    if(dido->WaterDec1 == 1){
                        strcpy(dido->szWaterDec1,"有水");
                    }else if(dido->WaterDec1 == 0){
                        strcpy(dido->szWaterDec1,"无水");
                    }else{
                        strcpy(dido->szWaterDec1,"未知");
                        MG_INFO(("%s 488 unknown value, Goto Err State",__func__));
                        appl_dido_set_state(ST_DIDO_ERR,ERR_DIDO_READ_FAIL);
                        continue;
                    }
                }
                close(fd_value);
            }

            // read 489 2#水浸
            sprintf(buf, "/sys/class/gpio/gpio489/value");
            fd_value = open(buf, O_RDONLY);
            if (fd_value == -1){
                syslog(LOG_INFO,"%s 489 open fail, Goto Err State",__func__);
                appl_dido_set_state(ST_DIDO_ERR,ERR_DIDO_READ_FAIL);
                continue;
            }else{
                memset(buf, 0, sizeof(buf));
                lseek(fd_value, 0, SEEK_SET);
                rdsize = read(fd_value, buf, sizeof(buf));
                if (rdsize < 0){
                    syslog(LOG_INFO,"%s 489 read fail, Goto Err State",__func__);
                    appl_dido_set_state(ST_DIDO_ERR,ERR_DIDO_READ_FAIL);
                    continue;
                }else{
                    dido->WaterDec2 = 1 - atoi(buf);
                    if(dido->WaterDec2 == 1){
                        strcpy(dido->szWaterDec2,"有水");
                    }else if(dido->WaterDec2 == 0){
                        strcpy(dido->szWaterDec2,"无水");
                    }else{
                        strcpy(dido->szWaterDec2,"未知");
                        syslog(LOG_INFO,"%s 489 unknown value, Goto Err State",__func__);
                        appl_dido_set_state(ST_DIDO_ERR,ERR_DIDO_READ_FAIL);
                        continue;
                    }
                }
                close(fd_value);
            }

            // read 491, 及安盾消防触发反馈
            sprintf(buf, "/sys/class/gpio/gpio491/value");
            fd_value = open(buf, O_RDONLY);
            if (fd_value == -1){
                syslog(LOG_INFO,"%s 491 open fail, Goto Err State",__func__);
            	appl_dido_set_state(ST_DIDO_ERR,ERR_DIDO_READ_FAIL);
                continue;
            }else{
            	memset(buf, 0, sizeof(buf));
                lseek(fd_value, 0, SEEK_SET);
                rdsize = read(fd_value, buf, sizeof(buf));
                if (rdsize < 0){
                    syslog(LOG_INFO,"%s 491 read fail, Goto Err State",__func__);
                    appl_dido_set_state(ST_DIDO_ERR,ERR_DIDO_READ_FAIL);
                    continue;
                }else{
                	dido->FeEruptFb = atoi(buf);
                    if(dido->FeEruptFb == 1){
                        strcpy(dido->szFeEruptFb,"触发");
                    }else if(dido->FeEruptFb == 0){
                        strcpy(dido->szFeEruptFb,"未触发");
                    }else{
                        strcpy(dido->szFeEruptFb,"未知");
                        syslog(LOG_INFO,"%s 491 unknown value, Goto Err State",__func__);
                        appl_dido_set_state(ST_DIDO_ERR,ERR_DIDO_READ_FAIL);
                        continue;
                    }
            	}
                close(fd_value);
            }

            // read 134 前门磁
            sprintf(buf, "/sys/class/gpio/gpio134/value");
            fd_value = open(buf, O_RDONLY);
            if (fd_value == -1){
                MG_INFO(("%s 134 open fail, Goto Err State",__func__));
                appl_dido_set_state(ST_DIDO_ERR,ERR_DIDO_READ_FAIL);
                continue;
            }else{
                memset(buf, 0, sizeof(buf));
                lseek(fd_value, 0, SEEK_SET);
                rdsize = read(fd_value, buf, sizeof(buf));
                if (rdsize < 0){
                    MG_INFO(("%s 134 read fail, Goto Err State",__func__));
                    appl_dido_set_state(ST_DIDO_ERR,ERR_DIDO_READ_FAIL);
                    continue;
                }else{
                    dido->FrontDoor = atoi(buf);
                    if(dido->FrontDoor == 0){
                        strcpy(dido->szFrontDoor,"关");
                    }else if(dido->FrontDoor == 1){
                        strcpy(dido->szFrontDoor,"开");
                    }else{
                        strcpy(dido->szFrontDoor,"未知");
                        MG_INFO(("%s 134 unknown value, Goto Err State",__func__));
                        appl_dido_set_state(ST_DIDO_ERR,ERR_DIDO_READ_FAIL);
                        continue;
                    }
                }
                close(fd_value);
            }	

            // read 135 后门磁
            sprintf(buf, "/sys/class/gpio/gpio135/value");
            fd_value = open(buf, O_RDONLY);
            if (fd_value == -1){
                MG_INFO(("%s 135 open fail, Goto Err State",__func__));
                appl_dido_set_state(ST_DIDO_ERR,ERR_DIDO_READ_FAIL);
                continue;
            }else{
                memset(buf, 0, sizeof(buf));
                lseek(fd_value, 0, SEEK_SET);
                rdsize = read(fd_value, buf, sizeof(buf));
                if (rdsize < 0){
                    MG_INFO(("%s 113534 read fail, Goto Err State",__func__));
                    appl_dido_set_state(ST_DIDO_ERR,ERR_DIDO_READ_FAIL);
                    continue;
                }else{
                    dido->BackDoor = atoi(buf);
                    if(dido->BackDoor == 0){
                        strcpy(dido->szBackDoor,"关");
                    }else if(dido->BackDoor == 1){
                        strcpy(dido->szBackDoor,"开");
                    }else{
                        strcpy(dido->szBackDoor,"未知");
                        MG_INFO(("%s 135 invalid value, Goto Err State",__func__));
                        appl_dido_set_state(ST_DIDO_ERR,ERR_DIDO_READ_FAIL);
                        continue;
                    }
                }
                close(fd_value);
            }			

            // read 137 急停
            sprintf(buf, "/sys/class/gpio/gpio137/value");
            fd_value = open(buf, O_RDONLY);
            if (fd_value == -1){
                appl_dido_set_state(ST_DIDO_ERR,ERR_DIDO_READ_FAIL);
                continue;
            }else{
                memset(buf, 0, sizeof(buf));
                lseek(fd_value, 0, SEEK_SET);
                rdsize = read(fd_value, buf, sizeof(buf));
                if (rdsize < 0){
                    appl_dido_set_state(ST_DIDO_ERR,ERR_DIDO_READ_FAIL);
                    continue;
                }else{
                    dido->EmgStop = atoi(buf);
                    if(dido->EmgStop == 0){
                        strcpy(dido->szEmgStop,"按下");
                    }else if(dido->EmgStop == 1){
                        strcpy(dido->szEmgStop,"未按下");
                    }else{
                        strcpy(dido->szEmgStop,"未知");
                        appl_dido_set_state(ST_DIDO_ERR,ERR_DIDO_READ_FAIL);
                        continue;
                    }
                }
                close(fd_value);
            }

            dido->LastUpdate = mg_millis();
            strcpy(dido->szLastUpdate, appl_get_datetime_long());	
            break;

        case ST_DIDO_ERR:
            break;

        default:
            break;
        }        
    }
}

void appl_chan485_lock(int idx)
{
    //pthread_mutex_lock(&APPL.chan485[idx].mutex);
}

void appl_chan485_unlock(int idx)
{
    //pthread_mutex_unlock(&APPL.chan485[idx].mutex);
}

void appl_485_set_state(int idx, int s, int e)
{
    struct chan485_t* c = &APPL.chan485[idx];
    c->state = s;
    switch( c->state){
        case ST_485_INIT:
            strcpy(c->szstate,"初始化");
        break;

        case ST_485_RUN:
            strcpy(c->szstate,"运行");
        break;

        case ST_485_ERR:
            strcpy(c->szstate,"故障");
            break;

        default:
            strcpy(c->szstate,"未知");
        break;
    }

    c->err = e;
    switch (e)
    {
    case ERR_485_NONE:
        strcpy(c->szerr,"无");
        break;
    case ERR_485_INIT_FAIL:
        strcpy(c->szerr,"初始化失败");
        break;
    
    default:
        strcpy(c->szerr,"未知");
        break;
    }
}

int appl_chan485_get_state(int idx)
{
    return APPL.chan485[idx].state;
}

void appl_can_set_state(int idx, int s, int e)
{
    struct chancan_t* c = &APPL.chancan[idx];
    c->State = s;
    switch( c->State){
        case ST_CAN_INIT:
            strcpy(c->szState,"初始化");
        break;

        case ST_CHANCAN_RUN:
            strcpy(c->szState,"运行");
        break;

        case ST_CHANCAN_ERR:
            strcpy(c->szState,"故障");
        break;        

        default:
            strcpy(c->szState,"未知");
        break;
    }

    c->Err = e;
    switch(e){
        case ERR_CAN_NONE:
        strcpy(c->szErr,"无");
        break;

        case ERR_CAN_INIT_FAIL:
        strcpy(c->szErr,"初始化失败");
        break;

        default:
        break;
    }
}

// 声光报警
void appl_dido_set_alarm( int en)
{
    struct Dido_t* dido = &APPL.Dido;
    char buf[128] = {0};
	int fd_value;
	int ret = -1;

    // write 482
    sprintf(buf, "/sys/class/gpio/gpio482/value");
    fd_value = open(buf, O_RDWR);
    if (fd_value == -1){
        ret = -1;
    }else{
        memset(buf, 0, sizeof(buf));
        lseek(fd_value, 0, SEEK_SET);
        sprintf(buf,"%d",en); // 0 : 不叫  1：叫
        write(fd_value, buf, strlen(buf));
        close(fd_value);
        ret = 0;
    }
    syslog(LOG_INFO,"%s, En:%d, ret:%d", __func__, en, ret);
}

// 消防喷发确认
void appl_dido_set_fe_confirm( int en )
{
    struct Dido_t* dido = &APPL.Dido;
    char buf[128] = {0};
	int fd_value;
	int ret = -1;

    // write 480
    sprintf(buf, "/sys/class/gpio/gpio480/value");
    fd_value = open(buf, O_RDWR);
    if (fd_value == -1){
        ret = -1;
    }else{
        memset(buf, 0, sizeof(buf));
        lseek(fd_value, 0, SEEK_SET);
        sprintf(buf,"%d",en); 
        write(fd_value, buf, strlen(buf));
        close(fd_value);
        ret = 0;
    }
    syslog(LOG_INFO,"%s, En:%d, ret:%d", __func__, en, ret);
}

void appl_dido_set_led(int m)
{
    struct Dido_t* dido = &APPL.Dido;
    char buf[128] = {0};
	int fd_value;
	int rc;
    
    dido->LedMode = m;
    switch (m)
    {
    case LEDMODE_OFF:
            strcpy(dido->szLedMode,"灭");
			// write 480 Red
	        sprintf(buf, "/sys/class/gpio/gpio480/value");
		    fd_value = open(buf, O_RDWR);
		    if (fd_value == -1){
				appl_dido_set_state(ST_DIDO_ERR, ERR_DIDO_WRITE_FAIL);
		    }else{
			    memset(buf, 0, sizeof(buf));
			    lseek(fd_value, 0, SEEK_SET);
				sprintf(buf,"%d",1); // !!!
			    rc = write(fd_value, buf, strlen(buf));
			    close(fd_value);
			}
			// write 481 Green
	        sprintf(buf, "/sys/class/gpio/gpio481/value");
		    fd_value = open(buf, O_RDWR);
		    if (fd_value == -1){
				appl_dido_set_state(ST_DIDO_ERR, ERR_DIDO_WRITE_FAIL);
		    }else{
			    memset(buf, 0, sizeof(buf));
			    lseek(fd_value, 0, SEEK_SET);
				sprintf(buf,"%d",1); // !!!
			    write(fd_value, buf, strlen(buf));
			    close(fd_value);
			}
			// write 482 Blue Blue
	        sprintf(buf, "/sys/class/gpio/gpio482/value");
		    fd_value = open(buf, O_RDWR);
		    if (fd_value == -1){
				;
		    }else{
			    memset(buf, 0, sizeof(buf));
			    lseek(fd_value, 0, SEEK_SET);
				sprintf(buf,"%d",1); // !!!
			    write(fd_value, buf, strlen(buf));
			    close(fd_value);
			}
        break;

    case LEDMODE_WHITE:
            strcpy(dido->szLedMode,"白");
			// write 480 Red
	        sprintf(buf, "/sys/class/gpio/gpio480/value");
		    fd_value = open(buf, O_RDWR);
		    if (fd_value == -1){
                printf("fd_value -1\n");
				appl_dido_set_state(ST_DIDO_ERR, ERR_DIDO_WRITE_FAIL);
		    }else{
			    memset(buf, 0, sizeof(buf));
			    lseek(fd_value, 0, SEEK_SET);
				sprintf(buf,"%d",0); // !!!
			    rc = write(fd_value, buf, strlen(buf));
			    close(fd_value);
			}
			// write 481 Green
	        sprintf(buf, "/sys/class/gpio/gpio481/value");
		    fd_value = open(buf, O_RDWR);
		    if (fd_value == -1){
				appl_dido_set_state(ST_DIDO_ERR, ERR_DIDO_WRITE_FAIL);
		    }else{
			    memset(buf, 0, sizeof(buf));
			    lseek(fd_value, 0, SEEK_SET);
				sprintf(buf,"%d",0);
			    write(fd_value, buf, strlen(buf));
			    close(fd_value);
			}
			// write 482 Blue
	        sprintf(buf, "/sys/class/gpio/gpio482/value");
		    fd_value = open(buf, O_RDWR);
		    if (fd_value == -1){
				;
		    }else{
			    memset(buf, 0, sizeof(buf));
			    lseek(fd_value, 0, SEEK_SET);
				sprintf(buf,"%d",0);
			    write(fd_value, buf, strlen(buf));
			    close(fd_value);
			}
        break;        

    case LEDMODE_RED:
            strcpy(dido->szLedMode,"红");
			// write 480 Red
	        sprintf(buf, "/sys/class/gpio/gpio480/value");
		    fd_value = open(buf, O_RDWR);
		    if (fd_value == -1){
				appl_dido_set_state(ST_DIDO_ERR, ERR_DIDO_WRITE_FAIL);
		    }else{
			    memset(buf, 0, sizeof(buf));
			    lseek(fd_value, 0, SEEK_SET);
				sprintf(buf,"%d",0);
			    write(fd_value, buf, strlen(buf));
			    close(fd_value);
			}
			// write 481 Green
	        sprintf(buf, "/sys/class/gpio/gpio481/value");
		    fd_value = open(buf, O_RDWR);
		    if (fd_value == -1){
				appl_dido_set_state(ST_DIDO_ERR, ERR_DIDO_WRITE_FAIL);
		    }else{
			    memset(buf, 0, sizeof(buf));
			    lseek(fd_value, 0, SEEK_SET);
				sprintf(buf,"%d",1);
			    write(fd_value, buf, strlen(buf));
			    close(fd_value);
			}
			// write 482 Blue
	        sprintf(buf, "/sys/class/gpio/gpio482/value");
		    fd_value = open(buf, O_RDWR);
		    if (fd_value == -1){
				;
		    }else{
			    memset(buf, 0, sizeof(buf));
			    lseek(fd_value, 0, SEEK_SET);
				sprintf(buf,"%d",1);
			    write(fd_value, buf, strlen(buf));
			    close(fd_value);
			}
        break;             
    
    case LEDMODE_GREEN:
            strcpy(dido->szLedMode,"绿");
			// write 480 Red
	        sprintf(buf, "/sys/class/gpio/gpio480/value");
		    fd_value = open(buf, O_RDWR);
		    if (fd_value == -1){
				appl_dido_set_state(ST_DIDO_ERR, ERR_DIDO_WRITE_FAIL);
		    }else{
			    memset(buf, 0, sizeof(buf));
			    lseek(fd_value, 0, SEEK_SET);
				sprintf(buf,"%d",1); // !!
			    write(fd_value, buf, strlen(buf));
			    close(fd_value);
			}
			// write 481 Green
	        sprintf(buf, "/sys/class/gpio/gpio481/value");
		    fd_value = open(buf, O_RDWR);
		    if (fd_value == -1){
				appl_dido_set_state(ST_DIDO_ERR, ERR_DIDO_WRITE_FAIL);
		    }else{
			    memset(buf, 0, sizeof(buf));
			    lseek(fd_value, 0, SEEK_SET);
				sprintf(buf,"%d",0); // !!
			    write(fd_value, buf, strlen(buf));
			    close(fd_value);
			}
			// write 482 Blue
	        sprintf(buf, "/sys/class/gpio/gpio482/value");
		    fd_value = open(buf, O_RDWR);
		    if (fd_value == -1){
				;
		    }else{
			    memset(buf, 0, sizeof(buf));
			    lseek(fd_value, 0, SEEK_SET);
				sprintf(buf,"%d",1);
			    write(fd_value, buf, strlen(buf));
			    close(fd_value);
			}
        break;     

    case LEDMODE_BLUE:
            strcpy(dido->szLedMode,"蓝");
			// write 480 Red
	        sprintf(buf, "/sys/class/gpio/gpio480/value");
		    fd_value = open(buf, O_RDWR);
		    if (fd_value == -1){
				appl_dido_set_state(ST_DIDO_ERR, ERR_DIDO_WRITE_FAIL);
		    }else{
			    memset(buf, 0, sizeof(buf));
			    lseek(fd_value, 0, SEEK_SET);
				sprintf(buf,"%d",1);
			    write(fd_value, buf, strlen(buf));
			    close(fd_value);
			}
			// write 481 Green
	        sprintf(buf, "/sys/class/gpio/gpio481/value");
		    fd_value = open(buf, O_RDWR);
		    if (fd_value == -1){
				appl_dido_set_state(ST_DIDO_ERR, ERR_DIDO_WRITE_FAIL);
		    }else{
			    memset(buf, 0, sizeof(buf));
			    lseek(fd_value, 0, SEEK_SET);
				sprintf(buf,"%d",1);
			    write(fd_value, buf, strlen(buf));
			    close(fd_value);
			}
			// write 482 Blue
	        sprintf(buf, "/sys/class/gpio/gpio482/value");
		    fd_value = open(buf, O_RDWR);
		    if (fd_value == -1){
				;
		    }else{
			    memset(buf, 0, sizeof(buf));
			    lseek(fd_value, 0, SEEK_SET);
				sprintf(buf,"%d",0);
			    write(fd_value, buf, strlen(buf));
			    close(fd_value);
			}
        break;     

    default:
        break;
    }
}

int appl_can_get_state(int idx)
{
    return APPL.chancan[idx].State;
}

eMBErrorCode eMBRegInputCB(fmodbus_t *ctx, UCHAR *pucRegBuffer, USHORT usAddress, USHORT usNRegs)
{
    eMBErrorCode eStatus = MB_ENOERR;
    int iRegIndex;


    return MB_ENOREG;
}

eMBErrorCode eMBRegHoldingCB(fmodbus_t *ctx, UCHAR *pucRegBuffer, USHORT usAddress, USHORT usNRegs, eMBRegisterMode eMode)
{
    eMBErrorCode eStatus = MB_ENOERR;
    int iRegIndex;
    int i = 0;

    return eStatus;
}

eMBErrorCode eMBRegCoilsCB(fmodbus_t *ctx, UCHAR *pucRegBuffer, USHORT usAddress, USHORT usNCoils, eMBRegisterMode eMode)
{
    return MB_ENOREG;
}

eMBErrorCode eMBRegDiscreteCB(fmodbus_t *ctx, UCHAR *pucRegBuffer, USHORT usAddress, USHORT usNDiscrete)
{
    return MB_ENOREG;
}

static void appl_ctl_set_workmode(int m){
    struct Ctl_t* ctl = &APPL.Ctl;
    ctl->WorkMode = m;
    switch (m)
    {
    case CTL_WORKMODE_SLAVE:
        strcpy(ctl->szWorkMode,"从机");
        break;

    case CTL_WORKMODE_PCURV:
        strcpy(ctl->szWorkMode,"功率曲线");
        break;           
    
    default:
        break;
    }

    syslog(LOG_INFO, "%s, Ctl Workmode is Set To %s", __func__, ctl->szWorkMode);
}

void appl_485_set_485mode( void ) {
    int fd;
    struct serial_rs485 rs485conf = {0};
    int ret;
    
    // 1# 485
    fd = open("/dev/ttymxc1", O_RDWR | O_NOCTTY);
    if (fd <= 0) {
        syslog(LOG_INFO,"%s, Open ttymxc1 Fail",__func__);
    }else{
        /* get configure from device */
        ret = ioctl(fd, TIOCGRS485, &rs485conf);
        if(ret < 0) {
            // failed
            syslog(LOG_INFO,"%s, ioctl ttymxc1 Fail",__func__);
        }
        /* set enable rs485 mode in configure */
        /* Enable RS485 mode: */
        rs485conf.flags |= SER_RS485_ENABLED;
        /* Set logical level for RTS pin equal to 1 when sending: */
        rs485conf.flags |= SER_RS485_RTS_ON_SEND;
        /* Set logical level for RTS pin equal to 0 after sending: */
        rs485conf.flags &= ~SER_RS485_RTS_AFTER_SEND;
        /* Set this flag if you want to receive data even whilst sending data */
        rs485conf.flags &= ~SER_RS485_RX_DURING_TX;
        /* Set rts delay before send, if needed: */
        rs485conf.delay_rts_before_send = 0; // in miliseconds
        /* Set rts delay after send, if needed: */
        rs485conf.delay_rts_after_send = 0; // in miliseconds
        ret = ioctl(fd, TIOCSRS485, &rs485conf);
        if (ret < 0) {
            /* Error handling. See errno. */
            syslog(LOG_INFO,"%s, Set ttymxc1 485 Fail",__func__);
        }
        close(fd);
    }

    // 2# 485
    fd = open("/dev/ttymxc2", O_RDWR | O_NOCTTY);
    if (fd <= 0) {
        syslog(LOG_INFO,"%s, Open ttymxc2 Fail",__func__);
    }else{
        /* get configure from device */
        ret = ioctl(fd, TIOCGRS485, &rs485conf);
        if(ret < 0) {
            // failed
            syslog(LOG_INFO,"%s, ioctl ttymxc2 Fail",__func__);
        }
        /* set enable rs485 mode in configure */
        /* Enable RS485 mode: */
        rs485conf.flags |= SER_RS485_ENABLED;
        /* Set logical level for RTS pin equal to 1 when sending: */
        rs485conf.flags |= SER_RS485_RTS_ON_SEND;
        /* Set logical level for RTS pin equal to 0 after sending: */
        rs485conf.flags &= ~SER_RS485_RTS_AFTER_SEND;
        /* Set this flag if you want to receive data even whilst sending data */
        rs485conf.flags &= ~SER_RS485_RX_DURING_TX;
        /* Set rts delay before send, if needed: */
        rs485conf.delay_rts_before_send = 0; // in miliseconds
        /* Set rts delay after send, if needed: */
        rs485conf.delay_rts_after_send = 0; // in miliseconds
        ret = ioctl(fd, TIOCSRS485, &rs485conf);
        if (ret < 0) {
            /* Error handling. See errno. */
            syslog(LOG_INFO,"%s, Set ttymxc2 485 Fail",__func__);
        }
        close(fd);
    }    

    // 3# 485
    fd = open("/dev/ttymxc3", O_RDWR | O_NOCTTY);
    if (fd <= 0) {
        syslog(LOG_INFO,"%s, Open ttymxc3 Fail",__func__);
    }else{
        /* get configure from device */
        ret = ioctl(fd, TIOCGRS485, &rs485conf);
        if(ret < 0) {
            // failed
            syslog(LOG_INFO,"%s, ioctl ttymxc3 Fail",__func__);
        }
        /* set enable rs485 mode in configure */
        /* Enable RS485 mode: */
        rs485conf.flags |= SER_RS485_ENABLED;
        /* Set logical level for RTS pin equal to 1 when sending: */
        rs485conf.flags |= SER_RS485_RTS_ON_SEND;
        /* Set logical level for RTS pin equal to 0 after sending: */
        rs485conf.flags &= ~SER_RS485_RTS_AFTER_SEND;
        /* Set this flag if you want to receive data even whilst sending data */
        rs485conf.flags &= ~SER_RS485_RX_DURING_TX;
        /* Set rts delay before send, if needed: */
        rs485conf.delay_rts_before_send = 0; // in miliseconds
        /* Set rts delay after send, if needed: */
        rs485conf.delay_rts_after_send = 0; // in miliseconds
        ret = ioctl(fd, TIOCSRS485, &rs485conf);
        if (ret < 0) {
            /* Error handling. See errno. */
            syslog(LOG_INFO,"%s, Set ttymxc3 485 Fail",__func__);
        }
        close(fd);
    }    

    // 4# 485
    fd = open("/dev/ttymxc5", O_RDWR | O_NOCTTY);
    if (fd <= 0) {
        syslog(LOG_INFO,"%s, Open ttymxc5 Fail",__func__);
    }else{
        /* get configure from device */
        ret = ioctl(fd, TIOCGRS485, &rs485conf);
        if(ret < 0) {
            // failed
            syslog(LOG_INFO,"%s, ioctl ttymxc5 Fail",__func__);
        }
        /* set enable rs485 mode in configure */
        /* Enable RS485 mode: */
        rs485conf.flags |= SER_RS485_ENABLED;
        /* Set logical level for RTS pin equal to 1 when sending: */
        rs485conf.flags |= SER_RS485_RTS_ON_SEND;
        /* Set logical level for RTS pin equal to 0 after sending: */
        rs485conf.flags &= ~SER_RS485_RTS_AFTER_SEND;
        /* Set this flag if you want to receive data even whilst sending data */
        rs485conf.flags &= ~SER_RS485_RX_DURING_TX;
        /* Set rts delay before send, if needed: */
        rs485conf.delay_rts_before_send = 0; // in miliseconds
        /* Set rts delay after send, if needed: */
        rs485conf.delay_rts_after_send = 0; // in miliseconds
        ret = ioctl(fd, TIOCSRS485, &rs485conf);
        if (ret < 0) {
            /* Error handling. See errno. */
            syslog(LOG_INFO,"%s, Set ttymxc5 485 Fail",__func__);
        }
        close(fd);
    }    
}

static void appl_ctl_set_state(int s, int e)
{
    struct Ctl_t* ctl = &APPL.Ctl;
    ctl->Step = 0;
    ctl->State = s;
    switch (s)
    {
    case CTL_ST_LAUNCH:
        strcpy(ctl->szState,"启动");
        break;

    case CTL_ST_STDBY:
        strcpy(ctl->szState,"监控");
        break;    

    case CTL_ST_STOP:
    strcpy(ctl->szState,"停机");
    break;    

    case CTL_ST_RUN:
    strcpy(ctl->szState,"运行");
    break;           

    case CTL_ST_ERR:
    strcpy(ctl->szState,"故障");
    break;           
    
    default:
        strcpy(ctl->szState,"未知");
        break;
    }

    ctl->Err = e;
    switch (e)
    {
    case CTL_ERR_NONE:
        strcpy(ctl->szErr,"无");
        break;
    case CTL_ERR_LAUNCH_COMMERR_DETECTED:
        strcpy(ctl->szErr,"启动状态下检测到通信故障");
    break;

    case CTL_ERR_LAUNCH_CFGERR:
        strcpy(ctl->szErr,"启动状态下检测到参数配置故障");
    break;

    case CTL_ERR_STDBY_COMMERR_DETECTED:
        strcpy(ctl->szErr,"监控状态下检测到通信故障");
    break;

    case CTL_ERR_STDBY_WAIT_PCS_STOP_TIMEOUT:
        strcpy(ctl->szErr,"监控状态下等待PCS停机超时");
    break; 

    case CTL_ERR_STDBY_WAIT_PCS_APS0_TIMEOUT:
        strcpy(ctl->szErr,"监控状态下等待PCS有功功率设定值为0超时");
    break;

    case CTL_ERR_STOP_COMMERR_DETECTED:
    strcpy(ctl->szErr,"停机状态下检测到通信故障");
    break;

    case CTL_ERR_STOP_PCS_NOT_STOP_DETECTED:
    strcpy(ctl->szErr,"停机状态下检测到PCS非停机");
    break;

    case CTL_ERR_STOP_WAIT_PCS_START_TIMEOUT:
    strcpy(ctl->szErr,"停机状态下等待PCS运行超时");
    break;

    case CTL_ERR_RUN_COMMERR_DETECTED:
    strcpy(ctl->szErr,"运行状态下检测到通信故障");
    break;

    case CTL_ERR_RUN_PCS_NOT_START_DETECTED:
    strcpy(ctl->szErr,"运行状态下检测到PCS非运行");
    break;

    case CTL_ERR_RUN_WAIT_PCS_STOP_TIMEOUT:
    strcpy(ctl->szErr,"运行状态下等待PCS停机超时");
    break;

    case CTL_ERR_RUN_WAIT_PCS_APS0_TIMEOUT:
    strcpy(ctl->szErr,"运行状态下等待PCS有功功率设定值为0超时");
    break;

    case CTL_ERR_ERR_COMMERR_DETECTED:
    strcpy(ctl->szErr,"故障状态下检测到通信故障");
    break;

    default:
        strcpy(ctl->szErr,"未知");
        break;
    }
}

static int appl_ctl_check_comm_state_slave( void ){
    struct Enjoy100kW_t* pcs = &APPL.Enjoy100kW;
    struct GaoteBms_t* bms = &APPL.GaoteBms;
    struct Envicool5kW_t* ac = &APPL.Envicool5kW;
    static int bFirstErr = 1;
    if(pcs->CommState == ST_COMM_NORM 
            && bms->CommState == ST_COMM_NORM 
            && ac->CommState == ST_COMM_NORM){
        if(bFirstErr == 0){
            bFirstErr = 1;
        }
        return 0;
    }else{
        if(bFirstErr == 1){
            bFirstErr = 0;
            syslog(LOG_INFO, "%s, Err Detected, PCS:%d,BMS:%d,AC:%d",
                __func__, pcs->CommState, bms->CommState, ac->CommState);
        }
        return -1;
    }
}

static int appl_ctl_check_comm_state_pcurv( void ){
    struct Enjoy100kW_t* pcs = &APPL.Enjoy100kW;
    struct GaoteBms_t* bms = &APPL.GaoteBms;
    struct Envicool5kW_t* ac = &APPL.Envicool5kW;
    static int bFirstErr = 1;
    if(pcs->CommState == ST_COMM_NORM 
            && bms->CommState == ST_COMM_NORM 
            && ac->CommState == ST_COMM_NORM
            /*&& gm->CommState == ST_COMM_NORM
            && tm->CommState == ST_COMM_NORM*/){
        if(bFirstErr == 0){
            bFirstErr = 1;
        }
        return 0;
    }else{
        if(bFirstErr == 1){
            bFirstErr = 0;
            syslog(LOG_INFO, "%s, Err Detected, PCS:%d,BMS:%d,AC:%d, MaxUpdate1Intv:%ld, MaxUpdate2Intv:%ld",
                __func__, pcs->CommState, bms->CommState, ac->CommState, ac->MaxUpdate1Intv, ac->MaxUpdate2Intv);
        }
        return -1;
    }
}

static void appl_ctl_update( void ){
    struct Settings_t* set = &APPL.Set.s;
    struct Ctl_t* ctl = &APPL.Ctl;
    struct Enjoy100kW_t* pcs = &APPL.Enjoy100kW;
    struct GaoteBms_t* bms = &APPL.GaoteBms;
    struct Envicool5kW_t* ac = &APPL.Envicool5kW;
    struct Dtsd1352_t* gm = &APPL.GateMeter;
    struct Dtsd1352_t* tm = &APPL.TransMeter;
    int i;
    int sum;

    ctl->Ap = pcs->Ap;
    sum = 0;
    for( i = 0; i < set->CtnMeterNbr; i++ ){
       sum += APPL.CtnMeter[ i + 1].com_active_p;
    }
    ctl->GateEsAp = sum; 

    //ctl->TransEsAp = APPL.CtnMeter[set->CtnMeterId].com_active_p;
    ctl->TransEsAp = ctl->GateEsAp;

    ctl->GateAp = gm->com_active_p;
    ctl->GateLoadAp = ctl->GateAp - ctl->GateEsAp;

    ctl->TransAp = tm->com_active_p;
    ctl->TransLoadAp = ctl->TransAp - ctl->TransEsAp;
}

static void appl_ctl_workmode_slave_run( void ){
    struct Settings_t* set = &APPL.Set.s;
    struct Ctl_t* ctl = &APPL.Ctl;
    struct chan485_t* c1 = &APPL.chan485[1];
    struct Enjoy100kW_t* pcs = &APPL.Enjoy100kW;
    struct GaoteBms_t* bms = &APPL.GaoteBms;
    struct Envicool5kW_t* ac = &APPL.Envicool5kW;
    switch (ctl->State)
    {
    case CTL_ST_LAUNCH:
        if(appl_ctl_check_comm_state_slave() == 0 ){
            if(set->bErr){
                appl_ctl_set_state(CTL_ST_ERR, CTL_ERR_LAUNCH_CFGERR);
                syslog(LOG_INFO,"[LAUNCH] Comm Check Ok, Goto ERR");
            }else{
                appl_ctl_set_state(CTL_ST_STDBY, CTL_ERR_NONE);
                syslog(LOG_INFO,"[LAUNCH] Comm Check Ok, Goto STDBY");
            }
        }else{
            appl_ctl_set_state(CTL_ST_ERR, CTL_ERR_LAUNCH_COMMERR_DETECTED);
            syslog(LOG_INFO,"[LAUNCH] Comm Check Fail, Goto ERR");
        }
        break;

    case CTL_ST_STDBY:
        if(appl_ctl_check_comm_state_slave() < 0 ){
            appl_ctl_set_state(CTL_ST_ERR, CTL_ERR_STDBY_COMMERR_DETECTED);
            syslog(LOG_INFO,"[STDBY] Comm Check Fail, Goto ERR");
        }else{
            if(ctl->Step == 0){ // wait cmd
                if( ctl->Cmd == CTL_CMD_STOP ){
                    ctl->Cmd = CTL_CMD_DONE;
                    syslog(LOG_INFO,"[STDBY] Get CTL_CMD_STOP");
                    if(pcs->WorkState == 0){
                        syslog(LOG_INFO,"[STDBY] Check PCS WorkState==0 Ok");
                        if( abs(pcs->Ap) < 0.1 ){
                            appl_ctl_set_state(CTL_ST_STOP, CTL_ERR_NONE);
                            syslog(LOG_INFO,"[STDBY] Check PCS Aps==0 Ok, Goto STOP");
                        }else{
                            c1->Cmd = CMD_485_PCS_SET_APS;
                            c1->CmdParam = 0;
                            ctl->Step = 20;
                            ctl->Cnt = 0;
                            syslog(LOG_INFO,"[STDBY] Check PCS Aps==0 Fail, Send Cmd And Check");
                        }
                    }else{
                        c1->Cmd = CMD_485_PCS_STOP;
                        ctl->Step = 10;
                        ctl->Cnt = 0;
                        syslog(LOG_INFO,"[STDBY] Check PCS WorkState==0 Fail, Send Cmd And Check");
                    }
                }else if( ctl->Cmd == CTL_CMD_SET_WORKMODE ){ 
                    ctl->Cmd = CTL_CMD_DONE;
                    appl_ctl_set_workmode(ctl->CmdPara);
                    syslog(LOG_INFO,"[STDBY] Get CTL_CMD_SET_WORKMODE");
                }
            }else if( ctl->Step == 10 ){ // wait pcs stop
                if(ctl->Cnt++ > 5){
                    appl_ctl_set_state(CTL_ST_ERR, CTL_ERR_STDBY_WAIT_PCS_STOP_TIMEOUT);
                    syslog(LOG_INFO,"[STDBY] Check PCS WorkState==0 Timeout, Goto ERR");
                }else{
                    if( pcs->WorkState == 0 ){
                        if( abs(pcs->Ap) < 0.1 ){
                            appl_ctl_set_state(CTL_ST_STOP, CTL_ERR_NONE);
                            syslog(LOG_INFO,"[STDBY] Check PCS Aps==0 Ok, Goto STOP");
                        }else{
                            c1->Cmd = CMD_485_PCS_SET_APS;
                            c1->CmdParam = 0;
                            ctl->Step = 20;
                            ctl->Cnt = 0;
                            syslog(LOG_INFO,"[STDBY] Check PCS Aps==0 Fail, Send Cmd And Check");
                        }
                    }else{
                        syslog(LOG_INFO,"[STDBY] Waiting PCS WorkState==0");
                    }
                }
            }else if( ctl->Step == 20 ){ // wait pcs aps = 0
                if(ctl->Cnt++ > 5){
                    appl_ctl_set_state(CTL_ST_ERR, CTL_ERR_STDBY_WAIT_PCS_APS0_TIMEOUT);
                    syslog(LOG_INFO,"[STDBY] Check PCS Aps==0 Timeout, Goto ERR");
                }else{
                    if( abs(pcs->Ap) < 0.1 ){
                        appl_ctl_set_state(CTL_ST_STOP, CTL_ERR_NONE);
                        syslog(LOG_INFO,"[STDBY] Check PCS Aps==0 Ok, Goto STOP");
                    }else{
                        syslog(LOG_INFO,"[STDBY] Waiting PCS Aps==0");
                    }
                }
            }
        }
    break;    

    case CTL_ST_STOP:
    if(appl_ctl_check_comm_state_slave() < 0 ){
        appl_ctl_set_state(CTL_ST_ERR, CTL_ERR_STOP_COMMERR_DETECTED);
        syslog(LOG_INFO,"[STOP] Comm Check Fail, Goto ERR");
    }else{
        if(ctl->Step == 0){ // wait cmd and check
            if(ctl->Cmd == CTL_CMD_RUN){
                ctl->Cmd = CTL_CMD_DONE;
                c1->Cmd = CMD_485_PCS_START;
                ctl->Step = 10;
                ctl->Cnt = 0;
                syslog(LOG_INFO,"[STOP] Get CTL_CMD_RUN");
            }else if(ctl->Cmd == CTL_CMD_STDBY){
                ctl->Cmd = CTL_CMD_DONE;
                appl_ctl_set_state(CTL_ST_STDBY, CTL_ERR_NONE);
                syslog(LOG_INFO,"[STOP] Get CTL_CMD_STDBY, Goto STDBY");
            }else if(ctl->Cmd == CTL_CMD_SET_WORKMODE ){ 
                ctl->Cmd = CTL_CMD_DONE;
                appl_ctl_set_workmode(ctl->CmdPara);
                syslog(LOG_INFO,"[STOP] Get CTL_CMD_SET_WORKMODE");
            }else{
                if(pcs->WorkState != 0){
                    appl_ctl_set_state(CTL_ST_ERR, CTL_ERR_STOP_PCS_NOT_STOP_DETECTED);
                    syslog(LOG_INFO,"[STOP] PCS NOT STOP Detected, Goto ERR");
                }
            }
        }else if(ctl->Step == 10){
            if(ctl->Cnt++ > 15){
                appl_ctl_set_state(CTL_ST_ERR, CTL_ERR_STOP_WAIT_PCS_START_TIMEOUT);
                syslog(LOG_INFO,"[STOP] Check PCS WorkState==1 Timeout, Goto ERR");
            }else{
                if(pcs->WorkState == 1){
                    appl_ctl_set_state(CTL_ST_RUN, CTL_ERR_NONE);
                    syslog(LOG_INFO,"[STOP] Check PCS WorkState==1 Ok, Goto RUN");
                }else{
                    syslog(LOG_INFO,"[STOP] Waiting PCS WorkState==1");
                }
            }
        }
    }
    break;

    case CTL_ST_RUN:
    if(appl_ctl_check_comm_state_slave() < 0 ){
        appl_ctl_set_state(CTL_ST_ERR, CTL_ERR_RUN_COMMERR_DETECTED);
        syslog(LOG_INFO,"[RUN] Comm Check Fail, Goto ERR");
    }else{
        if(ctl->Step == 0){ // wait cmd
            if(ctl->Cmd == CTL_CMD_STOP){
                ctl->Cmd = CTL_CMD_DONE;
                c1->Cmd = CMD_485_PCS_STOP;
                ctl->Step = 10;
                ctl->Cnt = 0;
                syslog(LOG_INFO,"[RUN] Get CTL_CMD_STOP");
            }else if(ctl->Cmd == CTL_CMD_SET_APS){
                ctl->Cmd = CTL_CMD_DONE;
                if(ctl->CmdPara < 0){ // dhg
                    if( ctl->bDhgAble ){
                        c1->Cmd = CMD_485_PCS_SET_APS;
                        c1->CmdParam = ctl->CmdPara;
                        syslog(LOG_INFO,"[RUN]New Aps:%d", ctl->CmdPara);
                        if(ctl->bChgAble == 0){
                            ctl->bChgAble = 1;
                        }                        
                    }else{
                        syslog(LOG_INFO,"[RUN]New Aps:%d, BUT NOT DHGABLE", ctl->CmdPara);
                    }
                }else if(ctl->CmdPara > 0){ // chg
                    if( ctl->bChgAble ){
                        c1->Cmd = CMD_485_PCS_SET_APS;
                        c1->CmdParam = ctl->CmdPara;
                        syslog(LOG_INFO,"[RUN]New Aps:%d", ctl->CmdPara);
                        if(ctl->bDhgAble == 0){
                            ctl->bDhgAble = 1;
                        }
                    }else{
                        syslog(LOG_INFO,"[RUN]New Aps:%d, BUT NOT CHGABLE", ctl->CmdPara);
                    }
                }else{
                    c1->Cmd = CMD_485_PCS_SET_APS;
                    c1->CmdParam = ctl->CmdPara;
                    syslog(LOG_INFO,"[RUN]New Aps:%d", ctl->CmdPara);
                }

            }else if( pcs->WorkState != 1 ){
                appl_ctl_set_state(CTL_ST_ERR, CTL_ERR_RUN_PCS_NOT_START_DETECTED);
                syslog(LOG_INFO,"[RUN] PCS NOT RUN Detected, Goto ERR");
            }else if( bms->MaxCellV >= set->ChgCellV ){
                ctl->bChgAble = 0;
                c1->Cmd = CMD_485_PCS_SET_APS;
                c1->CmdParam = 0;
                ctl->Step = 20;
                ctl->Cnt = 0;
                syslog(LOG_INFO,"[RUN] MaxCellV(%.3f) >= ChgCellV(%.3f) Detected, Set PCS Aps=0, Wait And Check", bms->MaxCellV,set->ChgCellV);
            }else if( bms->MinCellV <= set->DhgCellV ){ 
                ctl->bDhgAble = 0;
                c1->Cmd = CMD_485_PCS_SET_APS;
                c1->CmdParam = 0;
                ctl->Step = 20;
                ctl->Cnt = 0;
                syslog(LOG_INFO,"[RUN] MinCellV(%.3f) <= DhgCellV(%.3f) Detected, Set PCS Aps=0, Wait And Check", bms->MinCellV,set->DhgCellV);
            }
        }else if(ctl->Step == 10){ // wait pcs stop
            if(ctl->Cnt++ > 5){
                appl_ctl_set_state(CTL_ST_ERR, CTL_ERR_RUN_WAIT_PCS_STOP_TIMEOUT);
                syslog(LOG_INFO,"[RUN] Check PCS WorkState==0 Timeout, Goto ERR");
            }else{
                if(pcs->WorkState == 0){
                    if( abs(pcs->Ap) < 0.1 ){
                        appl_ctl_set_state(CTL_ST_STOP, CTL_ERR_NONE);
                        syslog(LOG_INFO,"[STDBY] Check PCS Aps==0 Ok, Goto STOP");
                    }else{
                        c1->Cmd = CMD_485_PCS_SET_APS;
                        c1->CmdParam = 0;
                        ctl->Step = 30;
                        ctl->Cnt = 0;
                        syslog(LOG_INFO,"[STDBY] Check PCS Aps==0 Fail, Send Cmd And Check");
                    }
                }else{
                    syslog(LOG_INFO,"[RUN] Waiting PCS WorkState==0");
                }
            }
        }else if( ctl->Step == 20 ){ // wait pcs aps = 0 and stay run
            if(ctl->Cnt++ > 5){
                appl_ctl_set_state(CTL_ST_ERR, CTL_ERR_RUN_WAIT_PCS_APS0_TIMEOUT);
                syslog(LOG_INFO,"[RUN] Check PCS Aps==0 Timeout, Goto ERR");
            }else{
                if( abs(pcs->Ap) < 0.1 ){
                    ctl->Step = 0;
                    syslog(LOG_INFO,"[RUN] Check PCS Aps==0 Ok, Stay RUN");
                }else{
                    syslog(LOG_INFO,"[RUN] Waiting PCS Aps==0");
                }
            }
        }else if( ctl->Step == 30 ){ // wait pcs aps = 0 and goto stop
            if(ctl->Cnt++ > 5){
                appl_ctl_set_state(CTL_ST_ERR, CTL_ERR_RUN_WAIT_PCS_APS0_TIMEOUT);
                syslog(LOG_INFO,"[RUN] Check PCS Aps==0 Timeout, Goto ERR");
            }else{
                if( abs(pcs->Ap) < 0.1 ){
                    appl_ctl_set_state(CTL_ST_STOP, CTL_ERR_NONE);
                    syslog(LOG_INFO,"[RUN] Check PCS Aps==0 Ok, Goto STOP");
                }else{
                    syslog(LOG_INFO,"[RUN] Waiting PCS Aps==0");
                }
            }
        }
    }
    break;    

    case CTL_ST_ERR:
        if(ctl->Step == 0){
            c1->Cmd = CMD_485_PCS_STOP;
            ctl->Step = 10;
        }else if(ctl->Step == 10){
            if(ctl->Cmd == CTL_CMD_STDBY){
                ctl->Cmd = CTL_CMD_DONE;
                syslog(LOG_INFO,"[ERR] Get CTL_CMD_STDBY");
                if(appl_ctl_check_comm_state_slave() == 0 ){
                    appl_ctl_set_state(CTL_ST_STDBY, CTL_ERR_NONE);
                    syslog(LOG_INFO,"[ERR] Comm Check Ok, Goto STDBY");
                }else{
                    appl_ctl_set_state(CTL_ST_ERR, CTL_ERR_ERR_COMMERR_DETECTED);
                    syslog(LOG_INFO,"[ERR] Comm Check Fail, Goto ERR");
                }
            }
        }
    break;  
    
    default:
        // NEVER REACH HERE
        break;
    }    
}

static void appl_ctl_workmode_pcurv_run( void ){
    struct Ctl_t* ctl = &APPL.Ctl;
    struct Settings_t* set = &APPL.Set.s;
    struct chan485_t* c1 = &APPL.chan485[1];
    struct Enjoy100kW_t* pcs = &APPL.Enjoy100kW;
    struct GaoteBms_t* bms = &APPL.GaoteBms;
    struct Envicool5kW_t* ac = &APPL.Envicool5kW;
    struct Dtsd1352_t* gm = &APPL.GateMeter;
    struct Dtsd1352_t* tm = &APPL.TransMeter;
    int yy,mm,dd,hh,nn,ss;
    int tgtaps;
    int CommChkOk = 0;
    int i;

    switch (ctl->State)
    {
    case CTL_ST_LAUNCH:
        if(appl_ctl_check_comm_state_pcurv() == 0 ){
            appl_ctl_set_state(CTL_ST_STDBY, CTL_ERR_NONE);
            syslog(LOG_INFO,"[LAUNCH] Comm Check Ok, Goto STDBY");
        }else{
            appl_ctl_set_state(CTL_ST_ERR, CTL_ERR_LAUNCH_COMMERR_DETECTED);
            syslog(LOG_INFO,"[LAUNCH] Comm Check Fail, Goto ERR");
        }
        break;

    case CTL_ST_STDBY:
        if(appl_ctl_check_comm_state_pcurv() < 0 ){
            appl_ctl_set_state(CTL_ST_ERR, CTL_ERR_STDBY_COMMERR_DETECTED);
            syslog(LOG_INFO,"[STDBY] Comm Check Fail, Goto ERR");
        }else{
            if(ctl->Step == 0){ // wait cmd
                if( ctl->Cmd == CTL_CMD_STOP ){
                    ctl->Cmd = CTL_CMD_DONE;
                    syslog(LOG_INFO,"[STDBY] Get CTL_CMD_STOP");
                    if( pcs->WorkState == 0 ){
                        syslog(LOG_INFO,"[STDBY] Check PCS WorkState==0 Ok");
                        if( abs(pcs->Ap) < 0.1 ){
                            appl_ctl_set_state(CTL_ST_STOP, CTL_ERR_NONE);
                            syslog(LOG_INFO,"[STDBY] Check PCS Aps==0 Ok, Goto STOP");
                        }else{
                            c1->Cmd = CMD_485_PCS_SET_APS;
                            c1->CmdParam = 0;
                            ctl->Step = 20;
                            ctl->Cnt = 0;
                            syslog(LOG_INFO,"[STDBY] Check PCS Aps==0 Fail, Send Cmd And Wait");
                        }
                    }else{
                        c1->Cmd = CMD_485_PCS_STOP;
                        ctl->Step = 10;
                        ctl->Cnt = 0;
                        syslog(LOG_INFO,"[STDBY] Check PCS WorkState==0 Fail, Send Cmd And Wait");
                    }
                }else if( CTL_CMD_SET_WORKMODE ){ 
                    ctl->Cmd = CTL_CMD_DONE;
                    appl_ctl_set_workmode(ctl->CmdPara);
                    syslog(LOG_INFO,"[STDBY] Get CTL_CMD_SET_WORKMODE");
                }
            }else if( ctl->Step == 10 ){ // wait pcs stop
                if(ctl->Cnt++ > 5){
                    appl_ctl_set_state(CTL_ST_ERR, CTL_ERR_STDBY_WAIT_PCS_STOP_TIMEOUT);
                    syslog(LOG_INFO,"[STDBY] Check PCS WorkState==0 Timeout, Goto ERR");
                }else{
                    if( pcs->WorkState == 0 ){
                        if( abs(pcs->Ap) < 0.1 ){
                            appl_ctl_set_state(CTL_ST_STOP, CTL_ERR_NONE);
                            syslog(LOG_INFO,"[STDBY] Check PCS Aps==0 Ok, Goto STOP");
                        }else{
                            c1->Cmd = CMD_485_PCS_SET_APS;
                            c1->CmdParam = 0;
                            ctl->Step = 20;
                            ctl->Cnt = 0;
                            syslog(LOG_INFO,"[STDBY] Check PCS Aps==0 Fail, Send Cmd And Wait");
                        }
                    }else{
                        syslog(LOG_INFO,"[STDBY] Waiting PCS WorkState==0");
                    }
                }
            }else if( ctl->Step == 20 ){ // wait pcs aps = 0
                if(ctl->Cnt++ > 5){
                    appl_ctl_set_state(CTL_ST_ERR, CTL_ERR_STDBY_WAIT_PCS_APS0_TIMEOUT);
                    syslog(LOG_INFO,"[STDBY] Check PCS Aps==0 Timeout, Goto ERR");
                }else{
                    if( abs(pcs->Ap) < 0.1 ){
                        appl_ctl_set_state(CTL_ST_STOP, CTL_ERR_NONE);
                        syslog(LOG_INFO,"[STDBY] Check PCS Aps==0 Ok, Goto STOP");
                    }else{
                        syslog(LOG_INFO,"[STDBY] Waiting PCS Aps==0");
                    }
                }
            }
        }
    break;    

    case CTL_ST_STOP:
    if(appl_ctl_check_comm_state_pcurv() < 0 ){
        appl_ctl_set_state(CTL_ST_ERR, CTL_ERR_STOP_COMMERR_DETECTED);
        syslog(LOG_INFO,"[STOP] Comm Check Fail, Goto ERR");
    }else{
        if(ctl->Step == 0){ // wait cmd and check
            if( ctl->Cmd == CTL_CMD_RUN ){ 
                ctl->Cmd = CTL_CMD_DONE;
                c1->Cmd = CMD_485_PCS_START;
                ctl->Step = 10;
                ctl->Cnt = 0;
                ctl->LastTune = 0;
                syslog(LOG_INFO,"[STOP] Get CTL_CMD_RUN");
            }else if( 0 ){  // ctl->Cmd = CTL_CMD_STDBY   !!NOT SUPPORTED
                ctl->Cmd = CTL_CMD_DONE;
                appl_ctl_set_state(CTL_ST_STDBY, CTL_ERR_NONE);
                syslog(LOG_INFO,"[STOP] Get CTL_CMD_STDBY, Goto STDBY");
            }else if( ctl->Cmd == CTL_CMD_SET_WORKMODE ){ 
                ctl->Cmd = CTL_CMD_DONE;
                appl_ctl_set_workmode(ctl->CmdPara);
                syslog(LOG_INFO,"[STOP] Get CTL_CMD_SET_WORKMODE, Para:%d",ctl->CmdPara);
            }else{
                if(pcs->WorkState != 0){
                    appl_ctl_set_state(CTL_ST_ERR, CTL_ERR_STOP_PCS_NOT_STOP_DETECTED);
                    syslog(LOG_INFO,"[STOP] PCS NOT STOP Detected, Goto ERR");
                }
            }
        }else if(ctl->Step == 10){ // wait PCS start
            if(ctl->Cnt++ > 15){
                appl_ctl_set_state(CTL_ST_ERR, CTL_ERR_STOP_WAIT_PCS_START_TIMEOUT);
                syslog(LOG_INFO,"[STOP] Check PCS WorkState==1 Timeout, Goto ERR");
            }else{
                if(pcs->WorkState == 1){
                    appl_ctl_set_state(CTL_ST_RUN, CTL_ERR_NONE);
                    syslog(LOG_INFO,"[STOP] Check PCS WorkState==1 Ok, Goto RUN");
                }else{
                    syslog(LOG_INFO,"[STOP] Waiting PCS WorkState==1");
                }
            }
        }
    }
    break;

    case CTL_ST_RUN:
    if(appl_ctl_check_comm_state_pcurv() < 0 ){
        appl_ctl_set_state(CTL_ST_ERR, CTL_ERR_RUN_COMMERR_DETECTED);
        syslog(LOG_INFO,"[RUN] Comm Check Fail, Goto ERR");
    }else{
        if(ctl->Step == 0){ // wait cmd and run
            if(ctl->Cmd == CTL_CMD_STOP){
                ctl->Cmd = CTL_CMD_DONE;
                c1->Cmd = CMD_485_PCS_STOP;
                ctl->Step = 10;
                ctl->Cnt = 0;
                syslog(LOG_INFO,"[RUN] Get CTL_CMD_STOP");
            }else if( mg_millis() - ctl->LastTune > 10000 ){
                ctl->LastTune = mg_millis();
                appl_get_datetime_num(&yy, &mm, &dd, &hh, &nn, &ss);
                tgtaps = set->pcurv[(hh*60+nn)/15];
                //syslog(LOG_INFO,"%s, [RUN] Get Target Aps:%d", __func__, tgtaps);
                if( tgtaps < 0 ){ // DHG
                    if(ctl->bDhgAble != 1){
                        //syslog(LOG_INFO,"%s, [RUN] NOT DHGABLE", __func__);
                        break;
                    }else{ // Aps Limit
                        if( tgtaps*set->TransCoupleNbr + ctl->TransLoadAp < set->DhgTransLim ){
                            tgtaps = (set->DhgTransLim - ctl->TransLoadAp)/set->TransCoupleNbr;
                            if(tgtaps > 0){
                                tgtaps = 0;
                            }
                        }
                        if(tgtaps*set->GateCoupleNbr + ctl->GateLoadAp < set->DhgGateLim){
                            tgtaps = (set->DhgGateLim - ctl->GateLoadAp)/set->GateCoupleNbr;
                            if(tgtaps > 0){
                                tgtaps = 0;
                            }
                        }
                    }
                }else if( tgtaps > 0 ){ // CHG
                    if(ctl->bChgAble != 1){
                        //syslog(LOG_INFO,"%s, [RUN] NOT CHGABLE", __func__);
                        break;
                    }else{
                        if( tgtaps*set->TransCoupleNbr + ctl->TransLoadAp > set->ChgTransLim ){
                            tgtaps = (set->ChgTransLim - ctl->TransLoadAp)/set->TransCoupleNbr;
                            if(tgtaps < 0){
                                tgtaps = 0;
                            }
                        }
                        if(tgtaps*set->GateCoupleNbr + ctl->GateLoadAp > set->ChgGateLim){
                            tgtaps = (set->ChgGateLim - ctl->GateLoadAp)/set->GateCoupleNbr;
                            if(tgtaps < 0){
                                tgtaps = 0;
                            }
                        }
                    }
                }
                CommChkOk = 0;
                if(gm->CommState != ST_COMM_NORM || tm->CommState != ST_COMM_NORM ){
                    CommChkOk = -1;
                }
                for( i = 0; i < set->CtnMeterNbr; i++){
                    if( APPL.CtnMeter[ i + 1 ].CommState != ST_COMM_NORM ){
                        CommChkOk = -1;
                        break;
                    }
                }
                if(CommChkOk < 0 ){
                    c1->Cmd = CMD_485_PCS_SET_APS;
                    c1->CmdParam = 0;
                }else{
                    c1->Cmd = CMD_485_PCS_SET_APS;
                    c1->CmdParam = tgtaps;
                }
                if(tgtaps < 0){ // DHG
                    if(ctl->bChgAble == 0){
                        ctl->bChgAble = 1;
                    }                        
                }
                if(tgtaps > 0){ // CHG
                    if(ctl->bDhgAble == 0){
                        ctl->bDhgAble = 1;
                    }
                }
                //syslog(LOG_INFO,"%s, [RUN] Target Aps Sent", __func__);
            }else if( pcs->WorkState != 1 ){
                appl_ctl_set_state(CTL_ST_ERR, CTL_ERR_RUN_PCS_NOT_START_DETECTED);
                syslog(LOG_INFO,"[RUN] PCS NOT RUN Detected, Goto ERR");
            }else if( bms->MaxCellV >= set->ChgCellV ){
                ctl->bChgAble = 0; // !!
                c1->Cmd = CMD_485_PCS_SET_APS;
                c1->CmdParam = 0;
                ctl->Step = 20;
                ctl->Cnt = 0;
                syslog(LOG_INFO,"[RUN] MaxCellV(%.3f) >= ChgCellV(%.3f) Detected, Set PCS Aps=0, Wait And Check", bms->MaxCellV,set->ChgCellV);
            }else if( bms->MinCellV <= set->DhgCellV ){ 
                ctl->bDhgAble = 0;
                c1->Cmd = CMD_485_PCS_SET_APS;
                c1->CmdParam = 0;
                ctl->Step = 20;
                ctl->Cnt = 0;
                syslog(LOG_INFO,"[RUN] MinCellV(%.3f) <= DhgCellV(%.3f) Detected, Set PCS Aps=0, Wait And Check", bms->MinCellV,set->DhgCellV);
            }
        }else if(ctl->Step == 10){ // wait pcs stop
            if(ctl->Cnt++ > 5){
                appl_ctl_set_state(CTL_ST_ERR, CTL_ERR_RUN_WAIT_PCS_STOP_TIMEOUT);
                syslog(LOG_INFO,"[RUN] Check PCS WorkState==0 Timeout, Goto ERR");
            }else{
                if(pcs->WorkState == 0){
                    appl_ctl_set_state(CTL_ST_STOP, CTL_ERR_NONE);
                    syslog(LOG_INFO,"[RUN] Check PCS WorkState==0 Ok, Goto STOP");
                }else{
                    syslog(LOG_INFO,"[RUN] Waiting PCS WorkState==0");
                }
            }
        }else if( ctl->Step == 20 ){ // wait pcs aps = 0 and stay run
            if(ctl->Cnt++ > 5){
                appl_ctl_set_state(CTL_ST_ERR, CTL_ERR_RUN_WAIT_PCS_APS0_TIMEOUT);
                syslog(LOG_INFO,"[RUN] Check PCS Aps==0 Timeout, Goto ERR");
            }else{
                if( abs(pcs->Ap) < 0.1 ){
                    ctl->Step = 0;
                    syslog(LOG_INFO,"[RUN] Check PCS Aps==0 Ok, Stay RUN");
                }else{
                    syslog(LOG_INFO,"[RUN] Waiting PCS Aps==0");
                }
            }
        }else if( ctl->Step == 30 ){ // wait pcs aps = 0 and goto stop
            if(ctl->Cnt++ > 5){
                appl_ctl_set_state(CTL_ST_ERR, CTL_ERR_RUN_WAIT_PCS_APS0_TIMEOUT);
                syslog(LOG_INFO,"[RUN] Check PCS Aps==0 Timeout, Goto ERR");
            }else{
                if( abs(pcs->Ap) < 0.1 ){
                    appl_ctl_set_state(CTL_ST_STOP, CTL_ERR_NONE);
                    syslog(LOG_INFO,"[RUN] Check PCS Aps==0 Ok, Goto STOP");
                }else{
                    syslog(LOG_INFO,"[RUN] Waiting PCS Aps==0");
                }
            }
        }
    }
    break;    

    case CTL_ST_ERR:        
        if(ctl->Step == 0){
            c1->Cmd = CMD_485_PCS_STOP;
            ctl->Step = 10;
        }else if(ctl->Step == 10){
            if(ctl->Cmd == CTL_CMD_STDBY){
                ctl->Cmd = CTL_CMD_DONE;
                syslog(LOG_INFO,"[ERR] Get CTL_CMD_STDBY");
                if(appl_ctl_check_comm_state_pcurv() == 0 ){
                    appl_ctl_set_state(CTL_ST_STDBY, CTL_ERR_NONE);
                    syslog(LOG_INFO,"[ERR] Comm Check Ok, Goto STDBY");
                }else{
                    appl_ctl_set_state(CTL_ST_ERR, CTL_ERR_ERR_COMMERR_DETECTED);
                    syslog(LOG_INFO,"[ERR] Comm Check Fail, Goto ERR");
                }
            }
        }
    break;  
    
    default:
        // NEVER REACH HERE
        break;
    }    
}

static void* thrd_ctl(void* param)
{
    struct Ctl_t* ctl = &APPL.Ctl;
    struct chan485_t* c1 = &APPL.chan485[1];
    struct Enjoy100kW_t* pcs = &APPL.Enjoy100kW;
    struct GaoteBms_t* bms = &APPL.GaoteBms;
    struct Envicool5kW_t* ac = &APPL.Envicool5kW;

    sleep(3);

    syslog(LOG_INFO,"%s ++",__func__);

    appl_ctl_set_state(CTL_ST_LAUNCH, CTL_ERR_NONE);
    ctl->Cmd = CTL_CMD_DONE;

    while(1){
        appl_ctl_update();
        switch (ctl->WorkMode){
            case CTL_WORKMODE_SLAVE:
                appl_ctl_workmode_slave_run();
                break;

            case CTL_WORKMODE_PCURV:
                appl_ctl_workmode_pcurv_run();
                break;  
            
            default:
                break;
        }
        usleep(1000000);
    }
    syslog(LOG_INFO,"%s --",__func__);
}

static void fn_mqtt1(struct mg_connection *c, int ev, void *ev_data) {
//   struct chanmqtt_t* m = &APPL.chanmqtt[1];
//   if (ev == MG_EV_OPEN) {
//     MG_INFO(("%lu CREATED", c->id));
//     // c->is_hexdumping = 1;
//   } else if (ev == MG_EV_CONNECT) {
//     if (mg_url_is_ssl(m->szs_url)) {
//       struct mg_tls_opts opts = {.ca = mg_unpacked("/certs/ca.pem"),
//                                  .name = mg_url_host(m->szs_url)};
//       mg_tls_init(c, &opts);
//     }
//   } else if (ev == MG_EV_ERROR) {
//     // On error, log error message
//     MG_ERROR(("%lu ERROR %s", c->id, (char *) ev_data));
//   } else if (ev == MG_EV_MQTT_OPEN) {
//     m->bConnected = 1;
//     strcpy(m->szState,"正常");
//     // MQTT connect is successful
//     struct mg_str subt = mg_str(m->szs_sub_topic);
//     MG_INFO(("%lu CONNECTED to %s", c->id, m->szs_url));
//     struct mg_mqtt_opts sub_opts;
//     memset(&sub_opts, 0, sizeof(sub_opts));
//     sub_opts.topic = subt;
//     sub_opts.qos = m->s_qos;
//     mg_mqtt_sub(c, &sub_opts);
//     MG_INFO(("%lu SUBSCRIBED to %.*s", c->id, (int) subt.len, subt.ptr));

//   } else if (ev == MG_EV_MQTT_MSG) {
//     // When we get echo response, print it
//     struct mg_mqtt_message *mm = (struct mg_mqtt_message *) ev_data;
//     MG_INFO(("%lu RECEIVED %.*s <- %.*s", c->id, (int) mm->data.len,
//              mm->data.ptr, (int) mm->topic.len, mm->topic.ptr));
//   } else if (ev == MG_EV_CLOSE) {
//     MG_INFO(("%lu CLOSED", c->id));
//     m->s_conn = NULL;  // Mark that we're closed
//     m->bConnected = 0;
//     strcpy(m->szState,"故障");
//   }
}

static void fn_mqtt2(struct mg_connection *c, int ev, void *ev_data) {
    struct Settings_t* set = &APPL.Set.s;
    struct chanmqtt_t* m = &APPL.chanmqtt[2];
    struct Ctl_t* ctl = &APPL.Ctl;
    struct Dtsd1352_t* gm = &APPL.GateMeter;
    struct Dtsd1352_t* tm = &APPL.TransMeter;
    struct Dtsd1352_t* cm = NULL;
    int i;
  if (ev == MG_EV_OPEN) {
    syslog(LOG_INFO,"%lu CREATED", c->id);
    // c->is_hexdumping = 1;
  } else if (ev == MG_EV_CONNECT) {
    if (mg_url_is_ssl(m->szs_url)) {
      struct mg_tls_opts opts = {.ca = mg_unpacked("/certs/ca.pem"),
                                 .name = mg_url_host(m->szs_url)};
      mg_tls_init(c, &opts);
    }
  } else if (ev == MG_EV_ERROR) {
    // On error, log error message
    syslog(LOG_INFO,"%lu ERROR %s", c->id, (char *) ev_data);
  } else if (ev == MG_EV_MQTT_OPEN) {
    m->bConnected = 1;
    strcpy(m->szState,"正常");
    // MQTT connect is successful
    syslog(LOG_INFO,"%lu CONNECTED to %s", c->id, m->szs_url);

    struct mg_mqtt_opts sub_opts;
    struct mg_str subt;
    // Control
    memset(&sub_opts, 0, sizeof(sub_opts));
    subt = mg_str(m->szs_sub_topic[0]);
    sub_opts.topic = subt;
    sub_opts.qos = m->s_qos;
    mg_mqtt_sub(c, &sub_opts);
    syslog(LOG_INFO,"%lu SUBSCRIBED to %.*s", c->id, (int) subt.len, subt.ptr);
    // GateMeter
    memset(&sub_opts, 0, sizeof(sub_opts));
    subt = mg_str(m->szs_sub_topic[1]);
    sub_opts.topic = subt;
    sub_opts.qos = m->s_qos;
    mg_mqtt_sub(c, &sub_opts);
    syslog(LOG_INFO,"%lu SUBSCRIBED to %.*s", c->id, (int) subt.len, subt.ptr);    
    // TransMeter
    memset(&sub_opts, 0, sizeof(sub_opts));
    subt = mg_str(m->szs_sub_topic[2]);
    sub_opts.topic = subt;
    sub_opts.qos = m->s_qos;
    mg_mqtt_sub(c, &sub_opts);
    syslog(LOG_INFO,"%lu SUBSCRIBED to %.*s", c->id, (int) subt.len, subt.ptr);     
    // CtnMeter1 
    for( i = 0; i < set->CtnMeterNbr; i++){
        memset(&sub_opts, 0, sizeof(sub_opts));
        subt = mg_str(m->szs_sub_topic[3 + i]);
        sub_opts.topic = subt;
        sub_opts.qos = m->s_qos;
        mg_mqtt_sub(c, &sub_opts);
        syslog(LOG_INFO,"%lu SUBSCRIBED to %.*s", c->id, (int) subt.len, subt.ptr); 
    }                  
  } else if (ev == MG_EV_MQTT_MSG) {
    // When we get echo response, print it
    struct mg_mqtt_message *mm = (struct mg_mqtt_message *) ev_data;
    //syslog(LOG_INFO,"%lu RECEIVED %.*s <- %.*s", c->id, (int) mm->data.len,
    //         mm->data.ptr, (int) mm->topic.len, mm->topic.ptr);
    struct mg_str json = mg_str(mm->data.ptr);
    struct mg_str new_json;
    int offset, length;
    char buf[512];
    char devid[128] = {0};
    char* str = NULL;
    double val;
    offset = mg_json_get(json, "$.data[0]", &length);
    if( offset > 0 ){
        memset(buf,0,sizeof(buf));
        strncpy(buf, json.ptr + offset, length);
        //syslog(LOG_INFO,"%s, Get data[0]:%s", __func__, buf);
        new_json = mg_str(buf);
        str = mg_json_get_str(new_json, "$.device_id");
        if( str != NULL){
            for( i = 0 ; i < set->CtnMeterNbr; i++){
                sprintf(devid,"CtnMeter%d", i+1);
                cm = &APPL.CtnMeter[i+1];
                if(strcmp(str, devid) == 0){
                    if(mg_json_get_num(new_json, "$.com_ap", &val)){
                        cm->com_active_p = val;
                        cm->LastUpdate = mg_millis();                     
                    }
                }
            }
            sprintf(devid,"TransMeter%d", set->TransId);
            if(strcmp(str, devid) == 0){
                if(mg_json_get_num(new_json, "$.com_ap", &val)){
                    tm->com_active_p = val;
                    tm->LastUpdate = mg_millis();                     
                }
            }
            if(strcmp(str, "GateMeter") == 0){
                if(mg_json_get_num(new_json, "$.com_ap", &val)){
                    gm->com_active_p = val;
                    gm->LastUpdate = mg_millis();
                    if(m->LastRecv == 0){ // First Recv
                        m->TotalRecvCnt = 0;
                        m->LastRecv = mg_millis();
                        m->TotalRecvIntv = 0;
                    }else{
                        m->TotalRecvCnt++;
                        int64_t CurrIntv = mg_millis() - m->LastRecv;
                        m->LastRecv = mg_millis();
                        if(CurrIntv > m->MaxRecvIntv){
                            m->MaxRecvIntv = CurrIntv;
                        }
                        m->TotalRecvIntv += CurrIntv;
                        m->AvgRecvIntv = m->TotalRecvIntv/m->TotalRecvCnt;
                    }   
                }
            }
            free(str);
        }
    }else{
        syslog(LOG_INFO,"%s, Fail to Get data[0]:%d", __func__, offset);
    }        
  } else if (ev == MG_EV_CLOSE) {
    syslog(LOG_INFO,"%lu CLOSED", c->id);
    m->s_conn = NULL;  // Mark that we're closed
    m->bConnected = 0;
    strcpy(m->szState,"故障");
  }
}

static void fn_mqtt3(struct mg_connection *c, int ev, void *ev_data) {
//     struct Dtsd1352_t* gm = &APPL.GateMeter;
//     struct Dtsd1352_t* tm = &APPL.TransMeter;
//     struct chanmqtt_t* m = &APPL.chanmqtt[3];
//     char buf[256];
//     if (ev == MG_EV_OPEN) {
//         syslog(LOG_INFO,"%s, %lu CREATED", __func__, c->id);
//         // c->is_hexdumping = 1;
//     } else if (ev == MG_EV_CONNECT) {
//         if (mg_url_is_ssl(m->szs_url)) {
//         struct mg_tls_opts opts = {.ca = mg_unpacked("/certs/ca.pem"),
//                                     .name = mg_url_host(m->szs_url)};
//         mg_tls_init(c, &opts);
//         }
//     } else if (ev == MG_EV_ERROR) {
//         // On error, log error message
//         syslog(LOG_INFO, "%s, %lu ERROR %s", __func__, c->id, (char *) ev_data);
//     } else if (ev == MG_EV_MQTT_OPEN) {
//         m->bConnected = 1;
//         strcpy(m->szState,"正常");
//         // MQTT connect is successful
//         syslog(LOG_INFO,"%s, %lu CONNECTED to %s", __func__, c->id, m->szs_url);
        
//         struct mg_str subt = mg_str(m->szs_sub_topic[0]);
//         struct mg_mqtt_opts sub_opts;
//         memset(&sub_opts, 0, sizeof(sub_opts));
//         sub_opts.topic = subt;
//         sub_opts.qos = m->s_qos;
//         mg_mqtt_sub(c, &sub_opts);
//         syslog(LOG_INFO,"%s, %lu SUBSCRIBED to %.*s", __func__, c->id, (int) subt.len, subt.ptr);

//         // subt = mg_str(m->szs_sub_topic[1]);
//         // memset(&sub_opts, 0, sizeof(sub_opts));
//         // sub_opts.topic = subt;
//         // sub_opts.qos = m->s_qos;
//         // mg_mqtt_sub(c, &sub_opts);
//         // syslog(LOG_INFO,"%s, %lu SUBSCRIBED to %.*s", __func__, c->id, (int) subt.len, subt.ptr);

//   } else if (ev == MG_EV_MQTT_MSG) {
//     // When we get echo response, print it
//     struct mg_mqtt_message *mm = (struct mg_mqtt_message *) ev_data;
//     //syslog(LOG_INFO,"%s, %lu RECEIVED %.*s <- %.*s", __func__, c->id, (int) mm->data.len,
//     //         mm->data.ptr, (int) mm->topic.len, mm->topic.ptr);
//     struct mg_str json = mg_str(mm->data.ptr);    
//     double dval;
//     bool ok;
//     if( strcmp(mm->topic.ptr, m->szs_sub_topic[0]) == 0){ // gate meter
//         syslog(LOG_INFO,"%s, Get Gate Meter Data", __func__);
//         ok = mg_json_get_num(json,"$.m1_com_ap", &dval);
//         if( ok ){
//             gm->com_active_p = dval;
//             gm->LastUpdate = mg_millis();
//             strcpy(gm->szLastUpdate, appl_get_datetime_long());
//         }
//         ok = mg_json_get_num(json,"$.m2_com_ap", &dval);
//         if( ok ){
//             tm->com_active_p = dval;
//             tm->LastUpdate = mg_millis();
//             strcpy(tm->szLastUpdate, appl_get_datetime_long());
//         }
//     }
//   } else if (ev == MG_EV_CLOSE) {
//     syslog(LOG_INFO,"%s, %lu CLOSED", __func__, c->id);
//     m->s_conn = NULL;  // Mark that we're closed
//     m->bConnected = 0;
//     strcpy(m->szState,"故障");
//   }
}

static void* thrd_mqtt_1(void* param){
//     struct chanmqtt_t* m = &APPL.chanmqtt[1];
//     struct dtsd1352_t* meter = NULL;
//     struct mg_mqtt_opts opts = {.user = mg_str(m->szusrname),
//                                 .clean = true,
//                                 .qos = m->s_qos,
//                                 .topic = mg_str(m->szs_pub_topic),
//                                 .version = 4,
//                                 .message = mg_str("bye")};
//     struct mg_mqtt_opts pub_opts;          
//     struct mg_str pubt = mg_str(m->szs_pub_topic);        
//     char msg[2048];
//     char buf[2048];      
//     int i;

//     mg_mgr_init(&mgr_mqtt1);    

//     MG_INFO(("%s ENTER, idx:1", __func__));

//     if (m->s_conn == NULL) m->s_conn = mg_mqtt_connect(&mgr_mqtt1, m->szs_url, &opts, fn_mqtt1, NULL);
//     while(1){
//         mg_mgr_poll(&mgr_mqtt1, 50);
//         if(mg_millis() - m->LastUpload > 5000){
//             m->LastUpload = mg_millis();
//             if(m->bConnected){
//                 for(i = 1; i <= 2; i++){
//                     meter = &APPL.Dtsd1352[i];
//                     if( meter->CommState == ST_COMM_NORM ){
//                         memset(&pub_opts, 0, sizeof(pub_opts));
//                         pub_opts.topic = pubt;
//                         sprintf(buf,
// "\"m%d_pf\":%.3f,\
// \"m%d_com_ap\":%.1f,\"m%d_com_ae\":%.1f,\"m%d_pos_ae\":%.1f,\"m%d_neg_ae\":%.1f, \
// \"m%d_ua\":%.1f,\"m%d_ub\":%.1f,\"m%d_uc\":%.1f, \
// \"m%d_ia\":%.1f,\"m%d_ib\":%.1f,\"m%d_ic\":%.1f",
// i, meter->pwr_factor,
// i, meter->com_active_p,i, meter->com_active_e,i, meter->pos_active_e,i, meter->neg_active_e,
// i, meter->ua,i, meter->ub,i, meter->uc,
// i, meter->ia,i, meter->ib,i, meter->ic);
//                         sprintf(msg,"{'ts':%lld,'values':{%s}}", (long long)time(NULL)*1000, buf);
//                         pub_opts.message = mg_str(msg);
//                         pub_opts.qos = m->s_qos, pub_opts.retain = false;
//                         mg_mqtt_pub(m->s_conn, &pub_opts);
//                     }
//                 }
//             }
//             if (m->s_conn == NULL) m->s_conn = mg_mqtt_connect(&mgr_mqtt1, m->szs_url, &opts, fn_mqtt1, NULL);
//         }
//     }

//     MG_INFO(("%s EXIT, idx:1", __func__));
}

static void* thrd_mqtt_2(void* param){
    struct Dtsd1352_t* gm = &APPL.GateMeter;
    struct Dtsd1352_t* tm = &APPL.TransMeter;
    struct dtsd1352_t* meter = NULL;
    struct chanmqtt_t* m = &APPL.chanmqtt[2];
    struct Ctl_t* ctl = &APPL.Ctl;
    struct GaoteBms_t* bms = &APPL.GaoteBms;
    struct Envicool5kW_t* ac = &APPL.Envicool5kW;
    struct FireAlarm_t* fa = NULL;
    struct Adl200_t* auxm = &APPL.Adl200;
    struct Co_t* co = &APPL.Co;
    struct Dehumi_t* dh = &APPL.Dehumi;
    struct Dido_t* dido = &APPL.Dido;
    struct Enjoy100kW_t* pcs = &APPL.Enjoy100kW;
    struct Settings_t* set = &APPL.Set.s;
    struct mg_mqtt_opts opts = {.user = mg_str(m->szusrname),
                                .pass = mg_str(m->szpasswd),
                                .client_id = mg_str(set->szSN),
                                .clean = true,
                                .qos = m->s_qos,
                                .topic = mg_str(m->szs_pub_topic),
                                .version = 4,
                                .message = mg_str("bye")};
    struct mg_mqtt_opts pub_opts;          
    struct mg_str pubt = mg_str(m->szs_pub_topic);     
    struct Dtsd1352_t* cm = NULL;   
    char msg[2048];
    char stakv[2048];
    char ctnkv[2048];
    char pcskv[2048];
    char bmskv[8192];
    char ackv[2048];
    char fakv[10][512];
    char amkv[512];
    char cokv[512];
    char dhkv[512];
    char buf[2048];   
    char didokv[512];
    int64_t LastCommCheck;
    int i,j;
    double avgcellv;
    int CommChkOk;

    mg_mgr_init(&mgr_mqtt2);        

    syslog(LOG_INFO,"%s ENTER idx:2", __func__);

    if (m->s_conn == NULL) m->s_conn = mg_mqtt_connect(&mgr_mqtt2, m->szs_url, &opts, fn_mqtt2, NULL);
    while(1){
         mg_mgr_poll(&mgr_mqtt2, 20);
         // Process Cmd
         if( m->Cmd == CMD_MQTT_REGISTER ){
            m->Cmd = CMD_MQTT_DONE;
            // // Register STA
            // sprintf(msg, "{\"project_id\":\"%s\",\"timestamp\":%lld,\"data\":[{\"device_id\":\"%s\",\"type\":0,\"idx\":1}]}", 
            // set->szCloudUserName, (long long)time(NULL)*1000, set->szSN);
            // pub_opts.message = mg_str(msg);
            // pub_opts.qos = m->s_qos, pub_opts.retain = false;
            // pub_opts.topic = mg_str("register");
            // mg_mqtt_pub(m->s_conn, &pub_opts);
            // m->TotalSendCnt++;

            // Register CTN
            sprintf(msg, "{\"project_id\":\"%s\",\"timestamp\":%lld,\"data\":[{\"device_id\":\"%s\",\"type\":1,\"idx\":1}]}", 
            set->szCloudUserName, (long long)time(NULL)*1000, set->szSN);
            pub_opts.message = mg_str(msg);
            pub_opts.qos = m->s_qos, pub_opts.retain = false;
            pub_opts.topic = mg_str("register");
            mg_mqtt_pub(m->s_conn, &pub_opts);
            m->TotalSendCnt++;

            // Register PCS
            sprintf(msg, "{\"project_id\":\"%s\",\"timestamp\":%lld,\"data\":[{\"device_id\":\"%s-PCS\",\"pid\":\"%s\",\"type\":2,\"idx\":1}]}", 
            set->szCloudUserName, (long long)time(NULL)*1000, set->szSN,set->szSN);
            pub_opts.message = mg_str(msg);
            pub_opts.qos = m->s_qos, pub_opts.retain = false;
            pub_opts.topic = mg_str("register");
            mg_mqtt_pub(m->s_conn, &pub_opts);
            m->TotalSendCnt++;
            // Register BMS
            sprintf(msg, "{\"project_id\":\"%s\",\"timestamp\":%lld,\"data\":[{\"device_id\":\"%s-BMS\",\"pid\":\"%s\",\"type\":3,\"idx\":1}]}", 
            set->szCloudUserName, (long long)time(NULL)*1000, set->szSN,set->szSN);
            pub_opts.message = mg_str(msg);
            pub_opts.qos = m->s_qos, pub_opts.retain = false;
            pub_opts.topic = mg_str("register");
            mg_mqtt_pub(m->s_conn, &pub_opts);       
            m->TotalSendCnt++;     
            // Register AC
            sprintf(msg, "{\"project_id\":\"%s\",\"timestamp\":%lld,\"data\":[{\"device_id\":\"%s-AC\",\"pid\":\"%s\",\"type\":4,\"idx\":1}]}", 
            set->szCloudUserName, (long long)time(NULL)*1000, set->szSN,set->szSN);
            pub_opts.message = mg_str(msg);
            pub_opts.qos = m->s_qos, pub_opts.retain = false;
            pub_opts.topic = mg_str("register");
            mg_mqtt_pub(m->s_conn, &pub_opts); 
            m->TotalSendCnt++;          
   
            // Register FA, FireAlarm
            for( i = 1; i <= 5; i++ ){
                sprintf(msg, "{\"project_id\":\"%s\",\"timestamp\":%lld,\"data\":[{\"device_id\":\"%s-FA%d\",\"pid\":\"%s\",\"type\":14,\"idx\":1}]}", 
                set->szCloudUserName, (long long)time(NULL)*1000, set->szSN, i, set->szSN);
                pub_opts.message = mg_str(msg);
                pub_opts.qos = m->s_qos, pub_opts.retain = false;
                pub_opts.topic = mg_str("register");
                mg_mqtt_pub(m->s_conn, &pub_opts); 
                m->TotalSendCnt++;  
            }   
            // Register AM, Aux Meter
            sprintf(msg, "{\"project_id\":\"%s\",\"timestamp\":%lld,\"data\":[{\"device_id\":\"%s-AM\",\"pid\":\"%s\",\"type\":10,\"idx\":1}]}", 
            set->szCloudUserName, (long long)time(NULL)*1000, set->szSN,set->szSN);
            pub_opts.message = mg_str(msg);
            pub_opts.qos = m->s_qos, pub_opts.retain = false;
            pub_opts.topic = mg_str("register");
            mg_mqtt_pub(m->s_conn, &pub_opts); 
            m->TotalSendCnt++;        
            // Register CO, Co Sensor
            sprintf(msg, "{\"project_id\":\"%s\",\"timestamp\":%lld,\"data\":[{\"device_id\":\"%s-CO\",\"pid\":\"%s\",\"type\":8,\"idx\":1}]}", 
            set->szCloudUserName, (long long)time(NULL)*1000, set->szSN,set->szSN);
            pub_opts.message = mg_str(msg);
            pub_opts.qos = m->s_qos, pub_opts.retain = false;
            pub_opts.topic = mg_str("register");
            mg_mqtt_pub(m->s_conn, &pub_opts); 
            m->TotalSendCnt++;     
            // Register DH, Dehumi
            sprintf(msg, "{\"project_id\":\"%s\",\"timestamp\":%lld,\"data\":[{\"device_id\":\"%s-DH\",\"pid\":\"%s\",\"type\":6,\"idx\":1}]}", 
            set->szCloudUserName, (long long)time(NULL)*1000, set->szSN,set->szSN);
            pub_opts.message = mg_str(msg);
            pub_opts.qos = m->s_qos, pub_opts.retain = false;
            pub_opts.topic = mg_str("register");
            mg_mqtt_pub(m->s_conn, &pub_opts); 
            m->TotalSendCnt++;        
            // Register DIDO
            sprintf(msg, "{\"project_id\":\"%s\",\"timestamp\":%lld,\"data\":[{\"device_id\":\"%s-DIDO\",\"pid\":\"%s\",\"type\":9,\"idx\":1}]}", 
            set->szCloudUserName, (long long)time(NULL)*1000, set->szSN,set->szSN);
            pub_opts.message = mg_str(msg);
            pub_opts.qos = m->s_qos, pub_opts.retain = false;
            pub_opts.topic = mg_str("register");
            mg_mqtt_pub(m->s_conn, &pub_opts); 
            m->TotalSendCnt++;                                                       
         }

        avgcellv = bms->AvgCellV;
        // ******************************************
        // Upload Fast Data
        // ******************************************
        if(mg_millis() - m->LastFastUpload > set->UploadHighSpeed){
            m->LastFastUpload = mg_millis();
            if(m->bConnected){
                // //STA
                // stakv[0] = 0;
                // sprintf(buf,"\"device_id\":\"%s\",\"type\":0,\"state\":%d,\"err\":\"%s\",", set->szCloudUserName, ctl->State, ctl->szErr);
                // strcat(stakv,buf);
                // if(pcs->CommState == ST_COMM_NORM){
                //     sprintf(buf,"\"es_ap\":%d,", ctl->EsAp);
                //     strcat(stakv,buf);
                // }
                // if( pm->CommState == ST_COMM_NORM ){
                //     sprintf(buf,"\"pv_ap\":%d,", ctl->PvAp);
                //     strcat(stakv,buf);
                // }
                // if( gm->CommState == ST_COMM_NORM ){
                //     sprintf(buf,"\"gate_ap\":%d,", ctl->GateAp);
                //     strcat(stakv,buf);
                // }                
                // if( tm->CommState == ST_COMM_NORM ){
                //     sprintf(buf,"\"trans_ap\":%d,", ctl->TransAp);
                //     strcat(stakv,buf);
                // }
                // if( pcs->CommState == ST_COMM_NORM && pm->CommState == ST_COMM_NORM 
                //         && gm->CommState == ST_COMM_NORM && tm->CommState == ST_COMM_NORM ){
                //     sprintf(buf,"\"load_ap\":%d,", ctl->LoadAp);
                //     strcat(stakv,buf);
                // }
                // if( bms->CommState == ST_COMM_NORM ){
                //     sprintf(buf,"\"soc\":%.4f,", bms->Soc);
                //     strcat(stakv,buf);
                // }
                // if( bms->CommState == ST_COMM_NORM ){
                //     sprintf(buf,"\"soh\":%.4f,", bms->Soh);
                //     strcat(stakv,buf);
                // }

                // CTN                
                ctnkv[0] = 0;
                sprintf(buf,"\"device_id\":\"%s\",\"type\":1,\"state\":%d,\"err\":\"%s\",", set->szSN, ctl->State, ctl->szErr);
                strcat(ctnkv,buf);
                if(pcs->CommState == ST_COMM_NORM){
                    sprintf(buf,"\"ap\":%d,", ctl->Ap);
                    strcat(ctnkv,buf);
                }
                if( bms->CommState == ST_COMM_NORM ){
                    sprintf(buf,"\"soc\":%.4f,", bms->Soc);
                    strcat(ctnkv,buf);
                }
                if( bms->CommState == ST_COMM_NORM ){
                    sprintf(buf,"\"soh\":%.4f,", bms->Soh);
                    strcat(ctnkv,buf);
                }
                sprintf(msg, "{\"project_id\":\"%s\",\"timestamp\":%lld,\"data\":[{%s}]}", 
                set->szCloudUserName, (long long)time(NULL)*1000, ctnkv);
                pub_opts.message = mg_str(msg);
                pub_opts.qos = m->s_qos, pub_opts.retain = false;
                pub_opts.topic = pubt;
                mg_mqtt_pub(m->s_conn, &pub_opts);
                m->TotalSendCnt++;  

                //PCS
                pcskv[0] = 0;
                sprintf(buf,"\"device_id\":\"%s-PCS\",\"type\":2,", set->szSN);
                strcat(pcskv,buf);
                if(pcs->CommState == ST_COMM_NORM){
                    sprintf(buf,"\"ap\":%.4f,\"aps\":%.4f,\"rap\":%.4f,\"dcv\":%.4f,\"dcc\":%.4f,\"errstat\":%d,\"runstat\":%d", 
                        pcs->Ap, pcs->Aps, pcs->Rap, pcs->BatV, pcs->BatC, pcs->ErrState, pcs->WorkState);
                    strcat(pcskv,buf);
                }
                sprintf(msg, "{\"project_id\":\"%s\",\"timestamp\":%lld,\"data\":[{%s}]}", 
                set->szCloudUserName, (long long)time(NULL)*1000, pcskv);
                pub_opts.message = mg_str(msg);
                pub_opts.qos = m->s_qos, pub_opts.retain = false;
                pub_opts.topic = pubt;
                mg_mqtt_pub(m->s_conn, &pub_opts);
                m->TotalSendCnt++;  

                // BMS
                bmskv[0] = 0;
                if(bms->CommState == ST_COMM_NORM){
                    sprintf(buf,"\"device_id\":\"%s-BMS\",\"type\":3,", set->szSN);
                    strcat(bmskv,buf);
                    sprintf(buf,"\"hv\":%d,\"v\":%.4f,\"c\":%.4f,\"cell_max_v\":%.4f,\"cell_ave_v\":%.4f,\"cell_min_v\":%.4f,", 
                            bms->HvState, bms->BatV, bms->BatI, bms->MaxCellV, bms->AvgCellV, bms->MinCellV);
                    strcat(bmskv,buf);
                    if(avgcellv < 3.0 || avgcellv > 3.45){
                        for( i = 1; i <= PACK_NBR; i++){
                            for( j = 1; j < PACK_CELL_NBR; j++){
                                sprintf(buf,"\"p%d_v%d\":%.4f,", i, j, bms->CellVolt[i][j]);
                                strcat(bmskv, buf);
                            }
                        }
                    }
                }
                sprintf(msg, "{\"project_id\":\"%s\",\"timestamp\":%lld,\"data\":[{%s}]}", 
                    set->szCloudUserName, (long long)time(NULL)*1000, bmskv);
                pub_opts.message = mg_str(msg);
                pub_opts.qos = m->s_qos, pub_opts.retain = false;
                pub_opts.topic = pubt;
                mg_mqtt_pub(m->s_conn, &pub_opts);
                m->TotalSendCnt++;  
            }
        }
        // ******************************************
        // Upload Medium Data
        // ******************************************
        if(mg_millis() - m->LastMediumUpload > set->UploadMediumSpeed){
            m->LastMediumUpload = mg_millis();
            if(m->bConnected){
                //PCS
                pcskv[0] = 0;
                if(pcs->CommState == ST_COMM_NORM){
                    sprintf(buf,"\"device_id\":\"%s-PCS\",\"type\":2,", set->szSN);
                    strcat(pcskv,buf);
                    sprintf(buf,"\"t_igbt\":%.4f,\"t_env\":%.4f,\"uab\":%.4f,\"ubc\":%.4f,\"uca\":%.4f,\"ia\":%.4f,\"ib\":%.4f,\"ic\":%.4f", 
                        pcs->Tigbt, pcs->Tenv, pcs->Uab, pcs->Ubc, pcs->Uca, pcs->Ia, pcs->Ib, pcs->Ic);
                    strcat(pcskv,buf);
                }
                sprintf(msg, "{\"project_id\":\"%s\",\"timestamp\":%lld,\"data\":[{%s}]}", 
                    set->szCloudUserName, (long long)time(NULL)*1000, pcskv);
                pub_opts.message = mg_str(msg);
                pub_opts.qos = m->s_qos, pub_opts.retain = false;
                pub_opts.topic = pubt;
                mg_mqtt_pub(m->s_conn, &pub_opts);
                m->TotalSendCnt++;                  
                // BMS
                bmskv[0] = 0;
                if(bms->CommState == ST_COMM_NORM){
                    sprintf(buf,"\"device_id\":\"%s-BMS\",\"type\":3,", set->szSN);
                    strcat(bmskv,buf);
                    sprintf(buf,"\"cell_max_t\":%.4f,\"cell_ave_t\":%.4f,\"cell_min_t\":%.4f,\"soc\":%.4f,\"soh\":%.4f,\
\"cell_v_diff\":%.4f,\"cell_t_diff\":%.4f,\"cell_max_v_index\":%d,\"cell_min_v_index\":%d,\"cell_max_t_index\":%d,\"cell_min_t_index\":%d,\
\"pos_ins\":%6f,\"neg_ins\":%6f,", 
                        bms->MaxCellT, bms->AvgCellT, bms->MinCellT,bms->Soc, bms->Soh,
                        bms->CellVDiff, bms->CellTDiff, bms->MaxCellVIdx, bms->MinCellVIdx, bms->MaxCellTIdx, bms->MinCellTIdx,
                        bms->PosRes, bms->NegRes);
                    strcat(bmskv,buf);
                    if(avgcellv >= 3.0 && avgcellv <= 3.45){
                        for( i = 1; i <= PACK_NBR; i++){
                            for( j = 1; j < PACK_CELL_NBR; j++){
                                sprintf(buf,"\"p%d_v%d\":%.4f,", i, j, bms->CellVolt[i][j]);
                                strcat(bmskv, buf);
                            }
                        }
                    }
                    for( i = 1; i <= PACK_NBR; i++){
                        for( j = 1; j < PACK_CELL_NBR; j++){
                            sprintf(buf,"\"p%d_t%d\":%.3f,", i, j, bms->CellTemp[i][j]);
                            strcat(bmskv, buf);
                        }
                    }
                }
                sprintf(msg, "{\"project_id\":\"%s\",\"timestamp\":%lld,\"data\":[{%s}]}", 
                    set->szCloudUserName, (long long)time(NULL)*1000, bmskv);
                pub_opts.message = mg_str(msg);
                pub_opts.qos = m->s_qos, pub_opts.retain = false;
                pub_opts.topic = pubt;
                mg_mqtt_pub(m->s_conn, &pub_opts);
                m->TotalSendCnt++;      

                // AC
                ackv[0] = 0;
                if(ac->CommState == ST_COMM_NORM){
                    sprintf(buf,"\"device_id\":\"%s-AC\",\"type\":4,", set->szSN);
                    strcat(ackv,buf);
                    sprintf(buf,"\"work_mode\":%d,\"outwater_t\":%d,\"rtnwater_t\":%d,\"outwater_pre\":%.3f,\"rtnwater_pre\":%.3f,\"err_code\":%d,\"err_level\":%d", 
                        ac->WorkMode, ac->OutWaterTemp,ac->InWaterTemp, ac->OutWaterPre,ac->InWaterPre, ac->ErrCode, ac->ErrLevel);
                    strcat(ackv,buf);
                }
                sprintf(msg, "{\"project_id\":\"%s\",\"timestamp\":%lld,\"data\":[{%s}]}", 
                    set->szCloudUserName, (long long)time(NULL)*1000, ackv);
                pub_opts.message = mg_str(msg);
                pub_opts.qos = m->s_qos, pub_opts.retain = false;
                pub_opts.topic = pubt;
                mg_mqtt_pub(m->s_conn, &pub_opts);
                m->TotalSendCnt++;  
        }
    }                    

    // ******************************************
    // Upload SLow Data
    // ******************************************
    if(mg_millis() - m->LastSlowUpload > set->UploadSlowSpeed){
        m->LastSlowUpload = mg_millis();
        if(m->bConnected){
            //FA
            for( i = 1; i <= 5; i++){
                fa = &APPL.Fa[i];
                fakv[i][0] = 0;
                if(fa->CommState == ST_COMM_NORM){
                    sprintf(buf,"\"device_id\":\"%s-FA%d\",\"type\":14,", set->szSN, i);
                    strcat(fakv[i],buf);
                    sprintf(buf,"\"T1\":%d,\"T2\":%d,\"Co\":%d,\"Voc\":%d,\"SmokeFlag\":%d,\"Level\":%d,\"ErrCode\":%d",
                        fa->T1, fa->T2, fa->Co, fa->Voc, fa->SmokeFlagVal,fa->LevelVal,fa->ErrCodeVal);
                    strcat(fakv[i],buf);
                    // Sending
                    sprintf(msg, "{\"project_id\":\"%s\",\"timestamp\":%lld,\"data\":[{%s}]}", 
                        set->szCloudUserName, (long long)time(NULL)*1000, fakv[i]);              
                    pub_opts.message = mg_str(msg);
                    pub_opts.qos = m->s_qos, pub_opts.retain = false;
                    pub_opts.topic = pubt;
                    mg_mqtt_pub(m->s_conn, &pub_opts);
                    m->TotalSendCnt++;                      
                }
            }
            // AM
            if(auxm->CommState == ST_COMM_NORM){
                amkv[0] = 0;
                sprintf(buf,"\"device_id\":\"%s-AM\",\"type\":10,", set->szSN);
                strcat(amkv,buf);
                sprintf(buf,"\"pos_ae\":%f,\"neg_ae\":%f,\"ap\":%.2f", auxm->PosAe, auxm->NegAe, auxm->Ap);
                strcat(amkv,buf);
                sprintf(msg, "{\"project_id\":\"%s\",\"timestamp\":%lld,\"data\":[{%s}]}", set->szCloudUserName, (long long)time(NULL)*1000, amkv);              
                pub_opts.message = mg_str(msg);
                pub_opts.qos = m->s_qos, pub_opts.retain = false;
                pub_opts.topic = pubt;
                mg_mqtt_pub(m->s_conn, &pub_opts);
                m->TotalSendCnt++;                  
            }
            // CO
            if(co->CommState == ST_COMM_NORM){
                cokv[0] = 0;
                sprintf(buf,"\"device_id\":\"%s-CO\",\"type\":8,", set->szSN);
                strcat(cokv,buf);
                sprintf(buf,"\"Density\":%u", co->Density);
                strcat(cokv,buf);
                // Sending
                sprintf(msg, "{\"project_id\":\"%s\",\"timestamp\":%lld,\"data\":[{%s}]}", set->szCloudUserName, (long long)time(NULL)*1000, cokv);              
                pub_opts.message = mg_str(msg);
                pub_opts.qos = m->s_qos, pub_opts.retain = false;
                pub_opts.topic = pubt;
                mg_mqtt_pub(m->s_conn, &pub_opts);
                m->TotalSendCnt++;   
            }  
            //DH
            if(dh->CommState){
                dhkv[0] = 0;
                sprintf(buf,"\"device_id\":\"%s-DH\",\"type\":7,", set->szSN);
                strcat(dhkv,buf);
                sprintf(buf,"\"temp\":%.3f,\"humi\":%.3f", dh->Temp,dh->Humi);
                strcat(dhkv,buf);
                // Sending
                sprintf(msg, "{\"project_id\":\"%s\",\"timestamp\":%lld,\"data\":[{%s}]}", set->szCloudUserName, (long long)time(NULL)*1000, dhkv);              
                pub_opts.message = mg_str(msg);
                pub_opts.qos = m->s_qos, pub_opts.retain = false;
                pub_opts.topic = pubt;
                mg_mqtt_pub(m->s_conn, &pub_opts);
                m->TotalSendCnt++;   
            }   
            // DIDO
            didokv[0] = 0;
            sprintf(buf,"\"device_id\":\"%s-DIDO\",\"type\":9,", set->szSN);
            strcat(didokv,buf);
            sprintf(buf,"\"WaterDec1\":%d,\"WaterDec2\":%d,\"FrontDoor\":%d,\"BackDoor\":%d,\"EmgStop\":%d,\"FeEruptFb\":%d", 
                dido->WaterDec1,dido->WaterDec2, dido->FrontDoor, dido->BackDoor, dido->EmgStop, dido->FeEruptFb);
            strcat(didokv,buf);
            // Sending
            sprintf(msg, "{\"project_id\":\"%s\",\"timestamp\":%lld,\"data\":[{%s}]}", set->szCloudUserName, (long long)time(NULL)*1000, didokv);              
            pub_opts.message = mg_str(msg);
            pub_opts.qos = m->s_qos, pub_opts.retain = false;
            pub_opts.topic = pubt;
            mg_mqtt_pub(m->s_conn, &pub_opts);
            m->TotalSendCnt++;  
            // sprintf(msg, "{\"project_id\":\"%s\",\"timestamp\":%lld,\"data\":[{%s},{%s},{%s},{%s},{%s},{%s},{%s},{%s},{%s}]}", 
            //     set->szCloudUserName, (long long)time(NULL)*1000, fakv[1],fakv[2],fakv[3],fakv[4],fakv[5],amkv,cokv,dhkv,didokv);
            // // sprintf(msg, "{\"project_id\":\"%s\",\"timestamp\":%lld,\"data\":[{%s},{%s},{%s},{%s},{%s},{%s}]}", 
            // //     set->szCloudUserName, (long long)time(NULL)*1000, fekv, fakv[1],fakv[2],fakv[3],fakv[4],fakv[5]);                
            // pub_opts.message = mg_str(msg);
            // pub_opts.qos = m->s_qos, pub_opts.retain = false;
            // pub_opts.topic = pubt;
            // mg_mqtt_pub(m->s_conn, &pub_opts);
            // m->TotalSendCnt++;  
        }
    }      

        // **************************************
        // Meters Comm Check
        // **************************************
        if( mg_millis() - LastCommCheck > 1000 ){
            LastCommCheck = mg_millis();
            if( mg_millis() - gm->LastUpdate > 10000){
                gm->CommState = ST_COMM_ERR;
                strcpy(gm->szCommState,"故障");
            }else{
                gm->CommState = ST_COMM_NORM;
                strcpy(gm->szCommState,"正常");
            }
            if( mg_millis() - tm->LastUpdate > 10000){
                tm->CommState = ST_COMM_ERR;
                strcpy(tm->szCommState,"故障");
            }else{
                tm->CommState = ST_COMM_NORM;
                strcpy(tm->szCommState,"正常");
            }
            CommChkOk = 0;
            for( i = 0; i < set->CtnMeterNbr; i++){
                cm = &APPL.CtnMeter[ i + 1];
                if( mg_millis() - cm->LastUpdate > 10000){
                    cm->CommState = ST_COMM_ERR;
                    strcpy(cm->szCommState,"故障");
                    CommChkOk = -1;
                }else{
                    cm->CommState = ST_COMM_NORM;
                    strcpy(cm->szCommState,"正常");
                } 
            }    
            if(CommChkOk < 0){
                APPL.CtnMeterCommState = ST_COMM_ERR;
                strcpy(APPL.szCtnMeterCommState,"故障");
            }else{
                APPL.CtnMeterCommState = ST_COMM_NORM;
                strcpy(APPL.szCtnMeterCommState,"正常");
            }               
        }     

        if (m->s_conn == NULL){
            m->s_conn = mg_mqtt_connect(&mgr_mqtt2, m->szs_url, &opts, fn_mqtt2, NULL);
            m->TotalReconnCnt++;
        } 
    }

    syslog(LOG_INFO, "%s EXIT, idx:2", __func__);
}

// Get data from mosquitto server
static void* thrd_mqtt_3(void* param){
    // struct chanmqtt_t* m = &APPL.chanmqtt[3];
    // struct Dtsd1352_t* gm = &APPL.GateMeter;
    // struct Dtsd1352_t* tm = &APPL.TransMeter;
    // struct mg_mqtt_opts opts = {.user = mg_str(m->szusrname),
    //                             .clean = true,
    //                             .qos = m->s_qos,
    //                             .topic = mg_str(m->szs_pub_topic),
    //                             .version = 4,
    //                             .keepalive = 3,
    //                             .message = mg_str("bye")};
    // struct mg_mqtt_opts pub_opts;          
    // struct mg_str pubt = mg_str(m->szs_pub_topic);        
    // char msg[2048]; 
    // int64_t LastReconn = 0;
    // int64_t LastCommCheck = 0;

    // mg_mgr_init(&mgr_mqtt3);    

    // syslog(LOG_INFO,"%s ENTER, idx:3", __func__);

    // if (m->s_conn == NULL) m->s_conn = mg_mqtt_connect(&mgr_mqtt3, m->szs_url, &opts, fn_mqtt3, NULL);
    // while(1){
    //     mg_mgr_poll(&mgr_mqtt3, 50);
    //     if(mg_millis() - LastReconn > 5000){ // 5s
    //         LastReconn = mg_millis();
    //         if (m->s_conn == NULL) m->s_conn = mg_mqtt_connect(&mgr_mqtt1, m->szs_url, &opts, fn_mqtt3, NULL);
    //     }
    //     if(mg_millis() - LastCommCheck > 1000){ // 1s
    //         LastCommCheck = mg_millis();
    //         if( mg_millis() - gm->LastUpdate > 10000){
    //             gm->CommState = ST_COMM_ERR;
    //             strcpy(gm->szCommState,"故障");
    //         }else{
    //             gm->CommState = ST_COMM_NORM;
    //             strcpy(gm->szCommState,"正常");
    //         }
    //         if( mg_millis() - tm->LastUpdate > 10000){
    //             tm->CommState = ST_COMM_ERR;
    //             strcpy(tm->szCommState,"故障");
    //         }else{
    //             tm->CommState = ST_COMM_NORM;
    //             strcpy(tm->szCommState,"正常");
    //         }
    //     }
    // }

    // syslog(LOG_INFO,"%s EXIT, idx:1", __func__);
}

static int mqtt4_connlost = 0;
static void fn_mqtt4_connlost(void *context, char *cause)
{
    syslog(LOG_INFO, "%s, mqtt connection lost, cause: %s\n", __func__, cause);
    mqtt4_connlost = 1;
}

static int fn_mqtt4_msgarrvd(void *context, char *topicName, int topicLen, MQTTClient_message *message)
{
    // struct Dtsd1352_t* gm = &APPL.GateMeter;
    // struct Dtsd1352_t* tm = &APPL.TransMeter;
    // struct Dtsd1352_t* pm = &APPL.PvMeter;
    // struct Dtsd1352_t* cm = &APPL.CtnMeter;
    // struct chanmqtt_t* mq = &APPL.chanmqtt[4];
    // int64_t CurrIntv;

    // //syslog(LOG_INFO,"%s, Message arrived, topic:%s topic len:%d payload len:%d", __func__, topicName,topicLen, message->payloadlen);
    // struct mg_str json = mg_str(message->payload);    
    // double dval;
    // bool ok;
    // if( strcmp(topicName, mq->szs_sub_topic[0]) == 0){
    //     if(mq->LastRecv == 0){ // First Recv
    //         mq->TotalCnt = 0;
    //         mq->LastRecv = mg_millis();
    //         mq->TotalIntv = 0;
    //     }else{
    //         mq->TotalCnt++;
    //         CurrIntv = mg_millis() - mq->LastRecv;
    //         mq->LastRecv = mg_millis();
    //         if(CurrIntv > mq->MaxIntv){
    //             mq->MaxIntv = CurrIntv;
    //         }
    //         mq->TotalIntv += CurrIntv;
    //         mq->AvgIntv = mq->TotalIntv/mq->TotalCnt;
    //     }        
    //     ok = mg_json_get_num(json,"$.m1_com_ap", &dval);
    //     if( ok ){
    //         cm->com_active_p = dval;
    //         cm->LastUpdate = mg_millis();
    //         strcpy(cm->szLastUpdate, appl_get_datetime_long());
    //         //syslog(LOG_INFO,"%s, Get Gate M1 Data", __func__);
    //     }
    //     ok = mg_json_get_num(json,"$.m2_com_ap", &dval);
    //     if( ok ){
    //         pm->com_active_p = dval;
    //         pm->LastUpdate = mg_millis();
    //         strcpy(pm->szLastUpdate, appl_get_datetime_long());
    //         //syslog(LOG_INFO,"%s, Get Gate M2 Data", __func__);
    //     }
    //     ok = mg_json_get_num(json,"$.m3_com_ap", &dval);
    //     if( ok ){
    //         tm->com_active_p = dval;
    //         tm->LastUpdate = mg_millis();
    //         strcpy(tm->szLastUpdate, appl_get_datetime_long());
    //         //syslog(LOG_INFO,"%s, Get Gate M2 Data", __func__);
    //     }        
    //     ok = mg_json_get_num(json,"$.m4_com_ap", &dval);
    //     if( ok ){
    //         gm->com_active_p = dval;
    //         gm->LastUpdate = mg_millis();
    //         strcpy(gm->szLastUpdate, appl_get_datetime_long());
    //         //syslog(LOG_INFO,"%s, Get Gate M2 Data", __func__);
    //     }          
    // }
}

static void mqtt4_connect( void ){
    int rc;
    struct chanmqtt_t* m = &APPL.chanmqtt[4];
    MQTTClient_deliveryToken token;
    MQTTClient_connectOptions conn_opts;
    MQTTClient cli;

    MQTTClient_connectOptions tmpconn_opts = MQTTClient_connectOptions_initializer5;
    conn_opts = tmpconn_opts;
    MQTTClient_createOptions createOpts = MQTTClient_createOptions_initializer;
    createOpts.MQTTVersion = MQTTVERSION_5;

    if ((rc = MQTTClient_createWithOptions(&cli, m->szs_url, m->szclientid, MQTTCLIENT_PERSISTENCE_NONE, NULL, &createOpts)) != MQTTCLIENT_SUCCESS){
        syslog(LOG_INFO,"%s, MQTTClient_createWithOptions fail, rc:%d msg:%s  %s    %s", __func__, rc, MQTTClient_strerror(rc));
    }
    conn_opts.keepAliveInterval = 8;
    conn_opts.cleansession = 0;
    conn_opts.username = m->szusrname;
    conn_opts.password = m->szpasswd;

    MQTTProperties props = MQTTProperties_initializer;
    MQTTProperties willProps = MQTTProperties_initializer;
    MQTTResponse response = MQTTResponse_initializer;
    MQTTClient_setCallbacks(cli, NULL, fn_mqtt4_connlost, fn_mqtt4_msgarrvd, NULL);
    response = MQTTClient_connect5(cli, &conn_opts, &props, &willProps);
    if (response.reasonCode != MQTTCLIENT_SUCCESS){
        syslog(LOG_INFO,"%s, MQTTClient_connect fail, rc:%d msg:%s", __func__, response.reasonCode, MQTTClient_strerror(response.reasonCode));
        mqtt4_connlost = 1;
    }else{
        syslog(LOG_INFO,"%s, Connect Ok",__func__);
        mqtt4_connlost = 0;
        response = MQTTClient_subscribe5(cli, m->szs_sub_topic[0], m->s_qos, NULL, NULL);
        if (response.reasonCode != MQTTCLIENT_SUCCESS && response.reasonCode != m->s_qos){
            syslog(LOG_INFO,"%s, MQTTClient_subscribe fail, rc: %d msg: %s", __func__, response.reasonCode, MQTTClient_strerror(response.reasonCode));
        }
    }
    MQTTResponse_free(response);
}

static void* thrd_mqtt4(void* param){
    // struct Dtsd1352_t* gm = &APPL.GateMeter;
    // struct Dtsd1352_t* tm = &APPL.TransMeter;
    // struct Dtsd1352_t* pm = &APPL.PvMeter;
    // struct Dtsd1352_t* cm = &APPL.CtnMeter;
    // int ReconnChk = 0;

    // syslog(LOG_INFO,"%s, ++",__func__);
    // mqtt4_connect();
    // while(1){
    //     if(++ReconnChk > 10){
    //         ReconnChk = 0;
    //         if(mqtt4_connlost == 1){
    //             mqtt4_connect();
    //         }
    //     }
    //     if( mg_millis() - gm->LastUpdate > 15000){
    //         gm->CommState = ST_COMM_ERR;
    //         strcpy(gm->szCommState,"故障");
    //     }else{
    //         gm->CommState = ST_COMM_NORM;
    //         strcpy(gm->szCommState,"正常");
    //     }
    //     if( mg_millis() - tm->LastUpdate > 15000){
    //         tm->CommState = ST_COMM_ERR;
    //         strcpy(tm->szCommState,"故障");
    //     }else{
    //         tm->CommState = ST_COMM_NORM;
    //         strcpy(tm->szCommState,"正常");
    //     }
    //     if( mg_millis() - cm->LastUpdate > 15000){
    //         cm->CommState = ST_COMM_ERR;
    //         strcpy(cm->szCommState,"故障");
    //     }else{
    //         cm->CommState = ST_COMM_NORM;
    //         strcpy(cm->szCommState,"正常");
    //     }
    //     if( mg_millis() - pm->LastUpdate > 15000){
    //         pm->CommState = ST_COMM_ERR;
    //         strcpy(pm->szCommState,"故障");
    //     }else{
    //         pm->CommState = ST_COMM_NORM;
    //         strcpy(pm->szCommState,"正常");
    //     }        
    //     sleep(1);
    // }
    // syslog(LOG_INFO,"%s, --",__func__);
}

void appl_snap_set_err( void )
{
    APPL.Snap.bErr = 1;
    strcpy(APPL.Snap.szState,"故障");
}

void appl_snap_reset_err( void )
{
    APPL.Snap.bErr = 0;
    strcpy(APPL.Snap.szState,"正常");
}

static int appl_snap_day_diff(int year_start, int month_start, int day_start, int year_end, int month_end, int day_end){
    int y2, m2, d2;
    int y1, m1, d1;

    m1 = (month_start + 9) % 12;
    y1 = year_start - m1 / 10;
    d1 = 365 * y1 + y1 / 4 - y1 / 100 + y1 / 400 + (m1 * 306 + 5) / 10 + (day_start - 1);

    m2 = (month_end + 9) % 12;
    y2 = year_end - m2 / 10;
    d2 = 365 * y2 + y2 / 4 - y2 / 100 + y2 / 400 + (m2 * 306 + 5) / 10 + (day_end - 1);

    return (d2 - d1);
}

int appl_snap_rmdir(const char *path) {
   DIR *d = opendir(path);
   size_t path_len = strlen(path);
   int r = -1;

   if (d) {
      struct dirent *p;

      r = 0;
      while (!r && (p=readdir(d))) {
          int r2 = -1;
          char *buf;
          size_t len;

          /* Skip the names "." and ".." as we don't want to recurse on them. */
          if (!strcmp(p->d_name, ".") || !strcmp(p->d_name, ".."))
             continue;

          len = path_len + strlen(p->d_name) + 2; 
          buf = malloc(len);

          if (buf) {
             struct stat statbuf;

             snprintf(buf, len, "%s/%s", path, p->d_name);
             if (!stat(buf, &statbuf)) {
                if (S_ISDIR(statbuf.st_mode))
                   r2 = appl_snap_rmdir(buf);
                else
                   r2 = unlink(buf);
             }
             free(buf);
          }
          r = r2;
      }
      closedir(d);
   }

   if (!r)
      r = rmdir(path);

   return r;
}

static void* thrd_snap(void* param){
    struct Settings_t* set = &APPL.Set.s;
    struct Snap_t* s = &APPL.Snap;
    struct Enjoy100kW_t* pcs = &APPL.Enjoy100kW;
    struct GaoteBms_t* bms = &APPL.GaoteBms;
    struct Envicool5kW_t* ac = &APPL.Envicool5kW;
    struct Ctl_t* ctl = &APPL.Ctl;
    struct Dtsd1352_t* cm = &APPL.CtnMeter[set->CtnMeterId];
    struct Dtsd1352_t* tm = &APPL.TransMeter;
    struct Dtsd1352_t* gm = &APPL.GateMeter;
    struct Adl200_t* aum = &APPL.Adl200;
    struct FireAlarm_t* fa = NULL;
    struct Dido_t* dido = &APPL.Dido;
    char buf[128];
    char szfn[128];
    int y, m, d, h, min, ss; // current
    int yy, mm, dd;//dir
    int diff_day;
    int rc;
    DIR* dir;
    struct dirent *ptr;
    char szyy[8];
    char szmm[8];
    char szdd[8];
    int i, j;

    sleep(5);
    syslog(LOG_INFO, "%s, ++",__func__);

    while(1){
        usleep(300000);
        if(s->bErr || set->bErr){
            continue;
        }
        if (s->bStart == 0){
            appl_get_datetime_num(&y, &m, &d, &h, &min, &ss);
            // 建立与序列号对应的文件夹
            sprintf(buf, "./snap_%s", set->szSN);
            if( access(buf, NULL) != 0 ){ // directory does not exists
                if(mkdir(buf, 0755) < 0){
                    s->bErr = 1;
                    continue;
                }
            }
            // 建立与日期对应的文件夹
            sprintf(s->szcurrDatePath, "./snap_%s/%04d-%02d-%02d", set->szSN, y, m, d);
            if( access(s->szcurrDatePath, NULL) != 0 ){ // directory does not exists
                if(mkdir(s->szcurrDatePath, 0755) < 0){
                    s->bErr = 1;
                    continue;
                }
            }
            // PCS Snap Start
            sprintf(szfn, "%s/PCS_%04d-%02d-%02d %02d-%02d-%02d.csv",s->szcurrDatePath, y, m, d, h, min, ss);
            s->fpcs = fopen(szfn, "w+");
            if (s->fpcs == NULL){
                s->bErr = 1;
                continue;
            }else{
                rc = fprintf(s->fpcs,"ts,Uab,Ubc,Uca,Ia,Ib,Ic,Ap,Rap,BatV,BatC,Tigbt,Tenv,Tind,WorkState,ErrState,Aps,TotalBusVolt,CommState,\
HwFault1,HwFault2,GridFault,BusFault,AcCapFault,SysFault,OnOffFault,OtherFault\n");
                if(rc < 0){
                    s->bErr = 1;
                    continue;
                }
            }
            // BMS Snap Start
            sprintf(szfn, "%s/BMS_%04d-%02d-%02d %02d-%02d-%02d.csv",s->szcurrDatePath, y, m, d, h, min, ss);
            s->fbms = fopen(szfn, "w+");
            if (s->fbms == NULL){
                s->bErr = 1;
                continue;
            }else{
                rc = fprintf(s->fbms,"ts,CommState,BatV,BatI,Soc,Soh,BatState,PosRes,NegRes,\
MaxCellV,AvgCellV,MinCellV,MaxCellT,AvgCellT,MinCellT,\
CellVDiff,CellTDiff,\
HvState,MaxCellVIdx,MinCellVIdx,MaxCellTIdx,MinCellTIdx,\
MaxChgCurr,MaxDhgCurr\n");
                if(rc < 0){
                    s->bErr = 1;
                    continue;
                }
            }            
            // PACK Snap Start
            for( i = 1; i <= PACK_NBR; i++ ){
                sprintf(szfn, "%s/PACK%d_%04d-%02d-%02d %02d-%02d-%02d.csv",s->szcurrDatePath, i, y, m, d, h, min, ss);
                s->fpack[i] = fopen(szfn, "w+");
                if (s->fpack[i] == NULL){
                    s->bErr = 1;
                    continue;
                }else{
                    rc = fprintf(s->fpack[i],"ts,");
                    if(rc < 0){
                        s->bErr = 1;
                        continue;
                    }
                    for( j = 1; j <= PACK_CELL_NBR; j++){
                        rc = fprintf(s->fpack[i], "v%d,",j);
                        if(rc < 0){
                            s->bErr = 1;
                            continue;
                        }
                    }
                    for( j = 1; j <= PACK_CELL_NBR; j++){
                        rc = fprintf(s->fpack[i], "t%d,",j);
                        if(rc < 0){
                            s->bErr = 1;
                            continue;
                        }
                    }         
                    rc = fprintf(s->fpack[i],"CommState\n");
                    if(rc < 0){
                        s->bErr = 1;
                        continue;
                    }              
                }
            }
            // FireAlarm Snap Start
            sprintf(szfn, "%s/FA_%04d-%02d-%02d %02d-%02d-%02d.csv",s->szcurrDatePath, y, m, d, h, min, ss);
                s->ffa = fopen(szfn, "w+");
                if (s->ffa == NULL){
                    s->bErr = 1;
                    continue;
                }else{
                    rc = fprintf(s->ffa, "ts,");
                    for( i = 1; i <= PACK_NBR; i++){
                        fprintf(s->ffa,"Fa%d_CommState,Fa%d_t1,Fa%d_t2,Fa%d_co,Fa%d_voc,Fa%d_ErrCode,Fa%d_SmokeFlag,Fa%d_Level,",
                                        i,i,i,i,i,i,i,i);
                    }
                    fprintf(s->ffa,"dummy\n");
                }
            // AC Snap Start
            sprintf(szfn, "%s/AC_%04d-%02d-%02d %02d-%02d-%02d.csv",s->szcurrDatePath, y, m, d, h, min, ss);
            s->fac = fopen(szfn, "w+");
            if (s->fac == NULL){
                s->bErr = 1;
                continue;
            }else{
                rc = fprintf(s->fac,"ts,SetTemp,SetMode,CoolTemp,CoolGap,HeatTemp,HeatGap,\
WorkMode,OutWaterTemp,InWaterTemp,OutWaterPre,InWaterPre,\
EnvTemp,ErrCode,ErrLevel,\
CompState,CompHeatStripState,ElecHeatState,PumpState,\
Fan1State,Fan2State,Fan3State,CompRpm,PumpRpm,\
CommState,CtlMode\n");
                if(rc < 0){
                    s->bErr = 1;
                    continue;
                }
            }   
            // **************************                 
            // CTL Snap Start
            // **************************
            sprintf(szfn, "%s/CTL_%04d-%02d-%02d %02d-%02d-%02d.csv",s->szcurrDatePath, y, m, d, h, min, ss);
            s->fctl = fopen(szfn, "w+");
            if (s->fctl == NULL){
                s->bErr = 1;
                continue;
            }else{
                rc = fprintf(s->fctl,"ts,State,Step,WorkMode,Err,Cmd,bChgAble,bDhgAble,\
Ap,GmAp,TransAp,CtnMeterAp,ChgGateLim,ChgTransLim,DhgGateLim,DhgTransLim\n");
                if(rc < 0){
                    s->bErr = 1;
                    continue;
                }
            }                     
            // MISC Snap Start
            sprintf(szfn, "%s/MISC_%04d-%02d-%02d %02d-%02d-%02d.csv",s->szcurrDatePath, y, m, d, h, min, ss);
            s->fmisc = fopen(szfn, "w+");
            if (s->fmisc == NULL){
                s->bErr = 1;
                continue;
            }else{
                rc = fprintf(s->fmisc,"ts,GmAp,TmAp,\
AuxmAp,AuxmPosAe,AuxmNegAe,AuxmCommState,\
DhTemp,DhHumi,DhCommState,\
CoDensity,CoFlag,CoCommState,\
DidoWaterDec1,DidoWaterDec2,DidoEmgStop,DidoFrontDoor,DidoBackDoor,FeEruptFb,DidoLedMode\n");
                if(rc < 0){
                    s->bErr = 1;
                    continue;
                }
            }                   
            s->LastSnap = 0;
            s->bStart = 1;
        }else{
            if (mg_millis() - s->LastSnap > 5000){ /* snap every 5 seconds */
                s->LastSnap = mg_millis();
                appl_get_datetime_num(&y, &m, &d, &h, &min, &ss);
                sprintf(buf, "./snap_%s/%04d-%02d-%02d", set->szSN, y, m, d);
                if (strcmp(buf, s->szcurrDatePath) != 0){ /* New Date or New SN*/
                    //syslog(LOG_INFO,"%s, New Date Or New SN, Detected", __func__);
                    if(s->fpcs != NULL){
                        fclose(s->fpcs);
                        s->fpcs = NULL;
                    }
                    //syslog(LOG_INFO,"%s, pcs Closed", __func__);
                    if(s->fbms != NULL){
                        fclose(s->fbms);
                        s->fbms = NULL;
                    }
                    //syslog(LOG_INFO,"%s, bms Closed", __func__);
                    for( i = 1; i <= PACK_NBR; i++ ){
                        if(s->fpack[i] != NULL){
                            fclose(s->fpack[i]);
                            s->fpack[i] = NULL;
                        }
                    }
                    //syslog(LOG_INFO,"%s, pack Closed", __func__);
                    if(s->ffa != NULL){
                        fclose(s->ffa);
                        s->ffa = NULL;
                    }
                    //syslog(LOG_INFO,"%s, ffa Closed", __func__);
                    if(s->fac != NULL){
                        fclose(s->fac);
                        s->fac = NULL;
                    }
                    //syslog(LOG_INFO,"%s, ac Closed", __func__);
                    if(s->fctl != NULL){
                        fclose(s->fctl);
                        s->fctl = NULL;
                    }
                    //syslog(LOG_INFO,"%s, ctl Closed", __func__);
                    if(s->fmisc != NULL){
                        fclose(s->fmisc);
                        s->fmisc = NULL;
                    }                    
                    //syslog(LOG_INFO,"%s, misc Done", __func__);
                    // del outofdate dir
                    sprintf(buf, "./snap_%s", set->szSN);
                    if ((dir = opendir(buf)) == NULL){
                        //s->bErr = 1;
                        //syslog(LOG_INFO,"%s, opendir : %s to del Fail, Maybe New SN", __func__, buf);
                        //continue; // Maybe New SN
                    }else{
                        //syslog(LOG_INFO,"%s, Begin To Del Outofdate File", __func__);
                        while ((ptr = readdir(dir)) != NULL){
                            if (strcmp(ptr->d_name, ".") == 0 || strcmp(ptr->d_name, "..") == 0){ /// current dir OR parrent dir
                                continue;
                            }else if (ptr->d_type == 8){ /// file
                            }
                            else if (ptr->d_type == 10){ /// link file
                                // printf("d_name:%s/%s\n",basePath,ptr->d_name);
                            }else if (ptr->d_type == 4){ /// dir
                                if(strlen(ptr->d_name) == 10 && ptr->d_name[4] == '-' && ptr->d_name[7] == '-'){ // target dir
                                    //syslog(LOG_INFO,"%s, Target Dir:%s Detedted", __func__, ptr->d_name);
                                    strncpy(szyy, ptr->d_name, 4);
                                    strncpy(szmm, ptr->d_name + 5, 2);
                                    strncpy(szdd, ptr->d_name + 8, 2);
                                    yy = atoi(szyy);
                                    mm = atoi(szmm);
                                    dd = atoi(szdd);
                                    diff_day = appl_snap_day_diff(yy, mm, dd, y, m, d);
                                    if (diff_day > set->DataKeepDay){
                                        sprintf(buf, "./snap_%s/%s", set->szSN, ptr->d_name);
                                        rc = appl_snap_rmdir(buf);
                                        if( rc < 0 ){
                                            syslog(LOG_INFO,"%s, Target Dir:%s Del Fail", __func__, ptr->d_name);
                                            s->bErr = 1;
                                            continue;
                                        }else{
                                            syslog(LOG_INFO,"%s, Target Dir:%s Del Ok", __func__, ptr->d_name);
                                        }
                                    }
                                }
                            }
                        }
                        closedir(dir);
                    }
                    s->bStart = 0; /* start again at next loop */
                    continue;
                }else{
                    s->LastSnap = mg_millis();
                    strcpy(buf, appl_get_datetime_short());
        
                // PCS Snap
                // HwFault1,HwFault2,GridFault,BusFault,AcCapFault,SysFault,OnOffFault,OtherFault
                if(s->fpcs != NULL && pcs->CommState == ST_COMM_NORM){
                    fprintf(s->fpcs,"%s,%.1f,%.1f,%.1f,%.1f,%.1f,%.1f,\
%.1f,%.1f,%.1f,%.1f,\
%.1f,%.1f,%.1f,\
%d,%d,%.1f,%.1f,%d,\
%u,%u,%u,%u,%u,%u,%u,%u\n",
buf, pcs->Uab,pcs->Ubc,pcs->Uca,pcs->Ia,pcs->Ib,pcs->Ic,
pcs->Ap,pcs->Rap,pcs->BatV,pcs->BatC,
pcs->Tigbt,pcs->Tenv,pcs->Tind,
pcs->WorkState,pcs->ErrState,pcs->Aps,pcs->TotalBusVolt,pcs->CommState,
pcs->HwFault1,pcs->HwFault2,pcs->GridFault,pcs->BusFault,pcs->AcCapFault,pcs->SysFault,pcs->OnOffFault,pcs->OtherFault);
                    fflush(s->fpcs);
                    }

                // BMS Snap
                if(s->fbms != NULL && bms->CommState == ST_COMM_NORM){
                    fprintf(s->fbms,
"%s,%d,%.1f,%.1f,%.1f,%.1f,%d,%.1f,%.1f,\
%.3f,%.3f,%.3f,%.1f,%.1f,%.1f,\
%.1f,%.1f,\
%d,%d,%d,%d,%d,\
%.1f,%.1f\n",
buf,bms->CommState, bms->BatV,bms->BatI,bms->Soc,bms->Soh,bms->BatState,bms->PosRes,bms->NegRes,
bms->MaxCellV,bms->AvgCellV,bms->MinCellV,bms->MaxCellT,bms->AvgCellT,bms->MinCellT,
bms->CellVDiff,bms->CellTDiff,
bms->HvState,bms->MaxCellVIdx,bms->MinCellVIdx,bms->MaxCellTIdx,bms->MinCellTIdx,
bms->MaxChgCurr,bms->MaxDhgCurr);
                    fflush(s->fbms);
                    }      
                // PACK Snap
                for( i = 1; i <= PACK_NBR; i++){
                    if(s->fpack[i] != NULL && bms->CommState == ST_COMM_NORM ){
                        fprintf(s->fpack[i],"%s,",buf);
                        for( j = 1; j <= PACK_CELL_NBR; j++){
                            fprintf(s->fpack[i],"%f,",bms->CellVolt[i][j]);
                        }
                        for( j = 1; j <= PACK_CELL_NBR; j++){
                            fprintf(s->fpack[i],"%f,",bms->CellTemp[i][j]);
                        }
                        fprintf(s->fpack[i],"%d\n",bms->CommState);
                    }
                }     
                // FireAlarm Snap
                if(s->ffa != NULL){
                    fprintf(s->ffa,"%s,",buf);
                    for( i = 1; i <= PACK_NBR; i++){
                        fa = &APPL.Fa[i];
                        fprintf(s->ffa,"%d,%d,%d,%d,%d,%d,%d,%d,",
                        fa->CommState,fa->T1,fa->T2,fa->Co,fa->Voc,fa->ErrCodeVal,fa->SmokeFlagVal,fa->LevelVal);
                    }
                    fprintf(s->ffa,"%d\n",0);
                    fflush(s->ffa);
                }
                // AC Snap
                if(s->fac != NULL && ac->CommState == ST_COMM_NORM){
                    fprintf(s->fac,
"%s,%d,%d,%d,%d,%d,%d,\
%d,%d,%d,%.1f,%.1f,\
%d,%d,%d,\
%d,%d,%d,%d,\
%d,%d,%d,%d,%d,\
%d,%d\n",
buf, ac->SetTemp,ac->SetMode,set->CoolTemp,set->CoolGap,set->HeatTemp,set->HeatGap,
ac->WorkMode,ac->OutWaterTemp,ac->InWaterTemp,ac->OutWaterPre,ac->InWaterPre,
ac->EnvTemp,ac->ErrCode,ac->ErrLevel,
ac->CompState,ac->CompHeatStripState,ac->ElecHeatState,ac->PumpState,
ac->Fan1State,ac->Fan2State,ac->Fan3State,ac->CompRpm,ac->PumpRpm,
ac->CommState,ac->CtlMode);
                    fflush(s->fac);
                }               
// Ap,GmAp,TransAp,CtnMeterAp,ChgGateLim,ChgTransLim,DhgGateLim,DhgTransLim
                // CTL Snap
                if(s->fctl != NULL){
                    fprintf(s->fctl,
"%s,%d,%d,%d,%d,%d,%d,%d,\
%d,%d,%d,%d,%d,%d,%d,%d\n",
buf, ctl->State,ctl->Step,ctl->WorkMode,ctl->Err,ctl->Cmd,ctl->bChgAble,ctl->bDhgAble,
ctl->Ap,(int)gm->com_active_p, (int)tm->com_active_p,(int)APPL.CtnMeter[set->CtnMeterId].com_active_p,set->ChgGateLim,set->ChgTransLim,set->DhgGateLim,set->DhgTransLim);
                    fflush(s->fctl);
                    }       
                // MISC Snap
                if(s->fmisc != NULL){
                    fprintf(s->fmisc,
"%s,%.1f,%.1f,\
%.1f,%.1f,%.1f,%d,\
%.1f,%.1f,%d,\
%d,%d,%d,\
%d,%d,%d,%d,%d,%d,%d\n",
/*1*/buf, gm->com_active_p, tm->com_active_p,
/*2*/aum->Ap, aum->PosAe, aum->NegAe, aum->CommState,
/*3*/APPL.Dehumi.Temp,APPL.Dehumi.Humi,APPL.Dehumi.CommState,
/*4*/APPL.Co.Density,APPL.Co.Flag,APPL.Co.CommState,
/*5*/dido->WaterDec1, dido->WaterDec2, dido->EmgStop, dido->FrontDoor, dido->BackDoor, dido->FeEruptFb, dido->LedMode);
                    fflush(s->fmisc);
                }                                                                  
                }
            }
        }
    }
    syslog(LOG_INFO, "%s, --",__func__);
}

void appl_start( void )
{
    pthread_t hthrd_485_1;
    pthread_t hthrd_485_2;
    pthread_t hthrd_485_3;
    pthread_t hthrd_485_4;
    pthread_t hthrd_can_1;
    pthread_t hthrd_can_2;
    pthread_t hthrd_dido;
    pthread_t hthrd_ctl;
    pthread_t hthrd_mqtt1;
    pthread_t hthrd_mqtt2;
    pthread_t hthrd_mqtt3;
    pthread_t hthrd_mqtt4;
    pthread_t hthrd_snap;

    struct Settings_t* set = &APPL.Set.s;
    struct Ctl_t* ctl = &APPL.Ctl;
    struct chan485_t* c1 = &APPL.chan485[1];
    struct Enjoy100kW_t* pcs = &APPL.Enjoy100kW;
    struct GaoteBms_t* bms = &APPL.GaoteBms;
    struct Envicool5kW_t* ac = &APPL.Envicool5kW;
    struct chanmqtt_t* m = NULL;
    struct Snap_t* snap = &APPL.Snap;
    char buf[128];
    char buf2[128];
    int len;
    int i;    

    syslog(LOG_INFO,"%s, ++", __func__);

    // Set
    if(appl_cfg_read() != 0){
        appl_cfg_set_err();
        syslog(LOG_INFO,"%s, appl_cfg_read fail", __func__);

        // Set Default Settings
        strcpy(set->szSN,"DefaultSN");
        set->ChgCellV = 3.50;
        set->DhgCellV = 2.80;

        set->DataKeepDay = 90;

        set->UploadHighSpeed = 5000; 
        set->UploadMediumSpeed = 60000;
        set->UploadSlowSpeed = 180000;

        // init AC Cfg
        set->HeatTemp = 15;
        set->HeatGap = 3;
        set->CoolTemp = 23;
        set->CoolGap = 3;    
        set->CoolTempSet = 18;
        set->HeatTempSet = 25;        

        // init Snap Cfg
        set->DataKeepDay = 90;
    }else{
        syslog(LOG_INFO,"%s, appl_cfg_read Ok", __func__);
        appl_cfg_reset_err();

        // //1# MQTT thingsboard
        // m = &APPL.chanmqtt[1];
        // m->s_conn = NULL;
        // strcpy(m->szs_url,"124.222.45.156:1883");
        // strcpy(m->szs_pub_topic,"v1/devices/me/telemetry");
        // strcpy(m->szs_sub_topic,"v1/devices/me/ctl");
        // m->s_qos = 1;
        // //测试设备
        // //strcpy(m->szusrname,"gFCNk8oSxC6VlYXkhs3a");
        // strcpy(m->szusrname,"DZHbY2HAGeATfRCfhlW7");
        // m->bConnected = 0;

        //2# MQTT Cloud
        m = &APPL.chanmqtt[2];
        m->s_conn = NULL;
        strncpy(m->szusrname,set->szCloudUserName, sizeof(m->szusrname) - 1);
        strncpy(m->szpasswd,set->szCloudPasswd, sizeof(m->szpasswd) - 1);        
        strncpy(m->szs_url, set->szCloudUrl, sizeof(m->szs_url)-1);
        strncpy(m->szclientid, set->szClientId, sizeof(m->szclientid)-1);
        sprintf(m->szs_pub_topic,"sequential/%s",m->szusrname);
        sprintf(m->szs_sub_topic[0],"control");
        sprintf(m->szs_sub_topic[1],"sequential/%s/%s",m->szusrname, "GateMeter");
        sprintf(m->szs_sub_topic[2],"sequential/%s/TransMeter%d",m->szusrname, set->TransId);
        for( i = 0; i < set->CtnMeterNbr; i++ ){
            sprintf(m->szs_sub_topic[3 + i],"sequential/%s/CtnMeter%d",m->szusrname, i + 1);  
        }  
        m->s_qos = 1;
        m->bConnected = 0;    
        m->LastRecv = 0;
        m->TotalReconnCnt = 0;

        // // 3# MQTT mosquitto server
        // m = &APPL.chanmqtt[3];
        // m->s_conn = NULL;
        // strcpy(m->szs_url,"119.45.101.222:1883");
        // strcpy(m->szs_pub_topic,"qcd215");
        // strcpy(m->szs_sub_topic[0],"zh_gate_m"); 
        // //strcpy(m->szs_sub_topic[1],"zh_testarea_m"); 
        // m->s_qos = 1;
        // m->bConnected = 0;    

        // // 4# MQTT EMQ server by paho
        // // 获取关口表等电表的数据
        // m = &APPL.chanmqtt[4];
        // m->s_conn = NULL;
        // strcpy(m->szs_url,"119.45.116.112:18883");
        // strcpy(m->szusrname,"bms");
        // strcpy(m->szpasswd,"KkK1iA71");
        // //strcpy(m->szclientid,APPL.Set.s.szSN);
        // strcpy(m->szclientid,"ZHES233230008");
        // strcpy(m->szs_pub_topic,"duolengduo_ctn");
        // strcpy(m->szs_sub_topic[0],"duolengduo_meter"); 
        // m->s_qos = 1;
        // m->bConnected = 0;       
        // m->MaxIntv = 0;
        // m->AvgIntv = 0;
        // m->LastRecv = 0;             

        pthread_create(&hthrd_mqtt2, NULL, thrd_mqtt_2, NULL);
        //pthread_create(&hthrd_mqtt3, NULL, thrd_mqtt_3, NULL);
        //pthread_create(&hthrd_mqtt4, NULL, thrd_mqtt4, NULL);        
    }

    appl_485_set_485mode();
    // CHAN 485 1
    strcpy(APPL.chan485[1].szdev, "/dev/ttymxc1");
    APPL.chan485[1].baud = 9600;
    APPL.chan485[1].parity = 'N';
    strcpy(APPL.chan485[1].szinfo, "PCS 辅助电表");
    // CHAN 485 2
    strcpy(APPL.chan485[2].szdev, "/dev/ttymxc2");
    APPL.chan485[2].baud = 9600;
    APPL.chan485[2].parity = 'N';
    strcpy(APPL.chan485[2].szinfo, "BMS");
    // CHAN 485 3
    strcpy(APPL.chan485[3].szdev, "/dev/ttymxc3");
    APPL.chan485[3].baud = 9600;
    APPL.chan485[3].parity = 'N';
    strcpy(APPL.chan485[3].szinfo, "未使用");
    // CHAN 485 4
    strcpy(APPL.chan485[4].szdev, "/dev/ttymxc5");
    APPL.chan485[4].baud = 9600;
    APPL.chan485[4].parity = 'N';
    strcpy(APPL.chan485[4].szinfo, "一氧化碳 除湿机 消防主机");

    // CHAN CAN 1
    strcpy(APPL.chancan[1].szdev, "can0");
    strcpy(APPL.chancan[1].szinfo, "电池包火灾探测器");
    // CHAN CAN 2
    strcpy(APPL.chancan[2].szdev, "can1");
    strcpy(APPL.chancan[2].szinfo, "液冷空调");

    // init PCS
    pcs->Adr = 1;
    pcs->LastUpdate = 0;
    pcs->CommFailTotalCnt = 0;

    // init BMS
    bms->Adr = 1;
    bms->LastUpdate = 0;
    bms->CommFailTotalCnt = 0;

    // init Envicool5kW
    ac->LastUpdate1 = 0;
    ac->LastUpdate2 = 0;
    appl_ac_set_ctlmod(AC_CTLMOD_EMS);

    // init AUXM
    APPL.Adl200.Adr = 7;
    APPL.Adl200.LastUpdate = 0;
    APPL.Adl200.CommFailTotalCnt = 0;

    //Dehumi
    APPL.Dehumi.Adr = 1;

    //Co
    APPL.Co.Adr = 5;

    //Dido
    appl_dido_set_led(LEDMODE_WHITE);

    //CTL
    ctl->bChgAble = 1;
    ctl->bDhgAble = 1;
    ctl->Cmd = CTL_CMD_DONE;
    appl_ctl_set_workmode(CTL_WORKMODE_SLAVE);
  

    // Snap
    appl_snap_reset_err();
    snap->bStart = 0;

    pthread_create(&hthrd_485_1, NULL, thrd_485_1, NULL);
    pthread_create(&hthrd_485_2, NULL, thrd_485_2, NULL);
    pthread_create(&hthrd_485_3, NULL, thrd_485_3, NULL);
    pthread_create(&hthrd_485_3, NULL, thrd_485_4, NULL);
    pthread_create(&hthrd_can_1, NULL, thrd_can1, NULL);
    pthread_create(&hthrd_can_2, NULL, thrd_can2, NULL);
    pthread_create(&hthrd_dido, NULL, thrd_dido, NULL);
    pthread_create(&hthrd_ctl, NULL, thrd_ctl, NULL);
    pthread_create(&hthrd_snap, NULL, thrd_snap, NULL);
}
