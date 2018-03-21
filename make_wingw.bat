set PATH=c:\MinGW\mingw64\bin;C:\MinGW\msys\1.0\bin;%PATH%
mingw32-make.exe -s -j 20 clean
mingw32-make.exe -s -j 20 all OTA_IDX=1
mingw32-make.exe -s -j 20 all OTA_IDX=2
pause
