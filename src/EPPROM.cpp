#include <Arduino.h>
#include <lib.h>

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

String readOperatorName1();
String readOperatorName2();
String readOperatorName3();
String readOperatorName4();
String readOperatorPass1();
String readOperatorPass2();
String readOperatorPass3();
String readOperatorPass4();
String readLoginOperator();
String readLoginOperatorID();
String readShift();
String readOrderID();
String readFabrication();
String readMachineID();
String readErrorCode_01();
String readErrorCode_02();
String readErrorCode_07();
String readErrorCode_23();

void getEPPROM()
{
    operatorName1 = readOperatorName1();
    operatorName2 = readOperatorName2();
    operatorName3 = readOperatorName3();
    operatorName4 = readOperatorName4();
    operatorPass1 = readOperatorPass1();
    operatorPass2 = readOperatorPass2();
    operatorPass3 = readOperatorPass3();
    operatorPass4 = readOperatorPass4();
    operatorName = readLoginOperator();
    operatorID = readLoginOperatorID();
    data1 = readShift();
    data2 = readOrderID();
    data3 = readFabrication();
    data4 = readMachineID();
    errorCode_M01 = readErrorCode_01();
    errorCode_M02 = readErrorCode_02();
    errorCode_M07 = readErrorCode_07();
    errorCode_M23 = readErrorCode_23();
    EEPROM.get(INT1_ADDR, elocoID_01);
    EEPROM.get(INT2_ADDR, elocoID_02);
    EEPROM.get(INT3_ADDR, elocoID_07);
    EEPROM.get(INT4_ADDR, elocoID_23);
    EEPROM.get(INT5_ADDR, ebreakCount);
    EEPROM.get(INT6_ADDR, eoneSecCount1);
    EEPROM.get(INT7_ADDR, eoneSecCount4);
    EEPROM.get(INT8_ADDR, eoneSecCount5);
    EEPROM.get(INT9_ADDR, eoneSecCount6);
    EEPROM.get(INT10_ADDR, eoneSecCount7);
    EEPROM.get(INT11_ADDR, ebreakflag);

    if (elocoID_01 == 0)
        statusID_01 = 3;
    else
        statusID_01 = elocoID_01;
    if (elocoID_02 == 0)
        statusID_02 = 3;
    else
        statusID_02 = elocoID_02;
    if (elocoID_07 == 0)
        statusID_07 = 3;
    else
        statusID_07 = elocoID_07;
    if (elocoID_23 == 0)
        statusID_23 = 3;
    else
        statusID_23 = elocoID_23;

    breakCount = ebreakCount;
    breakFlag = ebreakflag;
    oneSecCount1 = eoneSecCount1;
    oneSecCount4 = eoneSecCount4;
    oneSecCount5 = eoneSecCount5;
    oneSecCount6 = eoneSecCount6;
    oneSecCount7 = eoneSecCount7;

    Serial.println("****Read EPPROM Data****");
    Serial.println("User Name1:" + operatorName1);
    Serial.println("User Pass1:" + operatorPass1);
    Serial.println("User Name2:" + operatorName2);
    Serial.println("User Pass2:" + operatorPass2);
    Serial.println("User Name3:" + operatorName3);
    Serial.println("User Pass3:" + operatorPass3);
    Serial.println("User Name4:" + operatorName4);
    Serial.println("User Pass4:" + operatorPass4);
    Serial.println("operator Name:" + operatorName);
    Serial.println("Operator ID:" + operatorID);
    Serial.println("Shift ID:" + data1);
    Serial.println("Order ID:" + data2);
    Serial.println("Fabrication:" + data3);
    Serial.println("Machine ID:" + data4);
    Serial.println("Error Code_01:" + errorCode_M01);
    Serial.println("Error Code_02:" + errorCode_M02);
    Serial.println("Error Code_07:" + errorCode_M07);
    Serial.println("Error Code_23:" + errorCode_M23);
    Serial.print("statusID_01:");
    Serial.println(statusID_01);
    Serial.print("statusID_02:");
    Serial.println(statusID_02);
    Serial.print("statusID_07:");
    Serial.println(statusID_07);
    Serial.print("statusID_23:");
    Serial.println(statusID_23);
    Serial.print("Break Count:");
    Serial.println(breakCount);
    Serial.print("Break Flag:");
    Serial.println(breakFlag);
    Serial.print("Toatl Breakdown Time:");
    Serial.println(oneSecCount1);
    Serial.print("MachineID_01 Breakdown Time:");
    Serial.println(oneSecCount4);
    Serial.print("MachineID_02 Breakdown Time:");
    Serial.println(oneSecCount5);
    Serial.print("MachineID_07 Breakdown Time:");
    Serial.println(oneSecCount6);
    Serial.print("MachineID_23 Breakdown Timet:");
    Serial.println(oneSecCount7);

    Serial.println("************************");
}

