#ifndef __APPL_H__
#define __APPL_H__

#define PACK_CELL_NBR 52
#define PACK_NBR 5

enum CommState_t
{
    ST_COMM_ERR = 0,
    ST_COMM_NORM = 1,
};

enum chan485_state_t
{
    ST_485_INIT = 0,
    ST_485_RUN,
    ST_485_ERR,
};

enum chan485_err_t{
    ERR_485_NONE = 0,
    ERR_485_INIT_FAIL,
};

enum chan485_cmd_t
{
    CMD_485_DONE = 0,
    CMD_485_RESET = 1,
    CMD_485_PCS_START = 2,
    CMD_485_PCS_STOP = 3,
    CMD_485_PCS_SET_APS = 4,
    CMD_485_AC_SET_ONOFF = 5,
    CMD_485_AC_SET_MODESET = 6,
    CMD_485_AC_SET_CTLTEMPSEL = 7,
    CMD_485_AC_SET_COOLTEMP = 8,
    CMD_485_AC_SET_COOLGAP = 9,
    CMD_485_AC_SET_HEATTEMP = 10,
    CMD_485_AC_SET_HEATGAP = 11,
    CMD_485_AC_SET_TEMP = 12,

    CMD_485_FE_PACK_ERUPT,
    CMD_485_FE_CABINET_ERUPT,
    CMD_485_FE_RESET_ALARM,
};

#define CHAN485_NBR 8
struct chan485_t{
    pthread_mutex_t mutex;
    char szinfo[128];
    int state;
    char szstate[32];
    int err;
    char szerr[64];
    int Cmd;
    int CmdParam;

    int baud;
    char szdev[32];
    char parity;
    int64_t reqcnt;
    int64_t failcnt;
    int64_t loopcnt;
    int64_t looptime;
};

enum chancan_state_t
{
    ST_CAN_INIT = 1,
    ST_CHANCAN_RUN,
    ST_CHANCAN_ERR,
};

enum chancan_err_t{
    ERR_CAN_NONE = 0,
    ERR_CAN_INIT_FAIL,
};

enum chancan_cmd_t
{
    CMD_CHANCAN_DONE = 0,
    CMD_CHANCAN_RESET = 1,
};

enum dido_state_t
{
    ST_DIDO_INIT = 1,
    ST_DIDO_RUN,
    ST_DIDO_ERR,
};

enum dido_led_mode_t{
    LEDMODE_OFF = 0,
    LEDMODE_WHITE,
    LEDMODE_RED,
    LEDMODE_GREEN,
    LEDMODE_BLUE,
};

enum dido_errcode_t{
    ERR_DIDO_NONE = 0,
    ERR_DIDO_INIT_FAIL,
    ERR_DIDO_READ_FAIL,
    ERR_DIDO_WRITE_FAIL,
};

enum dido_cmd_t{
    CMD_DIDO_DONE = 0,
    CMD_DIDO_SET_LEDMOD,
    CMD_DIDO_FE_CONFIRM,
};

struct Dido_t{
    int State;
    char szState[32];
    int ErrCode;
    char szErrCode[128];
    int Cmd;
    int CmdParam;

    int WaterDec1;
    char szWaterDec1[32];
    int WaterDec2;
    char szWaterDec2[32];
    int EmgStop;
    char szEmgStop[32];
    int FrontDoor;
    char szFrontDoor[32];
    int BackDoor;
    char szBackDoor[32];
    int LedMode;
    char szLedMode[32];
    int64_t LastUpdate;
    char szLastUpdate[32];

    int FeEruptFb;
    char szFeEruptFb[64];
};

#define CHANCAN_NBR 3
struct chancan_t{
    int State;
    char szState[32];
    int Err;
    char szErr[128];
    int Cmd;
    int sock;
    char szdev[32];
    int64_t RdCnt;
    int64_t RdFailcnt;
    int64_t WrCnt;
    int64_t WrFailcnt;
    int64_t Loopcnt;
    int64_t LoopTime;
    char szinfo[128];

    pthread_mutex_t mutex;
};

