/*Main code for Kevin Group Project

   Edit History

   Name           Date          Operation
   Adam Duke      11.04.2018    Initial Creation

   Adam Duke      12.04.2018    Modidfy to place trial_pixyMon and mBlock move code into program

   Adam Duke      17.04.2018    Swap motor code for DC motors
                                Start building algorithm

   Adam Duke      24.04.2018    Troubleshoot loop structure and methods not being recognised in main loop
*/

#include <Arduino.h>
#include "MeMegaPi.h"
#include <Wire.h>
#include <SoftwareSerial.h>
#include <SPI.h>
#include <Pixy.h>

Pixy pixy;  //create pixy global variable

MeMegaPiDCMotor motor1(PORT1B); //Left Motor
MeMegaPiDCMotor motor2(PORT2B); //Right Motor
MeMegaPiDCMotor motor3(PORT4B); //Grabber
uint8_t motorSpeed = 100;
boolean found = false;

void setup()
{
  Serial.begin(9600);
  Serial.println("Starting...");
  openGrabber();
  pixy.init(); //initialise pixymon
  //closeGrabber();

}

void loop()
{
  if (found == false)
  {
    FindBall();
  }
  else
  {
    goToGoal();
  }

//moveFor();
  delay(1000);
  Serial.print("X coordinate: ");
  Serial.print(xCoor());
  Serial.print("      Signal code: ");
  Serial.println(Sig());

}

void FindBall()
{
  if ( Sig() == 1)
  {
    if ( xCoor() > 400)
    {
      Serial.print("ERROR:   ");
      Serial.println(xCoor());
    }
    if ( xCoor() > 165 && xCoor() < 175 )
    {
      Serial.println("Centralised");
      Serial.println("Move Forward_1");
      moveFor();
      //delay(500);
      if ( width() > 70)
      {
        Serial.println("Close Grabber");
        Stop();
        closeGrabber();
        found = true;
      }

    }
    else if (xCoor() < 165)
    {
      moveRight();
      Serial.println("Move Right");
      //delay(500);
    }
    else if (xCoor() > 175 && xCoor() < 400)
    {
      moveLeft();
      Serial.println("Move Left");
      // delay(500);
    }
  }
  else
  {
    Serial.println("Rotate Right");
    moveRight();
    //delay(500);
  }
}

void goToGoal()
{
  Serial.println("Go To Goal Code");
  if ( Sig() == 4) //see black goal
  {
    if ( xCoor() > 165 && xCoor() < 175 )
    {
      Serial.println("Centralised");
      Serial.println("Move Forward_1");
      moveFor();
      if ( width() > 180)
      {
        Stop();
      }
      Serial.println("Open Grabber");
      openGrabber();
      Serial.println("Reverse and turn");
      moveRev();
      delay(500);
      Stop();
      moveLeft();
      delay(500);
      Stop();
      Serial.println("Go to Find Ball code");
      FindBall();
      found = false;
    }
  }
  else if (xCoor() < 165)
  {
    moveRight();
    Serial.println("Move Right");
    //delay(500);
  }
  else if (xCoor() > 175 && xCoor() < 400)
  {
    moveLeft();
    Serial.println("Move Left");
  }

  else
  {
    Serial.println("Rotate Right");
    moveRight();
  }
}


//////////////////////////////////////////////////////////////////////////
//PixyMon setup
int xCoor()
{
  //delay(50);
  uint16_t blocks = pixy.getBlocks();
  if (blocks)
  {
    return pixy.blocks[0].x;
  }
}

int Sig()
{
  //delay(50);
  uint16_t blocks = pixy.getBlocks();
  if (blocks)
  {
    return pixy.blocks[0].signature;
  }
}

int width()
{
  uint16_t blocks = pixy.getBlocks();
  if (blocks)
  {
    return pixy.blocks[0].width;
  }
}

////////////////////////////////////////////////////////////////////////////
//Motor setup

void moveFor()
{
  motor1.run(motorSpeed);
  motor2.run(-motorSpeed);
  delay(1000);
  motor1.stop();
  motor2.stop();
}

void moveRev()
{
  motor1.run(-motorSpeed);
  motor2.run(motorSpeed);
  delay(1000);
  motor1.stop();
  motor2.stop();
}

void moveRight()
{
  motor1.run(motorSpeed);
  motor2.run(motorSpeed);
  delay(500);
  motor1.stop();
  motor2.stop();
}

void moveLeft()
{
  motor1.run(-motorSpeed);
  motor2.run(-motorSpeed);
  delay(500);
  motor1.stop();
  motor2.stop();
}

void Stop()
{
  motor1.stop();
  motor2.stop();
}
void openGrabber()
{
  motor3.run(-motorSpeed);
  delay(2000);
  motor3.stop();
}

void closeGrabber()
{
  motor3.run(motorSpeed);
  delay(2000);
  motor3.stop();
}

