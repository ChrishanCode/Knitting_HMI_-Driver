#include <Arduino.h>
#include <lib.h>

void initializeGPRS();
void sendCommand(String cmd);
void setVisibility(String code);
void changePage(int pageNumber);
void updateTextBox(String component, String value);
void fristTimeSetup();
void datapacket();
void handleServerPage();
bool sendATCommandWithResponse(String command, String expectedResponse, int timeout);

void sendTelemetryData()
{
    uint8_t postStatus = 0, count = 0;
    postDone = 0;
    blockSerial = 1;
    datapacket();
    while (1)
    {
        if (count >= 5)
        {
            count = 0;
            simSerial.flush();
            initializeGPRS();
        }
        switch (postStatus)
        {
        case 0:
            Serial.println("URL");
            // HMI_01 --->>> https://industry-app.utech-iiot.lk/api/v1/L1B6RXwjkaKNDWwwwGyu/telemetry
            // HMI Test --->>> https://energy-app.utech-iiot.lk/api/v1/kTVJAVJhUtDQANFFGV9I/telemetry
            httpOk = sendATCommandWithResponse("AT+HTTPPARA=\"URL\",\"https://industry-app.utech-iiot.lk/api/v1/L1B6RXwjkaKNDWwwwGyu/telemetry\"", "OK", 10000);
            if (httpOk)
            {
                postStatus = 1;
                Serial.println("-URL ok-");
            }
            else
            {
                Serial.println("-URL fail-");
                count++;
            }
            break;
        case 1:
            Serial.println("CONTENT");
            contentOk = sendATCommandWithResponse("AT+HTTPPARA=\"CONTENT\",\"application/json\"", "OK", 10000);
            if (contentOk)
            {
                postStatus = 2;
                Serial.println("-CONTENT ok-");
            }
            else
            {
                Serial.println("-CONTENT fail-");
                count++;
            }
            break;
        case 2:
            Serial.println("Data Packet");
            dataPacketOk = sendATCommandWithResponse(httpDataCommand, "DOWNLOAD", 10000);
            if (dataPacketOk)
            {
                postStatus = 3;
                Serial.println("-Data Packet ok-");
            }
            else
            {
                Serial.println("-Data Packet fail-");
                count++;
            }
            break;
        case 3:
            Serial.println("Data Packet Sending");
            dataSendtOk = sendATCommandWithResponse(post_data, "OK", 10000);
            if (dataSendtOk)
            {
                postStatus = 4;
                Serial.println("-Packet Sending ok-");
            }
            else
            {
                Serial.println("-Packet Sending fail-");
                count++;
            }
            break;
        case 4:
            Serial.println("HTTP Action");
            httpActionOk = sendATCommandWithResponse("AT+HTTPACTION=1", "OK", 10000);
            if (httpActionOk)
            {
                Serial.println("-HTTP Action ok-");
                postStatus = 5;
            }
            else
            {
                Serial.println("-HTTP Action fail-");
                count++;
            }
        case 5:
            if (simSerial.available())
            {
                String response = simSerial.readString();
                Serial.println("Response1: " + response);
                simSerial.flush();
                postDone = 1;    // Successful transmission
                blockSerial = 0; // Unblock the serial communication
            }
            break;
        default:
            postStatus = 0; // Reset the process if needed
            break;
        }
        if (postDone)
        {
            Serial.println("Telemetry data sent successfully!");
            break;
        }
        vTaskDelay(50 / portTICK_PERIOD_MS);
    }
}

