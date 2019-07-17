/****************************************************************************
* The confidential and proprietary information contained in this file may    *
* only be used by a person authorised under and to the extent permitted      *
* by a subsisting licensing agreement from ARM Limited or its affiliates.    *
* 	(C) COPYRIGHT [2001-2018] ARM Limited or its affiliates.	     *
*	    ALL RIGHTS RESERVED						     *
* This entire notice must be reproduced on all copies of this file           *
* and copies of this file may only be made by a person if such person is     *
* permitted to do so under the terms of a subsisting license agreement       *
* from ARM Limited or its affiliates.					     *
*****************************************************************************/

#ifndef _TESTS_MEMORY_H_
#define _TESTS_MEMORY_H_

/**
 * The function copies src buffer to dst with reversed bytes order.
 *
 * Note: Overlapping is not allowed, besides reversing of the buffer in place.
 *
 * @param dst
 * @param src
 * @param sizeInBytes
 */
int Tests_MemCpyReversed( void* dst_ptr, void* src_ptr, unsigned int sizeInBytes);

#endif /* _TESTS_MEMORY_H_ */

