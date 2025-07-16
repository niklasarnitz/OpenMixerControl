#include "uart.h"

// uart
int fd;
struct termios tty;
char buffer_uart[256]; // Puffer für UART-Lesevorgänge
ssize_t bytes_read;
int bytes_available;
uint8_t receivedBoardId;

int uartTxData(const unsigned char *data_buffer, int num_bytes) {
    if (fd < 0) {
        fprintf(stderr, "Error: Problem on opening serial port\n");
        return -1;
    }
    if (data_buffer == NULL || num_bytes <= 0) {
        fprintf(stderr, "Error: Wrong number of bytes.\n");
        return 0;
    }

    ssize_t bytes_written = write(fd, data_buffer, num_bytes);

    if (bytes_written < 0) {
        perror("On on writing to serial port");
        return -1;
    } else if (bytes_written != num_bytes) {
        fprintf(stderr, "Warning: Only %zd of %d Bytes written!\n", bytes_written, num_bytes);
    }

    return (int)bytes_written;
}

int uartOpen() {
    fd = open("/dev/ttymxc1", O_RDWR | O_NOCTTY | O_NDELAY);
    if (fd < 0) {
        perror("Error opening /dev/ttymxc1 !");
        return 1;
    }

    if (tcgetattr(fd, &tty) != 0) {
        perror("Error reading serial-port-attributes!");
        close(fd);
        return 1;
    }

    tty.c_iflag &= ~(IGNPAR | ICRNL);
    tty.c_iflag |= IGNPAR;
    tty.c_oflag &= ~(OPOST | ONLCR);
    tty.c_cflag |= (CLOCAL | CREAD);
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8;
    tty.c_cflag &= ~PARENB;
    tty.c_cflag &= ~CSTOPB;
    tty.c_lflag &= ~(ECHO | ECHOE | ECHOK | ICANON | ISIG);

    tty.c_cc[VMIN] = 0; // Nicht blockierend lesen
    tty.c_cc[VTIME] = 0;

    cfsetispeed(&tty, B115200);
    cfsetospeed(&tty, B115200);

    if (tcsetattr(fd, TCSANOW, &tty) != 0) {
        perror("Error setting serial-port-attributes!");
        close(fd);
        return 1;
    }

    return 0;
}

