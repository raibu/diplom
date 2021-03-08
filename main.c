#include "stm32f4xx.h"
#include <stdint.h>
#include "sdcard.h"
#define FCRYS 8000000UL
#define FCPU 48000000UL //x6
#define PLLM 0
#define PLLN 6
#define PLLP 16
#define PLLQ 24 
#define DELAY (48000000UL/3)

extern void delay(uint32_t);
// void delay(volatile uint32_t i){
// 	while(--i);
// }

int main(void)
{
	SD_Error errno;

	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN | RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIODEN;
	FLASH->ACR |= FLASH_ACR_LATENCY_3WS|FLASH_ACR_PRFTEN|FLASH_ACR_ICEN|FLASH_ACR_DCEN; //s zapasom
	__asm__ __volatile__("":::"memory");
	//output gpiob15
	GPIOD->MODER |= GPIO_MODER_MODER15_0;
	//TODO set the clocking from crystal
	delay(1000);
	RCC->CR |= RCC_CR_HSEON;                      //enable HSE
  	while((RCC->CR & RCC_CR_HSERDY) == 0) {}      //wait till HSE is ready
  	RCC->CFGR = RCC_CFGR_PPRE1_DIV2;
  	RCC->CFGR|= RCC_CFGR_PPRE2_DIV1;
  	RCC->CFGR|= RCC_CFGR_HPRE_DIV1;
	//input 8 mhz, /4=2 mhz,     *48*2=48*4mhz,     /4=48 mhz,    /2=48mhz
  	RCC->PLLCFGR = (4 << PLLM) | ((48*2) << PLLN) | (1 << PLLP) | (4 << PLLQ) | RCC_PLLCFGR_PLLSRC_HSE;
  	__asm__ __volatile__("":::"memory");
  	RCC->CR |= RCC_CR_PLLON;                    //enable PLL
  	while((RCC->CR & RCC_CR_PLLRDY) == 0) {}      //wait till PLL is ready
  	RCC->CFGR &= ~RCC_CFGR_SW;                    //clear SW bits
  	RCC->CFGR |= RCC_CFGR_SW_PLL;                 //select PLL as system clock
  	while((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL) {} //wait till PLL is used
	delay(DELAY/10);
	//rcc on gpio ab and usart
	// RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
	//AF b6 usart1
	// GPIOB->MODER |= GPIO_MODER_MODER6_1; //mode AF
	// GPIOB->OSPEEDR |= GPIO_OSPEEDR_OSPEED6; //high speed;
	// GPIOB->AFR[0] |= 0x7<<GPIO_AFRL_AFSEL6_Pos; //map AF7 to pin 6;

	// USART1->BRR = (FCPU/9600)&0xffff;
	// USART1->CR1 |= USART_CR1_TE | USART_CR1_UE;
	uint32_t buf[32*256];//32768 byte buffer
 
	errno = SD_Init();
	// USART1->DR = (uint8_t)errno;
	if (errno != SD_OK)
	{
		for (int i = 0; i < 1+errno; ++i)
		{
			GPIOD->ODR^=1<<15;
			delay(DELAY/4);
			GPIOD->ODR^=1<<15;
			delay(DELAY/4);
		}		
	}else{
			GPIOD->ODR^=1<<15;
			delay(DELAY/4);
			GPIOD->ODR^=1<<15;
			delay(DELAY/4);
	}
	delay(DELAY*2);
	errno = SD_EnableWideBusOperation(SDIO_BusWide_4b);
	// USART1->DR = (uint8_t)errno;
	if (errno != SD_OK)
	{
		for (int i = 0; i < 1+errno; ++i)
		{
			GPIOD->ODR^=1<<15;
			delay(DELAY/4);
			GPIOD->ODR^=1<<15;
			delay(DELAY/4);
		}		
	}else{
			GPIOD->ODR^=1<<15;
			delay(DELAY/4);
			GPIOD->ODR^=1<<15;
			delay(DELAY/4);
	}
	delay(DELAY*2);

	errno = SD_ReadMultiBlocks(0, buf, 512, 32*2);
	// USART1->DR = (uint8_t)errno;
	if (errno != SD_OK)
	{
		for (int i = 0; i < 1+errno; ++i)
		{
			GPIOD->ODR^=1<<15;
			delay(DELAY/4);
			GPIOD->ODR^=1<<15;
			delay(DELAY/4);
		}		
	}else{
			GPIOD->ODR^=1<<15;
			delay(DELAY/4);
			GPIOD->ODR^=1<<15;
			delay(DELAY/4);
	}

	while(1){
		// GPIOB->ODR^=1<<15;
		// USART1->DR = (uint8_t)errno;
		// delay(DELAY);
	}

	while(1);
	return 0;
}

