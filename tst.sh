#!/bin/bash
# tst.sh

./bld.sh;  if [ "$?" -ne 0 ]; then exit; fi

rm -rf *.log

./test_re

echo ""
./test_char >test_char.c.log
./test_char.pl >test_char.pl.log

diff test_char.c.log test_char.pl.log >test_char.log
if [ -s test_char.log ]; then :
  echo "test_char: FAILED; test_char.log is not empty:"
  cat test_char.log
else :
  echo "test_char: passed"
fi
