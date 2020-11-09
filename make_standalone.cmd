@echo off
md export\Include >nul 2>&1
md export\Lib >nul 2>&1

set src_folder=7zpp
set dst_folder=export\Include
set file_list=standalone.lst

for /f "tokens=*" %%i in (%file_list%) do (
   echo f | copy "%src_folder%\%%i" "%dst_folder%\%%i" >nul
)

copy Lib\*.lib export\Lib >nul
