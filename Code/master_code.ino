
#include "GY521.h"
#include <Wire.h>
#include <IBusBM.h>
#include <math.h>


// Analog Input Channels
#define SCL A5 // SCL for Gyroscope
#define SDA A4 // SDA for Gyroscope


/*
void setup() {
    pinMode(CH1, INPUT);
    pinMode(CH2, INPUT);
    pinMode(CH3, INPUT);
    pinMode(CH4, INPUT);
    pinMode(CH5, INPUT);
}
void loop() {
    
}
*/

const int MPU = 0x68;
int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;
int16_t AcXinit, AcYinit, AcZinit, GyXinit, GyYinit, GyZinit;
unsigned long pulseWidth;
double pitch,roll;
const int buttonPin=2;
const int TriggerPin =4;
const int MonitorPin = 5;
int buttonState =0;

void setup() 
{
 
    Wire.begin(); // 
    Wire.beginTransmission(MPU);  // Begin I2C transmission to the MPU (slave) with the given address (0x68)
    Wire.write(0x6B);             // Queue bytes for transmisssion
    Wire.write(0);                // Queue bytes for transmission
    Wire.endTransmission(true);   // Transmit bytes that were queued by Wire.write(num)
    Serial.begin(9600);           // sets data rate for bps (baud) for serial data transmission
    pinMode(buttonPin, INPUT);    // used to get the input of the button
    pinMode(TriggerPin, OUTPUT);
    pinMode(MonitorPin, INPUT);
    digitalWrite(TriggerPin, LOW);
}

void loop()
{
    Wire.beginTransmission(MPU);
    Wire.write(0x3B);
    Wire.endTransmission(false);
    Wire.requestFrom(MPU,14,true);

    int AcXoff,AcYoff,AcZoff,GyXoff,GyYoff,GyZoff;
    pulseWidth= pulseIn(MonitorPin,HIGH);
   
//    if(pulseWidth!= 0)
//    {
//      pulseWidth = pulseWidth/10;
//      Serial.print("The distance in cm is: ");
//      Serial.print(pulseWidth);
//      Serial.print("\n");
//    }

    //Acceleration data correction. NEED TO GET ADJUSTED FOR NEW MPU
    AcXoff = 0;
    AcYoff = 0;
    AcZoff = 0;

    //Gyro correction. NEED TO GET ADJUSTED FOR NEW MPU
    GyXoff = 0;
    GyYoff = 0;
    GyZoff = 0;

// Arduino Nano frequency = 490 Hz 
// Arduino Nano period = 0.00204081632 seconds (or 2.04081632653 ms)
// If we want to read data every 5 seconds, then counter value (0.005 ms)
// should be 2.04081632653/0.005 = 40.8163265306 = 41 

   buttonState = digitalRead(buttonPin);
if( buttonState==HIGH)
{
    
    //delay(1000); // delay by 5s
     // pulseWidth = pulseIn(MonitorPin, HIGH);
      Serial.print("You have pushed the button, Here are your saved values: \n");
    //read accel data
      AcXinit=(Wire.read()<<8|Wire.read()) + AcXoff;
      AcYinit=(Wire.read()<<8|Wire.read()) + AcYoff;
      AcZinit=(Wire.read()<<8|Wire.read()) + AcZoff;
    
    
    //read gyro data
      GyXinit=(Wire.read()<<8|Wire.read()) + GyXoff;
      GyYinit=(Wire.read()<<8|Wire.read()) + GyYoff;
      GyZinit=(Wire.read()<<8|Wire.read()) + GyZoff;

    
    //get pitch/roll
      getAngle(AcXinit,AcYinit,AcZinit);
    
    //send the data out the serial port
      print_data(AcXinit,AcYinit,AcZinit,GyXinit,GyYinit,GyZinit,pulseWidth);
  }
else 
  {
    getdata(AcXoff,AcYoff,AcZoff, GyXoff,GyYoff,GyZoff, pulseWidth);
  }

}

void getdata(int AcXoff, int AcYoff, int AcZoff, int GyXoff, int GyYoff, int GyZoff, unsigned long pulseWidth)
  {
   //delay(1000); // delay by 5s
    
    //read accel data
    AcX=(Wire.read()<<8|Wire.read()) + AcXoff;      // Get the X axis Accelerometer data
    AcY=(Wire.read()<<8|Wire.read()) + AcYoff;      // Get the Y axis Accelerometer data
    AcZ=(Wire.read()<<8|Wire.read()) + AcZoff;      // Get the Z axis Accelerometer data
    
    //read gyro data
    GyX=(Wire.read()<<8|Wire.read()) + GyXoff;      // Get the X axis Gyroscope data
    GyY=(Wire.read()<<8|Wire.read()) + GyYoff;      // Get the Y axis Gyroscope data
    GyZ=(Wire.read()<<8|Wire.read()) + GyZoff;      // Get the Z axis Gyroscope data
    
    //get pitch/roll
    getAngle(AcX,AcY,AcZ);                          // This function is used to get pitch and roll
    
    //send the data out the serial port
    print_data(AcX,AcY,AcZ,GyX,GyY,GyZ, pulseWidth);
  }

void getAngle(int Vx,int Vy,int Vz) {
    double x = Vx;
    double y = Vy;
    double z = Vz;
    pitch = atan(x/sqrt((y*y) + (z*z)));
    roll = atan(y/sqrt((x*x) + (z*z)));
    //convert radians into degrees
    pitch = pitch * (180.0/3.14);
    roll = roll * (180.0/3.14) ;
}


void print_data(int16_t Ax, int16_t Ay, int16_t Az, int16_t Gx, int16_t Gy, int16_t Gz, unsigned long pulseWidth)
  {
   if(pulseWidth!= 0)
    {
      pulseWidth = pulseWidth/10;
      Serial.print("The distance in cm is: ");
      Serial.print(pulseWidth);
      Serial.print("\n");
    }

    //send the data out the serial port
    Serial.print("Angle: ");
    Serial.print("Pitch = "); Serial.print(pitch);
    Serial.print(" | Roll = "); Serial.println(roll);
     
    Serial.print("Accelerometer: ");
    Serial.print("X = "); Serial.print(Ax);
    Serial.print(" | Y = "); Serial.print(Ay);
    Serial.print(" | Z = "); Serial.println(Az);
    
    Serial.print("Gyroscope: ");
    Serial.print("X = "); Serial.print(Gx);
    Serial.print(" | Y = "); Serial.print(Gy);
    Serial.print(" | Z = "); Serial.println(Gz);
    Serial.println(" ");
  }
