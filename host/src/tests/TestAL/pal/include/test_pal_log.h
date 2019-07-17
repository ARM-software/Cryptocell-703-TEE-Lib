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

#ifndef TEST_PAL_LOG_H_
#define TEST_PAL_LOG_H_

/*!
 @file
 @brief This file contains log APIs used by tests.
 In order to print the name of the function, while implementing
 a generic declaration and an OS-dependent definition, we use a macro that
 calls a PAL function with __FUNCTION__ as an argument.
 */

/*!
 @addtogroup pal_log_test
 @{
 */

#include <stdint.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************/
/*!
 * @brief This variadic function prints to stderr with or without the name of
 * the calling function.
 * When function is NULL, the name of the function is not printed.
 *
 *
 * @return void
 */
void Test_PalPrintfError(
 const char *function, /*<! The name of the calling function (can be NULL).*/
 const char *format, /*<! The printed string.*/
 ... /*<! Arguments.*/
);

/******************************************************************************/
/*!
 * @brief This variadic function prints to stderr and to a file with or without
 * the name of the calling function.
 * When function is NULL, the name of the function is not printed.
 *
 *
 * @return void
 */
void Test_PalFprintfError(void *fd, /*<! File descriptor.*/
 const char *function, /*<! The name of the calling function (can be NULL).*/
 const char *format, /*<! The printed string.*/
 ... /*<! Arguments.*/
);

/******************************************************************************/
/*!
 * @brief This variadic function prints messages to stderr.
 *
 *
 * @return void
 */
void Test_PalPrintfMessage(const char *format, /*<! The printed string.*/
 ... /*<! Arguments.*/
);

/******************************************************************************/
/*!
 * @brief This variadic function prints strings to stdout with or without
 * the name of the calling function.
 * When function is NULL, the name of the function is not printed.
 *
 *
 * @return void
 */
void Test_PalPrintf(
 const char *function, /*<! The name of the calling function (can be NULL).*/
 const char *format, /*<! The printed string.*/
 ... /*<! Arguments.*/
);

/******************************************************************************/
/*!
 * @brief  This variadic function prints strings to stdout and to a file
 * with or without the name of the calling function.
 * When function is NULL, the name of the function is not printed.
 *
 *
 * @return void
 */
void Test_PalFprintf(void *fd, /*<! File descriptor.*/
 const char *function, /*<! The name of the calling function (can be NULL).*/
 const char *format, /*<! The printed string.*/
 ... /*<! Arguments.*/
);

/******************************************************************************/
/*!
 * @brief This variadic function prints bytes in a buffer to stdout
 * with or without the name of the calling function.
 * When function is NULL, the name of the function is not printed.
 *
 *
 * @return void
 */
void Test_PalPrintByteBuff(
 const char *function, /*<! The name of the calling function (can be NULL).*/
 const char *buffName, /*<! The name of the buffer.*/
 uint8_t *buff, /*<! Buffer address.*/
 uint32_t size /*<! The size of the buffer.*/
);

/******************************************************************************/
/*!
 * @brief This variadic function prints bytes in a buffer to a file
 * with or without the name of the calling function.
 * When function is NULL, the name of the function is not printed.
 *
 *
 * @return void
 */
void Test_PalFprintByteBuff(void *fd, /*<! File descriptor.*/
 const char *function, /*<! The name of the calling function (can be NULL).*/
 const char *buffName, /*<! The name of the buffer.*/
 uint8_t *buff, /*<! Buffer address.*/
 uint32_t size /*<! The size of the buffer.*/
);

/******************************************************************************/
/*!
 * @brief This variadic function prints bytes in a buffer to a file
 * with or without the name of the calling function.
 * When function is NULL, the name of the function is not printed.
 *
 *
 * @return void
 */
void Test_PalFprintfByteBuffMax(void *fd, /*<! File descriptor.*/
 const char *function, /*<! The name of the calling function (can be NULL).*/
 const char *buffName, /*<! The name of the buffer.*/
 uint8_t *buff, /*<! Buffer address.*/
 uint32_t size, /*<! The size of the buffer.*/
 uint32_t maxSize /*<! Maximum size to print.*/
);

/******************************************************************************/
/*!
 * @brief This variadic function prints words in a buffer with or without the
 * name of the calling function.
 * When function is NULL, the name of the function is not printed (can be NULL).
 *
 *
 * @return void
 */
void Test_PalPrintWordBuff(
 const char *function, /*<! The name of the calling function.*/
 const char *buffName, /*<! The name of the buffer.*/
 uint32_t *buff, /*<! Buffer address.*/
 uint32_t size /*<! The size of the buffer.*/
);

/******************************************************************************/
/*!
 * @brief  This macro prints errors with the name of the calling function.
 *
 *
 * @return void
 */
