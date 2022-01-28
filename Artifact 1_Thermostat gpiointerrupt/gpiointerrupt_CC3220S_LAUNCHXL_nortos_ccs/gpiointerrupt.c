/* gpiointerrupt.c : This file contains the 'main' function. Program execution begins and ends there.
 * author : Keone Garcia
 * course : CS499 Capstone - Artifact_1 Thermostat Lab
 * date : 01/21/2022
 *
 *
 * -- Project Requirements --
 * Artifact One is the Thermostat Lab
 * The artifact originates from course CS350 Emerging Systems and uses Code Composer Studio
 * with a Simple Link CC3220S Launchpad in C language.
 * The goal of this artifact is to utilize components of the Launchpad to simulate a working thermostat
 * for an HVAC system. Indicating when the unit is active using an LED, while providing an output of
 * temperature sensor readings based on setpoint parameters sent to a wireless thermostat and records measures to a database.
 * Illustrated using the CC3220S Launchpad drivers, components and functions
 *
 *
 * -- Required Changes --
 *
 * Update Pseudocode with changes to functionality.
 * Change LED Output suited for both the Heat and Cool function - The red LED is the only option available and is set to blink.
 * Update Loops, branches, nesting, and use proper logic.
 * Update Cases, use defaults.
 * Ensure use of formating, date, logic, spacing, tabs, line breaks, curly braces, and brackets.
 * Consider whitespace, remove unnecessary spacing.
 * Ensure naming conventions of fields/constants/variables/parameters/classes/libraries clear and meaningful.
 * Remove redundant variables - CONFIG_GPIO_LED_1, no longer in use as I2C driver uses the same pins for CONFIG_GPIO_LED_1.
 * Remove excess comments - Over commenting, while useful can lead to distractions or large files.
 * Add commenting to functionality - Provide necessary comments to specific functions required .
 * Add parameters for cooling system displayed from terminal - In UART, add parameters to invoke cooling when temp is greater than setpoint.
 * Add required drivers - Add the Timer Driver for tick counts, Add the I2C Driver for temperature sensor measures.
 * Edit existing drivers - Update changes to existing drivers based on parameters set.
 * Remove non-functional drivers - Remove non-functional drivers, as some share the same pins and cause the project to fail.
 * Ensure functions have the variables that support them declared near by - Relocate declarations above where the function is called.
 * Ensure Project Requirements are fulfilled and improvments support quality, efficiency, and security.
 * Create ReadMe file and include project outcome and steps.
 *
 *
 * Copyright (c) 2015-2020, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */


/*
 *  ======== gpiointerrupt.c ========
 */
#include <stdint.h>
#include <stddef.h>
#include <ti/drivers/GPIO.h>    // Driver GPIO for LED display
#include "ti_drivers_config.h"  // Driver configuration for communication between CC3220S-Launchpad
#include <ti/drivers/Timer.h>   // Driver Timer for Tick counts
#include <ti/drivers/I2C.h>     // Driver I2C for Temperature sensor
#include <ti/drivers/UART.h>    // Driver UART for Terminal and COMPort access and display (Com4)


/*
========== UART Driver ==========
 */

/* UART write output PARAms */
#define DISPLAY(x) UART_write(uart, &output, x);

/* UART Global Variables */
char output[64];
int bytesToSend;

/* Driver Handles - Global variables */
UART_Handle uart;

void initUART(void)
{
    UART_Params uartParams;
    UART_init();

    // Configure driver
    UART_Params_init(&uartParams);
    uartParams.writeDataMode = UART_DATA_BINARY;
    uartParams.readDataMode = UART_DATA_BINARY;
    uartParams.readReturnMode = UART_RETURN_FULL;
    uartParams.baudRate = 115200;

    // Open driver
    uart = UART_open(CONFIG_UART_0, &uartParams);
    if (uart == NULL) {
        /* UART_open() failed */
        while (1);
    }
}


