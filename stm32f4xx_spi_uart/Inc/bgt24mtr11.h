#ifndef BGT24_H
#define BGT24_H

#define BGT_CONF_SPI	6215

void bgt24Init(void);
void bgt24PowerOn(void);
void bgt24PowerDown(void);

/*the tranmission functions*/
void Spi1Bgt24Tran(void);

#endif
