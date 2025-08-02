#ifndef UART_H_
#define UART_H_

#include "x32ctrl.h"

// Maximal mögliche Paketlänge inklusive Startbyte, Endbyte und Checksumme
// Langes Paket: 0xFE (1) + BoardID (1) + ID (1) + Value_MSB (1) + Value_LSB (1) + 0xFE (1) + CHECKSUM (1) = 7 Bytes
#define MAX_MESSAGE_SIZE 200

typedef struct {
    char buffer[MAX_MESSAGE_SIZE];
    size_t current_length;
} messageBuilder;

extern messageBuilder message;
extern int fdSurface;
extern int fdAdda;

void messageBuilderInit(messageBuilder *message);
int messageBuilderAddRawByte(messageBuilder *message, char byte);
int messageBuilderAddDataByte(messageBuilder *message, char byte);
int messageBuilderAddString(messageBuilder *message, const char *str);
int messageBuilderAddDataArray(messageBuilder *message, const char *data, uint8_t len);

uint8_t calculateChecksum(const char *data, uint16_t len);
int uartOpen(char *ttydev, uint32_t baudrate, int *fd);
int uartTx(int *fd, messageBuilder *builder, bool addChecksum);
int uartRx(int *fd, char *buf, uint16_t bufLen);

#endif
