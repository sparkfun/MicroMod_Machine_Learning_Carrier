/*
  This sketch just verifies that the camera is responding at a basic level.

  We must provide power the camera (enable the 1.8V regulator)
  We must provide a clock source via MCLK so the camera can run

  We check for an ack at 0x24 then check that the device IDs match. If
  everything is ok, attempt to start the camera interface.
*/

//#include <Wire.h>

#define cameraI2Caddress 0x24

#include "hm01b0_arduino.h" //Click here to get the library: http://librarymanager/All#SparkFun_HM01B0
HM01B0 myCamera;

void setup() {
  Serial.begin(115200);
  while (!Serial);
  Serial.println();
  Serial.println("Camera example");

//  Wire.begin();

  pinMode(PWM1, OUTPUT);

//  analogWriteFrequency(4000000); //Max square wave freq
//  analogWrite(18, 255 / 2); //Send 4MHz square wave to pin 18 / Camera MCLK

  //Reset camera via power cycle
  digitalWrite(PWM1, LOW); //Turn camera off
  delay(25);
  digitalWrite(PWM1, HIGH); //Turn camera on
  delay(25);


  // Start the camera
  if (myCamera.begin() != HM01B0_ERR_OK) {
    Serial.print("Camera.begin() failed with code: " + String(myCamera.status) + "\n");
  } else {
    Serial.print("Camera started successfully\n");
  }

//  Wire.beginTransmission(cameraI2Caddress);
//  if (Wire.endTransmission() != 0)
//  {
//    Serial.println("Camera not detected at 0x24. Please re-insert camera into connector facing down. Freezing...");
//    while(1);
//  }
//
//  if (getID() == 0x1B0)
//    Serial.println("Camera ID ok!");
//  else
//    Serial.println("Camera failed ID check");

}

void loop() {

}

//uint16_t getID()
//{
//  uint16_t pui16MID = 0x0000;
//  uint8_t ui8Data[1] = {0};
//
//  read(cameraI2Caddress, HM01B0_REG_MODEL_ID_H, ui8Data, sizeof(ui8Data));
//  pui16MID |= (ui8Data[0] << 8);
//
//  read(cameraI2Caddress, HM01B0_REG_MODEL_ID_L, ui8Data, sizeof(ui8Data));
//  pui16MID |= ui8Data[0];
//
//  return pui16MID;
//}
//
////Read a number of bytes into a provided container
//void read(uint8_t i2cAddress, uint16_t adr, uint8_t *buff, uint16_t bufferSize)
//{
//  Wire.beginTransmission(i2cAddress);
//  Wire.write((uint8_t)(adr >> 8));   // MSB
//  Wire.write((uint8_t)(adr & 0xFF)); // LSB
//  Wire.endTransmission();
//
//  Wire.requestFrom((uint8_t)i2cAddress, (uint8_t)bufferSize);
//
//  for (uint16_t x = 0; x < bufferSize; x++)
//    buff[x] = Wire.read();
//}
