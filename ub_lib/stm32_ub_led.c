//--------------------------------------------------------------
// File         : stm32_ub_led.c
// Datum        : 09.02.2013
// Version    : 1.1
// Author     : UB
// Email        : mc-4u(@)t-online.de
// Web            : www.mikrocontroller-4u.de
// CPU            : STM32F4
// IDE            : CooCox CoIDE 1.7.0
// Module     : GPIO
// Function : LED functions
//--------------------------------------------------------------

#include "stm32_ub_led.h"

//This structure is used to make statuses and names of LEDs simpler
LED_t LED[] = {
    //Name         , PORT , PIN        , CLOCK                    , Init
    {LED_GREEN , GPIOD, GPIO_Pin_12, RCC_AHB1Periph_GPIOD, LED_OFF},     // PD12 is green LED on discovery board
    {LED_ORANGE, GPIOD, GPIO_Pin_13, RCC_AHB1Periph_GPIOD, LED_OFF},     // PD13 is orange LED
    {LED_RED     , GPIOD, GPIO_Pin_14, RCC_AHB1Periph_GPIOD, LED_OFF},     // PD14 is red LED
    {LED_BLUE    , GPIOD, GPIO_Pin_15, RCC_AHB1Periph_GPIOD, LED_OFF},     // PD15 is blue LED
};

//Initialise the GPIO of the LEDs
void UB_Led_Init(void) {
    GPIO_InitTypeDef GPIO_InitStructure;
    LED_NAME_t led_name;

    for (led_name=0; led_name<LED_ANZ; led_name++) {
        RCC_AHB1PeriphClockCmd(LED[led_name].LED_CLK, ENABLE);

        //Configure as digital output
        GPIO_InitStructure.GPIO_Pin = LED[led_name].LED_PIN;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
        GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
        GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(LED[led_name].LED_PORT, &GPIO_InitStructure);

        //Set default value
        if (LED[led_name].LED_INIT==LED_OFF)
             UB_Led_Off(led_name);
        else
            UB_Led_On(led_name);
    }
}

//Switch a LED off
void UB_Led_Off(LED_NAME_t led_name) {
    LED[led_name].LED_PORT->BSRRH = LED[led_name].LED_PIN;
}

//Switch a LED on
void UB_Led_On(LED_NAME_t led_name) {
    LED[led_name].LED_PORT->BSRRL = LED[led_name].LED_PIN;
} 

//Toggle a LEDs status
void UB_Led_Toggle(LED_NAME_t led_name) {
    LED[led_name].LED_PORT->ODR ^= LED[led_name].LED_PIN;
}


//Switch a LEDs current status
void UB_Led_Switch(LED_NAME_t led_name, LED_STATUS_t wert) {
    if (wert==LED_OFF)
        UB_Led_Off(led_name);
    else
        UB_Led_On(led_name);
}
