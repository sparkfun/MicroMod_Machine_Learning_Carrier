/*
  
*/

const am_hal_gpio_pincfg_t g_AM_BSP_GPIO_CLKOUT =
    {
        //.uFuncSel = AM_HAL_PIN_0_CLKOUT,
        .uFuncSel = AM_HAL_PIN_7_CLKOUT,
        .eDriveStrength = AM_HAL_GPIO_PIN_DRIVESTRENGTH_12MA,
        .eGPOutcfg = AM_HAL_GPIO_PIN_OUTCFG_PUSHPULL,
        .eGPInput = AM_HAL_GPIO_PIN_INPUT_NONE,
        //.eIntDir = AM_HAL_GPIO_PIN_INTDIR_LO2HI,
        //.uIOMnum = 6,
        //.uNCE = 1,
        //.eCEpol = AM_HAL_GPIO_PIN_CEPOL_ACTIVELOW
};

void setup()
{
  Serial.begin(115200);
  Serial.println();
  Serial.println("Clockout CLKOUT example");

    int retval = 0;

    //
    // Enable the LFRC.
    //
    //am_hal_clkgen_osc_start(AM_HAL_CLKGEN_OSC_LFRC);
    //retval = am_hal_clkgen_control(AM_HAL_CLKGEN_CONTROL_LFRC_START, 0);
    retval = am_hal_clkgen_control(AM_HAL_CLKGEN_CONTROL_XTAL_START, 0);
    Serial.printf("retval1: %d\n", retval);

    // Enable the clockout to the desired pin.
    // And make it readable on the pin with AM_HAL_GPIO_INPEN
    //am_hal_gpio_pin_config(0, (AM_BSP_GPIO_CFG_CLKOUT_PIN | AM_HAL_GPIO_INPEN));
    pinMode(AM_BSP_GPIO_CLKOUT, OUTPUT);
    retval = am_hal_gpio_pinconfig(AM_BSP_GPIO_CLKOUT, g_AM_BSP_GPIO_CLKOUT);
    Serial.printf("retval2: %d\n", retval);

    // Initialize clkgen to output the selected clock.
    //retval = am_hal_clkgen_clkout_enable(true, AM_HAL_CLKGEN_CLKOUT_LFRC_32);
    retval = am_hal_clkgen_clkout_enable(true, AM_HAL_CLKGEN_CLKOUT_XTAL_32768);
    Serial.printf("retval3: %d\n", retval);
}

void loop() {
  delay(1000);
}
