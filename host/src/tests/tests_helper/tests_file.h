/*******************************************************************************
* The confidential and proprietary information contained in this file may      *
* only be used by a person authorised under and to the extent permitted        *
* by a subsisting licensing agreement from Arm Limited (or its affiliates).    *
*   (C) COPYRIGHT [2001-2018] Arm Limited (or its affiliates).                 *
*       ALL RIGHTS RESERVED                                                    *
* This entire notice must be reproduced on all copies of this file             *
* and copies of this file may only be made by a person if such person is       *
* permitted to do so under the terms of a subsisting license agreement         *
* from Arm Limited (or its affiliates).                                        *
*******************************************************************************/


#ifndef TESTS_FILE_H_
#define TESTS_FILE_H_

#ifdef __cplusplus
extern "C" {
#endif


/******************************************************************************/
/*
 * @brief This function writes data to a binary file.
 * @param[in]
 * data_fname - File name.
 *
 * @param[in]
 * inBuff - A pointer to the data buffer.
 *
 * @param[out]
 * inBuffLen - data length.
 *
 * @return - int
 */
int Tests_CopyDataToBinFile (unsigned char *fileName, unsigned char *inBuff, unsigned int inBuffLen);

/******************************************************************************/
/*
 * @brief This function reads data from a binary file.
 * @param[in]
 * data_fname - File name.
 *
 * @param[in]
 * outBuff - A pointer to the data buffer.
 *
 * @param[out]
 * outBuffLen - data length.
 *
 * @return - int
 */
int Tests_CopyDataFromBinFile (unsigned char *fileName, unsigned char *outBuff, unsigned int *outBuffLen);

int Tests_BinFileSize(char *fileName, size_t *outBuffLen);

int Tests_CopyDataFromTextCommaSepFile (unsigned char *fileName, unsigned char *outBuff, unsigned int *outBuffLen);



#ifdef __cplusplus
}
#endif

#endif /* TESTS_FILE_H_ */
