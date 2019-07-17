/*******************************************************************************
* The confidential and proprietary information contained in this file may      *
* only be used by a person authorised under and to the extent permitted        *
* by a subsisting licensing agreement from ARM Limited or its affiliates.      *
*   (C) COPYRIGHT [2018] ARM Limited or its affiliates.                        *
*       ALL RIGHTS RESERVED                                                    *
* This entire notice must be reproduced on all copies of this file             *
* and copies of this file may only be made by a person if such person is       *
* permitted to do so under the terms of a subsisting license agreement         *
* from ARM Limited or its affiliates.                                          *
*******************************************************************************/

/**
 * The function copies src buffer to dst with reversed bytes order.
 *
 * Note: Overlapping is not allowed, besides reversing of the buffer in place.
 *
 * @param dst
 * @param src
 * @param sizeInBytes
 */
int Tests_MemCpyReversed( void* dst_ptr, void* src_ptr, unsigned int sizeInBytes)
{
	unsigned int i;
	unsigned char *dst, *src;

	src = (unsigned char *)src_ptr;
	dst = (unsigned char *)dst_ptr;

	if (((dst < src) && (dst+sizeInBytes > src)) ||
	    ((src < dst) && (src+sizeInBytes > dst)))
		return -1;

	if (dst == src) {
		unsigned char tmp;
		for (i=0; i<sizeInBytes/2; i++) {
			tmp = dst[sizeInBytes-i-1];
			dst[sizeInBytes-i-1] = src[i];
			src[i] = tmp;
		}
	} else {
		for (i=0; i<sizeInBytes; i++) {
			dst[i] = src[sizeInBytes-i-1];
		}
	}

	return 0;
}
