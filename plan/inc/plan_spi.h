//****************************************************************************
// MIT Media Lab - Biomechatronics
// Jean-Francois (Jeff) Duval
// jfduval@mit.edu
// 07/2014
//****************************************************************************
// plan_spi: SPI driver for the Plan board
//****************************************************************************

#ifndef INC_PLAN_SPI_H
#define INC_PLAN_SPI_H

//****************************************************************************
// Include(s)
//****************************************************************************


//****************************************************************************
// Prototype(s):
//****************************************************************************

void flexsea_spi_open(void);
void flexsea_spi_close(void);
void flexsea_spi_print_details(void);
void flexsea_spi_transmit(char bytes_to_send, unsigned char spi_tx_data[], unsigned char verbal);
void pabort(const char *s);

//****************************************************************************
// Definition(s):
//****************************************************************************

#endif
