#include <HardwareSerial.h>
#include <freertos/FreeRTOS.h>
#include <FreeRTOSConfig.h>
#include <ArduinoJson.h>
#include <EEPROM.h>

extern HardwareSerial nextionSerial; // Create a one serial for nextionSerial communication
extern HardwareSerial simSerial;     // Create a second serial for SIM module communication

#define EEPROM_SIZE 512
#define nextionSerial_RX_PIN 14
#define nextionSerial_TX_PIN 13

#define SIM_RX_PIN 16
#define SIM_TX_PIN 17
#define SIM_ON 32 // Define the pin connected to SIM module's power-on pin

#define MAX_STRING_LENGTH 25

#define STRING1_ADDR 0
#define STRING2_ADDR 20
#define STRING3_ADDR 40
#define STRING4_ADDR 60
#define STRING5_ADDR 80
#define STRING6_ADDR 100
#define STRING7_ADDR 120
#define STRING8_ADDR 140
#define STRING9_ADDR 160
#define STRING10_ADDR 180
#define STRING11_ADDR 200
#define STRING12_ADDR 220
#define STRING13_ADDR 240
#define STRING14_ADDR 260
#define STRING15_ADDR 280
#define STRING16_ADDR 300
#define STRING17_ADDR 320
#define STRING18_ADDR 340

// Align ints on 4-byte boundaries
#define INT1_ADDR 360
#define INT2_ADDR 364
#define INT3_ADDR 368
#define INT4_ADDR 372
#define INT5_ADDR 376
#define INT6_ADDR 380
#define INT7_ADDR 384
#define INT8_ADDR 388
#define INT9_ADDR 392
#define INT10_ADDR 396
#define INT11_ADDR 400

extern bool netCheck;
extern bool httpOk;
extern bool contentOk;
extern bool dataPacketOk;
extern bool dataSendtOk;
extern bool httpActionOk;
extern bool EPPROM_Active;
extern bool SEND_Active;
extern bool breakFlag;
extern bool postDone;
extern bool blockSerial;

extern uint8_t statusID_01;
extern uint8_t statusID_02;
extern uint8_t statusID_07;
extern uint8_t statusID_23;

extern uint8_t elocoID_01;
extern uint8_t elocoID_02;
extern uint8_t elocoID_07;
extern uint8_t elocoID_23;

extern char post_data[1500];
extern char httpDataCommand[50];
extern char hexValue[50];
extern const char delimiter[]; // ";"

extern uint8_t state;
extern uint8_t pageActive;
extern uint8_t epageActive;
extern uint16_t post_data_size;

extern String str;
extern String username;
extern String password;
extern String startString;
extern String regiStatus;
extern String breakdownFinish;
extern String breakdown;

extern String operatorName;
extern String operatorName1;
extern String operatorName2;
extern String operatorName3;
extern String operatorName4;
extern String operatorPass1;
extern String operatorPass2;
extern String operatorPass3;
extern String operatorPass4;
extern String operatorIndex;
extern String breakOperatorName;
extern String breakOperatorID;
extern String errorCode;
extern String operatorPass;
extern String operatorID;
extern String data1;
extern String data2;
extern String data3;
extern String data4;
extern String deviceId;
extern String errorCode_M01;
extern String errorCode_M02;
extern String errorCode_M07;
extern String errorCode_M23;

extern String operatorName_M01;
extern String operatorID_M01;
extern String orderNmber_M01;
extern String fabrication_M01;
extern String operatorName_M02;
extern String operatorID_M02;
extern String orderNmber_M02;
extern String fabrication_M02;
extern String operatorName_M07;
extern String operatorID_M07;
extern String orderNmber_M07;
extern String fabrication_M07;
extern String operatorName_M23;
extern String operatorID_M23;
extern String orderNmber_M23;
extern String fabrication_M23;
extern String breakOperatorName_M01;
extern String breakOperatorName_M02;
extern String breakOperatorName_M07;
extern String breakOperatorName_M23;
extern String breakOperatorID_M01;
extern String breakOperatorID_M02;
extern String breakOperatorID_M07;
extern String breakOperatorID_M23;

extern uint32_t oneSecCount1;
extern uint32_t oneSecCount2;
extern uint32_t oneSecCount3;
extern uint32_t oneSecCount4;
extern uint32_t oneSecCount5;
extern uint32_t oneSecCount6;
extern uint32_t oneSecCount7;
extern uint32_t breakCount;

extern uint32_t ebreakCount;
extern bool ebreakflag;
extern uint32_t eoneSecCount1;
extern uint32_t eoneSecCount4;
extern uint32_t eoneSecCount5;
extern uint32_t eoneSecCount6;
extern uint32_t eoneSecCount7;

// Declare a pointer to hw_timer_t
extern hw_timer_t *OneSec_Timer;