/*
========== I2C Driver ==========
 */

/* I2C Global Variables */
static const struct {
    uint8_t address;
    uint8_t resultReg;
char *id;
} sensors[3] = {
    { 0x48, 0x0000, "11X" },
    { 0x49, 0x0000, "116" },
    { 0x41, 0x0001, "006" }
};

uint8_t txBuffer[1];
uint8_t rxBuffer[2];
I2C_Transaction i2cTransaction;

/* Driver Handles - Global variables */
I2C_Handle i2c;

unsigned char i = 0;    // set i to 0, default.

void initI2C(void)      // Ensure initUART() is called before initI2C, project will not compile otherwise.
{
    int8_t i, found;
    I2C_Params i2cParams;
    DISPLAY(snprintf(output, 64, "Initializing I2C Driver - "))
    I2C_init(); // Init I2C driver

    /* Configure driver */
    I2C_Params_init(&i2cParams);
    i2cParams.bitRate = I2C_400kHz;

    /* Open driver */
    i2c = I2C_open(CONFIG_I2C_0, &i2cParams);
    if (i2c == NULL)
    {
        DISPLAY(snprintf(output, 64, "Failed\n\r"))
        while (1);
    }
    DISPLAY(snprintf(output, 32, "Passed\n\r"))

    /* Common I2C transaction setup */
    i2cTransaction.writeBuf = txBuffer;
    i2cTransaction.writeCount = 1;
    i2cTransaction.readBuf = rxBuffer;
    i2cTransaction.readCount = 0;

    /* Test board specifications and print out results */
    // Boards were shipped with different sensors.
    // Welcome to the world of embedded systems.
    found = false;
    for (i = 0; i < 3; ++i)
    {
        i2cTransaction.slaveAddress = sensors[i].address;
        txBuffer[0] = sensors[i].resultReg;

    DISPLAY(snprintf(output, 64, "Is this %s? ", sensors[i].id))
    if (I2C_transfer(i2c, &i2cTransaction))
    {
        DISPLAY(snprintf(output, 64, "Found\n\r"))
        found = true;
        break;
        }
        DISPLAY(snprintf(output, 64, "No\n\r"))
    }

    /* Scan through the possible sensor addresses and determine which sensor is on the Launchpad. */
    if(found)
    {
        DISPLAY(snprintf(output, 64, "Detected TMP%s I2C address: %x\n\r", sensors[i].id, i2cTransaction.slaveAddress))
    } else {
        DISPLAY(snprintf(output, 64, "Temperature sensor not found, contact professor\n\r"))
    }
}

int16_t readTemp(void)
{
    int j;
    int16_t temperature = 0;

    i2cTransaction.readCount = 2;
    if (I2C_transfer(i2c, &i2cTransaction))
    {
        /*
         * Extract degrees C from the received data;
         * see TMP sensor datasheet
         */
        temperature = (rxBuffer[0] << 8) | (rxBuffer[1]);
        temperature *= 0.0078125;
            /*
            * If the MSB is set '1', then we have a 2's complement
            * negative value which needs to be sign extended
            */
            if (rxBuffer[0] & 0x80)
            {
                temperature |= 0xF000;
            }
    } else {
        DISPLAY(snprintf(output, 64, "Error reading temperature sensor (%d)\n\r", i2cTransaction.status))
        DISPLAY(snprintf(output, 64, "Please power cycle your board by unplugging USB and plugging back in.\n\r"))
    }
    return temperature;
}


/*
========== Timer Driver ==========
 */

/* set TimerFlag, timer variable to 0 */
volatile unsigned char TimerFlag = 0;

/* Driver Handles - Global variables */
Timer_Handle timer0;

void timerCallback(Timer_Handle myHandle, int_fast16_t status)
{
/* Raise timer flag */
    TimerFlag = 1;
}

/*
 * Initialize Timer and set Params
 */