#define TEST_PRINTF_ERROR(format, /*<! The printed string.*/ \
 ... /*<! Arguments.*/)  {\
 Test_PalPrintfError(__FUNCTION__, format, ##__VA_ARGS__);\
}

/******************************************************************************/
/*!
 * @brief  This macro prints to a file with the name of the calling function.
 *
 *
 * @return  void
 */
#define TEST_FPRINTF_ERROR(fd, /*<! File descriptor.*/ \
 format, /*<! The printed string.*/ \
 ... /*<! Arguments.*/)  {\
    Test_PalFprintfError(fd, __FUNCTION__, format, ##__VA_ARGS__);\
}

/******************************************************************************/
/*!
 * @brief This macro prints messages.
 *
 *
 * @return void
 */
#define TEST_PRINTF_MESSAGE(format, /*<! The printed string.*/ \
 ... /*<! Arguments.*/)  {\
 Test_PalPrintfMessage(format, ##__VA_ARGS__);\
}

#ifdef TEST_DEBUG

/******************************************************************************/
/*!
 * @brief This macro prints strings with the name of the calling function.
 *
 *
 * @return void
 */
#define TEST_PRINTF(format, /*<! The printed string.*/ \
 ... /*<! Arguments.*/)  {\
 Test_PalPrintf(__FUNCTION__, format, ##__VA_ARGS__);\
 Test_PalPrintf(NULL, "\n");\
}

/******************************************************************************/
/*!
 * @brief This macro prints strings to a file with the name of the calling
 * function.
 *
 *
 * @return void
 */
#define TEST_FPRINTF(fd, /*<! File descriptor.*/ \
 format, /*<! The printed string.*/ \
 ... /*<! Arguments.*/)  {\
    Test_PalFprintf(fd, __FUNCTION__, format, ##__VA_ARGS__);\
    Test_PalFprintf(fd, NULL, "\n");\
}

/******************************************************************************/
/*!
 * @brief This macro prints strings without the name of the calling function.
 *
 *
 * @return void
 */
#define TEST_PRINTF_NO_FUNC(format, /*<! The printed string.*/ \
 ... /*<! Arguments.*/)  {\
    Test_PalPrintf(NULL, format, ##__VA_ARGS__);\
    Test_PalPrintf(NULL, "\n");\
}

/******************************************************************************/
/*!
 * @brief This macro prints strings to a file without the name of the calling
 * function.
 *
 *
 * @return void
 */
#define TEST_FPRINTF_NO_FUNC(fd, /*<! File descriptor.*/ \
 format, /*<! The printed string.*/ \
 ... /*<! Arguments.*/)  {\
    Test_PalFprintf(fd, NULL, format, ##__VA_ARGS__);\
    Test_PalFprintf(fd, NULL, "\n");\
}

/******************************************************************************/
/*!
 * @brief This macro prints bytes in a buffer with the name of the calling
 * function.
 *
 *
 * @return void
 */
#define TEST_PRINT_BYTE_BUFF(buffName, /*<! The name of the buffer.*/ \
 buff, /*<! Buffer address.*/ \
 size /*<! The size of the buffer.*/)  {\
    Test_PalPrintByteBuff(__FUNCTION__, buffName, buff, size);\
}

/******************************************************************************/
/*!
 * @brief This macro prints bytes in a buffer to a file with
 * the name of the calling function.
 *
 *
 * @return void
 */
#define TEST_FPRINT_BYTE_BUFF(fd, /*<! File descriptor.*/ \
 buffName, /*<! The name of the buffer.*/ \
 buff, /*<! Buffer address.*/ \
 size /*<! The size of the buffer.*/)  {\
    Test_PalFprintByteBuff(fd, __FUNCTION__, buffName, buff, size);\
}

/******************************************************************************/
/*!
 * @brief This macro prints bytes in a buffer to a file with the name of the
 * calling function.
 *
 *
 * @return void
 */
#define TEST_FPRINT_BYTE_BUFF_MAX(fd, /*<! File descriptor.*/ \
 buffName, /*<! The name of the buffer.*/ \
 buff, /*<! Buffer address.*/ \
 size, /*<! The size of the buffer.*/ \
 maxSize /*<! Maximum size to print.*/) {\
    Test_PalFprintfByteBuffMax(fd, __FUNCTION__, buffName, buff, size,\
                maxSize);\
}

/******************************************************************************/
/*!
 * @brief This macro prints words in a buffer with the name of the calling
 * function.
 *
 *
 * @return  void
 */
#define TEST_PRINT_WORD_BUFF(buffName, /*<! The name of the buffer.*/ \
 buff, /*<! Buffer address.*/ \
 size /*<! The size of the buffer.*/) {\
    Test_PalPrintWordBuff(__FUNCTION__, buffName, buff, size);\
}

/* void TEST_PRINT_BYTE_BUFFP(buffName, buff, size);
   void TEST_FPRINT_LONG_NUM(const char *fd, const char *buffName,
   uint32_t *buff, uint32_t size); */
#else
#define TEST_PRINTF(format, ...) do { } while (0)
#define TEST_FPRINTF(fd, format, ...) do { } while (0)
#define TEST_PRINTF_NO_FUNC(format, ...) do { } while (0)
#define TEST_FPRINTF_NO_FUNC(fd, format, ...) do { } while (0)
#define TEST_PRINT_BYTE_BUFF(buffName, buff, size) do { } while (0)
#define TEST_FPRINT_BYTE_BUFF(fd, buffName, buff, size) do { } while (0)
#define TEST_FPRINT_BYTE_BUFF_MAX(fd, buffName, buff, size, maxSize) do { } while (0)
#define TEST_PRINT_WORD_BUFF(buffName, buff, size) do { } while (0)
#endif

#ifdef __cplusplus
}
#endif
/*!
 @}
 */
#endif /* TEST_PAL_LOG_H_ */
