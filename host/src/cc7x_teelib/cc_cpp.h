/*
 * Copyright (c) 2001-2019, Arm Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause OR Arm’s non-OSI source license
 *
 */

/*!
@file
@brief This file contains all the enums and definitions that are used for the
        CryptoCell CPP APIs, as well as the APIs themselves.

        The TEE side support for CPP includes the "control" APIs that will allow
        the following functionality:
        1. Configure the watchdog (enable/disable and setting the timer)
        2. Set the in and out sMMU stream ID to be used by the operation
        3. Set watchdog timer

        TBD: Register and unregister a callback to be called on when the HW
        indicates the TEE needs to accept or reject an REE CPP key slot usage.

        And "flow" API that will allow to implement "policy" reference
        application:
        1. Get the parameters of the requested operation (key index, algorithm,
           operation, mode and data size), verifying them and reloading the
           watchdog timer.
        2. Get anciliry parameters about the requested operations: IV or CTR,
           depending on the requested mode of operation.
        3. Get an array of the bus addresses and sizes of the buffers involved
           in the operation.
*/

#ifndef _CC_CPP_H_
#define _CC_CPP_H_

#include "cc_pal_types.h"
#include "cc_address_defs.h"
#include "cc_lli_defs.h"

/*!< 128 bits key length equal to 4 words */
#define CC_128_BIT_KEY_SIZE_IN_WORDS    4
/*!< 256 bits key length equal to 8 words */
#define CC_256_BIT_KEY_SIZE_IN_WORDS    8
/*!< 128 bits key length equal to 16 bytes */
#define CC_128_BIT_KEY_SIZE_IN_BYTES    16
/*!< 256 bits key length equal to 32 bytes */
#define CC_256_BIT_KEY_SIZE_IN_BYTES    32

/*!< This error is returned when Secure Disable control is set */
#define CC_CPP_SD_ENABLED_ERROR         (CC_CPP_MODULE_ERROR_BASE + 0)
/*!< This error is returned when one pf function parameters is wrong*/
#define CC_CPP_WRONG_PARAMETERS_ERROR   (CC_CPP_MODULE_ERROR_BASE + 1)
/*!< This error is returned when watchdog is expired*/
#define CC_CPP_EXPIRED_WATCHDOG_ERROR   (CC_CPP_MODULE_ERROR_BASE + 2)
/*!< This error is returned when required operation is not supported*/
#define CC_CPP_NOT_SUPPORTED_OP_ERROR   (CC_CPP_MODULE_ERROR_BASE + 3)


/*!
@brief The structure is used to report back buffers used by CPP operations
*/
typedef struct CCCppBuffer_t {
    CCDmaAddr_t bus_addr;   /*!< buffer address */
    size_t      size;       /*!< buffer size*/
}CCCppBuffer_t;

/*!
@brief The structure is used to report back buffers used by CPP operations
*/
typedef struct CCCppBufInfo_t {
    CCCppBuffer_t buffers[LLI_MAX_NUM_OF_ENTRIES];  /*!< buffer parameters */
    uint32_t numberOfEntries;                       /*!< number of entries */
}CCCppBufInfo_t;

/*!
@brief The structure is used to report CPP operation parameters
*/
typedef enum CCCppOp_t {
     CC_CPP_ENCRYPT_OP = 0, /*!< encrypt operation */
     CC_CPP_DECRYPT_OP = 1  /*!< decrypt operation */
}CCCppOp_t;

/*!
@brief The structure is used to report CPP operation parameters
*/
typedef enum CCCppMode_t {
    CC_CPP_CBC_MODE = 1,    /*!< CBC mode */
    CC_CPP_CTR_MODE = 2,    /*!< CTR mode */
    CC_CPP_NOT_SUPPORTED_MODE,
}CCCppMode_t;

/*!
@brief The structure is used to report CPP key size parameters
*/
typedef enum CCCppKeySize_t {
    CC_CPP_KEY_SIZE_128 = 0,    /*!< 128 bit key */
    CC_CPP_KEY_SIZE_256 = 2,    /*!< 256 bit key */
    CC_CPP_NOT_SUPPORTED_KEY_SIZE,
}CCCppKeySize_t;


/*!
@brief The structure is used to report CPP operation parameters
*/
typedef enum CCCppEngine_t {
     CC_CPP_AES_ENGINE = 0, /*!< AES engine */
     CC_CPP_SM4_ENGINE = 1  /*!< SM4 engine */
}CCCppEngine_t;

/*!
@brief The structure is used to report CPP operation parameters
*/
typedef struct CCCppOpParams_t {
     uint32_t       dataSize;   /*!< data size */
     uint8_t        keySlot;    /*!< key slot  */
     CCCppKeySize_t keySize;    /*!< key size */
     CCCppMode_t    mode;       /*!< mode CTR/CBC */
     CCCppEngine_t  engine;     /*!< engine AES/SM4 */
     CCCppOp_t      direction;  /*!< direction encrypt/decrypt */
     union {
         /*!< key by bytes */
         unsigned char iv[CC_128_BIT_KEY_SIZE_IN_BYTES];
         /*!< key by words */
         uint32_t iv_data[CC_128_BIT_KEY_SIZE_IN_WORDS];
     }ivData;
}CCCppOpParams_t;

/*!
@brief The typedef of cpp event function ("policy")
*/
typedef void (*CCCppEventFunction)(void*);

/*!
@brief This function sets stream ID to apropriate register.

@return CC_OK on success.
@return A non-zero value in case of failure.
*/
CCError_t CC_CppStreamIdSet(
        uint16_t read_stream_id, /*!< [in]  read stream Id value */
        uint16_t write_stream_id /*!< [in]  read stream Id value */ );

/*!
@brief This function sets watchdog.

@return CC_OK on success.
@return A non-zero value in case of failure.
*/
CCError_t CC_CppWatchdogSet(
        CCBool enable,        /*!< [in]  enable or disable watchdog */
        uint32_t cycles       /*!< [in]  watchdog cycles number     */  );

/*!
@brief This function sSet CPP Operation Key

@return CC_OK on success.
@return A non-zero value in case of failure.
*/
CCError_t CC_CppKeySet(
        CCCppEngine_t  engine,   /*!< [in]  cpp engine type */
        CCCppKeySize_t keySize, /*!< [in]  cpp key size  */
        uint8_t *pKey           /*!< [in]  cpp key value  */);
/*!
@brief This function parses descriptor loaded to register by HW and
       fill bufInfoIn and bufInfoOut structs

@return CC_OK on success.
@return A non-zero value in case of failure.
*/
CCError_t CC_CppBufInfoGet (
        CCCppBufInfo_t *bufInfoIn,/*!< [out] bufferIn data */
        CCCppBufInfo_t *bufInfoOut/*!< [out] bufferOut data */);

/*!
@brief This function handles CPP Operation

@return CC_OK on success.
@return A non-zero value in case of failure.
*/
CCError_t CC_CppHandleOp(
        CCBool accept/*!< [in] accept or reject CPP operation */
        );

/*!
@brief This function receives the currently pending CPP operation for processing

@return CC_OK on success.
@return A non-zero value in case of failure.
*/
CCError_t CC_CppRecvOp(
        CCCppOpParams_t *opParams /*!< [out] CPP operation params */
        );

/*!
@brief This function register external function ("policy") to handle CPP event

@return void
*/
void CC_CppRegisterEventRoutine (CCCppEventFunction pFunc);

#endif // _CC_CPP_H_
