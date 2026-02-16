@echo off
rem tst.bat

call bld.bat
if ERRORLEVEL 1 (
  echo ERROR: build failed
  exit /b 1
)

test_re
