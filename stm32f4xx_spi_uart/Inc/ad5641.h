#ifndef AD5641_H
#define AD5641_H

#define DAC_START 		8191
#define DAC_MAX 		16383
#define DAC_MIN 		0

void dacAddtion(void);
void dacSubtraction(void);
void dacMultiplication(void);
void dacDivision(void);
void Spi3DacTran(void);

#endif