void saveOperatorName1(String text)
{
    int len1 = text.length();
    if (len1 > MAX_STRING_LENGTH)
        len1 = MAX_STRING_LENGTH;

    for (int i = 0; i < len1; i++)
    {
        EEPROM.write(STRING1_ADDR + i, text[i]);
    }
    EEPROM.write(STRING1_ADDR + len1, '\0'); // Null-terminate the string
    EEPROM.commit();
}

String readOperatorName1()
{
    char text[MAX_STRING_LENGTH + 1]; // +1 for the null terminator
    for (int i = 0; i < MAX_STRING_LENGTH; i++)
    {
        text[i] = EEPROM.read(STRING1_ADDR + i);
        if (text[i] == '\0')
            break; // Stop if null terminator is found
    }
    text[MAX_STRING_LENGTH] = '\0'; // Ensure null termination
    return String(text);
}

void saveOperatorName2(String text)
{
    int len1 = text.length();
    if (len1 > MAX_STRING_LENGTH)
        len1 = MAX_STRING_LENGTH;

    for (int i = 0; i < len1; i++)
    {
        EEPROM.write(STRING2_ADDR + i, text[i]);
    }
    EEPROM.write(STRING2_ADDR + len1, '\0'); // Null-terminate the string
    EEPROM.commit();
}

String readOperatorName2()
{
    char text[MAX_STRING_LENGTH + 1]; // +1 for the null terminator
    for (int i = 0; i < MAX_STRING_LENGTH; i++)
    {
        text[i] = EEPROM.read(STRING2_ADDR + i);
        if (text[i] == '\0')
            break; // Stop if null terminator is found
    }
    text[MAX_STRING_LENGTH] = '\0'; // Ensure null termination
    return String(text);
}

void saveOperatorName3(String text)
{
    int len1 = text.length();
    if (len1 > MAX_STRING_LENGTH)
        len1 = MAX_STRING_LENGTH;

    for (int i = 0; i < len1; i++)
    {
        EEPROM.write(STRING3_ADDR + i, text[i]);
    }
    EEPROM.write(STRING3_ADDR + len1, '\0'); // Null-terminate the string
    EEPROM.commit();
}

String readOperatorName3()
{
    char text[MAX_STRING_LENGTH + 1]; // +1 for the null terminator
    for (int i = 0; i < MAX_STRING_LENGTH; i++)
    {
        text[i] = EEPROM.read(STRING3_ADDR + i);
        if (text[i] == '\0')
            break; // Stop if null terminator is found
    }
    text[MAX_STRING_LENGTH] = '\0'; // Ensure null termination
    return String(text);
}

void saveOperatorName4(String text)
{
    int len1 = text.length();
    if (len1 > MAX_STRING_LENGTH)
        len1 = MAX_STRING_LENGTH;

    for (int i = 0; i < len1; i++)
    {
        EEPROM.write(STRING4_ADDR + i, text[i]);
    }
    EEPROM.write(STRING4_ADDR + len1, '\0'); // Null-terminate the string
    EEPROM.commit();
}

String readOperatorName4()
{
    char text[MAX_STRING_LENGTH + 1]; // +1 for the null terminator
    for (int i = 0; i < MAX_STRING_LENGTH; i++)
    {
        text[i] = EEPROM.read(STRING4_ADDR + i);
        if (text[i] == '\0')
            break; // Stop if null terminator is found
    }
    text[MAX_STRING_LENGTH] = '\0'; // Ensure null termination
    return String(text);
}

