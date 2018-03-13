#include "bgt24mtr11.h"
#include "stm32f4xx_hal.h"

extern void MX_SPI1_BGT24_Init(void);
extern SPI_HandleTypeDef hspi1;

uint16_t spi1DataTxBgt = BGT_CONF_SPI;

void bgt24Init(void){
	MX_SPI1_BGT24_Init();
	Spi1Bgt24Tran();
}

void bgt24PowerOn(void){
	static uint16_t spi1DataTxpower = 2311; //spi transport 0000100000111111b to bgt24mtr11, TX power enabled and no reduce
	MX_SPI1_BGT24_Init();
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi1, (uint8_t*) &spi1DataTxpower, 1, 5000);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_SET);
}

void bgt24PowerDown(void){
	static uint16_t spi1DataTxpower = 6215; //TX power disabled
	MX_SPI1_BGT24_Init();
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi1, (uint8_t*) &spi1DataTxpower, 1, 5000);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_SET);
}

void Spi1Bgt24Tran(void){
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi1, (uint8_t*) &spi1DataTxBgt, 1, 5000);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_SET);
}
