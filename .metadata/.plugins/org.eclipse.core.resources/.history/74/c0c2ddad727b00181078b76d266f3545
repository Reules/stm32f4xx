#include "bgt24mtr11.h"
#include "stm32f4xx_hal.h"

extern void MX_SPI1_BGT24_Init(void);
extern SPI_HandleTypeDef hspi1;

uint16_t spi1DataTxBgt = BGT_CONF_SPI;

void bgt24Init(void)
{
	MX_SPI1_BGT24_Init();
	Spi1Bgt24Tran();
}

void bgt24PowerUp(void)
{
	static uint16_t spi1DataTxpowerUp = 2113; //spi transport 0001 1000 0100 0001b to bgt24mtr11, TX power enabled and no reduce
	MX_SPI1_BGT24_Init();
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi1, (uint8_t*) &spi1DataTxpowerUp, 1, 5000);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_SET);
}

void bgt24PowerDown(void)
{
	static uint16_t spi1DataTxpowerDown = 6215; //TX power disabled
	MX_SPI1_BGT24_Init();
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi1, (uint8_t*) &spi1DataTxpowerDown, 1, 5000);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_SET);
}

void Spi1Bgt24Tran(void)
{
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi1, (uint8_t*) &spi1DataTxBgt, 1, 5000);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_SET);
}