struct Dtsd1352_t{
    char szinfo[128];
    int Adr;

    double com_active_e;
    double com_active_p;
    double pos_active_e;
    double pos_active_dem;
    double neg_active_e;
    double pwr_factor;

    int PT;
    int CT;

    double ua;
    double ub;
    double uc;
    double ia;
    double ib;
    double ic;
    double freq;

    int64_t LastUpdate;
    char szLastUpdate[32];
    int CommState;
    char szCommState[32]; 
    int64_t CommFailTotalCnt;    
};

#define CHAN_MQTT_NBR 4
struct chanmqtt_t{
    char szs_url[128];
    // 0 : Control
    // 1 : CtnMeter
    // 2 : PvMeter
    // 3 : TransMeter
    // 4 : GateMeter
    char szs_sub_topic[16][128];     
    char szs_pub_topic[128];
    char szusrname[128];
    char szpasswd[128];
    char szclientid[128];
    int s_qos;                             // MQTT QoS
    struct mg_connection* s_conn;              // Client connection
    char szState[64];
    int bConnected;
    int64_t LastFastUpload;
    int64_t LastMediumUpload;
    int64_t LastSlowUpload;

    int Cmd;
    int CmdParam;

    int MaxRecvIntv;
    int AvgRecvIntv;
    int64_t TotalRecvIntv;
    int64_t TotalRecvCnt;
    int64_t TotalSendCnt;
    int64_t LastRecv;    
    int64_t TotalReconnCnt;
};

enum MqttCmd_t{
    CMD_MQTT_DONE = 0,
    CMD_MQTT_REGISTER = 1,
};

struct Enjoy100kW_t{
    int Adr;
    double Uab;
    double Ubc;
    double Uca;
    double Ia;
    double Ib;
    double Ic;
    double Ap;
    double Rap;
    double BatV;
    double BatC;
    double Tigbt;
    double Tenv;
    double Tind;
    int WorkState;
    char szWorkState[16];
    int ErrState;
    char szErrState[16];
    double Aps;

    unsigned short HwFault1;
    char szHwFault1[512];
    unsigned short HwFault2;
    char szHwFault2[512];    
    unsigned short GridFault;
    char szGridFault[512];
    unsigned short BusFault;
    char szBusFault[512];
    unsigned short AcCapFault;
    char szAcCapFault[512];
    unsigned short SysFault;
    char szSysFault[512];
    unsigned short OnOffFault;
    char szOnOffFault[512];
    unsigned short OtherFault;
    char szOtherFault[512];

    double TotalBusVolt;

    int64_t LastUpdate;
    char szLastUpdate[32];
    int CommState;
    char szCommState[16];
    int64_t CommFailTotalCnt;
};

struct GaoteBms_t{
    int Adr;
    double BatV;
    double BatI;
    double Soc;
    double Soh;
    double PosRes;
    double NegRes;
    double BatState;
    char szBatState[64];
    ushort DI;
    int HvState;
    char szHvState[32];
    double MaxCellT;
    double MinCellT;
    double AvgCellT;
    double MaxCellV;
    double MinCellV;
    double AvgCellV;
    double CellVDiff;
    double CellTDiff;

    int MaxCellTIdx;
    int MinCellTIdx;
    int MaxCellTModIdx;
    int MinCellTModIdx;
    int MaxCellVIdx;
    int MinCellVIdx;
    int MaxCellVModIdx;
    int MinCellVModIdx;

    double MaxChgCurr;
    double MaxDhgCurr;

    int bChgNotAllowed;
    int bDhgNotAllowed;
    int bTotalFatalErr;
    int bWarning;

    int bTotalOv; // 组端过压
    int bTotalUv; //组端欠压
    int bTotalDhgOc;
    int bTotalChgOc;
    int bTotalResWarn; // 组端绝缘
    int bCellOt;
    int bCellUt;
    int bCellOv;
    int bCellUv;
    int bCellVDiffErr;
    int bCellTDiffErr;
    int bSocLowErr;
    int bPowerPackOtErr;
    int bPackOv;
    int bPackUv;
    int bDI1Err;
    int bDI2Err;
    int bDI3Err;
    int bDI4Err;
    int bDI5Err;
    int bDI6Err;
    int bDI7Err;
    int bDI8Err;
    int bMasterSlaveCommErr; // 主从内网通讯失联
    int bCellVoltDacErr; // 单体电压采集故障
    int bCellTempDacErr; // 单体温度采集故障
    int bJumpErr; //跳机故障
    int bBatLimErr; //电池极限故障

