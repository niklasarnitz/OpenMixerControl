#ifndef UART_H_
#define UART_H_

#include "x32ctrl.h"

// Maximal mögliche Paketlänge inklusive Startbyte, Endbyte und Checksumme
// Langes Paket: 0xFE (1) + BoardID (1) + ID (1) + Value_MSB (1) + Value_LSB (1) + 0xFE (1) + CHECKSUM (1) = 7 Bytes
#define MAX_BUFFER_SIZE 32 // Etwas größer als die maximale Paketlänge, um Überläufe zu handhaben

int uartTxData(const unsigned char *data_buffer, int num_bytes);
int uartOpen();
int uartRead();

#endif
