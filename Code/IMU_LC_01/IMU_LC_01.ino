// Teensy LC in head with IMU attached

#include "Wire.h"
#include "I2Cdev.h"
#include "MPU6050.h"

MPU6050 accelgyro;
int16_t ax, ay, az;
int16_t gx, gy, gz;

#define Gyr_Gain 0.00763358 

#define LED_PIN 13
bool blinkState = false;

float AccelX;
float AccelY;
float AccelZ;

int sendX;
int sendY;
int sendZ;

long previousMillis = 0;    // set up timers
unsigned long currentMillis;    
long interval = 20;        // time constant for timers

void setup() {
  Wire.begin();
  accelgyro.initialize();
  accelgyro.setFullScaleAccelRange(MPU6050_ACCEL_FS_16);
  pinMode(LED_PIN, OUTPUT);  
  Serial.begin(115200);         // debug
  Serial1.begin(115200);        // port to send data
}

void loop() {

  currentMillis = millis();          // bookmark current time
  
  if (currentMillis - previousMillis >= interval)      // if the right amount of time has passed then...
      {
      previousMillis = currentMillis;                      // boomark the time again
    
      accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  
      AccelX = (float) ax / 2100;   // Accel offsets
      AccelY = (float) ay / 2010;
      AccelZ = (float) az / 1860;

      /*
      Serial.print(AccelX,2);
      Serial.print('\t');
      Serial.print(AccelY,2);
      Serial.print('\t');
      Serial.print(AccelZ,2);
      Serial.println();

      */

      sendX = AccelX * 100;       // multiply by 100 so we can send Integers
      sendY = AccelY * 100;
      sendZ = AccelZ * 100;

      Serial1.print(5000);        // send marker value that we will never esperience in Gs
      Serial1.print(",");
      Serial1.print(sendX);       // send the other values (on Serial 1)
      Serial1.print(",");
      Serial1.print(sendY);
      Serial1.print(",");
      Serial1.print(sendZ);
      Serial1.print(",");   
      
      /*
      Serial.print(sendX);
      Serial.print('\t');
      Serial.print(sendY);
      Serial.print('\t');
      Serial.print(sendZ);
      Serial.println();
      */

            
      // blink LED to indicate activity
      blinkState = !blinkState;
      digitalWrite(LED_PIN, blinkState);
    
      }
      


}
