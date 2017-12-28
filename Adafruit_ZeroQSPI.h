#ifndef LIB_ZERO_QSPI_H
#define LIB_ZERO_QSPI_H

#include "SPI.h"
#include <Arduino.h>

typedef enum {
	QSPI_SPI_MODE = 0,
	QSPI_MEMORY_MODE,
} QSPIMode_t;

typedef enum {
	QSPI_DATAWIDTH_8_BITS = QSPI_CTRLB_DATALEN_8BITS_Val,
	QSPI_DATAWIDTH_9_BITS,
	QSPI_DATAWIDTH_10_BITS,
	QSPI_DATAWIDTH_11_BITS,
	QSPI_DATAWIDTH_12_BITS,
	QSPI_DATAWIDTH_13_BITS,
	QSPI_DATAWIDTH_14_BITS,
	QSPI_DATAWIDTH_15_BITS,
	QSPI_DATAWIDTH_16_BITS,
} QSPIDataWidth_t;

typedef enum {
	QSPI_ADDRLEN_24_BITS = QSPI_INSTRFRAME_ADDRLEN_24BITS_Val,
	QSPI_ADDRLEN_32_BITS,
} QSPIAddrLen_t;

typedef enum {
	QSPI_OPCODE_LEN_1_BITS = QSPI_INSTRFRAME_OPTCODELEN_1BIT_Val,
	QSPI_OPCODE_LEN_2_BITS,
	QSPI_OPCODE_LEN_4_BITS,
	QSPI_OPCODE_LEN_8_BITS,
} QSPIOpcodeLen_t;

typedef enum {
	QSPI_IO_FORMAT_SINGLE = QSPI_INSTRFRAME_WIDTH_SINGLE_BIT_SPI_Val,
	QSPI_IO_FORMAT_SINGLE_DUAL_DATA,
	QSPI_IO_FORMAT_SINGLE_QUAD_DATA,
	QSPI_IO_FORMAT_SINGLE_INSTR_DUAL_ADDR_DUAL_DATA,
	QSPI_IO_FORMAT_SINGLE_INSTR_QUAD_ADDR_QUAD_DATA,
	QSPI_IO_FORMAT_DUAL,
	QSPI_IO_FORMAT_QUAD,
} QSPIIOFormat_t;

typedef enum {
	QSPI_READ = QSPI_INSTRFRAME_TFRTYPE_READ_Val,
	QSPI_READ_MEMORY,
	QSPI_WRITE,
	QSPI_WRITE_MEMORY,
} QSPITransferType_t;

#define QSPI_OPTION_INSTREN QSPI_INSTRFRAME_INSTREN
#define QSPI_OPTION_ADDREN QSPI_INSTRFRAME_ADDREN
#define QSPI_OPTION_OPCODEEN QSPI_INSTRFRAME_OPTCODEEN
#define QSPI_OPTION_DATAEN QSPI_INSTRFRAME_DATAEN

typedef struct {
	bool nContinuousRead = false;
	QSPIAddrLen_t addrLen;
	QSPIOpcodeLen_t opcodeLen;
	QSPIIOFormat_t ioFormat;
	uint8_t options;
	QSPITransferType_t type;
	uint8_t instruction;
} QSPIInstr;

class QSPIClass
{
public:
	QSPIClass() {};
	~QSPIClass() {};

	byte transfer(uint16_t data);
	void transfer(void *buf, size_t count);

	// Transaction Functions
	void usingInterrupt(int interruptNumber);
	void beginTransaction(SPISettings settings);
	void endTransaction(void);

	// SPI Configuration methods
	void attachInterrupt();
	void detachInterrupt();

	void begin();
	void end();

	void runInstruction(QSPIInstr *instr);

	void setMemoryMode(QSPIMode_t mode);
	void setClockDivider(uint8_t uc_div);
	void setDataWidth(QSPIDataWidth_t width);

	private:
	void config(SPISettings settings);
};

extern QSPIClass QSPI0;

#endif
