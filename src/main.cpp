#include <Arduino.h>
#include <lib.h>

void initializeGPRS();
void sendCommand(String cmd);
void setVisibility(String code);
void changePage(int pageNumber);
void updateTextBox(String component, String value);
void fristTimeSetup();
void nextionSerialToEsp();
void pageController();
void sendTelemetryData();
void datapacket();
void getEPPROM();
void dataSaveEPPROM(void *parameter);
void simConect(void *parameter);
void hendlenextionSerial(void *parameter);
void handleServerPage();
void saveOperatorName1(String text);
void saveOperatorName2(String text);
void saveOperatorName3(String text);
void saveOperatorName4(String text);

bool sendATCommandWithResponse(String command, String expectedResponse, int timeout);

String readOperatorName1();
String readOperatorName2();
String readOperatorName3();
String readOperatorName4();

void IRAM_ATTR onTimer()
{
  if (breakFlag == 1)
  {
    oneSecCount1++; // Total Breakdown Time
    oneSecCount2++; // epprom open time
  }
  if (statusID_01 == 2)
  {
    oneSecCount4++; // Machine ID-01 breakdown time
  }
  if (statusID_02 == 2)
  {
    oneSecCount5++; // Machine ID-02 breakdown time
  }
  if (statusID_07 == 2)
  {
    oneSecCount6++; // Machine ID-07 breakdown time
  }
  if (statusID_23 == 2)
  {
    oneSecCount7++; // Machine ID-23 breakdown time
  }

  oneSecCount3++; // post data time
}

void setup()
{
  Serial.begin(115200);
  EEPROM.begin(EEPROM_SIZE);
  nextionSerial.begin(9600, SERIAL_8N1, nextionSerial_RX_PIN, nextionSerial_TX_PIN);
  simSerial.begin(115200, SERIAL_8N1, SIM_RX_PIN, SIM_TX_PIN);

  pinMode(SIM_ON, OUTPUT); // Set SIM_ON as output for powering on the SIM module
  OneSec_Timer = timerBegin(0, 80, true);
  timerAttachInterrupt(OneSec_Timer, &onTimer, true);
  timerAlarmWrite(OneSec_Timer, 1000000, true);
  timerAlarmEnable(OneSec_Timer);
  delay(1000);

  // for (int i = 0; i < 512; i++)
  // {
  //   EEPROM.write(i, 0);
  //   Serial.println("eeprom cleaning");
  // }
  // EEPROM.commit();

  getEPPROM();
  Serial.println("Initializing SIM module...");
  initializeGPRS();

  xTaskCreate(dataSaveEPPROM, "dataSaveEPPROM", 1024 * 12, NULL, 1, NULL);
  xTaskCreate(simConect, "simConect", 1024 * 12, NULL, 2, NULL);
  xTaskCreate(hendlenextionSerial, "nextionSerial1", 1024 * 12, NULL, 3, NULL);

  // fristTimeSetup();
  simSerial.flush();
  nextionSerial.flush();
}

void loop()
{
}

void simConect(void *parameter)
{
  while (1)
  {
    if (breakFlag)
    {
      if (oneSecCount3 > 35)
      {
        oneSecCount3 = 0;
        sendTelemetryData();
      }
    }
    else
    {
      if (oneSecCount3 > 500)
      {
        oneSecCount3 = 0;
        sendTelemetryData();
      }
    }

    Serial.println("EPPROM Open Time(100s):" + String(oneSecCount2));
    Serial.println("Posting Time(35s/500s):" + String(oneSecCount3));
    Serial.println("Total Breakdown Time:" + String(oneSecCount1));
    Serial.println("MachineID_01 Breakdown Time:" + String(oneSecCount4));
    Serial.println("MachineID_02 Breakdown Time:" + String(oneSecCount5));
    Serial.println("MachineID_07 Breakdown Time:" + String(oneSecCount6));
    Serial.println("MachineID_23 Breakdown Time:" + String(oneSecCount7));

    vTaskDelay(800 / portTICK_PERIOD_MS);
  }
}

void hendlenextionSerial(void *parameter)
{
  while (1)
  {
    nextionSerialToEsp();
    if (postDone == 1 && SEND_Active == 1)
    {
      Serial.println("Post Done!!");
      postDone = 0;
      SEND_Active = 0;
      changePage(2);

      if (statusID_01 == 1) // set 01
      {
        setVisibility("vis p2,0");
        setVisibility("vis p3,0");
        setVisibility("vis p5,1");
      }
      else if (statusID_01 == 2)
      {
        setVisibility("vis p2,0");
        setVisibility("vis p3,1");
        setVisibility("vis p5,0");
      }
      else if (statusID_01 == 3)
      {
        setVisibility("vis p2,1");
        setVisibility("vis p3,0");
        setVisibility("vis p5,0");
      }

      if (statusID_02 == 1) // set 02
      {
        setVisibility("vis p8,0");
        setVisibility("vis p7,0");
        setVisibility("vis p6,1");
      }
      else if (statusID_02 == 2)
      {
        setVisibility("vis p8,0");
        setVisibility("vis p7,1");
        setVisibility("vis p6,0");
      }
      else if (statusID_02 == 3)
      {
        setVisibility("vis p8,1");
        setVisibility("vis p7,0");
        setVisibility("vis p6,0");
      }

      if (statusID_07 == 1) // set 03
      {
        setVisibility("vis p11,0");
        setVisibility("vis p10,0");
        setVisibility("vis p9,1");
      }
      else if (statusID_07 == 2)
      {
        setVisibility("vis p11,0");
        setVisibility("vis p10,1");
        setVisibility("vis p9,0");
      }
      else if (statusID_07 == 3)
      {
        setVisibility("vis p11,1");
        setVisibility("vis p10,0");
        setVisibility("vis p9,0");
      }

      if (statusID_23 == 1) // set 04
      {
        setVisibility("vis p14,0");
        setVisibility("vis p13,0");
        setVisibility("vis p12,1");
      }
      else if (statusID_23 == 2)
      {
        setVisibility("vis p14,0");
        setVisibility("vis p13,1");
        setVisibility("vis p12,0");
      }
      else if (statusID_23 == 3)
      {
        setVisibility("vis p14,1");
        setVisibility("vis p13,0");
        setVisibility("vis p12,0");
      }
      updateTextBox("t10", username);
    }
    vTaskDelay(800 / portTICK_PERIOD_MS);
  }
}

void dataSaveEPPROM(void *parameter)
{
  while (1)
  {
    if (breakFlag == 1 && oneSecCount2 > 100)
    {
      oneSecCount2 = 0;
      Serial.println("****Write EPPROM Breakdown Time****");
      EEPROM.put(INT6_ADDR, oneSecCount1);  // Total Breakdown Time
      EEPROM.put(INT7_ADDR, oneSecCount4);  // Machine ID-20 Breakdown Time
      EEPROM.put(INT8_ADDR, oneSecCount5);  // Machine ID-08 Breakdown Time
      EEPROM.put(INT9_ADDR, oneSecCount6);  // Machine ID-09 Breakdown Time
      EEPROM.put(INT10_ADDR, oneSecCount7); // Machine ID-30 Breakdown Time
      EEPROM.commit();
      Serial.println("****Write EPPROM data successfully!****");
    }
    vTaskDelay(2000 / portTICK_PERIOD_MS);
  }
}