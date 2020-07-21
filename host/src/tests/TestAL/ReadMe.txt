********** Official Versions **********
TestAL v3.04:
	- Removed unnecessary variable PAL_BIN.
	- Added CI support.
	- test_pal_thread: Fixed bug by adding a new field to ThreadStr, isCompleted.
	- Updated Makfile to use LITE=1 variable.
	- Added new tests for TestAL APIs.
	- Tests: Added compatibility to more configurations.
	- New configuration (#13): no_os on Juno.

TestAL v3.03:
	- No-OS board addresses: Changed all addresses to be 4-bytes aligned.
	- doxygen: Improved comments and functions descriptions.
	- Checkpatch: added a script to run checkpatch on all TestAL files.
	- test_pal_thread.c: bug fix, added a new field to ThreadStr.
	- doxygen: Additional improvements in comments and functions descriptions.
	- TestAL-Lite: Remove scripts directory when switching to TestAL-Lite.

TestAL v3.02:
	- Build process: Fixed linux_ca72.ca53 static library name.
	- FreeRTOS PAL thread: Fixed Coverity issue.
	- Armclang CM33: Disabled floating point usage.

TestAL v3.01:
	- Makefile: Removed cflag for suppressing license warnings.
	- Makefile: Fixed armclang flags towards the addition of CM3 support.
	- CM3 armclang: Added new configuration (#12).
	- Makefile: Deleted --md/-MD cflag (dependency files creation).

TestAL v3.00:
	- Build process: New build process which uses cfg files ("setconfig").
	- Build process: Updated build_config to use the new build process.
	- ReadMe: Updated ReadMe file with the new build instructions.
	- TestAL: Added armclang support.
	- Compiler path: Changed COMPILER_TYPE_NAME to COMPILER_TYPE_DIR (as in freertos).
	- Makefile: Exit setconfig rule in case config file does not exist.
	- Configurations: Removed deprecated configurations #2 and #5.

TestAL v2.09:
	- test_pal_thread: Allocates stack according to the caller's security attribute.
	- Juno: Adjusted dma base address and size.
	- Zynq: Adjusted hal dma area size.
	- FreeRTOS: Fixed cmse_is_nsfptr argument.
	- FreeRTOS: thread: Verifies s/ns allocation is performed from s/ns context.
	- Test_PalThreadJoin: Receiving (void **ret) instead of (void *ret).

TestAL v2.07:
	- cortex-a72.cortex-a53 aarch64-br-7.3: Added new configuration (#10).
	- cortex-a72.cortex-a53 aarch64-br-7.3: Added configuration #10 to TestAL release.

TestAL v2.06:
	- ARMCM3 GCC: Added new configuration (#9).
	- ARMCM3 GCC: Added configuration #9 to TestAL release.

TestAL v2.05:
	- test_pal_log: Fixed bug DEBUG=1. Wrong argument assignments.

TestAL v2.04:
	- SSE_200: Fixed target architecture.
	- no_os/board_addrs: Added getter functions.
	- test_pal_thread.c: Fixed arm-none-eabi compilation warning.

TestAL v2.03:
	- HAL MPS2+: Using getter functions instead of definitions.

TestAL v2.02:
	- SSE_200: Added support of TZM Cortex-M Security Extentions (CMSE).
	- SSE_200: Added Secure/Non-Secure buffers allocations support.


********** Building Process **********
There are two ways to build the TestAL libraries:

1. USING build_config.sh

	1. Export KERNEL_DIR in case of mbedOS or FreeRTOS.
	2. Run the build_config.sh script, with or without a configuration number.
	3. The static libraries will be located in the project directory.

2. BUILDING MANUALLY
	1. Configure the toolchain.
	2. Run "make distclean".
	3. Run "make setconfig_testal_<OS>_<TARGET>"
	4. Run "make"
	5. The static libraries will be located in the project directory.

For building ALL the configurations of TestAL:
	1. Run the build_all_configs.sh script.

********** Releasing Process **********
For creating a Release directory which contains all the output files:
	1. Build the necessary TestAL libraries as described above.
	2. Run the release.sh script.
	3. The Release directory will be located in the project directory.

********** Cleaning Process **********
For cleaning object files, config file, static libraries and Release directory:
	1. Run “make distclean".

For cleaning object files:
	1. Run “make clean".

For cleaning the current config file:
	1. Run "make clrconfig".

