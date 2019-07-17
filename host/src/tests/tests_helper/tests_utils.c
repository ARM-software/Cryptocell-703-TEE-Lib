/****************************************************************************
 * The confidential and proprietary information contained in this file may   *
 * only be used by a person authorised under and to the extent permitted     *
 * by a subsisting licensing agreement from Arm Limited (or its affiliates). *
 *     (C) COPYRIGHT [2018] Arm Limited (or its affiliates).                 *
 *         ALL RIGHTS RESERVED                                               *
 * This entire notice must be reproduced on all copies of this file          *
 * and copies of this file may only be made by a person if such person is    *
 * permitted to do so under the terms of a subsisting license agreement      *
 * from Arm Limited (or its affiliates).                                     *
 *****************************************************************************/
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

/**
 * Tests_InitRand() - Init the random seed for using rand()
 * This function should be called at least once before calls to Tests_FillRandBuf().
 */
void Tests_InitRand(void){
    srand(time(NULL));
}

/**
 * Tests_FillRandBuf() - Fill buffer with random bytes values
 *
 * @buf_p:          The buffer to fill
 * @buf_size:       The size in bytes to fill
 */
void Tests_FillRandBuf(uint8_t *buf_p, uint32_t buf_size)
{
    uint8_t cur_val = 0;
    uint32_t i = 0;

    for(i = 0; i < buf_size; i++){
        cur_val = rand() % 256;
        buf_p[i] = cur_val;
    }
}


/**
 * Tests_FillValueBuf() - Fill buffer with random bytes values
 *
 * @buf_p:          The buffer to fill
 * @buf_size:       The size in bytes to fill
 * @value:          The value to copy to each byte of the buffer
 */
void Tests_FillValueBuf(uint8_t *buf_p, uint32_t buf_size, uint8_t value)
{
    uint8_t cur_val = 0;
    uint32_t i = 0;

    for(i = 0; i < buf_size; i++){
        cur_val = value;
        buf_p[i] = cur_val;
    }
}


