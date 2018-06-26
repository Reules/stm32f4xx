#include "fsk_crc16.h"
#include "stm32f4xx_hal.h"

void fskCRC16 ( uint16_t data[], uint8_t size)
{
	uint16_t reminder = 0;
	uint8_t bits_read = 0, bit_flag;
	uint8_t dataIndex = 0;
	while (size > 0)
	{
			bit_flag = reminder >> 15;

			reminder <<= 1;
			reminder |= (data[dataIndex] >> bits_read) & 1;

			bits_read++;

			if(bits_read > 15)
			{
				bits_read = 0;
				dataIndex++;
				size--;
			}

			if(bit_flag)
				reminder^= CRC16;
	}
}
