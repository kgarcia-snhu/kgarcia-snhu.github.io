/*
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

/* Driver Header file */
#include <ti/drivers/GPIO.h>

/* Driver configuration */
#include "ti_drivers_config.h"

/* Timer header file */
#include <ti/drivers/Timer.h>

/* I2C header file */
#include <ti/drivers/I2C.h>

/* UART header file */
#include <ti/drivers/UART.h>

/* UART write output PARAms */
#define DISPLAY(x) UART_write(uart, &output, x);

/* set i to 0 */
unsigned char i = 0;

/* set timer variable to 0 */
volatile unsigned char TimerFlag = 0;

/* set button timer flag for gpioButtonFxn0 and gpioButtonFxn1 */
unsigned char Button0_Flag = 0; // Button 0 Decrease setpoint temperature <25,-2,0,0339> <temperature, setpoint, heat, seconds>
unsigned char Button1_Flag = 0; // Button 1 Increase setpoint temperature <30,26,1,0340> <temperature, setpoint, heat, seconds>

/*
========== UART Driver ==========
 */

// UART Global Variables
char output[64];
int bytesToSend;

// Driver Handles - Global variables
UART_Handle uart;

void initUART(void)
{
    UART_Params uartParams;

    UART_init();

    // Configure the driver
    UART_Params_init(&uartParams);
    uartParams.writeDataMode = UART_DATA_BINARY;
    uartParams.readDataMode = UART_DATA_BINARY;
    uartParams.readReturnMode = UART_RETURN_FULL;
    uartParams.baudRate = 115200;

    // Open the driver
    uart = UART_open(CONFIG_UART_0, &uartParams);
    if (uart == NULL) {
        /* UART_open() failed */
        while (1);
    }
}

/*
========== I2C Driver ==========
 */

// I2C Global Variables
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

// Driver Handles - Global variables
I2C_Handle i2c;

