
#ifndef SDRAM_H_
#define SDRAM_H_

void SDRAM_Init(void);

void SDRAM_Write(uint32_t address, uint16_t data);

uint16_t SDRAM_Read(uint32_t address);

void SDRAM_WriteArray(uint32_t address, const uint16_t* data, uint32_t length);

#endif /* SDRAM_H_ */