void saveOperatorPass1(String text)
{
    int len1 = text.length();
    if (len1 > MAX_STRING_LENGTH)
        len1 = MAX_STRING_LENGTH;

    for (int i = 0; i < len1; i++)
    {
        EEPROM.write(STRING5_ADDR + i, text[i]);
    }
    EEPROM.write(STRING5_ADDR + len1, '\0'); // Null-terminate the string
    EEPROM.commit();
}

String readOperatorPass1()
{
    char text[MAX_STRING_LENGTH + 1]; // +1 for the null terminator
    for (int i = 0; i < MAX_STRING_LENGTH; i++)
    {
        text[i] = EEPROM.read(STRING5_ADDR + i);
        if (text[i] == '\0')
            break; // Stop if null terminator is found
    }
    text[MAX_STRING_LENGTH] = '\0'; // Ensure null termination
    return String(text);
}

void saveOperatorPass2(String text)
{
    int len1 = text.length();
    if (len1 > MAX_STRING_LENGTH)
        len1 = MAX_STRING_LENGTH;

    for (int i = 0; i < len1; i++)
    {
        EEPROM.write(STRING6_ADDR + i, text[i]);
    }
    EEPROM.write(STRING6_ADDR + len1, '\0'); // Null-terminate the string
    EEPROM.commit();
}

String readOperatorPass2()
{
    char text[MAX_STRING_LENGTH + 1]; // +1 for the null terminator
    for (int i = 0; i < MAX_STRING_LENGTH; i++)
    {
        text[i] = EEPROM.read(STRING6_ADDR + i);
        if (text[i] == '\0')
            break; // Stop if null terminator is found
    }
    text[MAX_STRING_LENGTH] = '\0'; // Ensure null termination
    return String(text);
}

void saveOperatorPass3(String text)
{
    int len1 = text.length();
    if (len1 > MAX_STRING_LENGTH)
        len1 = MAX_STRING_LENGTH;

    for (int i = 0; i < len1; i++)
    {
        EEPROM.write(STRING7_ADDR + i, text[i]);
    }
    EEPROM.write(STRING7_ADDR + len1, '\0'); // Null-terminate the string
    EEPROM.commit();
}

String readOperatorPass3()
{
    char text[MAX_STRING_LENGTH + 1]; // +1 for the null terminator
    for (int i = 0; i < MAX_STRING_LENGTH; i++)
    {
        text[i] = EEPROM.read(STRING7_ADDR + i);
        if (text[i] == '\0')
            break; // Stop if null terminator is found
    }
    text[MAX_STRING_LENGTH] = '\0'; // Ensure null termination
    return String(text);
}

void saveOperatorPass4(String text)
{
    int len1 = text.length();
    if (len1 > MAX_STRING_LENGTH)
        len1 = MAX_STRING_LENGTH;

    for (int i = 0; i < len1; i++)
    {
        EEPROM.write(STRING8_ADDR + i, text[i]);
    }
    EEPROM.write(STRING8_ADDR + len1, '\0'); // Null-terminate the string
    EEPROM.commit();
}

String readOperatorPass4()
{
    char text[MAX_STRING_LENGTH + 1]; // +1 for the null terminator
    for (int i = 0; i < MAX_STRING_LENGTH; i++)
    {
        text[i] = EEPROM.read(STRING8_ADDR + i);
        if (text[i] == '\0')
            break; // Stop if null terminator is found
    }
    text[MAX_STRING_LENGTH] = '\0'; // Ensure null termination
    return String(text);
}

void saveLoginOperatorName(String text)
{
    int len1 = text.length();
    if (len1 > MAX_STRING_LENGTH)
        len1 = MAX_STRING_LENGTH;

    for (int i = 0; i < len1; i++)
    {
        EEPROM.write(STRING9_ADDR + i, text[i]);
    }
    EEPROM.write(STRING9_ADDR + len1, '\0'); // Null-terminate the string
    EEPROM.commit();
}

