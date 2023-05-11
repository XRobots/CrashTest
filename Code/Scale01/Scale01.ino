// load cell Teendy and Sparkfun Qwiic scale board.

#include <Wire.h>
#include "SparkFun_Qwiic_Scale_NAU7802_Arduino_Library.h" // Click here to get the library: http://librarymanager/All#SparkFun_NAU8702

NAU7802 myScale; //Create instance of the NAU7802 class

long startupReading;    // used for zeroing
long currentReading;    // the actual reading
float scaledReading;    // scaled for Kg
int sendS;              // value to send as Integer

void setup()
{

  Serial.begin(115200);     // debug
  Serial1.begin(115200);    // port to send value

  Wire.begin();
  Wire.setClock(400000); //Qwiic Scale is capable of running at 400kHz if desired

  if (myScale.begin() == false)
  {
    Serial.println("Scale not detected. Please check wiring. Freezing...");
    while (1);
  }
  Serial.println("Scale detected!");

  

  myScale.setSampleRate(NAU7802_SPS_320); //Increase to max sample rate
  myScale.calibrateAFE(); //Re-cal analog front end when we change gain, sample rate, or channel   

  delay (1000);

  startupReading = myScale.getReading();      // get startup value to zero offset

  delay (1000);

}

void loop()
{


  if (myScale.available() == true)
  {
    currentReading = myScale.getReading();
    currentReading = currentReading - startupReading;   // remove offset / zero the scale

    scaledReading = (float) currentReading / 80000;            // approx scale for Kg

    //Serial.print("Reading: ");
    //Serial.println(scaledReading);

    sendS = scaledReading * 100;

    Serial1.print(sendS);
    Serial1.print(",");

  }

  delay(20);
}



