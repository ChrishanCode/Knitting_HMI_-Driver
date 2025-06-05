#include <Arduino.h>
#include <lib.h>

HardwareSerial nextionSerial(1);
HardwareSerial simSerial(2);

bool netCheck = 0;
bool httpOk = 0;
bool contentOk = 0;
bool dataPacketOk = 0;
bool dataSendtOk = 0;
bool httpActionOk = 0;
bool EPPROM_Active = 0;
bool SEND_Active = 0;
bool breakFlag = 0;
bool postDone = 0;
bool blockSerial = 0;

uint8_t statusID_01 = 3;
uint8_t statusID_02 = 3;
uint8_t statusID_07 = 3;
uint8_t statusID_23 = 3;

uint8_t elocoID_01 = 0;
uint8_t elocoID_02 = 0;
uint8_t elocoID_07 = 0;
uint8_t elocoID_23 = 0;

char post_data[1500];
char httpDataCommand[50];
char hexValue[50];
const char delimiter[] = "3B"; // ";"

uint8_t state = 0;
uint8_t pageActive = 0;
uint8_t epageActive = 0;
uint16_t post_data_size = 0;

String str = "";
String username = "";
String password = "";
String startString = "";
String regiStatus = "";
String breakdownFinish = "";
String breakdown = "";
String operatorName = "";
String operatorName1 = "";
String operatorName2 = "";
String operatorName3 = "";
String operatorName4 = "";
String operatorPass1 = "";
String operatorPass2 = "";
String operatorPass3 = "";
String operatorPass4 = "";
String operatorIndex = "";
String breakOperatorName = "";
String breakOperatorID = "";
String errorCode = "";
String operatorPass = "";
String operatorID = "";
String data1 = "";
String data2 = "";
String data3 = "";
String data4 = "";
String deviceId = "";
String errorCode_M01 = "";
String errorCode_M02 = "";
String errorCode_M07 = "";
String errorCode_M23 = "";

String operatorName_M01 = "";
String operatorID_M01 = "";
String orderNmber_M01 = "";
String fabrication_M01 = "";
String operatorName_M02 = "";
String operatorID_M02 = "";
String orderNmber_M02 = "";
String fabrication_M02 = "";
String operatorName_M07 = "";
String operatorID_M07 = "";
String orderNmber_M07 = "";
String fabrication_M07 = "";
String operatorName_M23 = "";
String operatorID_M23 = "";
String orderNmber_M23 = "";
String fabrication_M23 = "";
String breakOperatorName_M01 = "";
String breakOperatorName_M02 = "";
String breakOperatorName_M07 = "";
String breakOperatorName_M23 = "";

uint32_t oneSecCount1 = 0;
uint32_t oneSecCount2 = 0;
uint32_t oneSecCount3 = 0;
uint32_t oneSecCount4 = 0;
uint32_t oneSecCount5 = 0;
uint32_t oneSecCount6 = 0;
uint32_t oneSecCount7 = 0;

uint32_t breakCount = 0;
bool ebreakflag = 0;
uint32_t ebreakCount = 0;
uint32_t eoneSecCount1 = 0;
uint32_t eoneSecCount2 = 0;
uint32_t eoneSecCount3 = 0;
uint32_t eoneSecCount4 = 0;
uint32_t eoneSecCount5 = 0;
uint32_t eoneSecCount6 = 0;
uint32_t eoneSecCount7 = 0;

hw_timer_t *OneSec_Timer = NULL;