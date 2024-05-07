/*
 * yacp_api.h
 * Yet Another Calibration Protocol (YACP)
 * 
 * This is the consumer facing API for use in firmware projects.
 * It is automatically included in the cal.h generated file.
 * 
 * Matthew Bergman 2021
 * 
 * MIT license, all text above must be included in any redistribution.
 * See license.txt at the root of the repository for full license text.
 */

#ifndef YACP_API_H_
#define YACP_API_H_

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#define CAL_PASSTHRU 0
#define CAL_OVERRIDDEN 1

#define YACP_COMMAND_ID 0x100

#define CAL_PROTOCOL_VERSION 1

// Remote data
extern bool yacp_eeprom_version_mismatch_f;
extern bool yacp_eeprom_crc_mismatch_f;

// Struct defs

// Each override has a 4 byte value. Depending on the override's actual cal_type
// use one of the data types in the union.
typedef union cal_value
{
  uint8_t u8;
  int8_t i8;
  uint16_t u16;
  int16_t i16;
  uint32_t u32;
  int32_t i32;
  float f;
} cal_value;

// Each override has a status (OVERRIDDEN, PASSTHROUGH), and a value.

#pragma pack(push)
#pragma pack(1)
typedef struct cal_override
{
  uint8_t status;
  cal_value value;
} cal_override;
#pragma pack(pop)

// Driver Functions

// Sends a CAN message onto the bus
// id: CAN arbitration ID
// buf: 8 byte buffer containing the data to be sent
void yacp_can_send(uint32_t id, uint8_t* buf);

// Called frequenty to check if new CAN messages have arrived from main code.
// Calls handle_can() when messages are received.
void yacp_can_recv();

// Initialize mailboxes
void yacp_can_init();

// Handle a CAN message
void yacp_handle_can(uint32_t id, uint8_t* buf);

// Returns one byte of data from EEPROM address addr.
uint8_t yacp_eeprom_load_byte(uint16_t addr);

// Stores one byte of data in val to EEPROM address addr.
void yacp_eeprom_store_byte(uint16_t addr, uint8_t val);

// For memory mapped NVM systems this will actually persist the data
void yacp_eeprom_persist();

// memcpy function that knows native byte order and types
void yacp_memcpy(void* s1, const void* s2, uint16_t n);
void yacp_update_setting(uint8_t* dst, uint16_t var_start, uint8_t var_len, uint8_t* buf);

// YACP API
// Called once at the start of execution by the main code. Loads default and stored settings.
void yacp_init();

#endif
