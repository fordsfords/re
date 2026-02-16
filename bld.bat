rem bld.bat

cl /std:c11 /W4 /O2 /MT /nologo /D_CRT_SECURE_NO_WARNINGS /D_CRT_NONSTDC_NO_DEPRECATE re.c test_re.c /Fe:test_re.exe
exit /b %ERRORLEVEL%
