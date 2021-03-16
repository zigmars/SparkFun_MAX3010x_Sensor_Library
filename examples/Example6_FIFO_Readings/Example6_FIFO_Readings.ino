/*
  MAX30105 Breakout: Take readings from the FIFO
  By: Nathan Seidle @ SparkFun Electronics
  Date: October 2nd, 2016
  https://github.com/sparkfun/MAX30105_Breakout

  Outputs all Red/IR/Green values at 25Hz by polling the FIFO

  Hardware Connections (Breakoutboard to Arduino):
  -5V = 5V (3.3V is allowed)
  -GND = GND
  -SDA = A4 (or SDA)
  -SCL = A5 (or SCL)
  -INT = Not connected
 
  The MAX30105 Breakout can handle 5V or 3.3V I2C logic. We recommend powering the board with 5V
  but it will also run at 3.3V.

  This code is released under the [MIT License](http://opensource.org/licenses/MIT).
*/

#include <Wire.h>
extern TwoWire Wire1;
#include "MAX30105.h"

MAX30105 ps1, ps2;

long startTime;
long samplesTaken = 0; //Counter for calculating the Hz or read rate

void setup()
{
  Serial.begin(115200);
  Serial.println("Initializing...");

  // Initialize sensor
  if (ps1.begin(Wire, I2C_SPEED_FAST) == false) //Use default I2C port, 400kHz speed
  {
    Serial.println("1st MAX30105 on 'Wire' was not found. Please check wiring/power. ");
    while (1);
  }

  if (ps2.begin(Wire1, I2C_SPEED_FAST) == false) //Use default I2C port, 400kHz speed
  {
    Serial.println("2nd MAX30105 on 'Wire1' was not found. Please check wiring/power. ");
    while (1);
  }
  //Setup to sense up to 18 inches, max LED brightness
  byte ledBrightness = 0xFF; //Options: 0=Off to 255=50mA
  byte sampleAverage = 4; //Options: 1, 2, 4, 8, 16, 32
  byte ledMode = 2; //Options: 1 = Red only, 2 = Red + IR, 3 = Red + IR + Green
  int sampleRate = 400; //Options: 50, 100, 200, 400, 800, 1000, 1600, 3200
  int pulseWidth = 411; //Options: 69, 118, 215, 411
  int adcRange = 2048; //Options: 2048, 4096, 8192, 16384

  ps1.setup(ledBrightness, sampleAverage, ledMode, sampleRate, pulseWidth, adcRange); //Configure sensor with these settings
//  ps1.setup(); //Configure sensor. Use 6.4mA for LED drive
  ps2.setup(ledBrightness, sampleAverage, ledMode, sampleRate, pulseWidth, adcRange); //Configure sensor with these settings

  startTime = millis();
}

void loop()
{
  ps1.check(); //Check the sensor, read up to 3 samples
  ps2.check(); //Check the sensor, read up to 3 samples

  if (ps1.available()) //do we have new data?
  {
    Serial.print("PS1:");
    Serial.print(ps1.getFIFORed());
    Serial.print(',');
    Serial.print(ps1.getFIFOIR());
    Serial.print(',');
    Serial.print(ps1.getFIFOGreen());
    Serial.print(',');
    Serial.println(millis() - startTime);
    ps1.nextSample(); //We're finished with this sample so move to next sample
  }
  if (ps2.available()) //do we have new data?
  {
    Serial.print("PS2:");
    Serial.print(ps2.getFIFORed());
    Serial.print(',');
    Serial.print(ps2.getFIFOIR());
    Serial.print(',');
    Serial.print(ps2.getFIFOGreen());
    Serial.print(',');
    Serial.println(millis() - startTime);
    ps2.nextSample(); //We're finished with this sample so move to next sample
  }
}
