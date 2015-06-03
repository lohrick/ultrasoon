#ifndef __STM32F4_UB_HCSR04_H
    #define __STM32F4_UB_HCSR04_H

#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_tim.h"
#include "misc.h"

//Factor for delay in microseconds to centimeters (factor 1/58)
#define    HCSR04_FAKTOR_US_2_CM  0.01724

//Measurement time-out in microseconds
#define    HCSR04_TIMEOUT         9000    //9000us = 9ms => ~1.5m distance

//Delay after each measurement (in microseconds)
//Required according to datasheet
#define    HCSR04_DELAY           60000

// Echo Pin of the HC-SR04 (PA0)
//Note: the pin must be an input capture-pin of the used timer
#define    HCSR04_ECHO_PORT       GPIOA
#define    HCSR04_ECHO_CLK        RCC_AHB1Periph_GPIOA
#define    HCSR04_ECHO_PIN        GPIO_Pin_0
#define    HCSR04_ECHO_SOURCE     GPIO_PinSource0

// Trigger Pin of the HC-SR04 (PD3)
#define    HCSR04_TRIGGER_PORT    GPIOD
#define    HCSR04_TRIGGER_CLK     RCC_AHB1Periph_GPIOD
#define    HCSR04_TRIGGER_PIN     GPIO_Pin_3

//Input-capture settings (frequency of TIM2)
//Base-frequency 2*APB1 (APB1 = 42MHz) => TIM_CLK = 84MHz
//Prescaler can be 0 to 0xFFFF
//TIM2 frequency = TIM_CLK / (prescaler + 1)
#define    HCSR04_TIM2_PRESCALE    83

//Delay settings (of TIM7)
//Base-frequency 2*APB1 (APB1 = 42MHz) => TIM_CLK = 84MHz
//Prescaler can be 0 to 0xFFFF
//TIM7 delay = TIM_CLK / (prescaler + 1) / (period + 1)
#define    HCSR04_TIM7_PRESCALE    83
#define    HCSR04_TIM7_PERIOD      9

typedef struct {
    uint32_t t2_akt_time;
    uint32_t t7_akt_time;
    uint32_t delay_us;
} HCSR04_t;

HCSR04_t HCSR04;

//Main initialisation of the library
void UB_HCSR04_Init(void);

//Function for measuring the distance
float UB_HCSR04_Distance_cm(void);

//Function to change the line of the multiplexer by toggling PC1, PC2
void EXT_HCSR04_Sel(void);

#endif // __STM32F4_UB_HCSR04_H
