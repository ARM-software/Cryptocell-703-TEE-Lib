/*******************************************************************************
* The confidential and proprietary information contained in this file may      *
* only be used by a person authorised under and to the extent permitted        *
* by a subsisting licensing agreement from ARM Limited or its affiliates.      *
*   (C) COPYRIGHT [2001-2017] ARM Limited or its affiliates.                   *
*       ALL RIGHTS RESERVED                                                    *
* This entire notice must be reproduced on all copies of this file             *
* and copies of this file may only be made by a person if such person is       *
* permitted to do so under the terms of a subsisting license agreement         *
* from ARM Limited or its affiliates.                                          *
*******************************************************************************/

#ifndef TEST_PAL_MAP_ADDRS_H_
#define TEST_PAL_MAP_ADDRS_H_

/*!
 @file
 @brief This file contains PAL map address integration tests.
 */

/*!
 @addtogroup pal_address_test
 @{
 */

#ifdef __cplusplus
extern "C" {
#endif

#define VALID_MAPPED_ADDR(addr) ((addr != 0) && (addr != 0xFFFFFFFF))

/* Bit Masks - Used by Linux */
#define BM_READ        0x01 /*!< Pages may be read */
#define BM_WRITE    0x02 /*!< Pages may be written */
#define BM_EXEC        0x04 /*!< Pages may be executed */
#define BM_NONE        0x08 /*!< Pages may not be accessed */
#define BM_SHARED    0x10 /*!< Share this mapping */
#define BM_PRIVATE    0x20 /*!< Create a private copy-on-write mapping */
#define BM_FIXED    0x40 /*!< Do not interpret address as a hint:
                place the mapping at exactly that address. */

/******************************************************************************/
/*!
 * @brief This function maps IO physical address to OS accessible address.
 *
 *
 * @return A valid virtual address.
 * @return Null on case of failure.
 */
void *Test_PalIOMap(void *physAddr, /*!< Physical address.*/
 size_t size /*!< Size in bytes.*/
);

/******************************************************************************/
/*!
 * @brief This function maps a physical address to a virtual address.
 *
 *
 * @return A valid virtual address
 * @return Null on failure.
 */
void *Test_PalMapAddr(void *physAddr,/*!< A physical address.*/
 void *startingAddr, /*!< Preferred static address for mapping.*/
 const char *filename, /*!< File name.*/
 size_t size, /*!< Size in bytes.*/
 uint8_t protAndFlagsBitMask /*!< Protection and update visibility bit mask.*/
);

/******************************************************************************/
/*!
 * @brief This function unmaps a virtual address.
 *
 * @return
 */
void Test_PalUnmapAddr(void *virtAddr, /*!< Virtual address.*/
 size_t size /*!< Size in bytes.*/
);

#ifdef __cplusplus
}
#endif
/*!
 @}
 */
#endif /* TEST_PAL_MAP_ADDRS_H_ */