    double CellVolt[8 + 1][52 + 1];
    double CellTemp[8 + 1][51 + 1];

    char szErrMsg[512];

    int64_t LastUpdate;
    char szLastUpdate[32];
    int CommState;
    char szCommState[16];
    int64_t CommFailTotalCnt;
};

enum AcCmd_t{
    AC_CMD_ONOFF = 0,
    AC_CMD_MODESET = 1,
    AC_CMD_CTLTEMPSEL = 2,
    AC_CMD_SET_COOLTEMP = 3,
    AC_CMD_SET_COOLGAP = 4,
    AC_CMD_SET_HEATTEMP = 5,
    AC_CMD_SET_HEATGAP = 6,
    AC_CMD_SET_TEMP = 7,
    AC_CMD_SET_CTLMOD = 8,
    AC_CMD_SET_HEATTEMPSET = 9,
    AC_CMD_SET_COOLTEMPSET = 10,
};

enum FeCmd_t{
    FE_CMD_PACK_ERUPT = 0,
    FE_CMD_CABINET_ERUPT = 1,
    FE_CMD_ERUPT_CONFIRM = 2,
    FE_CMD_RESET_ALARM = 3,
    FE_CMD_LOCK = 4,
};

enum PcsCmd_t{
    PCS_CMD_START = 0,
    PCS_CMD_STOP = 1,
    PCS_CMD_SET_APS = 2,
};

enum AcCtlMode_t{
    AC_CTLMOD_EMS = 0,
    AC_CTLMOD_NON_EMS = 1,
};

struct Envicool5kW_t{
    int Step;
    int64_t LastUpdate1;
    char szLastUpdate1[32];
    int SetTemp; // Manual Set
    int SetMode; // Manual Set

    int SetHeatCtlGap;
    int WorkMode;
    char szWorkMode[32];
    int OutWaterTemp;
    int InWaterTemp;
    double OutWaterPre;
    double InWaterPre;
    int EnvTemp;
    int ErrCode;
    int ErrLevel;
    int64_t LastUpdate2;
    char szLastUpdate2[32];
    int CompState;
    char szCompState[16];
    int CompHeatStripState;
    int ElecHeatState;
    char szElecHeatState[16];
    int PumpState;
    char szPumpState[16];
    int Fan1State;
    char szFan1State[64];
    int Fan2State;
    char szFan2State[64];
    int Fan3State;
    char szFan3State[64];
    int CompRpm;
    int PumpRpm;

    char szErrMsg1[512]; // 0x18068040
    char szErrMsg2[2048]; // 0x18078040
    char szErrMsg3[1024]; // 0x18088040

    int64_t MaxUpdate1Intv;
    int64_t MaxUpdate2Intv;

    int CommState;
    char szCommState[16];

    int CtlMode;
    char szCtlMode[32];
};

struct Adl200_t{
    int Adr;
    double Volt;
    double Curr;
    double Ap;
    double Rap;
    double Pf;
    double Gf;
    double PosAe;
    double NegAe;

    int64_t LastUpdate;
    char szLastUpdate[32];
    int CommState;
    char szCommState[32]; 
    int64_t CommFailTotalCnt;
};

// 及安盾 QRR0.15G/S-PFK热气溶胶灭火装置
struct FireEngine_t{
    int EruptFb; 
    char szEruptFb[64];

    int64_t LastUpdate;
    char szLastUpdate[32];
    int CommState;
    char szCommState[32]; 
    int64_t CommFailTotalCnt;    
};

