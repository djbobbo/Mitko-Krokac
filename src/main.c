#include "stm8s.h"
#include "milis.h"
#include "spse_stm8.h"
#include "delay.h"
#include <stdio.h>
/*#include "uart1.h"*/

#define _ISOC99_SOURCE
#define _GNU_SOURCE

#define LED_PORT GPIOC
#define LED_PIN  GPIO_PIN_5
#define LED_HIGH   GPIO_WriteHigh(LED_PORT, LED_PIN)
#define LED_LOW  GPIO_WriteLow(LED_PORT, LED_PIN)
#define LED_REVERSE GPIO_WriteReverse(LED_PORT, LED_PIN)

#define BTN_PORT GPIOE
#define BTN_PIN  GPIO_PIN_4
#define BTN_PUSH (GPIO_ReadInputPin(BTN_PORT, BTN_PIN)==RESET) 

#define IN1_HIGH   GPIO_WriteHigh(GPIOB, GPIO_PIN_5)
#define IN1_LOW  GPIO_WriteLow(GPIOB, GPIO_PIN_5)

#define IN2_HIGH   GPIO_WriteHigh(GPIOB, GPIO_PIN_4)
#define IN2_LOW  GPIO_WriteLow(GPIOB, GPIO_PIN_4)

#define IN3_HIGH   GPIO_WriteHigh(GPIOB, GPIO_PIN_3)
#define IN3_LOW  GPIO_WriteLow(GPIOB, GPIO_PIN_3)

#define IN4_HIGH   GPIO_WriteHigh(GPIOB, GPIO_PIN_2)
#define IN4_LOW  GPIO_WriteLow(GPIOB, GPIO_PIN_2)


void setup(void)
{
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);      // taktovani MCU na 16MHz
    GPIO_Init(LED_PORT, LED_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);
    GPIO_Init(BTN_PORT, BTN_PIN, GPIO_MODE_IN_FL_NO_IT);

    GPIO_Init(GPIOB, GPIO_PIN_5, GPIO_MODE_OUT_PP_LOW_SLOW);
    GPIO_Init(GPIOB, GPIO_PIN_4, GPIO_MODE_OUT_PP_LOW_SLOW);
    GPIO_Init(GPIOB, GPIO_PIN_3, GPIO_MODE_OUT_PP_LOW_SLOW);
    GPIO_Init(GPIOB, GPIO_PIN_2, GPIO_MODE_OUT_PP_LOW_SLOW);

    GPIO_Init(GPIOG,GPIO_PIN_4,GPIO_MODE_IN_FL_NO_IT); //brána

    GPIO_Init(GPIOG, GPIO_PIN_5, GPIO_MODE_OUT_PP_LOW_SLOW); //buzzer

    init_milis();
    /*init_uart1();*/
}

void delay_ms(uint16_t ms) {
    uint16_t  i;
    for (i=0; i<ms; i = i+1){
        _delay_us(250);
        _delay_us(248);
        _delay_us(250);
        _delay_us(250);
    }
}



void levo(void) {
    //1 krok full step
    IN1_HIGH;
    IN2_HIGH;
    IN3_LOW;
    IN4_LOW;
    delay_ms(3);
    //2 krok
    IN1_LOW;
    IN2_HIGH;
    IN3_HIGH;
    IN4_LOW;
    delay_ms(3);
    //3 krok
    IN1_LOW;
    IN2_LOW;
    IN3_HIGH;
    IN4_HIGH;
    delay_ms(3);
    //4 krok
    IN1_HIGH;
    IN2_LOW;
    IN3_LOW;
    IN4_HIGH;
    delay_ms(3);
}

void pravo(void) {
    //1 krok full step
    IN1_HIGH;
    IN2_LOW;
    IN3_LOW;
    IN4_HIGH;
    delay_ms(3);
    //2 krok
    IN1_LOW;
    IN2_LOW;
    IN3_HIGH;
    IN4_HIGH;
    delay_ms(3);
    //3 krok
    IN1_LOW;
    IN2_HIGH;
    IN3_HIGH;
    IN4_LOW;
    delay_ms(3);
    //4 krok
    IN1_HIGH;
    IN2_HIGH;
    IN3_LOW;
    IN4_LOW;
    delay_ms(3);
}


int main(void)
{   
    uint32_t stav = 0;
    int i;

    setup();

    while (1) {

        if ( GPIO_ReadInputPin(GPIOG,GPIO_PIN_4)) {
            if ( stav == 0) {
            pravo();
            }

            if ( stav == 1) {
            levo(); 
            }

        }
        else{ 
        
            if ( stav == 0) {
            stav = 1;
            delay_ms(500);
            GPIO_Init(GPIOG,GPIO_PIN_5,GPIO_MODE_OUT_PP_HIGH_SLOW); 
            for (i = 1; i < 11; ++i)
            {
                levo();
            }
            GPIO_Init(GPIOG,GPIO_PIN_5,GPIO_MODE_OUT_PP_LOW_SLOW); 
            }

            else  {
            stav = 0;
            delay_ms(500);
            GPIO_Init(GPIOG,GPIO_PIN_5,GPIO_MODE_OUT_PP_HIGH_SLOW); 
            for (i = 1; i < 11; ++i)
            {
                pravo();
            }
            GPIO_Init(GPIOG,GPIO_PIN_5,GPIO_MODE_OUT_PP_LOW_SLOW); 
            }

        }
        
    }
}

/*-------------------------------  Assert -----------------------------------*/
#include "__assert__.h"
