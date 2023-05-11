// Main Teensy 4.1 in body that receives all the data logs to SD

#include "Wire.h"
#include "I2Cdev.h"
#include "MPU6050.h"

#include <SD.h>
#include <SPI.h>

const int chipSelect = BUILTIN_SDCARD;

MPU6050 accelgyro;
int16_t ax, ay, az;
int16_t gx, gy, gz;

#define Gyr_Gain 0.00763358 

#define LED_PIN 13
bool blinkState = false;

int sw1;
int sw2;
int switchFlag = 0;

float AccelX;           // local Accel values
float AccelY;
float AccelZ;

float check1;           // head marker and accel values
float var1;
float var2;
float var3;

float var4;             // load cells
float var5;

long previousMillis = 0;    // set up timers
unsigned long currentMillis;    
long interval1 = 5;        // time constant for timers

long previousMillis2 = 0;    // set up timers
long interval2 = 50;        // time constant for timers

float dt=0.02;

void setup() {
  Wire.begin();
  accelgyro.initialize();
  accelgyro.setFullScaleAccelRange(MPU6050_ACCEL_FS_16);
  pinMode(LED_PIN, OUTPUT);  
  Serial.begin(115200);         // debug
  Serial1.begin(115200);        // port to receive data from head IMU/Teensy

  Serial2.begin(115200);        // port to receive data from load cell
  Serial3.begin(115200);        // port to receive data from load cell

  pinMode(31, INPUT_PULLUP);    // start logging
  pinMode(32, INPUT_PULLUP);    // stop logging

  Serial.print("Initializing SD card...");

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    while (1) {
      // No SD card, so don't do anything more - stay stuck here
    }
  }
  Serial.println("card initialized.");
  
}

void loop() {

  currentMillis = millis();          // bookmark current time
  
  if (currentMillis - previousMillis >= interval1)      // if the right amount of time has passed then...
            {
            previousMillis = currentMillis;                      // boomark the time again

            sw1 = digitalRead(31);
            sw2 = digitalRead(32);
                
            if (Serial1.available() > 1){      
              check1 = Serial1.parseInt();
                  if (check1 == 5000) {              //wait for the check value to come around before reading the rest of the data
                      var1 = Serial1.parseInt();     // head Accel X
                      var2 = Serial1.parseInt();     // head Accel Y
                      var3 = Serial1.parseInt();     // head Accel Z 
                      var1 = var1 / 100;             // divide values back down
                      var2 = var2 / 100;
                      var3 = var3 / 100;
                  }
            }

            if (Serial2.available() > 1){      
                    var4 = Serial2.parseInt();     // load cell
                    var4 = var4 / 100;             // divide values back down
            }

            if (Serial3.available() > 1){   
                    var5 = Serial3.parseInt();     // load cell
                    var5 = var5 / 100;             // divide values back down
            }
                
      }  // end of timed loop 1

      if (currentMillis - previousMillis2 >= interval2)      // if the right amount of time has passed then...
            {
            previousMillis2 = currentMillis;                      // boomark the time again

            accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
        
            AccelX = (float) ax / 2085;   // Accel offsets
            AccelY = (float) ay / 2030;
            AccelZ = (float) az / 2045;

            /*

            Serial.print(AccelX,2);
            Serial.print(" , ");
            Serial.print(AccelY,2);
            Serial.print(" , ");
            Serial.print(AccelZ,2);
            Serial.print(" , ");
            Serial.print(var1,2);
            Serial.print(" , ");
            Serial.print(var2,2);
            Serial.print(" , ");
            Serial.print(var3,2);
            Serial.print(" , ");            
            Serial.print(var4,2);
            Serial.print(" , ");
            Serial.print(var5,2);
            Serial.println();

            */

     

    


            // make a string for assembling the data to log:
            String dataString = "";
      
            File dataFile = SD.open("datalog.csv", FILE_WRITE);   // open the file   
            
      
            if (sw1 == 0 && switchFlag == 0) {                      // start logging switch is pressed
              switchFlag = 1;                                       // toggle the flag
              File dataFile = SD.open("datalog.csv", FILE_WRITE);   // open the file
            }
      
            if (switchFlag == 1) {
              dataString += String(AccelX,2);
              dataString += ",";
              dataString += String(AccelY,2);
              dataString += ",";
              dataString += String(AccelZ,2);
              dataString += ","; 
              dataString += String(var1,2);
              dataString += ",";
              dataString += String(var2,2);
              dataString += ",";
              dataString += String(var3,2);
              dataString += ",";
              dataString += String(var4,2);
              dataString += ",";
              dataString += String(var5,2);
              dataString += ",";
              dataFile.println(dataString);                          // write the datastring to the file as a new line    
            }
      
            if (sw2 == 0) {
              dataFile.close();                                      // close file
              switchFlag = 0;                                        // reset switch flag so we can record again
            }

            // blink LED to indicate activity
            blinkState = !blinkState;
            digitalWrite(LED_PIN, blinkState);
      
      }     // end of timed loop2
      
} // end of main loop