String readLoginOperator()
{
    char text[MAX_STRING_LENGTH + 1]; // +1 for the null terminator
    for (int i = 0; i < MAX_STRING_LENGTH; i++)
    {
        text[i] = EEPROM.read(STRING9_ADDR + i);
        if (text[i] == '\0')
            break; // Stop if null terminator is found
    }
    text[MAX_STRING_LENGTH] = '\0'; // Ensure null termination
    return String(text);
}

void saveLoginOperatorID(String text)
{
    int len1 = text.length();
    if (len1 > MAX_STRING_LENGTH)
        len1 = MAX_STRING_LENGTH;

    for (int i = 0; i < len1; i++)
    {
        EEPROM.write(STRING10_ADDR + i, text[i]);
    }
    EEPROM.write(STRING10_ADDR + len1, '\0'); // Null-terminate the string
    EEPROM.commit();
}

String readLoginOperatorID()
{
    char text[MAX_STRING_LENGTH + 1]; // +1 for the null terminator
    for (int i = 0; i < MAX_STRING_LENGTH; i++)
    {
        text[i] = EEPROM.read(STRING10_ADDR + i);
        if (text[i] == '\0')
            break; // Stop if null terminator is found
    }
    text[MAX_STRING_LENGTH] = '\0'; // Ensure null termination
    return String(text);
}

void saveShift(String text)
{
    int len1 = text.length();
    if (len1 > MAX_STRING_LENGTH)
        len1 = MAX_STRING_LENGTH;

    for (int i = 0; i < len1; i++)
    {
        EEPROM.write(STRING11_ADDR + i, text[i]);
    }
    EEPROM.write(STRING11_ADDR + len1, '\0'); // Null-terminate the string
    EEPROM.commit();
}

String readShift()
{
    char text[MAX_STRING_LENGTH + 1]; // +1 for the null terminator
    for (int i = 0; i < MAX_STRING_LENGTH; i++)
    {
        text[i] = EEPROM.read(STRING11_ADDR + i);
        if (text[i] == '\0')
            break; // Stop if null terminator is found
    }
    text[MAX_STRING_LENGTH] = '\0'; // Ensure null termination
    return String(text);
}

void saveOrderID(String text)
{
    int len1 = text.length();
    if (len1 > MAX_STRING_LENGTH)
        len1 = MAX_STRING_LENGTH;

    for (int i = 0; i < len1; i++)
    {
        EEPROM.write(STRING12_ADDR + i, text[i]);
    }
    EEPROM.write(STRING12_ADDR + len1, '\0'); // Null-terminate the string
    EEPROM.commit();
}

String readOrderID()
{
    char text[MAX_STRING_LENGTH + 1]; // +1 for the null terminator
    for (int i = 0; i < MAX_STRING_LENGTH; i++)
    {
        text[i] = EEPROM.read(STRING12_ADDR + i);
        if (text[i] == '\0')
            break; // Stop if null terminator is found
    }
    text[MAX_STRING_LENGTH] = '\0'; // Ensure null termination
    return String(text);
}

void saveFabrication(String text)
{
    int len1 = text.length();
    if (len1 > MAX_STRING_LENGTH)
        len1 = MAX_STRING_LENGTH;

    for (int i = 0; i < len1; i++)
    {
        EEPROM.write(STRING13_ADDR + i, text[i]);
    }
    EEPROM.write(STRING13_ADDR + len1, '\0'); // Null-terminate the string
    EEPROM.commit();
}

String readFabrication()
{
    char text[MAX_STRING_LENGTH + 1]; // +1 for the null terminator
    for (int i = 0; i < MAX_STRING_LENGTH; i++)
    {
        text[i] = EEPROM.read(STRING13_ADDR + i);
        if (text[i] == '\0')
            break; // Stop if null terminator is found
    }
    text[MAX_STRING_LENGTH] = '\0'; // Ensure null termination
    return String(text);
}

void saveMachineID(String text)
{
    int len1 = text.length();
    if (len1 > MAX_STRING_LENGTH)
        len1 = MAX_STRING_LENGTH;

    for (int i = 0; i < len1; i++)
    {
        EEPROM.write(STRING14_ADDR + i, text[i]);
    }
    EEPROM.write(STRING14_ADDR + len1, '\0'); // Null-terminate the string
    EEPROM.commit();
}

