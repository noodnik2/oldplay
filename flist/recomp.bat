@echo off
erase flist.exe
FOR %%X IN (%1 %2 %3 %4 %5 %6 %7 %8 %9) DO ERASE %%X.obj
make /f makefile.p6s
