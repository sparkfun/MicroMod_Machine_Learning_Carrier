/*
  This sketch just verifies that the camera is responding at a basic level.

  We must provide power the camera (enable the 1.8V regulator)
  We must provide a clock source via MCLK so the camera can run

  We check for an ack at 0x24 then check that the device IDs match. If
  everything is ok, attempt to start the camera interface.
*/

#include <Wire.h>

void setup()
{
  Serial.begin(115200);
  Serial.println("I2C Scanner");

  Wire.begin();

  pinMode(PWM1, OUTPUT);

  #define MCLK_UI64PATTERN 0x55555555
  #define MCLK_UI64PATTERNLEN 31

//  int timerNumber = 1;
//  int timerPin = 18;
//  // Set up timer.
//  am_hal_ctimer_clear(timerNumber, AM_HAL_CTIMER_TIMERA);
//  am_hal_ctimer_config_single(timerNumber, AM_HAL_CTIMER_TIMERA, (AM_HAL_CTIMER_FN_PTN_REPEAT | AM_HAL_CTIMER_HFRC_12MHZ));
//
//  // Set the pattern in the CMPR registers.
//  am_hal_ctimer_compare_set(timerNumber, AM_HAL_CTIMER_TIMERA, 0, (uint32_t)(MCLK_UI64PATTERN & 0xFFFF));
//  am_hal_ctimer_compare_set(timerNumber, AM_HAL_CTIMER_TIMERA, 1, (uint32_t)((MCLK_UI64PATTERN >> 16) & 0xFFFF));
//
//  // Set the timer trigger and pattern length.
//  am_hal_ctimer_config_trigger(timerNumber, AM_HAL_CTIMER_TIMERA, ((MCLK_UI64PATTERNLEN << CTIMER_AUX0_TMRA0LMT_Pos) | (CTIMER_AUX0_TMRB0TRIG_DIS << CTIMER_AUX0_TMRA0TRIG_Pos)));
//
//  // Configure timer output pin.
//  am_hal_ctimer_output_config(timerNumber, AM_HAL_CTIMER_TIMERA, timerPin, AM_HAL_CTIMER_OUTPUT_NORMAL, AM_HAL_GPIO_PIN_DRIVESTRENGTH_12MA);
//
//  // Start the timer.
//  am_hal_ctimer_start(timerNumber, AM_HAL_CTIMER_TIMERA);  

  analogWriteFrequency(4000000); //Max square wave freq
  analogWrite(18, 255 / 2);

  //  analogWriteResolution(16);
  //  analogWrite(5, pow(2,15));

  //Reset camera via power cycle
  digitalWrite(PWM1, LOW); //Turn camera off
  delay(250);  
  digitalWrite(PWM1, HIGH); //Turn camera on
  delay(250);  

  Serial.println();
  for (byte address = 1; address < 127; address++ )
  {
    Wire.beginTransmission(address);
    if (Wire.endTransmission() == 0)
    {
      Serial.print("Device found at address 0x");
      if (address < 0x10)
        Serial.print("0");
      Serial.println(address, HEX);
    }
  }
  Serial.println("Done");
  delay(100);
}

void loop()
{

}