void initTimer(void)
{
    //Timer_Handle timer0;
    Timer_Params params;

    /* Init Timer driver */
    Timer_init();

    /* Configure driver */
    Timer_Params_init(&params);
    params.period = 100000;     // 100ms = 100000us used for temperature, setpoint, heat, cool, seconds - default.
    params.periodUnits = Timer_PERIOD_US;
    params.timerMode = Timer_CONTINUOUS_CALLBACK;
    params.timerCallback = timerCallback;

    // Open driver
    timer0 = Timer_open(CONFIG_TIMER_0, &params);

    if (timer0 == NULL) {
        /* Failed to initialized timer */
        while (1) {}
    }
    if (Timer_start(timer0) == Timer_STATUS_ERROR) {
        /* Failed to start timer */
        while (1) {}
    }
}


/*
======== gpioButtonFxn0 ========
 *  Callback function for the GPIO interrupt on CONFIG_GPIO_BUTTON_0.
 *  Note: GPIO interrupts are cleared prior to invoking callbacks.
 */

/* set button timer flag for gpioButtonFxn0 and gpioButtonFxn1 */
unsigned char Button0_Flag = 0;     // Button 0 decrease setpoint temp
unsigned char Button1_Flag = 0;     // Button 1 increase setpoint temp

void gpioButtonFxn0(uint_least8_t index)
{
/* Toggle a temperature setpoint for LED params */
    Button0_Flag = 1;
}

/*
======== gpioButtonFxn1 ========
 *  Callback function for the GPIO interrupt on CONFIG_GPIO_BUTTON_1.
 *  Note: GPIO interrupts are cleared prior to invoking callbacks.
 */

void gpioButtonFxn1(uint_least8_t index)
{
/* Toggle a temperature setpoint for LED params */
    Button1_Flag = 1;
}


/*
======== Declare Temperature variables  ========
 */

int16_t setpoint; // Desired temp set by buttons to increase or decrease setpoint value (UART simulation uses terminal on Com4 port)
int16_t temperature; // Sensor temp located on Launchpad near WIFI logo, small square chip TMP006 (via I2C)
int8_t heat; // Indicates 0 or 1, 0 = Heater off, 1 = Heater on (via GPIO and interrupt)
int8_t cool; // Indicates 0 or 1, 0 = Cool off, 1 = Cool on (via GPIO and interrupt)


/*
========== State Machine's States ==========
 */

/* Increase or decrease setpoint from gpioButtonFxn0 or gpioButtonFxn1 buttons */
enum BF_STATES {BF_SMStart, BF_SMWaitRise, BF_SMWaitFall} BF_STATE;

/*
 * BF_SMStart - Start
 *  BF_SMWaitRise - Button Flag Wait Rise
 *  BF_SMWaitFall - Button Flag Wait Fall
 *  Button Flag callback if gpioButtonFxn0 or gpioButtonFxn1 are selected as 0 or 1
 */

void TickFct_ButtonFlag() {
    switch(BF_STATE) { // Transitions
        case BF_SMStart:
            /* Initialize buttons to 0 */
            Button0_Flag = 0; // Initialize outputs
            Button1_Flag = 0; // Initialize outputs
            BF_STATE = BF_SMWaitRise;
            break;
        case BF_SMWaitRise:
            // Wait for button flag 0(Right) or 1(Left) to be selected
            if (Button0_Flag || Button1_Flag) {
                BF_STATE = BF_SMWaitFall;
            }
            break;
        case BF_SMWaitFall:
            // If no button flag is raised go to BF_SMWaitRiseLow
            if (!(Button0_Flag || Button1_Flag)){
                BF_STATE = BF_SMWaitRise;
            }
            break;
        default:
            BF_STATE = BF_SMStart; // Indicates initial call
            break;
    }

    switch(BF_STATE) { // State actions
        case BF_SMStart:
            break;
        case BF_SMWaitRise:
            break;
        case BF_SMWaitFall:
            // Update setpoint temperature value
            if (Button0_Flag) {
                setpoint -= 1;       // Decrease setpoint by 1° C // <30,25,1,0339> <temperature, setpoint, heat, seconds>
                Button0_Flag = 0;    // Update Button0_Flag
            }
            if (Button1_Flag) {
                setpoint += 1;       // Increase setpoint by 1° C // <30,32,0,0339> <temperature, setpoint, heat, seconds>
                Button1_Flag = 0;    // Update Button1_Flag
            }
            break;
        default:
            break;
    }
}


