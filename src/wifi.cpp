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

            len += sprintf((char *)post_data + len, "{\"api_token\": \"%s\", ", "G9MdAAmiTeYvwqmC7Y6T");
            len += sprintf((char *)post_data + len, "\"HMI_ID\": \"%s\", ", "HMI-02");
            len += sprintf((char *)post_data + len, "\"Username\": \"%s\", ", username);

            // machine_N01
            len += sprintf((char *)post_data + len, "\"machine_M05\": {");
            len += sprintf((char *)post_data + len, "\"StatusID_M05\": \"%d\", ", statusID_01);
            len += sprintf((char *)post_data + len, "\"OrderNmber_M05\": \"%s\", ", orderNmber_M01);
            len += sprintf((char *)post_data + len, "\"OperatorName_M05\": \"%s\", ", operatorName_M01);
            len += sprintf((char *)post_data + len, "\"operatorID_M05\": \"%s\", ", operatorID_M01);
            len += sprintf((char *)post_data + len, "\"manualReason_M05\": \"%s\", ", breakOperatorName_M01);
            len += sprintf((char *)post_data + len, "\"breakOperatorID_M05\": \"%s\", ", breakOperatorID_M01);
            len += sprintf((char *)post_data + len, "\"ShiftID\": \"%s\", ", data1);
            len += sprintf((char *)post_data + len, "\"ErrorCode_M05\": \"%s\", ", errorCode_M01);
            len += sprintf((char *)post_data + len, "\"BreakdownTime_ID05\": \"%u\",", oneSecCount4);
            len += sprintf((char *)post_data + len, "\"Fabrication_M05\": \"%s\"},", fabrication_M01);

            // machine_M02
            len += sprintf((char *)post_data + len, "\"machine_M06\": {");
            len += sprintf((char *)post_data + len, "\"StatusID_M06\": \"%d\", ", statusID_02);
            len += sprintf((char *)post_data + len, "\"OrderNmber_M06\": \"%s\", ", orderNmber_M02);
            len += sprintf((char *)post_data + len, "\"OperatorName_M06\": \"%s\", ", operatorName_M01);
            len += sprintf((char *)post_data + len, "\"operatorID_M06\": \"%s\", ", operatorID_M02);
            len += sprintf((char *)post_data + len, "\"manualReason_M06\": \"%s\", ", breakOperatorName_M02);
            len += sprintf((char *)post_data + len, "\"breakOperatorID_M06\": \"%s\", ", breakOperatorID_M02);
            len += sprintf((char *)post_data + len, "\"ShiftID\": \"%s\", ", data1);
            len += sprintf((char *)post_data + len, "\"OperatorName_M06\": \"%s\", ", operatorName_M02);
            len += sprintf((char *)post_data + len, "\"ErrorCode_M06\": \"%s\", ", errorCode_M02);
            len += sprintf((char *)post_data + len, "\"BreakdownTime_ID06\": \"%u\",", oneSecCount5);
            len += sprintf((char *)post_data + len, "\"Fabrication_M06\": \"%s\"},", fabrication_M02);

            len += sprintf((char *)post_data + len, "\"TotalBreakdownTime\": \"%u\",", oneSecCount1);
            len += sprintf((char *)post_data + len, "\"TotalBreakCount\": \"%u\", ", breakCount);
            len += sprintf((char *)post_data + len, "\"FW_version\": \"%s\"}", "24/04/2025"); // close JSON

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