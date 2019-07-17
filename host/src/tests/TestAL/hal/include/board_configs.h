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

#ifndef TEST_HAL_H_
#define TEST_HAL_H_

/*!
 @file
 @brief This file contains board initialization functions.
 */

/*!
 @addtogroup board_hal_test
 @{
 */

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************/
/*!
 * @brief This function initializes the board.
 *
 *
 * @return 0 on success.
 * @return 1 on failure.
 */
uint32_t Test_HalBoardInit(void);

/******************************************************************************/
/*!
 * @brief This function unmaps the addresses related to the board.
 *
 *
 * @return void
 */
void Test_HalBoardFree(void);

#ifdef __cplusplus
}
#endif
/*!
 @}
 */
#endif /* TEST_HAL_H_ */
