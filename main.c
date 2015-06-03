//--------------------------------------------------------------
// File        : main.c
// Datum    : 22.12.2013
// Version    : 1.0
// Author    : UB
// Email    : mc-4u(@)t-online.de
// Web        : www.mikrocontroller-4u.de
// CPU        : STM32F4
// IDE        : CooCox CoIDE 1.7.4
// GCC        : 4.7 2012q4
// Module    : CMSIS_BOOT, M4_CMSIS_CORE
// Function    : Demo of the HC-SR04-Library
// Note        : These two files must stay at 8MHz
//                            "cmsis_boot/stm32f4xx.h"
//                            "cmsis_boot/system_stm32f4xx.c"
//--------------------------------------------------------------

#include "main.h"
#include "stm32_ub_led.h"
#include "stm32_ub_hcsr04.h"
#include <stdio.h>

//Needed to enable system delays
uint32_t multiplier;

//This function initialises the RCC
void TM_Delay_Init(void) {
    RCC_ClocksTypeDef RCC_Clocks;

    //Get system clocks
    RCC_GetClocksFreq(&RCC_Clocks);

    //While loop takes 4 cycles
    //For 1 us delay, we need to divide with 4M
    multiplier = RCC_Clocks.HCLK_Frequency / 4000000;
}

//This function allows for exact delays to be used
void TM_DelayMicros(uint32_t micros) {
    micros = micros * multiplier - 10;

    //4 cycles for one loop
    while (micros--);
}

//Main function, all initialisations are called here and
//contains the main loop to switch HCSR04s
int main(void) {
    float distance;

    SystemInit(); //Initialize quartz settings
    UB_Led_Init();

    //Mux GPIO code
    EXT_HCSR04_Sel();

    UB_HCSR04_Init();

    TM_Delay_Init();

    uint8_t i;

    while (1) {
        //The for loop is required to communicate with several HCSR04s
        for (i = 0; i < 4; i++) {
            //Switch statement to switch the correct selection pins of the MUX
            switch (i) {
                case 0:
                    GPIO_ResetBits(GPIOC, GPIO_Pin_1);
                    GPIO_ResetBits(GPIOC, GPIO_Pin_2);
                    break;
                case 1:
                    GPIO_ResetBits(GPIOC, GPIO_Pin_1);
                    GPIO_SetBits(GPIOC, GPIO_Pin_2);
                    break;
                case 2:
                    GPIO_SetBits(GPIOC, GPIO_Pin_1);
                    GPIO_ResetBits(GPIOC, GPIO_Pin_2);
                    break;
                case 3:
                    GPIO_SetBits(GPIOC, GPIO_Pin_1);
                    GPIO_SetBits(GPIOC, GPIO_Pin_2);
                    break;
                default:
                	assert(false);
                    break;
            }

            //Measure the distance
            distance = UB_HCSR04_Distance_cm();

            if (distance>0) {
                //Switch LEDs based on distance
                UB_Led_Off(LED_GREEN);
                UB_Led_Off(LED_ORANGE);
                UB_Led_Off(LED_RED);
                UB_Led_On(LED_BLUE);
            } else {
                //Outside of the measuring range
                UB_Led_Off(LED_GREEN);
                UB_Led_Off(LED_BLUE );
                UB_Led_Off(LED_ORANGE);
                UB_Led_On(LED_RED);
            }

            printf("[%d] %d\r\n", i, (int)distance);
            //printf("%d\r\n", (int)distance);
            TM_DelayMicros(1000000);
        }
    }
}
