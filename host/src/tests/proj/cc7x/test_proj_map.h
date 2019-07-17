/****************************************************************************
 * The confidential and proprietary information contained in this file may    *
 * only be used by a person authorised under and to the extent permitted      *
 * by a subsisting licensing agreement from Arm Limited (or its affiliates).  *
 *   (C) COPYRIGHT [2001-2018] Arm Limited (or its affiliates).                    *
 *       ALL RIGHTS RESERVED                                                  *
 * This entire notice must be reproduced on all copies of this file           *
 * and copies of this file may only be made by a person if such person is     *
 * permitted to do so under the terms of a subsisting license agreement       *
 * from Arm Limited (or its affiliates).                                      *
*****************************************************************************/

#ifndef _TEST_PROJ_MAP_H_
#define _TEST_PROJ_MAP_H_

/****************************************************************************/
/*   							External API  								*/
/*
 * @brief This function Maps the proj HW.
 *  map the REE CC HW base, to write to some registers to enable TEE work
 *
 *
 * @param[in]
 *
 * @param[out]
 *
 * @return rc - 0 for success, 1 for failure.
 */
int Test_ProjReeMap(void);

/****************************************************************************/
/*
 * @brief This function unmaps the proj HW.
 *
 * @param[in]
 *
 * @param[out]
 *
 * @return none.
 */
void Test_ProjReeUnmap(void);

/****************************************************************************/
/*
 * @brief This function Maps the proj HW.
 *
 * @param[in]
 *
 * @param[out]
 *
 * @return rc - 0 for success, 1 for failure.
 */
int Test_ProjTeeMap(void);

/****************************************************************************/
/*
 * @brief This function unmaps the proj HW.
 *
 * @param[in]
 *
 * @param[out]
 *
 * @return none.
 */
void Test_ProjTeeUnmap(void);



#endif /* _TEST_PROJ_MAP_H_ */
