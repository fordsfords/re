#!/bin/bash
# tst.sh

./bld.sh;  if [ "$?" -ne 0 ]; then exit; fi

rm -rf *.log

./test_re