String readMachineID()
{
    char text[MAX_STRING_LENGTH + 1]; // +1 for the null terminator
    for (int i = 0; i < MAX_STRING_LENGTH; i++)
    {
        text[i] = EEPROM.read(STRING14_ADDR + i);
        if (text[i] == '\0')
            break; // Stop if null terminator is found
    }
    text[MAX_STRING_LENGTH] = '\0'; // Ensure null termination
    return String(text);
}

void saveErrorCode_01(String text)
{
    int len1 = text.length();
    if (len1 > MAX_STRING_LENGTH)
        len1 = MAX_STRING_LENGTH;

    for (int i = 0; i < len1; i++)
    {
        EEPROM.write(STRING15_ADDR + i, text[i]);
    }
    EEPROM.write(STRING15_ADDR + len1, '\0'); // Null-terminate the string
    EEPROM.commit();
}

String readErrorCode_01()
{
    char text[MAX_STRING_LENGTH + 1]; // +1 for the null terminator
    for (int i = 0; i < MAX_STRING_LENGTH; i++)
    {
        text[i] = EEPROM.read(STRING15_ADDR + i);
        if (text[i] == '\0')
            break; // Stop if null terminator is found
    }
    text[MAX_STRING_LENGTH] = '\0'; // Ensure null termination
    return String(text);
}

void saveErrorCode_02(String text)
{
    int len1 = text.length();
    if (len1 > MAX_STRING_LENGTH)
        len1 = MAX_STRING_LENGTH;

    for (int i = 0; i < len1; i++)
    {
        EEPROM.write(STRING16_ADDR + i, text[i]);
    }
    EEPROM.write(STRING16_ADDR + len1, '\0'); // Null-terminate the string
    EEPROM.commit();
}

String readErrorCode_02()
{
    char text[MAX_STRING_LENGTH + 1]; // +1 for the null terminator
    for (int i = 0; i < MAX_STRING_LENGTH; i++)
    {
        text[i] = EEPROM.read(STRING16_ADDR + i);
        if (text[i] == '\0')
            break; // Stop if null terminator is found
    }
    text[MAX_STRING_LENGTH] = '\0'; // Ensure null termination
    return String(text);
}

void saveErrorCode_07(String text)
{
    int len1 = text.length();
    if (len1 > MAX_STRING_LENGTH)
        len1 = MAX_STRING_LENGTH;

    for (int i = 0; i < len1; i++)
    {
        EEPROM.write(STRING17_ADDR + i, text[i]);
    }
    EEPROM.write(STRING17_ADDR + len1, '\0'); // Null-terminate the string
    EEPROM.commit();
}

String readErrorCode_07()
{
    char text[MAX_STRING_LENGTH + 1]; // +1 for the null terminator
    for (int i = 0; i < MAX_STRING_LENGTH; i++)
    {
        text[i] = EEPROM.read(STRING17_ADDR + i);
        if (text[i] == '\0')
            break; // Stop if null terminator is found
    }
    text[MAX_STRING_LENGTH] = '\0'; // Ensure null termination
    return String(text);
}

void saveErrorCode_23(String text)
{
    int len1 = text.length();
    if (len1 > MAX_STRING_LENGTH)
        len1 = MAX_STRING_LENGTH;

    for (int i = 0; i < len1; i++)
    {
        EEPROM.write(STRING18_ADDR + i, text[i]);
    }
    EEPROM.write(STRING18_ADDR + len1, '\0'); // Null-terminate the string
    EEPROM.commit();
}

String readErrorCode_23()
{
    char text[MAX_STRING_LENGTH + 1]; // +1 for the null terminator
    for (int i = 0; i < MAX_STRING_LENGTH; i++)
    {
        text[i] = EEPROM.read(STRING18_ADDR + i);
        if (text[i] == '\0')
            break; // Stop if null terminator is found
    }
    text[MAX_STRING_LENGTH] = '\0'; // Ensure null termination
    return String(text);
}