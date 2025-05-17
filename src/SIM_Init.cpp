#include <Arduino.h>
#include <lib.h>

void changePage(int pageNumber);
void sendCommand(String cmd);
void updateTextBox(String component, String value);
void setVisibility(String code);
bool sendATCommandWithResponse(String command, String expectedResponse, int timeout);
void sendTelemetryData();

void initializeGPRS()
{
    bool success = false;

    for (int i = 0; i < 15; i++)
    {
        if (i < 2)
        {
            changePage(0);
        }
        String command = "j0.val=" + String(i * 6);
        sendCommand(command);
        switch (i)
        {
        case 0:
            // Power on the SIM module
            digitalWrite(SIM_ON, HIGH);
            delay(3000);
            digitalWrite(SIM_ON, LOW);
            delay(6000); // Allow time for the SIM module to initialize
            Serial.println("power SIM module...");
            success = sendATCommandWithResponse("AT", "OK", 10000);
            break;
        case 1:
            Serial.println("ATE0...");
            success = sendATCommandWithResponse("ATE0", "OK", 10000);
            break;
        case 2:
            Serial.println("AT+CFUN=1...");
            success = sendATCommandWithResponse("AT+CFUN=1", "OK", 10000); // Example expected response for network registration
            break;
        case 3:
            Serial.println("AT+CGREG=0...");
            success = sendATCommandWithResponse("AT+CGREG=0", "OK", 10000); // Example expected response for network registration
            break;
        case 4:
            Serial.println("AT+CNSMOD=1...");
            success = sendATCommandWithResponse("AT+CNSMOD=1", "OK", 10000);
            break;
        case 5:
            Serial.println("AT+CNMP=2...");
            success = sendATCommandWithResponse("AT+CNMP=2", "OK", 10000); // Replace "dialogbb" with actual APN
            break;
        case 6:
            Serial.println("AT+CGDCONT=1...");
            success = sendATCommandWithResponse("AT+CGDCONT=1,\"IPV4V6\",\"dialogbb\"", "OK", 10000);
            break;
        case 7:
            Serial.println("AT+CREG?...");
            success = sendATCommandWithResponse("AT+CREG?", "+CREG: 0,1", 10000); // Look for any IP format response
            break;
        case 8:
            Serial.println("AT+CGREG?...");
            success = sendATCommandWithResponse("AT+CGREG?", "+CGREG: 0,1", 10000); // Look for any IP format response
            break;
        case 9:
            Serial.println("AT+CGATT?...");
            success = sendATCommandWithResponse("AT+CGATT?", "OK", 10000); // Replace "dialogbb" with actual APN
            break;
        case 10:
            Serial.println("AT+CGACT=1,1...");
            success = sendATCommandWithResponse("AT+CGACT=1,1", "OK", 10000);
            break;
        case 11:
            Serial.println("AT+CGPADDR...");
            success = sendATCommandWithResponse("AT+CGPADDR", "OK", 10000); // Look for any IP format response
            break;
        case 12:
            Serial.println("AT+CPSI?...");
            success = sendATCommandWithResponse("AT+CPSI?", "OK", 10000); // Look for any IP format response
            break;
        case 13:
            Serial.println("AT+CMGF=1...");
            success = sendATCommandWithResponse("AT+CMGF=1", "OK", 10000); // Look for any IP format response
            break;
        case 14:
            Serial.println("AT+HTTPINIT...");
            success = sendATCommandWithResponse("AT+HTTPINIT", "OK", 10000); // Look for any IP format response
            netCheck = success;
            Serial.print("netCheck:");
            Serial.println(netCheck);
            break;

        default:
            success = true;
            break;
        }
        if (!success)
        {
            Serial.println("Failed to get correct response, retrying...");
            i--; // Retry the current command if it failed
            delay(2000);
        }
        else
        {
            Serial.println("Command successful, proceeding to next command...");
        }
    }
    if (netCheck == 1)
    {
        changePage(3);
    }
    else
    {
        ESP.restart();
    }
}

bool sendATCommandWithResponse(String command, String expectedResponse, int timeout)
{
    simSerial.println(command);
    long int time = millis();
    while ((time + timeout) > millis())
    {
        if (simSerial.available())
        {
            String response = simSerial.readString();
            Serial.println("Response: " + response);

            // Check if the expected response is found in the response from the SIM module
            if (response.indexOf(expectedResponse) != -1)
            {
                return true;
            }
            if (response.indexOf("ERROR") != -1)
            {
                Serial.println("Error received, adding delay before retrying...");
                delay(2000); // Add a delay before retrying
                return false;
            }
        }
    }
    return false;
}
