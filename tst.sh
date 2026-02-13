#!/bin/bash
# tst.sh - Sunny-day test for dual_cap.

./bld.sh;  if [ "$?" -ne 0 ]; then exit; fi

rm -rf *.log

./test_re
