#ifndef __LIB_CRC_H_
#define __LIB_CRC_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "BSP_STC32G_system.h"

/**
 * @brief This function is to get the crc32 update object.
 *
 * @param init_vect is value to clear certain positions
 * @param buffer is a buffer that requires A CRC check
 * @param len is buffer len
 * @return uint32_t return 32 bits unsigned CRC32 value
 */
uint32_t getcrc32_update(uint32_t init_vect, const uint8_t *buffer, uint32_t len);

/**
 * @brief This function is to get crc32 value.
 *
 * @param buffer is a buffer that requires A CRC check
 * @param len is buffer len
 * @return uint32_t return 32 bits unsigned CRC32 check value
 */
uint32_t getcrc32(const uint8_t *buffer, uint32_t len);

/**
 * @brief This function is to get the crc16 update object
 *
 * @param init_vect is value to clear certain positions
 * @param buffer is a buffer that requires A CRC check
 * @param len is buffer len
 * @return uint16_t 16 bits unsigned CRC32 check value
 */
uint16_t getcrc16_update(uint16_t init_vect, const uint8_t *buffer, uint32_t len);

/**
 * @brief This function is to get the crc32 h16 object
 *
 * @param buffer is a buffer that requires A CRC check
 * @param len is buffer len
 * @return uint16_t 16 bits unsigned CRC32 check value
 */
uint16_t getcrc32_h16(const uint8_t *buffer, uint32_t len);

/**
 * @brief This function is to get crc16 value.
 *
 * @param buffer is a buffer that requires A CRC check
 * @param len is buffer len
 * @return uint16_t 16 bits unsigned CRC16 check value
 */
uint16_t getcrc16(const uint8_t *buffer, uint32_t len);

/**
 * @brief This function is to get the crc16 ccitt update object
 *
 * @param init_vect is value to clear certain positions
 * @param buffer is a buffer that requires A CRC check
 * @param len is buffer len
 * @return uint16_t 16 bits unsigned CRC16 ccitt check value
 */
uint16_t getcrc16_ccitt_update(uint16_t init_vect, const uint8_t *buffer,
                               uint32_t len);

/**
 * @brief This function is to Get the crc16 ccitt object
 *
 * @param buffer is a buffer that requires A CRC check
 * @param len is buffer len
 * @return uint16_t 16 bits unsigned CRC16 ccitt check value
 */
uint16_t getcrc16_ccitt(const uint8_t *buffer, uint32_t len);

/**
 * @brief This function is to Get the crc8 update object
 *
 * @param init_vect is value to clear certain positions
 * @param buffer is a buffer that requires A CRC check
 * @param len is buffer len
 * @return uint8_t 8 bits unsigned CRC8 check value
 */
uint8_t getcrc8_update(uint8_t init_vect, const uint8_t *buffer, uint32_t len);

/**
 * @brief This function is to get crc8 value.
 *
 * @param buffer is a buffer that requires A CRC check
 * @param len is buffer len
 * @return uint8_t 8 bits unsigned CRC8 check value
 */
uint8_t getcrc8(const uint8_t *buffer, uint32_t len);

#ifdef __cplusplus
}
#endif

#endif /* __LIB_CRC_H_ */
