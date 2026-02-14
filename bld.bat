rem bld.bat

cl /std:c11 /W4 /O2 /MT /nologo re.c test_re.c plat_win.c ws2_32.lib /Fe:test_re.exe
