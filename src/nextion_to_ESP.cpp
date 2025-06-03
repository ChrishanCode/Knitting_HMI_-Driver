#include <Arduino.h>
#include <lib.h>

void setVisibility(String code);
void changePage(int pageNumber);
void updateTextBox(String component, String value);
void pageController();
void sendTelemetryData();
void saveOperatorName1(String text);
void saveOperatorName2(String text);
void saveOperatorName3(String text);
void saveOperatorName4(String text);
void saveOperatorPass1(String text);
void saveOperatorPass2(String text);
void saveOperatorPass3(String text);
void saveOperatorPass4(String text);
void saveLoginOperatorName(String text);
void saveLoginOperatorID(String text);
void saveShift(String text);
void saveOrderID(String text);
void saveFabrication(String text);
void saveMachineID(String text);
void saveErrorCode_01(String text);
void saveErrorCode_02(String text);
void saveErrorCode_07(String text);
void saveErrorCode_23(String text);

bool isReadable(const char *data)
{
    for (int i = 0; data[i] != '\0'; i++)
    {
        if (!isPrintable(data[i]))
        {
            return false;
        }
    }
    return true;
}

void nextionSerialToEsp()
{
    if (nextionSerial.available() > 0 && blockSerial == 0)
    {
        Serial.println("***************data coming***********************");

        size_t length = nextionSerial.readBytesUntil('\n', hexValue, sizeof(hexValue) - 1);
        hexValue[length] = '\0';

        Serial.print("Received Hex Value: ");
        Serial.println(hexValue);

        // Check if data is clean
        if (!isReadable(hexValue))
        {
            Serial.println("❌ Invalid (non-readable) data received");
            memset(hexValue, 0, sizeof(hexValue));
            str = "";
            state = 0;
            // setVisibility("vis p8,0");
            nextionSerial.flush();
            return;
        }
        else
        {
            Serial.println("✅ Valid data received");
            str = String(hexValue);
        }

        // Now safe to parse
        if (strstr(hexValue, "login") != NULL)
        {
            state = 1;
            Serial.println("login is done");
        }
        else if (strstr(hexValue, "newRegistor") != NULL)
        {
            state = 2;
            Serial.println("New Registation call");
        }
        else if (strstr(hexValue, "memberRegistor") != NULL)
        {
            state = 2;
            Serial.println("New Registation is done");
        }
        else if (strstr(hexValue, "register1") != NULL)
        {
            state = 3;
            Serial.println("New order register is done");
        }
        else if (strstr(hexValue, "breakReason") != NULL)
        {
            state = 4;

            Serial.println("Breakdown is done");
        }
        else if (strstr(hexValue, "breakdownCompleted") != NULL)
        {
            state = 5;
            Serial.println("Breakdown Is Completed");
        }
        else if (strstr(hexValue, "orderCompleted") != NULL)
        {
            state = 6;
            Serial.println("Order Is Completed");
        }
        else if (strstr(hexValue, "cancelAny") != NULL)
        {
            state = 7;
            Serial.println("Cancel All is done");
        }
        nextionSerial.flush();
    }
    else
    {
        Serial.println("Data Coming but not read during the posting time!!");
        nextionSerial.flush();
    }
    pageController();
}

