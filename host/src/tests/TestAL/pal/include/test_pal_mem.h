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

#ifndef TEST_PAL_MEM_H_
#define TEST_PAL_MEM_H_

/*!
 @file
 @brief This file contains PAL memory integration tests.
 */

/*!
 @addtogroup pal_memory_test
 @{
 */

#include <stdint.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************/
/*!
 * @brief This function allocates "size" bytes.
 * When TZM is supported, it is used only for NON SECURE memory allocations.
 *
 *
 * @return Pointer to the allocated memory.
 * @return NULL on failure.
 */
void *Test_PalMalloc(size_t size /*!< Size in bytes.*/
);

/******************************************************************************/
/*!
 * @brief This function frees allocated memory pointed by pvAddress.
 * When TZM is supported, it is used only for NON SECURE memory blocks.
 *
 * @return void
 */
void Test_PalFree(void *pvAddress /*!< Pointer to the allocated memory.*/
);

/******************************************************************************/
/*!
 * @brief This function changes the size of the memory block pointed by
 * pvAddress.
 * If the function fails to allocate the requested block of memory:
 * <ul><li> A null pointer is returned.</li>
 * <li>The memory block pointed by argument pvAddress is NOT
 * deallocated.</li></ul>
 * When TZM is supported, it is used only for NON SECURE memory blocks.
 *
 *
 * @return A pointer to the new allocated memory on success.
 * @return NULL on failure.
 */
void *Test_PalRealloc(void *pvAddress, /*!< Pointer to the allocated memory.*/
 size_t newSize /*!< New size.*/
);

/******************************************************************************/
/*!
 * @brief  This function allocates a DMA-contiguous buffer and returns its
 * address.
 * When TZM is supported, it is used only for NON SECURE buffer allocations.
 *
 *
 * @return Address of the allocated buffer.
 * @return NULL on failure.
 */
void *Test_PalDMAContigBufferAlloc(size_t size /*!< Buffer size in bytes.*/
);

/******************************************************************************/
/*!
 * @brief This function frees resources previously allocated by
 * Test_PalDMAContigBufferAlloc.
 * When TZM is supported, it is used only for NON SECURE buffers.
 *
 *
 * @return void
 */
void Test_PalDMAContigBufferFree(
 void *pvAddress /*!< Address of the allocated buffer.*/
);

/******************************************************************************/
/*!
 * @brief This function changes the size of the memory block pointed by
 * pvAddress.
 * If the function fails to allocate the requested block of memory:
 * <ul>
 * <li> A null pointer is returned.</li>
 * <li> The memory block pointed by argument \c pvAddress
 * is NOT deallocated.</li></ul>
 * When TZM is supported, it is used only for NON SECURE buffers.
 *
 *
 * @return A pointer to the new allocated memory.
 */
void *Test_PalDMAContigBufferRealloc(
 void *pvAddress, /*!< Pointer to the allocated memory.*/
 size_t newSize /*!<  New size in bytes.*/
);

/******************************************************************************/
/*!
 * @brief This function returns DMA base address, i.e. the start address
 * of the DMA region.
 * When TZM is supported, it returns the NON SECURE DMA base address.
 *
 *
 * @return DMA base address
 */
unsigned long Test_PalGetDMABaseAddr(void);

/******************************************************************************/
/*!
 * @brief This function returns the unmanaged base address.
 * When TZM is supported, it returns the NON SECURE unmanaged base address.
 *
 * @return Unmanaged base address.
 */
unsigned long Test_PalGetUnmanagedBaseAddr(void);

/******************************************************************************/
/*!
 * @brief This function initializes DMA memory management.
 * When TZM is supported, it initializes the NON SECURE DMA memory management.
 *
 *
 *
 * @return 0 on success
 * @return 1 on failure.
 */
uint32_t Test_PalMemInit(
 unsigned long newDMABaseAddr, /*!< New DMA start address.*/
 unsigned long newUnmanagedBaseAddr, /*!< New unmanaged start address.*/
 size_t DMAsize /*!< DMA region size.*/
);

/******************************************************************************/
/*!
 * @brief This function sets this driver to its initial state.
 * When TZM is supported, it sets the NON SECURE management to its initial
 * state.
 *
 *
 * @return 0 on success
 * @return 1 on failure.
 */
uint32_t Test_PalMemFin(void);

#ifdef __cplusplus
}
#endif
/*!
 @}
 */
#endif /* TEST_PAL_MEM_H_ */