/*
========== State Machine's States Continued ==========
 */

/* Temperature heat or cool indicator activates LED red ON or OFF indicating heater is on or off */
enum TL_STATES {TL_SMStart, TL_SMOff, TL_SMOn} TL_STATE;

/*
 *  TL_SMStart - Start
 *  TL_SMOff - Temperature Light Off (Heater or Cooling Off)
 *  TL_SMOn - Temperature Light On (Heater or Cooling On)
 */

void TickFct_TmpLED() {
    switch(TL_STATE) { // Transitions
        case TL_SMStart:
            heat = 0;
            cool = 0;
            TL_STATE = TL_SMOff; // default Temperature Light is off
            break;
        case TL_SMOff:
            /* If temperature is less than setpoint proceed to STATE TL_On */
            if (temperature < setpoint) {
                TL_STATE = TL_SMOn;
                GPIO_write(CONFIG_GPIO_LED_0, CONFIG_GPIO_LED_ON);
                heat = 1; // Temperature sensor heat is on (Heater activates) LED red on
            }
            /* If temperature is greater than setpoint proceed to STATE TL_On */
            if (temperature > setpoint) {
                TL_STATE = TL_SMOn;
                GPIO_write(CONFIG_GPIO_LED_0, CONFIG_GPIO_LED_ON);
                cool = 1; // Temperature sensor cooling is off (A/C is turned off) LED red on
            }
            break;
        case TL_SMOn:
            /* If temperature is greater than setpoint proceed to STATE TL_Off */
            if (!(temperature < setpoint)){
                TL_STATE = TL_SMOff;
                GPIO_write(CONFIG_GPIO_LED_0, CONFIG_GPIO_LED_OFF);
                heat = 0; // Temperature sensor heat is off (Heater is turned off) LED red off
            }
            /* If temperature is greater than setpoint proceed to STATE TL_Off */
            if (!(temperature > setpoint)){
                TL_STATE = TL_SMOff;
                GPIO_write(CONFIG_GPIO_LED_0, CONFIG_GPIO_LED_OFF);
                cool = 0; // Temperature sensor cooling is on (A/C activates) LED red off
            }
            break;
        default:
            TL_STATE = TL_SMStart; // Initialize outputs
            break;
    }

    switch(TL_STATE) { // State actions
        case TL_SMStart: // Start
            break;
        case TL_SMOff: // Temperature Light Off
            break;
        case TL_SMOn: // Temperature Light On
            break;
        default:    // default
            break;
    }
}


/*
======== mainThread ========
 */

