
/*


 This file contains the module definitions
 */



 /*
  ############################TOP-LEVEL APIs###################################
 */

   /*!
   @defgroup cc_gen_defs CryptoCell definitions
   @brief Contains CryptoCell definitions.
   */

      /*!
  @defgroup cc_ecpki CryptoCell ECC APIs
  @brief Contains functions and definitions for handling keys used in Elliptic Curves Cryptography (ECC).
 */

 /*!
 @defgroup cc_pal CryptoCell PAL APIs
 @brief Contains all PAL APIs and definitions.
 */


   /*!
  @defgroup ch_crypto Chinese certification cryptographic APIs and definitions
  @brief Contains Chinese certification cryptographic APIs and definitions
  */

  /*!
  @defgroup cc_sm2 SM2 APIs
  @brief Contains SM2 APIs and definitions
  */

  /*!
  @defgroup cc_sm3 SM3 APIs
  @brief Contains SM3 APIs and definitions
  */

  /*!
  @defgroup cc_sm4 SM4 APIs
  @brief Contains SM4  APIs and definitions
*/


 /*!
  @defgroup rng_module Random number definitions
  @brief Contains all random number definitions
 */


/*!
  @defgroup trng_apis TRNG APIs
  @brief Contains TRNG APIs
 */




  /*
  ############################true-random-number and TRNG APIs###################################
 */

/*!
 @defgroup cc_rnd CryptoCell random-number generation definitions.
 @brief Contains the CryptoCell random-number generation definitions.
 @ingroup rng_module
 */

 /*!
 @defgroup cc_rnd_defines CryptoCell true-random-number generation definitions.
 @brief Contains the CryptoCell true-random-number generation defines.
 @ingroup rng_module
 */

 /*!
 @defgroup cc_rnd_error CryptoCell random-number specific errors
 @brief Contains the definitions of the CryptoCell RND errors.
 @ingroup rng_module
*/

/*!
 @defgroup cc_trng_error CryptoCell TRNG specific errors
 @brief Contains the definitions of the CryptoCell TRNG errors.
 @ingroup trng_apis
*/

 /*!
 @defgroup trng_fe_apis TRNG API definition
  @brief Contains API and definitions for generating TRNG buffer in full entropy mode.
  @ingroup trng_apis
 */

  /*
  ############################chinese crypto APIs###################################
 */

  /*!
 @defgroup ch_cert_defs Chinese certification cryptographic definitions
 @brief Contains definitions and APIs that are used in the CryptoCell Chinese Certification module.
 @ingroup ch_crypto
*/

/*!
 @defgroup ch_cert_errors Chinese certification errors
 @brief Contains Chinese certification error definitions
 @ingroup ch_crypto
 */

 /*!
 @defgroup cc_sm3_defs CryptoCell SM3 type definitions
 @brief Contains CryptoCell SM3 type definitions.
 @ingroup cc_sm3
*/

/*!
@defgroup cc_sm3_error CryptoCell SM3 specific errors
@brief Contains the definitions of the CryptoCell SM3 errors.
@ingroup cc_sm3
*/

/*!
 @defgroup cc_sm4_defs CryptoCell SM4 type definitions
 @brief Contains CryptoCell SM4 type definitions.
 @ingroup cc_sm4
 */

 /*!
 @defgroup cc_sm4_error CryptoCell SM4 specific errors
 @brief Contains the definitions of the CryptoCell SM4 errors.
 @ingroup cc_sm4
*/

 /*
 ################################## AES APIs #######################################
  */
  /*!
 @defgroup cc_aes_defs CryptoCell AES type definitions
 @brief Contains CryptoCell AES type definitions.
 @ingroup cc_gen_defs

 */


 /*
 ################################### CryptoCell macros ################################
 */

 /*!
  @defgroup bit_field_apis bit-field operations macros
  @brief Contains bit-field operation macros
  @ingroup cc_gen_defs
  */

  /*!
 @defgroup cc_error General base error codes for CryptoCell
 @brief Contains general base-error codes for CryptoCell.
 @ingroup cc_gen_defs
 */

 /*!
  @defgroup cc_cert_defs CryptoCell general certification definitions
  @ingroup cc_gen_defs
  */

 /*!
 @defgroup cc_hash_defs CryptoCell hash type definitions
 @brief Contains CryptoCell hash type definitions.
 @ingroup cc_gen_defs
*/

  /*!
   @defgroup cc_defs  CryptoCell library enums and definitions
   @brief Cntains all the enums and definitions that are used for the
        CryptoCell library initialization and terminate APIs, as well as the APIs themselves.
   @ingroup cc_gen_defs
   */

  /*!
  @defgroup cc_pka_defs PKA enums and definitions
  @brief Contains all the enums and definitions that are used in the PKA related code.
  @ingroup cc_gen_defs
  */

 /*!
  @defgroup cc_regs CryptoCell register APIs
  @brief Contains macro definitions for accessing Arm CryptoCell's registers.
  @ingroup cc_gen_defs
  */

  /*!
 @defgroup cc_utils_errors Specific errors of the CryptoCell utility module APIs
 @brief Contains utility API error definitions.
 @ingroup cc_gen_defs
 */

