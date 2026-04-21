#include <Arduino.h>
#include <lib.h>

#if Wifi_

void changePage(int pageNumber);
void sendCommand(String cmd);

void wifisetup()
{
    WiFi.begin(ssid, wifiKey);
    Serial.println("Connecting");
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(500);
    }
    Serial.println("");
    Serial.print("Connected to WiFi network with IP Address: ");
    Serial.println(WiFi.localIP());
    Serial.print("Gateway: ");
    Serial.println(WiFi.gatewayIP());
    Serial.print("DNS: ");
    Serial.println(WiFi.dnsIP());
    Serial.print("Subnet Mask: ");
    Serial.println(WiFi.subnetMask());

    for (int i = 0; i < 20; i++)
    {
        changePage(0);
        String command = "j0.val=" + String(i * 4);
        sendCommand(command);
    }

    if (WiFi.status() != WL_CONNECTED)
    {
        Serial.println("Failed to connect to WiFi, restarting...");
        ESP.restart();
    }
    else
        changePage(3);
}

void wifiUpdate()
{
    if (oneSecCount3 >= POST_Active_Time)
    {
        if (WiFi.status() == WL_CONNECTED)
        {
            uint16_t len = 0;
            memset(post_data, 0, sizeof(post_data));
            WiFiClientSecure client;
            HTTPClient http;
            client.setCACert(rootCACertificate);
            http.begin(client, (String)telemetryUrl);
            http.addHeader("Content-Type", "application/json");

            len += sprintf((char *)post_data + len, "{\"api_token\": \"%s\", ", "LhkZfoqAh3QA746UIBte");
            len += sprintf((char *)post_data + len, "\"HMI_ID\": \"%s\", ", "HMI-03");
            len += sprintf((char *)post_data + len, "\"Username\": \"%s\", ", username);

            // machine_N01
            len += sprintf((char *)post_data + len, "\"machine_M03\": {");
            len += sprintf((char *)post_data + len, "\"StatusID_M03\": \"%d\", ", statusID_01);
            len += sprintf((char *)post_data + len, "\"OrderNmber_M03\": \"%s\", ", orderNmber_M01);
            len += sprintf((char *)post_data + len, "\"OperatorName_M03\": \"%s\", ", operatorName_M01);
            len += sprintf((char *)post_data + len, "\"OperatorID_M03\": \"%s\", ", operatorID_M01);
            len += sprintf((char *)post_data + len, "\"manualReason_M03\": \"%s\", ", breakOperatorName_M01);
            len += sprintf((char *)post_data + len, "\"breakOperatorID_M03\": \"%s\", ", breakOperatorID_M01);
            len += sprintf((char *)post_data + len, "\"ShiftID\": \"%s\", ", data1);
            len += sprintf((char *)post_data + len, "\"ErrorCode_M03\": \"%s\", ", errorCode_M01);
            len += sprintf((char *)post_data + len, "\"BreakdownTime_ID03\": \"%u\",", oneSecCount4);
            len += sprintf((char *)post_data + len, "\"Fabrication_M03\": \"%s\"},", fabrication_M01);

            // machine_M02
            len += sprintf((char *)post_data + len, "\"machine_M04\": {");
            len += sprintf((char *)post_data + len, "\"StatusID_M04\": \"%d\", ", statusID_02);
            len += sprintf((char *)post_data + len, "\"OrderNmber_M04\": \"%s\", ", orderNmber_M02);
            len += sprintf((char *)post_data + len, "\"OperatorName_M04\": \"%s\", ", operatorName_M02);
            len += sprintf((char *)post_data + len, "\"OperatorID_M04\": \"%s\", ", operatorID_M02);
            len += sprintf((char *)post_data + len, "\"manualReason_M04\": \"%s\", ", breakOperatorName_M02);
            len += sprintf((char *)post_data + len, "\"breakOperatorID_M04\": \"%s\", ", breakOperatorID_M02);
            len += sprintf((char *)post_data + len, "\"ShiftID\": \"%s\", ", data1);
            len += sprintf((char *)post_data + len, "\"ErrorCode_M04\": \"%s\", ", errorCode_M02);
            len += sprintf((char *)post_data + len, "\"BreakdownTime_ID04\": \"%u\",", oneSecCount5);
            len += sprintf((char *)post_data + len, "\"Fabrication_M04\": \"%s\"},", fabrication_M02);

            // machine_M07
            len += sprintf((char *)post_data + len, "\"machine_M24\": {");
            len += sprintf((char *)post_data + len, "\"StatusID_M24\": \"%d\", ", statusID_07);
            len += sprintf((char *)post_data + len, "\"OrderNmber_M24\": \"%s\", ", orderNmber_M07);
            len += sprintf((char *)post_data + len, "\"OperatorName_M24\": \"%s\", ", operatorName_M07);
            len += sprintf((char *)post_data + len, "\"OperatorID_M24\": \"%s\", ", operatorID_M07);
            len += sprintf((char *)post_data + len, "\"manualReason_M24\": \"%s\", ", breakOperatorName_M07);
            len += sprintf((char *)post_data + len, "\"breakOperatorID_M24\": \"%s\", ", breakOperatorID_M07);
            len += sprintf((char *)post_data + len, "\"ShiftID\": \"%s\", ", data1);
            len += sprintf((char *)post_data + len, "\"ErrorCode_M24\": \"%s\", ", errorCode_M07);
            len += sprintf((char *)post_data + len, "\"BreakdownTime_ID24\": \"%u\",", oneSecCount6);
            len += sprintf((char *)post_data + len, "\"Fabrication_M24\": \"%s\"},", fabrication_M07);

            // machine_M23
            len += sprintf((char *)post_data + len, "\"machine_M25\": {");
            len += sprintf((char *)post_data + len, "\"StatusID_M25\": \"%d\", ", statusID_23);
            len += sprintf((char *)post_data + len, "\"OrderNmber_M25\": \"%s\", ", orderNmber_M23);
            len += sprintf((char *)post_data + len, "\"OperatorName_M25\": \"%s\", ", operatorName_M23);
            len += sprintf((char *)post_data + len, "\"OperatorID_M25\": \"%s\", ", operatorID_M23);
            len += sprintf((char *)post_data + len, "\"manualReason_M25\": \"%s\", ", breakOperatorName_M23);
            len += sprintf((char *)post_data + len, "\"breakOperatorID_M25\": \"%s\", ", breakOperatorID_M23);
            len += sprintf((char *)post_data + len, "\"ShiftID\": \"%s\", ", data1);
            len += sprintf((char *)post_data + len, "\"ErrorCode_M25\": \"%s\", ", errorCode_M23);
            len += sprintf((char *)post_data + len, "\"BreakdownTime_ID25\": \"%u\",", oneSecCount7);
            len += sprintf((char *)post_data + len, "\"Fabrication_M25\": \"%s\"},", fabrication_M23);

            len += sprintf((char *)post_data + len, "\"TotalBreakdownTime\": \"%u\",", oneSecCount1);
            len += sprintf((char *)post_data + len, "\"TotalBreakCount\": \"%u\", ", breakCount);
            len += sprintf((char *)post_data + len, "\"FW_version\": \"%s\"}", "02/23/2025"); // close JSON

            int httpResponseCode = http.POST(post_data);
            Serial.print("post_data--->");
            Serial.println(post_data);

            if (httpResponseCode > 0)
            {
                Serial.print("HTTP Response code: ");
                Serial.println(httpResponseCode);

                if (httpResponseCode == 200)
                {
                    postDone = 1;    // Successful transmission
                    blockSerial = 0; // Unblock the serial communication
                    String payload = http.getString();
                    Serial.println(payload);
                    // serverOut(payload.c_str());
                    // parseJson(payload.c_str());
                }
                else
                {
                    Serial.println("!!!!!!! Server Error !!!!!!!");
                }
            }
            else
            {
                Serial.print("Error code: ");
                Serial.println(httpResponseCode);
            }
            // Free resources
            http.end();
        }
        else
        {
            if (WiFi.status() != WL_CONNECTED)
            {
                Serial.println("WiFi Disconnected. Try to connect...!!!!");
                WiFi.reconnect();
                int attempts = 0;
                while (WiFi.status() != WL_CONNECTED && attempts < 20)
                {
                    delay(500);
                    Serial.print(".");
                    attempts++;
                }
            }
            oneSecCount3 = 0;
        }
    }
}

#endif