int uartRead() {
    uint8_t receivedClass = 0;
    uint8_t receivedIndex = 0;
    uint16_t receivedValue = 0;
    //uint8_t receivedChecksum = 0;
    uint8_t currentByte;
    uint8_t packet_buffer[MAX_BUFFER_SIZE];
    int packet_buffer_len = 0;

	if (ioctl(fd, FIONREAD, &bytes_available) == -1) {
		perror("Error on ioctl FIONREAD");
		close(fd);
		return 1;
	}

	if (bytes_available > 0) {
		bytes_read = read(fd, buffer_uart, (bytes_available < sizeof(buffer_uart)) ? bytes_available : sizeof(buffer_uart));

		if (bytes_read < 0) {
			perror("Error reading from serial-port");
			close(fd);
			return 1;
		} else if (bytes_read > 0) {
			for (int i = 0; i < bytes_read; i++) {
				currentByte = (uint8_t)buffer_uart[i];
				//printf("%02X ", currentByte); // empfangene Bytes als HEX-Wert ausgeben

				// add received byte to buffer
				if (packet_buffer_len < MAX_BUFFER_SIZE) {
					packet_buffer[packet_buffer_len++] = currentByte;
				} else {
					// buffer full -> remove oldest byte
					memmove(packet_buffer, packet_buffer + 1, MAX_BUFFER_SIZE - 1);
					packet_buffer[MAX_BUFFER_SIZE - 1] = currentByte;
				}

				// we are expecting two types of messages: long (standard) and short (repeating message)
				// Short:                          Index + Value_LSB + 0xFE + CHECKSUM = 5 Bytes
				// Short:                          Index + Value_LSB + Value_MSB + 0xFE + CHECKSUM = 6 Bytes
				// Long:  0xFE + BoardID + Class + Index + Value_LSB             + 0xFE + CHECKSUM = 7 Bytes
				// Long:  0xFE + BoardID + Class + Index + Value_LSB + Value_MSB + 0xFE + CHECKSUM = 8 Bytes
				int receivedPacketLength = 0; // Länge des erkannten Pakets

				// check if we received enought data to process at least the shortest message
				if (packet_buffer_len >= 4) {
					// check for expected end-sequence (0xFE CHECKSUM)
					if (packet_buffer[packet_buffer_len - 2] == 0xFE) {
						// check for long 16-bit-packet (8 bytes)
						// 0xFE + BoardID + Class + Index + Value_LSB + Value_MSB + 0xFE + CHECKSUM = 8 Bytes
						if (packet_buffer_len >= 8 && packet_buffer[packet_buffer_len - 8] == 0xFE) {
							receivedBoardId = packet_buffer[packet_buffer_len - 7] & 0x7F;
							receivedClass = packet_buffer[packet_buffer_len - 6];
							receivedIndex = packet_buffer[packet_buffer_len - 5];
							receivedValue = ((uint16_t)packet_buffer[packet_buffer_len - 3] << 8) | packet_buffer[packet_buffer_len - 4];
							//receivedChecksum = packet_buffer[packet_buffer_len - 1];
							receivedPacketLength = 8;
						}
						// check for long 8-bit-packet (7 bytes)
						// 0xFE + BoardID + Class + Index + Value_LSB             + 0xFE + CHECKSUM = 7 Bytes
						else if (packet_buffer_len >= 7 && packet_buffer[packet_buffer_len - 7] == 0xFE) {
							receivedBoardId = packet_buffer[packet_buffer_len - 6] & 0x7F;
							receivedClass = packet_buffer[packet_buffer_len - 5];
							receivedIndex = packet_buffer[packet_buffer_len - 4];
							receivedValue = packet_buffer[packet_buffer_len - 3];
							//receivedChecksum = packet_buffer[packet_buffer_len - 1];
							receivedPacketLength = 7;
						}
						// check for short 16-bit-packet (6 bytes)
						// Class + Index + Value_LSB + Value_MSB + 0xFE + CHECKSUM = 8 Bytes
						else if (packet_buffer_len >= 6) {
							// 0xFE and BoardID is missing here
							receivedClass = packet_buffer[packet_buffer_len - 6];
							receivedIndex = packet_buffer[packet_buffer_len - 5];
							receivedValue = ((uint16_t)packet_buffer[packet_buffer_len - 3] << 8) | packet_buffer[packet_buffer_len - 4];
							//receivedChecksum = packet_buffer[packet_buffer_len - 1];
							receivedPacketLength = 8;
						}
						// check for short 8-bit-packet (5 bytes)
						// ID Value_MSB Value_LSB 0xFE CHECKSUM
						else if (packet_buffer_len >= 5) {
							// 0xFE and BoardID is missing here
							receivedClass = packet_buffer[packet_buffer_len - 5];
							receivedIndex = packet_buffer[packet_buffer_len - 4];
							receivedValue = packet_buffer[packet_buffer_len - 3];
							//receivedChecksum = packet_buffer[packet_buffer_len - 1];
							receivedPacketLength = 5;
						}

/*
						// now recalculate the checksum and check against received one
						unsigned char buf[receivedPacketLength];
						for (uint8_t i=0; i<(receivedPacketLength-1); i++) {
							buf[i] = packet_buffer[packet_buffer_len - receivedPacketLength - i];
						}
						calculateChecksum(buf, sizeof(buf));
						if ((receivedPacketLength > 0) && (receivedChecksum == buf[sizeof(buf)-1])) {
							surfaceCallback(receivedBoardId, receivedClass, receivedIndex, receivedValue);
						}
*/
						if (receivedPacketLength > 0) {
							surfaceCallback(receivedBoardId, receivedClass, receivedIndex, receivedValue);
						}
						/*
						if (packet_buffer_len > 0) {
						  printf("Warnung: verbleibende Bytes im Puffer: %d\n", packet_buffer_len);
						}
						*/

						// shift remaining bytes byte by processed amount of data
						memmove(packet_buffer, packet_buffer + receivedPacketLength, packet_buffer_len - receivedPacketLength);
						packet_buffer_len -= receivedPacketLength;
					}
				}
			}
			fflush(stdout);
		}
	}

	return 0;
}
