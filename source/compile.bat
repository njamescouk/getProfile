set COMPILE_SRCS=..\source\dependencies\nickPbm\PBitMap.cpp ..\source\dependencies\nickPbm\PbmPixel.cpp ..\source\dependencies\nickPbm\PbmPoint.cpp ..\source\dependencies\nickPbm\fileio.c ..\source\dependencies\nickPbm\libpbm2.c ..\source\dependencies\nickPbm\libpbm3.c ..\source\dependencies\parseCmdline\source\lex.pcl.c ..\source\dependencies\parseCmdline\source\pcl.tab.c ..\source\dependencies\split_string\split_string.cpp I:\usr\NIK\dev\pbmGetProfile\source\GetProfileMain.cpp
set COMPILE_INCLUDES=-I..\source\dependencies\nickPbm -I..\source\dependencies\parseCmdline\source -I..\source\dependencies\split_string -II:\usr\NIK\dev\pbmGetProfile\source
set COMPILE_DEFINES=-DSQLITE_CORE -DSQLITE_ENABLE_MD5
rem set these for your machine...
set YACCPATH="C:\Program Files (x86)\GnuWin32\bin\yacc.exe"
set SEDPATH="C:\Program Files (x86)\GnuWin32\bin\sed.exe"
set FLEXPATH="C:\Program Files (x86)\GnuWin32\bin\flex.exe"
set GPPPATH="c:\bin\MinGWNew\bin\g++.exe"
set GCCPATH="c:\bin\MinGWNew\bin\gcc.exe"
%GPPPATH% -o pbmGetProfile.exe %COMPILE_INCLUDES% %COMPILE_DEFINES% %COMPILE_SRCS% 
