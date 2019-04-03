#include <PLabBTCallback.h>
#include <PLabBTLEController.h>
#include <PLabBTSerial.h>
#include <PLCUP.h>

/*
 * Kjør denne for å sette navnet riktig til arduinoen
 */

#include <SoftwareSerial.h>
#include <PLabBTSerial.h>

#define txPin 2  // Tx pin on Bluetooth unit
#define rxPin 3  // Rx pin on Bluetooth unit

char BTName[] = "G12";
char ATCommand[] = "AT+NAMEPLab_";
PLabBTSerial btSerial(txPin, rxPin);

void setup()
{
    Serial.begin(9600);   // Open serial communication to Serial Monitor
    btSerial.begin(9600); // Open serial communication to Bluetooth unit
}


void loop() {
    Serial.print("Setting new name for device to: ");
    Serial.print(ATCommand);
    Serial.println(BTName);

    btSerial.write(ATCommand); 
    btSerial.write(BTName);
    btSerial.write(0x0D); btSerial.write(0x0A);
    delay(1000);
}