// Make sure you call initUART() before calling this function.
void initI2C(void)
{
    int8_t i, found;
    I2C_Params i2cParams;

    DISPLAY(snprintf(output, 64, "Initializing I2C Driver - "))

    // Init the driver
    I2C_init();

    // Configure the driver
    I2C_Params_init(&i2cParams);
    i2cParams.bitRate = I2C_400kHz;

    // Open the driver
    i2c = I2C_open(CONFIG_I2C_0, &i2cParams);
    if (i2c == NULL)
    {
        DISPLAY(snprintf(output, 64, "Failed\n\r"))
        while (1);
    }

    DISPLAY(snprintf(output, 32, "Passed\n\r"))

    // Boards were shipped with different sensors.
    // Welcome to the world of embedded systems.
    // Try to determine which sensor we have.
    // Scan through the possible sensor addresses

    /* Common I2C transaction setup */
    i2cTransaction.writeBuf = txBuffer;
    i2cTransaction.writeCount = 1;
    i2cTransaction.readBuf = rxBuffer;
    i2cTransaction.readCount = 0;

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

// Driver Handles - Global variables
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

    /* Init the driver */
    Timer_init();

    /* Configure the driver */
    Timer_Params_init(&params);
    params.period = 100000; // 100ms = 100000us used for temperature, setpoint, and heat 200ms, 500ms, 1000ms default 1000000 or 1sec
    params.periodUnits = Timer_PERIOD_US;
    params.timerMode = Timer_CONTINUOUS_CALLBACK;
    params.timerCallback = timerCallback;

    // Open the driver
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
 *
 *  Note: GPIO interrupts are cleared prior to invoking callbacks.
 */
void gpioButtonFxn0(uint_least8_t index)
{
    /* Toggle a temperature setpoint for LED params */
    Button0_Flag = 1;
}

/*
======== gpioButtonFxn1 ========
 *  Callback function for the GPIO interrupt on CONFIG_GPIO_BUTTON_1.
 *
 *  Note: GPIO interrupts are cleared prior to invoking callbacks.
 */
void gpioButtonFxn1(uint_least8_t index)
{
    /* Toggle a temperature setpoint for LED params */
    Button1_Flag = 1;
}

// Temperature variables
int16_t setpoint; // Desired Temperature set by buttons to increase or decrease value (UART simulation sent to server via wifi using terminal on Com3 port)
int16_t temperature; // Sensor Temperature located on Launchpad near WIFI logo, small square chip TMP006 (via I2C)
int8_t heat; // Indicates a 0 or 1, 0 = Heater and Temperature Light is off, 1 = Heater and Temperature Light are on (via GPIO and interrupt)

/*
========== State Machine's States ==========
 */

enum BF_STATES {BF_SMStart, BF_SMWaitRise, BF_SMWaitFall} BF_STATE; //  ... standard switch statements for State Machine
/* Increase or decrease setpoint from gpioButtonFxn0 or gpioButtonFxn1 buttons */

/*
 *  BF_SMStart - Start
 *  BF_SMWaitRise - Button Flag Wait Rise
 *  BF_SMWaitFall - Button Flag Wait Fall

 *  Button Flag callback if gpioButtonFxn0 or gpioButtonFxn1 are selected 0 or 1
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
                setpoint -= 1;       // Decrease setpoint by 1° C // <25,-2,0,0339> <temperature, setpoint, heat, seconds>
                Button0_Flag = 0;    // Update Button0_Flag
            }
            if (Button1_Flag) {
                setpoint += 1;       // Increase setpoint by 1° C // <30,26,1,0339> <temperature, setpoint, heat, seconds>
                Button1_Flag = 0;    // Update Button1_Flag
            }
            break;
        default:
            break;
    }
}


enum TL_STATES {TL_SMStart, TL_SMOff, TL_SMOn} TL_STATE; //  ... standard switch statements for State Machine
// Temperature heat indicator activates LED red ON or OFF indicating heater is on or off

/*
 *  TL_SMStart - Start
 *  TL_SMOff - Temperature Light Off (Heater Off)
 *  TL_SMOn - Temperature Light On (Heater On)
 */

void TickFct_TmpLED() {
    switch(TL_STATE) { // Transitions
        case TL_SMStart:
            heat = 0;
            TL_STATE = TL_SMOff; // default Temperature Light is off
            break;
        case TL_SMOff:
            // If temperature is less than setpoint proceed to STATE TL_On <14,26,1,0339> <temperature(14), setpoint(26), heat(1), seconds>
            if (temperature < setpoint) {
                TL_STATE = TL_SMOn;
                GPIO_write(CONFIG_GPIO_LED_0, CONFIG_GPIO_LED_ON);
                heat = 1; // Temperature sensor heat is on (Heater activates) LED red on
            }
            break;
        case TL_SMOn:
            // If temperature is greater than setpoint proceed to STATE TL_Off <30,26,0,0339> <temperature(30), setpoint(26), heat(0), seconds>
            if (!(temperature < setpoint)){
                TL_STATE = TL_SMOff;
                GPIO_write(CONFIG_GPIO_LED_0, CONFIG_GPIO_LED_OFF);
                heat = 0; // Temperature sensor heat is off (Heater is turned off) LED red off
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


/* Change image.sys from development mode to production mode to allow access to the COM3 Port from Launchpad
 * Otherwise error is shown that the Launchpad cannot be accessed while in development mode requested to check usb cable or unplug device and reconnect
 */
void *mainThread(void *arg0)
{
    /* Call driver init functions */
    initUART(); //  read and write for output display
    initI2C();  // sensor output and display
    initTimer(); // timer elapsed and callback
    GPIO_init();

    // Set timer period to 100000us 100ms
    const unsigned int timerPeriod = 100000;
    Timer_setPeriod(timer0, Timer_PERIOD_US, timerPeriod);

    // Button check period: 200000us or 200ms
    unsigned long BT_elapsedTime = 200000; // Button Flag Time checker

    // Temperature check and LED period: 500000us or 500ms
    unsigned long TL_elapsedTime = 500000; // Temperature Light Time read and update

    // UART Output: 1000000us or 1000ms, 1sec
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
        // Check button flags every 200ms
        if (BT_elapsedTime >= 200000) {
            TickFct_ButtonFlag();
            BT_elapsedTime = 0;
        }

        // Check temperature sensor every 500ms and update the LED
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
        * S = ‘0’ if heat is off, ‘1’ if heat is on
        * CCCC = decimal count of seconds since board has been reset, Resetting the board returns to Echoing characters
        * <%02d,%02d,%d,%04d> = temperature, set-point, heat, seconds
        */

        // Output UART and report to the server every second or 1000000ms
        if (Output_elapsedTime >= 1000000) { // Every 1 sec output to UART temperature, setpoint, heat, seconds
            DISPLAY(snprintf(output, 64, "<%02d,%02d,%d,%04d>\n\r", temperature, setpoint, heat, seconds))
            seconds += 1;
            Output_elapsedTime = 0;
        }

        // Wait for timer period
        while(!TimerFlag){}

        // Lower flag raised by timer
        TimerFlag = 0; // set i to 0

        // Increment elapsed time by period
        BT_elapsedTime += timerPeriod; // Button Flag timer period increment
        TL_elapsedTime += timerPeriod; // Temperature Light timer period increment
        Output_elapsedTime += timerPeriod; // Output timer period
    }
}