/*
############################################## ECC APIs ############################
*/

 /*!
 @defgroup cc_ecpki_error CryptoCell ECC specific errors
 @brief Contains errors that are specific to ECC.
 @ingroup cc_ecpki
*/

/*!
 @defgroup cc_ecpki_types CryptoCell ECPKI type definitions
 @brief Contains CryptoCell ECPKI type definitions.
 @ingroup cc_ecpki
 */

 /*!
 @defgroup cc_ecpki_kg CryptoCell APIs for generation of ECC private and public keys
 @brief Contains CryptoCell APIs for generation of ECC private and public keys.
 @ingroup cc_ecpki
*/
/*
#################################################### PAL APIs ##############################
*/

 /*!
 @defgroup cc_pal_abort CryptoCell PAL abort operations
 @brief Contains CryptoCell PAL abort operations.
 @ingroup cc_pal
 */

  /*!
 @defgroup cc_pal_barrier CryptoCell PAL memory Barrier APIs
 @brief Contains memory-barrier implementation definitions and APIs.
 @ingroup cc_pal
*/

/*!
 @defgroup cc_pal_cert CERT definitions
 @brief Contains definitions that are used by the CERT related APIs. The implementation of these functions
need to be replaced according to the Platform and TEE_OS.
 @ingroup cc_pal
 */

 /*!
 @defgroup cc_pal_compiler CryptoCell PAL platform-dependent compiler-specific definitions
 @brief Contains CryptoCell PAL platform-dependent compiler-related definitions.
 @ingroup cc_pal
 */

 /*!
 @defgroup ssi_pal_dma CryptoCell PAL DMA related APIs
 @brief Contains definitions that are used for DMA-related APIs.
 @ingroup cc_pal
 */

 /*!
 @defgroup cc_pal_error Specific errors of the CryptoCell PAL APIs
 @brief Contains platform-dependent PAL-API error definitions.
 @ingroup cc_pal
 */
 /*!
 @defgroup cc_pal_init CryptoCell PAL entry or exit point APIs
 @brief Contains PAL initialization and termination APIs.
 @ingroup cc_pal
 */

 /*!
 @defgroup cc_pal_log CryptoCell PAL logging APIs and definitions
 @brief Contains CryptoCell PAL layer log definitions.
 @ingroup cc_pal
 */

 /*!
 @defgroup cc_pal_mem CryptoCell PAL memory operations
 @brief Contains memory-operation functions.
 @ingroup cc_pal
 */

 /*!
 @defgroup cc_pal_memmap CryptoCell PAL memory mapping APIs
 @brief Contains memory mapping functions.
 @ingroup cc_pal
 */

 /*!
 @defgroup cc_pal_mutex CryptoCell PAL mutex APIs
 @brief Contains resource management functions.
 @ingroup cc_pal
 */

 /*!
 @defgroup cc_pal_pm CryptoCell PAL power-management APIs
 @brief Contains PAL power-management APIs.
 @ingroup cc_pal
 */

 /*!
 @defgroup cc_pal_trng CryptoCell PAL TRNG APIs
 @brief Contains APIs for retrieving TRNG user parameters.
 @ingroup cc_pal
 */

 /*!
 @defgroup cc_pal_types CryptoCell platform-dependent PAL layer definitions and types
  @brief Contains platform-dependent definitions and types of the PAL layer.
 @ingroup cc_pal
*/