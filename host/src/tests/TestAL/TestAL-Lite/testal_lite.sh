#!/bin/bash

# Description:
# This script prepares the directory for building TestAL-Lite, which is
# a partial version of the full TestAL that can be released to customers.

# It replaces the original TestAL files with TestAL-Lite files and
# removes ALL THE INTERNAL FILES of TestAL, including the TestAL-Lite directory
# and scripts.

# Note that the procedure is irreversible!


echo -e "\033[42m===========================\033[0m"
echo -e "\033[42m        TestAL_Lite        \033[0m"
echo -e "\033[42m===========================\033[0m"
echo


cd "$( dirname "${BASH_SOURCE[0]}" )"


echo -e "\033[44mReplaces TestAL files with TestAL-Lite files:\033[0m"
rm -rf ../pal/linux/malloc.c ../pal/linux/test_pal_mem.c ../ReadMe.txt
rm -rf ../build_config.sh
rm -rf ../freertos
rm -rf ../scripts
rm -rf ../.gitmodules
cp pal/linux/test_pal_mem.c ../pal/linux/test_pal_mem.c
cp ReadMe.txt ../ReadMe.txt
cp build_config.sh ../build_config.sh

echo "[RM] pal/linux/malloc.c"
echo "[RM] pal/linux/test_pal_mem.c"
echo "[RM] ReadMe.txt"
echo "[RM] build_config.sh"
echo "[RM] freertos"
echo "[RM] scripts"
echo "[RM] .gitmodules"
echo "[CP] TestAL-Lite/pal/linux/test_pal_mem.c --> pal/linux/test_pal_mem.c"
echo "[CP] TestAL-Lite/ReadMe.txt --> ReadMe.txt"
echo "[CP] TestAL-Lite/build_config.sh --> build_config.sh"

echo

echo -e "\033[44mDeletes ALL INTERNAL FILES:\033[0m"
cd ..
rm -rf TestAL-Lite release.sh build_all_configs.sh

echo "[RM] TestAL-Lite"
echo "[RM] release.sh"
echo "[RM] build_all_configs.sh"
echo

echo -e "\033[42m========\033[0m"
echo -e "\033[42m  DONE  \033[0m"
echo -e "\033[42m========\033[0m"
echo
