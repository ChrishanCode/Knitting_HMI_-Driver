#include <Arduino.h>
#include <lib.h>

void sendCommand(String cmd);

void changePage(int pageNumber)
{
    String cmd = "page " + String(pageNumber);
    sendCommand(cmd);
}

void sendCommand(String cmd)
{
    nextionSerial.print(cmd);
    nextionSerial.write(0xFF); // End of command
    nextionSerial.write(0xFF); // End of command
    nextionSerial.write(0xFF); // End of command
}

void updateTextBox(String component, String value)
{
    String cmd = component + ".txt=\"" + value + "\""; //"t2.txt=\"" + operatorName + "\""
    Serial.print("Sending command to update text box: ");
    Serial.println(cmd);
    sendCommand(cmd);
}

void setVisibility(String code)
{
    String cmd = code;
    Serial.println(cmd);
    sendCommand(cmd);
}