struct FireAlarm_t{
    int T1;
    int T2;
    int Co;
    int Voc; // mV
    int SmokeFlagVal;
    int LevelVal;
    int ErrCodeVal;
    char szSmokeFlag[64];
    char szLevel[64];
    char szErrCode[64];

    int64_t LastUpdate;
    int CommState;
    char szCommState[32]; 
};

struct Dehumi_t{
    int64_t LastUpdate;
    char szLastUpdate[32];
    int Adr;
    double Temp;
    double Humi;

    int CommState;
    char szCommState[32]; 
    int64_t CommFailTotalCnt;
};

struct Co_t{
    int64_t LastUpdate;
    char szLastUpdate[32];
    int Adr;
    unsigned short Density;
    unsigned char Flag;
    char szFlag[16];

    int CommState;
    char szCommState[32]; 
    int64_t CommFailTotalCnt;
};

// !!! 注意，不要插入
struct Settings_t{
    long long chksum;
    char szSN[128];
    double ChgCellV;
    double DhgCellV;
    short pcurv[96];

    int ChgGateLim;
    int ChgTransLim;
    int DhgGateLim;
    int DhgTransLim;

    int bErr;
    char szState[32];

    char szCloudUserName[128];
    char szCloudPasswd[128];
    char szCloudUrl[128];
    char szClientId[128];

    int DataKeepDay;

    int UploadHighSpeed; // ms
    int UploadMediumSpeed; //ms
    int UploadSlowSpeed; //ms

    int CoolTemp; 
    int CoolGap;
    int HeatTemp;
    int HeatGap;
    int CoolTempSet; // 空调制冷时的温度设定，注意，与制冷温度的区别（制冷温度：高于该温度时开启制冷）
    int HeatTempSet; // 空调加热时的温度设定，注意，与加热温度的区别（加热温度：低于该温度时开启加热）

    int GateCoupleNbr;
    int TransCoupleNbr;

    char szReserved1[128];
    char szReserved2[128];

    int TransId; // 本储能柜所在变压器编号
    int CtnMeterId; // 计量该变压器下所有储能柜的电量
    int CtnMeterNbr; // 储能表数量
};

union UnSettings_t{
    struct Settings_t s;
    char buf[1024*64];
};

enum CtlWorkMode_t{
    CTL_WORKMODE_SLAVE = 0,
    CTL_WORKMODE_PCURV = 1,
};

struct Ctl_t{
    int State;
    int Step;
    int Cnt;
    char szState[64];
    int WorkMode;
    char szWorkMode[128];
    int Err;
    char szErr[256];
    int Cmd;
    int CmdPara;

    int Ap; // 本储能柜功率
    int GateAp; 
    int GateLoadAp;
    int GateEsAp;
    int TransAp; // 本储能柜所在变压器功率
    int TransLoadAp;
    int TransEsAp;

    int bChgAble;
    int bDhgAble;

    int64_t LastTune;
};

enum CtlState_t{
    CTL_ST_LAUNCH = 10,
    CTL_ST_STDBY = 12, // 监控
    CTL_ST_STOP = 17,
    CTL_ST_RUN = 8,
    CTL_ST_ERR = 16,
};

enum CtlErr_t{
    CTL_ERR_NONE = 0,
    CTL_ERR_LAUNCH_COMMERR_DETECTED,
    CTL_ERR_LAUNCH_CFGERR,
    CTL_ERR_STDBY_COMMERR_DETECTED,
    CTL_ERR_STDBY_WAIT_PCS_STOP_TIMEOUT,
    CTL_ERR_STDBY_WAIT_PCS_APS0_TIMEOUT,
    CTL_ERR_STOP_COMMERR_DETECTED,
    CTL_ERR_STOP_PCS_NOT_STOP_DETECTED,
    CTL_ERR_STOP_WAIT_PCS_START_TIMEOUT,
    CTL_ERR_RUN_COMMERR_DETECTED,
    CTL_ERR_RUN_PCS_NOT_START_DETECTED,
    CTL_ERR_RUN_WAIT_PCS_STOP_TIMEOUT,
    CTL_ERR_RUN_WAIT_PCS_APS0_TIMEOUT,
    CTL_ERR_ERR_COMMERR_DETECTED,
};

