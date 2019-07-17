/****************************************************************************
* The confidential and proprietary information contained in this file may    *
* only be used by a person authorised under and to the extent permitted      *
* by a subsisting licensing agreement from Arm Limited (or its affiliates).    *
* 	(C) COPYRIGHT [2001-2018] Arm Limited (or its affiliates).	     *
*	    ALL RIGHTS RESERVED						     *
* This entire notice must be reproduced on all copies of this file           *
* and copies of this file may only be made by a person if such person is     *
* permitted to do so under the terms of a subsisting license agreement       *
* from Arm Limited (or its affiliates).					     *
*****************************************************************************/

#ifndef _TEST_PROJ_COMMON_H_
#define _TEST_PROJ_COMMON_H_



/* Error types */
/*! Defines test proj base error. */
#define TEST_PROJ_BASE_ERROR     (0x00FFFF00)

#define TEST_OK                 (0x00000000)
#define TEST_INVALID_PARAM_ERR  (TEST_PROJ_BASE_ERROR + 0x00000001)
#define TEST_COMPARE_ERR        (TEST_PROJ_BASE_ERROR + 0x00000002)
#define TEST_HW_FAIL_ERR        (TEST_PROJ_BASE_ERROR + 0x00000003)
#define TEST_MAPPING_ERR        (TEST_PROJ_BASE_ERROR + 0x00000004)
#define TEST_EXECUTE_FAIL       (TEST_PROJ_BASE_ERROR + 0x00000005)
#define TEST_NOT_SUPPORTED_FIELD (TEST_PROJ_BASE_ERROR + 0x00000006)
#define TEST_MAX_THREADS        16
#define TEST_MAX_FILE_NAME      256
#define THREAD_STACK_SIZE       (128*1024) /* stack has 128KB for 64bit CPU */

#define TEST_PROJ_256BIT_KEY_SIZE_WORDS  8
#define TEST_PROJ_128BIT_KEY_SIZE_WORDS  4


#ifdef BIG__ENDIAN
#define TEST_CONVERT_BYTE_ARR_TO_WORD(inPtr, outWord) {\
    outWord = (*inPtr<<24);\
    outWord |= (*(inPtr+1)<<16);\
    outWord |= (*(inPtr+2)<<8);\
    outWord |= (*(inPtr+3));\
}
#else
#define TEST_CONVERT_BYTE_ARR_TO_WORD(inPtr, outWord) {\
    outWord = (*(inPtr+3))<<24;\
    outWord |= (*(inPtr+2))<<16;\
    outWord |= (*(inPtr+1))<<8;\
    outWord |= (*inPtr);\
}
#endif

#endif /* _TEST_PROJ_COMMON_H_ */
