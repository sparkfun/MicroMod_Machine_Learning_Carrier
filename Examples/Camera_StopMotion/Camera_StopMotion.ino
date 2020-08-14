/*
  Saving camera stills over UART on SparkFun Edge board
  By: Owen Lyke
  SparkFun Electronics
  Date: November 18th 2019
  This example code is in the public domain

  SparkFun labored with love to create this code. Feel like supporting open source hardware?
  Buy a board from SparkFun! https://www.sparkfun.com/products/15170

  This example shows how to use the HM01B0 camera. It will stream frames captured by the
  camera over the UART.

  To see images:
  1. upload this sketch (using baud 460800 is recommended)
  2. navigate to the library folder (~/.../Arduino/libraries/SparkFun_Himax_HM01B0_Camera)
  3. run the command 'python3 utils/Example1_StopMotion.py -b 500000 -p COM_PORT (COM_PORT is the same as the port used for upload in Arduino)
  4. say "cheese"
*/

#include "hm01b0_arduino.h"
#include "String.h"

///////////////////
// Begin User Setup

#define BAUD_RATE   460800
#define BAUD_RATE   500000

//#define DEMO_HM01B0_TEST_MODE_ENABLE          // Uncomment to enable test pattern generation
//#define DEMO_HM01B0_FRAMEBUFFER_DUMP_ENABLE   // Uncomment to enable frame output
#define DEMO_HM01B0_PYTHON                    // Uncomment to use Python script to visualize data
// (go to the sketch directory and use 'python3 utils/Example1_StopMotion.py -p {COM_PORT}'

// End User Setup
/////////////////

HM01B0 myCamera;            // Declare an HM01B0 object called 'myCamera'
// The camera will try to specialize for the host architecture
// however it will fall back to a slow generic interface if no
// specialization is available.
// The default is not guaranteed to work due to the high amount
// of data the camera needs to transfer


// Auto-configure for python if requested
#ifdef DEMO_HM01B0_PYTHON
#define DEMO_HM01B0_FRAMEBUFFER_DUMP_ENABLE
#undef  DEMO_HM01B0_TEST_MODE_ENABLE
#endif // DEMO_HM01B0_PYTHON

// Forward declarations
void printWord(uint32_t num);
void printByte(uint8_t num);

void setup() {

  // Start up serial monitor
  Serial.begin(BAUD_RATE);
  do {
    delay(500);
  } while (!Serial);
  Serial.println();
  Serial.println("Camera stop motion");

  //  // Turn on camera regulator if using Edge board
  //#if defined (ARDUINO_SFE_EDGE)
  //  pinMode(AM_BSP_GPIO_CAMERA_HM01B0_DVDDEN, OUTPUT);
  //  digitalWrite(AM_BSP_GPIO_CAMERA_HM01B0_DVDDEN, HIGH);
  //  Serial.println("Turned on Edge camera regulator");
  //#endif
  pinMode(PWM1, OUTPUT);
  digitalWrite(PWM1, HIGH); //Turn camera on

  //pinMode(D1, OUTPUT);

  // Start the camera
  if (myCamera.begin() != HM01B0_ERR_OK) {
    Serial.print("Camera.begin() failed with code: " + String(myCamera.status) + ". Consider re-seating the camera.\n");
  } else {
    Serial.print("Camera started successfully\n");
  }

  // Calibrate Autoexposure
//  Serial.println("Calibrating Auto Exposure...");
//  myCamera.calibrateAutoExposure();
//  if (myCamera.status != HM01B0_ERR_OK) {
//    Serial.println("\tnot converged");
//  } else {
//    Serial.println("\tconverged!");
//  }

#ifdef DEMO_HM01B0_TEST_MODE_ENABLE
  // Enable test mode (generates a 'walking 1s' pattern to verify interface function
  Serial.print("Enabling test mode...\n");
  myCamera.enableTestMode();
  if (myCamera.status != HM01B0_ERR_OK) {
    Serial.print("\tfailed\n");
  } else {
    Serial.print("\tsucceeded!\n");
  }

  // In test mode capturing a frame fills the buffer with the test pattern
  myCamera.capture();

  uint32_t mismatches = myCamera.countTestMismatches();
  Serial.print("Self-test mismatches: 0x");
  printWord(mismatches);
  Serial.print("\n");
#endif

  Serial.write(0x55);                                                    // Special character to sync Python script
  Serial.print("\n\n");                                                  // Newlines allow Python script to find frame start
}

void loop() {
  // Take an image
  myCamera.capture();

#ifdef DEMO_HM01B0_FRAMEBUFFER_DUMP_ENABLE
  // Print out a frame for the Python script to pick up
  framebuffer_dump();
#else
  // Print auto exposure state
  Serial.print("AE convergance(0x");
  printByte(myCamera.aeConvergenceStatus);
  Serial.print(") TargetMean 0x");
  printByte(myCamera.aecfg.ui8AETargetMean);
  Serial.print(", ConvergeInTh 0x");
  printByte(myCamera.aecfg.ui8ConvergeInTh);
  Serial.print(", AEMean 0x");
  printByte(myCamera.aecfg.ui8AEMean);
  Serial.print("\n");
#endif

  // Wait a second
  //delay(1000);
}

//
// Utility functions

// hex formating
// Thanks to bootsector on the Arduino forums:
// https://forum.arduino.cc/index.php?topic=38107.msg282336#msg282336
void printWord(uint32_t num) {
  char tmp[9];                  // 8 hex digits + null terminator
  sprintf(tmp, "%08X", num);
  Serial.print(tmp);
}

void printByte(uint8_t num) {
  char tmp[3];                  // 2 hex digits + null terminator
  sprintf(tmp, "%02X", num);
  Serial.print(tmp);
}

// frame buffer dump (formatted for python script)
void framebuffer_dump( void ) {
  Serial.print("+++ frame +++");                                         // Mark frame start
  for (uint32_t ui32Idx = 0; ui32Idx < myCamera.frameBufferSize; ui32Idx++) { // Process all bytes in frame
    if ((ui32Idx & 0xF) == 0x00) {                                            // Print address every 16 bytes
      Serial.print("\n0x");
      printWord(ui32Idx);
      Serial.print(" ");
    }
    printByte(myCamera.frameBuffer[ui32Idx]);                                 // Print byte value
    Serial.print(" ");
  }
  Serial.print("\n--- frame ---\n");
  memset(myCamera.frameBuffer, 0x00, sizeof(myCamera.frameBufferSize));       // Zero out frame buffer for help identifying errors
}