void pageController()
{
    int delim1, delim2, delim3, delim4, delim5, delim6;
    switch (state)
    {
    case 1: // login
        Serial.println("state 1");
        delim1 = str.indexOf(";");
        delim2 = str.indexOf(";", delim1 + 1);
        startString = str.substring(0, delim1);
        username = str.substring(delim1 + 1, delim2);
        password = str.substring(delim2 + 1);
        Serial.println("startString: " + startString);
        Serial.println("Username: " + username);
        Serial.println("Password: " + password);

        if (username == "admin" && password == "admin")
        {
            Serial.println("Admin Login");
            state = 0;
            changePage(5);
            updateTextBox("t10", username);
            sendTelemetryData();
            if (postDone)
            {
                postDone = 0;
                changePage(4);
                setVisibility("vis p8,0");
                updateTextBox("t11", username);
                updateTextBox("t0", operatorName1);
                updateTextBox("t1", operatorPass1);
                updateTextBox("t2", operatorName2);
                updateTextBox("t3", operatorPass2);
                updateTextBox("t4", operatorName3);
                updateTextBox("t5", operatorPass3);
                updateTextBox("t6", operatorName4);
                updateTextBox("t7", operatorPass4);
            }
        }
        if ((username == operatorName1 && password == operatorPass1) || (username == operatorName2 && password == operatorPass2))
        {
            Serial.println("Operator Login Now");
            state = 0;
            changePage(5);
            updateTextBox("t10", username);
            sendTelemetryData();
            if (postDone)
            {
                // postDone = 0;
                SEND_Active = 1;
                changePage(2);
                updateTextBox("t10", username);
            }
            else
            {
                // need to handle the error meg
            }
        }
        else if ((username == operatorName3 && password == operatorPass3) || (username == operatorName4 && password == operatorPass4))
        {
            Serial.println("Operator Login Now");
            state = 0;
            changePage(5);
            updateTextBox("t10", username);
            sendTelemetryData();
            if (postDone)
            {
                // postDone = 0;
                SEND_Active = 1;
                changePage(2);
                updateTextBox("t10", username);
            }
            else
            {
                // need to handle the error meg
            }
        }
        else
        {
            Serial.println("Invalid Username or Password");
            state = 0;
        }
        break;

    case 2: // New Registration
        Serial.println("state 2");
        if (strstr(hexValue, "newRegistor") != NULL)
        {
            changePage(4);
            setVisibility("vis p8,0");
            Serial.println("Not Processing the Incoming Data!!");
            state = 0;
        }
        else
        {
            changePage(4);
            delim1 = str.indexOf(";");
            delim2 = str.indexOf(";", delim1 + 1);
            delim3 = str.indexOf(";", delim2 + 1);
            startString = str.substring(0, delim1);
            operatorIndex = str.substring(delim1 + 1, delim2);
            Serial.println("startString: " + startString);
            Serial.println("New Operator Index: " + operatorIndex);
            if (operatorIndex == "1")
            {
                operatorName = str.substring(delim2 + 1, delim3);
                operatorPass = str.substring(delim3 + 1);
                Serial.println("New Operator Name: " + operatorName);
                Serial.println("New Operator Password: " + operatorPass);
                operatorName1 = operatorName;
                operatorPass1 = operatorPass;
                saveOperatorName1(operatorName1);
                saveOperatorPass1(operatorPass1);
                setVisibility("vis p8,0");
                updateTextBox("t0", operatorName1);
                updateTextBox("t1", operatorPass1);
                state = 0;
                operatorName = "";
                operatorPass = "";
                operatorIndex = "";
            }
            else if (operatorIndex == "2")
            {
                operatorName = str.substring(delim2 + 1, delim3);
                operatorPass = str.substring(delim3 + 1);
                Serial.println("New Operator Name: " + operatorName);
                Serial.println("New Operator Password: " + operatorPass);
                operatorName2 = operatorName;
                operatorPass2 = operatorPass;
                saveOperatorName2(operatorName2);
                saveOperatorPass2(operatorPass2);
                updateTextBox("t2", operatorName2);
                updateTextBox("t3", operatorPass2);
                state = 0;
                setVisibility("vis p8,0");
                operatorName = "";
                operatorPass = "";
                operatorIndex = "";
            }
            else if (operatorIndex == "3")
            {
                operatorName = str.substring(delim2 + 1, delim3);
                operatorPass = str.substring(delim3 + 1);
                Serial.println("New Operator Name: " + operatorName);
                Serial.println("New Operator Password: " + operatorPass);
                operatorName3 = operatorName;
                operatorPass3 = operatorPass;
                saveOperatorName3(operatorName3);
                saveOperatorPass3(operatorPass3);
                updateTextBox("t4", operatorName3);
                updateTextBox("t5", operatorPass3);
                state = 0;
                setVisibility("vis p8,0");
                operatorName = "";
                operatorPass = "";
                operatorIndex = "";
            }
            else if (operatorIndex == "4")
            {
                operatorName = str.substring(delim2 + 1, delim3);
                operatorPass = str.substring(delim3 + 1);
                Serial.println("New Operator Name: " + operatorName);
                Serial.println("New Operator Password: " + operatorPass);
                operatorName4 = operatorName;
                operatorPass4 = operatorPass;
                saveOperatorName4(operatorName4);
                saveOperatorPass4(operatorPass4);
                state = 0;
                setVisibility("vis p8,0");
                updateTextBox("t6", operatorName4);
                updateTextBox("t7", operatorPass4);
                operatorName = "";
                operatorPass = "";
                operatorIndex = "";
            }
            else
            {
                Serial.println("Index invalid!!");
                changePage(4);
                setVisibility("vis p8,1");
                state = 0;
            }
        }
        break;

    case 3: // new order registation and send server activation
        Serial.println("state 3");

        delim1 = str.indexOf(";");
        delim2 = str.indexOf(";", delim1 + 1);
        delim3 = str.indexOf(";", delim2 + 1);
        delim4 = str.indexOf(";", delim3 + 1);
        delim5 = str.indexOf(";", delim4 + 1);
        delim6 = str.indexOf(";", delim5 + 1);

        startString = str.substring(0, delim1);
        operatorName = str.substring(delim1 + 1, delim2);
        operatorID = str.substring(delim2 + 1, delim3);
        data1 = str.substring(delim3 + 1, delim4);
        data2 = str.substring(delim4 + 1, delim5);
        data3 = str.substring(delim5 + 1, delim6);
        data4 = str.substring(delim6 + 1);

        Serial.println("Start String: " + startString);
        Serial.println("Operator Name: " + operatorName);
        Serial.println("Operator ID: " + operatorID);
        Serial.println("Data 1:" + data1);
        Serial.println("Data 2:" + data2);
        Serial.println("Data 3:" + data3);
        Serial.println("Data 4:" + data4);

        // saveLoginOperatorName(operatorName);
        // saveLoginOperatorID(operatorID);
        // saveShift(data1);
        // saveOrderID(data2);
        // saveFabrication(data3);
        // saveMachineID(data4);

        if (data4 == "M01")
        {
            statusID_01 = 1; // green
            if (operatorName == operatorName1)
                operatorName_M01 = operatorName1;
            else
                operatorName_M01 = operatorName;
            operatorID_M01 = operatorID;
            orderNmber_M01 = data2;
            fabrication_M01 = data3;
        }
        else if (data4 == "M02")
        {
            statusID_02 = 1;
            if (operatorName == operatorName2)
                operatorName_M02 = operatorName2;
            else
                operatorName_M02 = operatorName;
            operatorID_M02 = operatorID;
            orderNmber_M02 = data2;
            fabrication_M02 = data3;
        }
        else if (data4 == "M07")
        {
            statusID_07 = 1;
            if (operatorName == operatorName3)
                operatorName_M07 = operatorName3;
            else
                operatorName_M07 = operatorName;

            operatorID_M07 = operatorID;
            orderNmber_M07 = data2;
            fabrication_M07 = data3;
        }
        else if (data4 == "M23")
        {
            statusID_23 = 1;
            if (operatorName == operatorName4)
                operatorName_M23 = operatorName4;
            else
                operatorName_M23 = operatorName;
            operatorID_M23 = operatorID;
            orderNmber_M23 = data2;
            fabrication_M23 = data3;
        }
        else
        {
            Serial.println("No Data Matched!!");
        }
        EEPROM.put(INT1_ADDR, statusID_01);
        EEPROM.put(INT2_ADDR, statusID_02);
        EEPROM.put(INT3_ADDR, statusID_07);
        EEPROM.put(INT4_ADDR, statusID_23);
        EEPROM.commit();
        changePage(5);
        updateTextBox("t10", username);
        sendTelemetryData();
        SEND_Active = 1;
        state = 0;
        startString = "";
        operatorName = "";
        operatorID = "";
        data1 = "";
        data2 = "";
        data3 = "";
        data4 = "";
        break;

    case 4: // Set Breakdown and send server activation
        Serial.println("state 4");
        delim1 = str.indexOf(";");
        delim2 = str.indexOf(";", delim1 + 1);
        delim3 = str.indexOf(";", delim2 + 1);
        delim4 = str.indexOf(";", delim3 + 1);

        startString = str.substring(0, delim1);
        errorCode = str.substring(delim1 + 1, delim2);
        operatorName = str.substring(delim2 + 1, delim3);
        operatorID = str.substring(delim3 + 1, delim4);
        deviceId = str.substring(delim4 + 1);

        Serial.println("Start String: " + startString);
        Serial.println("Error Code: " + errorCode);
        Serial.println("Operator Name: " + operatorName);
        Serial.println("Operator ID: " + operatorID);
        Serial.println("Device ID: " + deviceId);

        if (deviceId == "M01")
        {
            statusID_01 = 2; // green
            if (operatorName == operatorName1)
                operatorName_M01 = operatorName1;
            else
                operatorName_M01 = operatorName;
            operatorID_M01 = operatorID;
            errorCode_M01 = errorCode;
        }
        else if (deviceId == "M02")
        {
            statusID_02 = 2;
            if (operatorName == operatorName2)
                operatorName_M02 = operatorName2;
            else
                operatorName_M02 = operatorName;
            operatorID_M02 = operatorID;
            errorCode_M02 = errorCode;
        }
        else if (deviceId == "M07")
        {
            statusID_07 = 2;
            if (operatorName == operatorName3)
                operatorName_M07 = operatorName3;
            else
                operatorName_M07 = operatorName;

            operatorID_M07 = operatorID;
            orderNmber_M07 = data2;
            errorCode_M07 = errorCode;
        }
        else if (deviceId == "M23")
        {
            statusID_23 = 2;
            if (operatorName == operatorName4)
                operatorName_M23 = operatorName4;
            else
                operatorName_M23 = operatorName;
            operatorID_M23 = operatorID;
            errorCode_M23 = errorCode;
        }
        {
            Serial.println("No Data Matched!!");
        }

        // saveErrorCode_01(errorCode_M01);
        // saveErrorCode_02(errorCode_M02);
        // saveErrorCode_07(errorCode_M07);
        // saveErrorCode_23(errorCode_M23);
        breakFlag = 1;
        EEPROM.put(INT1_ADDR, statusID_01);
        EEPROM.put(INT2_ADDR, statusID_02);
        EEPROM.put(INT3_ADDR, statusID_07);
        EEPROM.put(INT4_ADDR, statusID_23);
        EEPROM.put(INT11_ADDR, breakFlag);
        EEPROM.commit();
        changePage(5);
        updateTextBox("t10", username);
        oneSecCount3 = 0;
        sendTelemetryData();
        breakFlag = 1;
        SEND_Active = 1;
        state = 0;
        startString = "";
        errorCode = "";
        operatorName = "";
        operatorID = "";
        deviceId = "";

        break;

    case 5: // finished the breakdown
        Serial.println("state 5");
        delim1 = str.indexOf(";");
        startString = str.substring(0, delim1);
        deviceId = str.substring(delim1 + 1);

        Serial.println("Start String: " + startString);
        Serial.println("Device ID: " + deviceId);

        if (startString == "breakdownCompleted")
        {
            breakCount++;

            if (deviceId == "M01")
            {
                // statusID_01 = 3;
                errorCode_M01 = "0";
            }
            else if (deviceId == "M02")
            {
                // statusID_02 = 3;
                errorCode_M02 = "0";
            }
            else if (deviceId == "M07")
            {
                // statusID_07 = 3;
                errorCode_M07 = "0";
            }
            else if (deviceId == "M23")
            {
                // statusID_23 = 3;
                errorCode_M23 = "0";
            }
            else
            {
                Serial.println("No Data Matched!!");
            }

            // saveErrorCode_01(errorCode_M01);
            // saveErrorCode_02(errorCode_M02);
            // saveErrorCode_07(errorCode_M07);
            // saveErrorCode_23(errorCode_M23);
            breakFlag = 0;
            EEPROM.put(INT1_ADDR, statusID_01);
            EEPROM.put(INT2_ADDR, statusID_02);
            EEPROM.put(INT3_ADDR, statusID_07);
            EEPROM.put(INT4_ADDR, statusID_23);
            EEPROM.put(INT5_ADDR, breakCount);
            EEPROM.put(INT11_ADDR, breakFlag);
            EEPROM.commit();
            changePage(5);
            updateTextBox("t10", username);
            oneSecCount3 = 0;
            sendTelemetryData();
            SEND_Active = 1;
            state = 0;
            EPPROM_Active = 1;
            startString = "";
            deviceId = "";
        }
        break;

    case 6: // finished the order
        Serial.println("state 6");
        delim1 = str.indexOf(";");
        startString = str.substring(0, delim1);
        deviceId = str.substring(delim1 + 1);

        Serial.println("Start String: " + startString);
        Serial.println("Device ID: " + deviceId);

        if (startString == "orderCompleted")
        {
            if (deviceId == "M01")
            {
                statusID_01 = 3;
            }
            else if (deviceId == "M02")
            {
                statusID_02 = 3;
            }
            else if (deviceId == "M07")
            {
                statusID_07 = 3;
            }
            else if (deviceId == "M23")
            {
                statusID_23 = 3;
            }
            else
            {
                Serial.println("No Data Matched!!");
            }

            EEPROM.put(INT1_ADDR, statusID_01);
            EEPROM.put(INT2_ADDR, statusID_02);
            EEPROM.put(INT3_ADDR, statusID_07);
            EEPROM.put(INT4_ADDR, statusID_23);
            EEPROM.commit();
            changePage(5);
            updateTextBox("t10", username);
            oneSecCount3 = 0;
            sendTelemetryData();
            SEND_Active = 1;
            state = 0;
            startString = "";
            deviceId = "";
        }
        break;

    case 7: // cancel any
        Serial.println("state 7");
        delim1 = str.indexOf(";");
        startString = str.substring(0, delim1);
        Serial.println("Start String: " + startString);
        if (startString == "cancelAny")
        {
            changePage(2);
            updateTextBox("t10", username);
            EPPROM_Active = 0;
            oneSecCount2 = 0;
            oneSecCount3 = 0;
            state = 0;
            startString = "";
        }
        break;
    default:
        break;
    }
}