void datapacket()
{
    uint16_t len = 0;

    len += sprintf((char *)post_data + len, "{\"api_token\": \"%s\", ", "L1B6RXwjkaKNDWwwwGyu");
    len += sprintf((char *)post_data + len, "\"HMI_ID\": \"%s\", ", "HMI-01");
    len += sprintf((char *)post_data + len, "\"Username\": \"%s\", ", username);

    // machine_N01
    len += sprintf((char *)post_data + len, "\"machine_M01\": {");
    len += sprintf((char *)post_data + len, "\"StatusID_M01\": \"%d\", ", statusID_01);
    len += sprintf((char *)post_data + len, "\"ShiftID\": \"%s\", ", data1);
    len += sprintf((char *)post_data + len, "\"OrderNmber_M01\": \"%s\", ", orderNmber_M01);
    len += sprintf((char *)post_data + len, "\"OperatorName_M01\": \"%s\", ", operatorName_M01);
    len += sprintf((char *)post_data + len, "\"OperatorID_M01\": \"%s\", ", operatorID_M01);
    len += sprintf((char *)post_data + len, "\"breakOperatorName_M01\": \"%s\", ", breakOperatorName_M01);
    len += sprintf((char *)post_data + len, "\"breakOperatorID_M01\": \"%s\", ", breakOperatorID_M01);
    len += sprintf((char *)post_data + len, "\"ErrorCode_M01\": \"%s\", ", errorCode_M01);
    len += sprintf((char *)post_data + len, "\"BreakdownTime_ID01\": \"%u\",", oneSecCount4);
    len += sprintf((char *)post_data + len, "\"Fabrication_M01\": \"%s\"},", fabrication_M01);

    // machine_M02
    len += sprintf((char *)post_data + len, "\"machine_M02\": {");
    len += sprintf((char *)post_data + len, "\"StatusID_M02\": \"%d\", ", statusID_02);
    len += sprintf((char *)post_data + len, "\"ShiftID\": \"%s\", ", data1);
    len += sprintf((char *)post_data + len, "\"OrderNmber_M02\": \"%s\", ", orderNmber_M02);
    len += sprintf((char *)post_data + len, "\"OperatorName_M02\": \"%s\", ", operatorName_M02);
    len += sprintf((char *)post_data + len, "\"OperatorID_M02\": \"%s\", ", operatorID_M02);
    len += sprintf((char *)post_data + len, "\"breakOperatorName_M02\": \"%s\", ", breakOperatorName_M02);
    len += sprintf((char *)post_data + len, "\"breakOperatorID_M02\": \"%s\", ", breakOperatorID_M02);
    len += sprintf((char *)post_data + len, "\"ErrorCode_M02\": \"%s\", ", errorCode_M02);
    len += sprintf((char *)post_data + len, "\"BreakdownTime_ID02\": \"%u\",", oneSecCount5);
    len += sprintf((char *)post_data + len, "\"Fabrication_M02\": \"%s\"},", fabrication_M02);

    // machine_M07
    len += sprintf((char *)post_data + len, "\"machine_M07\": {");
    len += sprintf((char *)post_data + len, "\"StatusID_M07\": \"%d\", ", statusID_07);
    len += sprintf((char *)post_data + len, "\"ShiftID\": \"%s\", ", data1);
    len += sprintf((char *)post_data + len, "\"OrderNmber_M07\": \"%s\", ", orderNmber_M07);
    len += sprintf((char *)post_data + len, "\"OperatorName_M07\": \"%s\", ", operatorName_M07);
    len += sprintf((char *)post_data + len, "\"OperatorID_M07\": \"%s\", ", operatorID_M07);
    len += sprintf((char *)post_data + len, "\"breakOperatorName_M07\": \"%s\", ", breakOperatorName_M07);
    len += sprintf((char *)post_data + len, "\"breakOperatorID_M07\": \"%s\", ", breakOperatorID_M07);
    len += sprintf((char *)post_data + len, "\"ErrorCode_M07\": \"%s\", ", errorCode_M07);
    len += sprintf((char *)post_data + len, "\"BreakdownTime_ID07\": \"%u\",", oneSecCount6);
    len += sprintf((char *)post_data + len, "\"Fabrication_M07\": \"%s\"},", fabrication_M07);

    // machine_M23
    len += sprintf((char *)post_data + len, "\"machine_M23\": {");
    len += sprintf((char *)post_data + len, "\"StatusID_M23\": \"%d\", ", statusID_23);
    len += sprintf((char *)post_data + len, "\"ShiftID\": \"%s\", ", data1);
    len += sprintf((char *)post_data + len, "\"OrderNmber_M23\": \"%s\", ", orderNmber_M23);
    len += sprintf((char *)post_data + len, "\"OperatorName_M23\": \"%s\", ", operatorName_M23);
    len += sprintf((char *)post_data + len, "\"OperatorID_M23\": \"%s\", ", operatorID_M23);
    len += sprintf((char *)post_data + len, "\"breakOperatorName_M23\": \"%s\", ", breakOperatorName_M23);
    len += sprintf((char *)post_data + len, "\"breakOperatorID_M23\": \"%s\", ", breakOperatorID_M23);
    len += sprintf((char *)post_data + len, "\"ErrorCode_M23\": \"%s\", ", errorCode_M23);
    len += sprintf((char *)post_data + len, "\"BreakdownTime_ID23\": \"%u\",", oneSecCount7);
    len += sprintf((char *)post_data + len, "\"Fabrication_M23\": \"%s\"},", fabrication_M23);

    len += sprintf((char *)post_data + len, "\"TotalBreakdownTime\": \"%u\",", oneSecCount1);
    len += sprintf((char *)post_data + len, "\"TotalBreakCount\": \"%u\", ", breakCount);
    len += sprintf((char *)post_data + len, "\"FW_version\": \"%s\"}", "06/09/2025"); // close JSON

    // len = sprintf((char *)post_data, "{\"operatorName\": \"%s\", \"operatorID\": \"%s\",\"breakReason\": \"%s\",\"oneSecCount1\": \"%u\"}", operatorName, operatorID, breakReason, oneSecCount1);
    // len = sprintf((char *)post_data, "{\"temperature\": \"%d\", \"humidity\": \"%d\", \"pressure\": \"%d\", \"altitude\": \"%d\"}",temperature, humidity, pressure, altitude);
    Serial.println(post_data);
    Serial.print("length:");
    Serial.println(len);
    post_data_size = len;
    sprintf(httpDataCommand, "AT+HTTPDATA=%d,5000", post_data_size);
}

void handleServerPage()
{
    Serial.println("Handle the sending page");
    if (postDone)
    {
        if (startString == "register1" && data4 == "ID-29")
        {
            changePage(2);
            setVisibility("vis p2,0");
            setVisibility("vis p3,0");
            setVisibility("vis p5,1");
            updateTextBox("t10", username);
            state = 0;
        }
        if (startString == "register1" && data4 == "ID-08")
        {
            changePage(2);
            setVisibility("vis p2,0");
            setVisibility("vis p3,0");
            setVisibility("vis p5,1");
            updateTextBox("t10", username);
            state = 0;
        }
        if (startString == "register1" && data4 == "ID-09")
        {
            changePage(2);
            setVisibility("vis p2,0");
            setVisibility("vis p3,0");
            setVisibility("vis p5,1");
            updateTextBox("t10", username);
            state = 0;
        }
        if (startString == "register1" && data4 == "ID-30")
        {
            changePage(2);
            setVisibility("vis p2,0");
            setVisibility("vis p3,0");
            setVisibility("vis p5,1");
            updateTextBox("t10", username);
            state = 0;
        }
    }
}