enum CtlCmd_t{
    CTL_CMD_DONE = 0,
    CTL_CMD_STDBY = 1,
    CTL_CMD_STOP = 2,
    CTL_CMD_RUN = 3,
    CTL_CMD_SET_APS = 4,
    CTL_CMD_SET_WORKMODE = 5,
};

enum SettingsCmd_t{
    SETTINGS_CMD_SET_CHG_CELLV = 0,
    SETTINGS_CMD_SET_DHG_CELLV = 1,
    SETTINGS_CMD_SET_PCURV = 2,
    SETTINGS_CMD_SET_CHGGATELIM = 3,
    SETTINGS_CMD_SET_CHGTRANSLIM = 4,
    SETTINGS_CMD_SET_DHGGATELIM = 5,
    SETTINGS_CMD_SET_DHGTRANSLIM = 6,
    SETTINGS_CMD_SET_SN = 100,
    SETTINGS_CMD_SET_CLOUD_USERNAME = 101,
    SETTINGS_CMD_SET_CLOUD_PASSWD = 102,
    SETTINGS_CMD_SET_CLOUD_URL = 103,
    SETTINGS_CMD_REGISTER = 104,
    SETTINGS_CMD_SET_CLOUD_CLIENTID = 105,
    SETTINGS_CMD_SET_DATAKEEPDAY = 200,
    SETTINGS_CMD_SET_UPLOADHIHGSPEED = 201,
    SETTINGS_CMD_SET_UPLOADMEDIUMSPEED = 202,
    SETTINGS_CMD_SET_UPLOADSLOWSPEED = 203,

    SETTINGS_CMD_SET_CTNMETER_NBR = 300,
    SETTINGS_CMD_SET_TRANS_ID = 301,
    SETTINGS_CMD_SET_GATE_COUPLE_NBR = 302,
    SETTINGS_CMD_SET_TRANS_COUPLE_NBR = 303,
    SETTINGS_CMD_SET_CTNMETER_ID = 304,

    SETTINGS_CMD_TEST = 1000,
};

struct Snap_t{
    int enable;
    int timer;
    int intv;
    int bStart;
    int bErr;
    char szState[32];
    char szcurrDatePath[128];
    int datechk_timer;
    FILE* fpcs;
    FILE* fbms;
    FILE* fpack[10];
    FILE* ffa;
    FILE* fac;
    FILE* fctl;
    FILE* fmisc;
    int64_t LastSnap;
};

#define ERRCODE_REG_NBR 8
#define ERRCODE_NBR (ERRCODE_REG_NBR*16)
#define FIREALARM_NBR 8
struct appl_t{
    struct chan485_t chan485[CHAN485_NBR + 1];
    struct chancan_t chancan[CHANCAN_NBR + 1];
    struct chanmqtt_t chanmqtt[CHAN_MQTT_NBR + 1];
    struct Enjoy100kW_t Enjoy100kW;
    struct GaoteBms_t GaoteBms;
    struct Envicool5kW_t Envicool5kW;
    struct Adl200_t Adl200;
    struct FireAlarm_t Fa[FIREALARM_NBR + 1];
    struct Dehumi_t Dehumi;
    struct Co_t Co;
    struct Dido_t Dido;
    struct Ctl_t Ctl;
    union  UnSettings_t Set;
    struct Dtsd1352_t GateMeter;
    struct Dtsd1352_t TransMeter;
    int CtnMeterCommState;
    char szCtnMeterCommState[64];
    struct Dtsd1352_t CtnMeter[512];
    struct Snap_t Snap;
};

extern char* VERSION;
extern char* CFG_FN;
extern struct appl_t APPL;
extern void appl_chan485_lock(int idx);
extern void appl_chan485_unlock(int idx);
extern void appl_pcs_set_start(void);
extern void appl_pcs_set_stop(void);
extern void appl_pcs_set_aps(int aps);
extern void appl_ac_set_ctlmod( int m );
extern void appl_dido_set_fe_confirm( int en );
extern char* appl_get_datetime_long( void );
#endif