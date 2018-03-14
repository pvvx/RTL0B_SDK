set PATH=c:\MinGW\mingw64\bin;C:\MinGW\msys\1.0\bin;%PATH%
rem mingw32-make.exe -j 20 -s clean
set OTA_IDX=1
mingw32-make.exe -s -j 20 mp
set OTA_IDX=2
mingw32-make.exe -s -j 20 mp
pause
