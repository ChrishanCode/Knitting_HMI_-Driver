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
bool breakCompleted = 0;

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
String breakOperatorID_M01 = "";
String breakOperatorID_M02 = "";
String breakOperatorID_M07 = "";
String breakOperatorID_M23 = "";

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

// wifi

const char *rootCACertificate = R"EOF(
-----BEGIN CERTIFICATE-----
MIIFazCCA1OgAwIBAgIRAIIQz7DSQONZRGPgu2OCiwAwDQYJKoZIhvcNAQELBQAw
TzELMAkGA1UEBhMCVVMxKTAnBgNVBAoTIEludGVybmV0IFNlY3VyaXR5IFJlc2Vh
cmNoIEdyb3VwMRUwEwYDVQQDEwxJU1JHIFJvb3QgWDEwHhcNMTUwNjA0MTEwNDM4
WhcNMzUwNjA0MTEwNDM4WjBPMQswCQYDVQQGEwJVUzEpMCcGA1UEChMgSW50ZXJu
ZXQgU2VjdXJpdHkgUmVzZWFyY2ggR3JvdXAxFTATBgNVBAMTDElTUkcgUm9vdCBY
MTCCAiIwDQYJKoZIhvcNAQEBBQADggIPADCCAgoCggIBAK3oJHP0FDfzm54rVygc
h77ct984kIxuPOZXoHj3dcKi/vVqbvYATyjb3miGbESTtrFj/RQSa78f0uoxmyF+
0TM8ukj13Xnfs7j/EvEhmkvBioZxaUpmZmyPfjxwv60pIgbz5MDmgK7iS4+3mX6U
A5/TR5d8mUgjU+g4rk8Kb4Mu0UlXjIB0ttov0DiNewNwIRt18jA8+o+u3dpjq+sW
T8KOEUt+zwvo/7V3LvSye0rgTBIlDHCNAymg4VMk7BPZ7hm/ELNKjD+Jo2FR3qyH
B5T0Y3HsLuJvW5iB4YlcNHlsdu87kGJ55tukmi8mxdAQ4Q7e2RCOFvu396j3x+UC
B5iPNgiV5+I3lg02dZ77DnKxHZu8A/lJBdiB3QW0KtZB6awBdpUKD9jf1b0SHzUv
KBds0pjBqAlkd25HN7rOrFleaJ1/ctaJxQZBKT5ZPt0m9STJEadao0xAH0ahmbWn
OlFuhjuefXKnEgV4We0+UXgVCwOPjdAvBbI+e0ocS3MFEvzG6uBQE3xDk3SzynTn
jh8BCNAw1FtxNrQHusEwMFxIt4I7mKZ9YIqioymCzLq9gwQbooMDQaHWBfEbwrbw
qHyGO0aoSCqI3Haadr8faqU9GY/rOPNk3sgrDQoo//fb4hVC1CLQJ13hef4Y53CI
rU7m2Ys6xt0nUW7/vGT1M0NPAgMBAAGjQjBAMA4GA1UdDwEB/wQEAwIBBjAPBgNV
HRMBAf8EBTADAQH/MB0GA1UdDgQWBBR5tFnme7bl5AFzgAiIyBpY9umbbjANBgkq
hkiG9w0BAQsFAAOCAgEAVR9YqbyyqFDQDLHYGmkgJykIrGF1XIpu+ILlaS/V9lZL
ubhzEFnTIZd+50xx+7LSYK05qAvqFyFWhfFQDlnrzuBZ6brJFe+GnY+EgPbk6ZGQ
3BebYhtF8GaV0nxvwuo77x/Py9auJ/GpsMiu/X1+mvoiBOv/2X/qkSsisRcOj/KK
NFtY2PwByVS5uCbMiogziUwthDyC3+6WVwW6LLv3xLfHTjuCvjHIInNzktHCgKQ5
ORAzI4JMPJ+GslWYHb4phowim57iaztXOoJwTdwJx4nLCgdNbOhdjsnvzqvHu7Ur
TkXWStAmzOVyyghqpZXjFaH3pO3JLF+l+/+sKAIuvtd7u+Nxe5AW0wdeRlN8NwdC
jNPElpzVmbUq4JUagEiuTDkHzsxHpFKVK7q4+63SM1N95R1NbdWhscdCb+ZAJzVc
oyi3B43njTOQ5yOf+1CceWxG1bQVs5ZufpsMljq4Ui0/1lvh+wjChP4kqKOJ2qxq
4RgqsahDYVvTH9w7jXbyLeiNdd8XM2w9U/t7y0Ff/9yi0GE44Za4rF2LN9d11TPA
mRGunUHBcnWEvgJBQl9nJEiU0Zsnvgc/ubhPgXRR4Xq37Z0j4r7g1SgEEzwxA57d
emyPxgcYxn/eR44/KJ4EBs+lVDR3veyJm+kXQ99b21/+jh5Xos1AnX5iItreGCc=
-----END CERTIFICATE-----
)EOF";

const char *ssid = "UTECH2.4G";
const char *wifiKey = "Utech@54321";

const char *telemetryUrl = "https://hayleys1.utech-iiot.lk/api/v1/LhkZfoqAh3QA746UIBte/telemetry";

unsigned long len = 0;
bool otaBit = 0;
const byte POST_Active_Time = 10;