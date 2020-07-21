#!/bin/bash
 . /etc/profile.d/fw_devel.sh

cd "$( dirname "${BASH_SOURCE[0]}" )"
echo "PWD is now: " && pwd
echo
cd ..
echo -e "\033[1;44m===========================\033[0m"
echo -e "\033[1;44mRun checkpatch on TestAL files\033[0m"
echo -e "\033[1;44m===========================\033[0m"

# All .c and .h files
find . -name '*.h' -exec ./scripts/checkpatch.pl -f  --summary-file --no-summary {} \;
find . -name '*.c' ! -name "malloc.c" -exec ./scripts/checkpatch.pl -f  --summary-file --no-summary {} \;
i
# All makefiles files
find . -name '*akefile*' -exec ./scripts/checkpatch.pl -f  --summary-file --no-summary {} \;

echo
echo -e "\033[1;44m========\033[0m"
echo -e "\033[1;44m= Done =\033[0m"
echo -e "\033[1;44m========\033[0m"