/* Change image.sys from development mode to production mode */
void *mainThread(void *arg0)
{
    /* Call driver init functions */
    initUART();     // Read and write for output display
    initI2C();      // Temp sensor output and display
    initTimer();    // Timer elapsed and callback
    GPIO_init();    // LED output

    /* Set timer period to 100000us 100ms */
    const unsigned int timerPeriod = 100000;
    Timer_setPeriod(timer0, Timer_PERIOD_US, timerPeriod);

    /* Button check period: 200000us or 200ms */
    unsigned long BT_elapsedTime = 200000; // Button Flag Time checker

    /* Temperature check and LED period: 500000us or 500ms */
    unsigned long TL_elapsedTime = 500000; // Temperature Light Time read and update

    /* UART Output: 1000000us or 1000ms, 1sec */
    unsigned long Output_elapsedTime = 1000000; // Output Time of UART

    /* Configure the LED and button pins */
    GPIO_setConfig(CONFIG_GPIO_LED_0, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
    GPIO_setConfig(CONFIG_GPIO_BUTTON_0, GPIO_CFG_IN_PU | GPIO_CFG_IN_INT_FALLING);
    GPIO_setConfig(CONFIG_GPIO_BUTTON_1, GPIO_CFG_IN_PU | GPIO_CFG_IN_INT_FALLING);

    /* Turn on user LED */
    GPIO_write(CONFIG_GPIO_LED_0, CONFIG_GPIO_LED_ON);

    /* Install Button callback */
    GPIO_setCallback(CONFIG_GPIO_BUTTON_0, gpioButtonFxn0);
    GPIO_setCallback(CONFIG_GPIO_BUTTON_1, gpioButtonFxn1);

    /* Enable interrupts */
    GPIO_enableInt(CONFIG_GPIO_BUTTON_0);
    GPIO_enableInt(CONFIG_GPIO_BUTTON_1);

    /* Turn LED off */
    GPIO_write(CONFIG_GPIO_LED_0, CONFIG_GPIO_LED_OFF);

    /* Initialize temperature and setpoint as current temperature */
    temperature = readTemp();
    setpoint = readTemp();

    /* Set initial state */
    BF_STATE = BF_SMStart;
    TL_STATE = TL_SMStart;

    /* Define and initialize variable seconds to 0 */
    int32_t seconds = 0;

    while(1){
        /* Check button flags every 200ms */
        if (BT_elapsedTime >= 200000) {
            TickFct_ButtonFlag();
            BT_elapsedTime = 0;
        }

        /* Check temperature sensor every 500ms and update the LED */
        if (TL_elapsedTime >= 500000) {
            temperature = readTemp();
            TickFct_TmpLED();
            TL_elapsedTime = 0;
        }

        /*
        * Check the buttons every 200ms, check the temperature every 500ms, and update the LED and report to the server every second (via the UART).
        * If you push a button, it increases or decreases the temperature set-point by 1 degree every 200ms.
        * If the temperature is greater than the set-point, the LED should turn off.
        * If the temperature is less than the set-point, the LED should turn on (the LED controls a heater).
        * You can simulate a heating or cooling room by putting your finger on the temperature sensor.
        * The output to the server (via UART) should be formatted as <AA,BB,S,CCCC>. This can be broken down as follows:
        * AA = ASCII decimal value of room temperature (00 - 99) degrees Celsius, Room temperature is (24C)
        * BB = ASCII decimal value of set-point temperature (00-99) degrees Celsius, Set temperature from (-99C to 99C)
        * S = 0 if heat is off, 1 if heat is on
        * CCCC = decimal count of seconds since board has been reset, Resetting the board returns to Echoing characters
        * <%02d,%02d,%d,%04d> = temperature, set-point, heat, seconds
        */

        /* Output UART and report to the server every second or 1000000ms */
        if (Output_elapsedTime >= 1000000) { // Every 1 sec output to UART temperature, setpoint, heat, cool, seconds
            DISPLAY(snprintf(output, 64, "<%02d,%02d,%d,%d,%04d>\n\r", temperature, setpoint, heat, cool, seconds))
            seconds += 1;
            Output_elapsedTime = 0;
        }

        /* Wait for timer period */
        while(!TimerFlag){}

        /* Lower flag raised by timer */
        TimerFlag = 0; // set i to 0, default.

        /* Increment elapsed time by period */
        BT_elapsedTime += timerPeriod;      // Button Flag timer period increment.
        TL_elapsedTime += timerPeriod;      // Temperature Light timer period increment.
        Output_elapsedTime += timerPeriod;  // Output timer period increment.
    }
}
