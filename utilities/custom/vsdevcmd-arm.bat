for /f "tokens=6 delims=\" %%V in ('dir /s /b "C:\Program Files (x86)\Windows Kits\10\Lib\arm" ^| findstr "\\um\\arm$"') do set "VERSION=%%V"
echo %VERSION%
call "C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Auxiliary\Build\vcvarsall.bat" amd64_arm %VERSION%
where clang-cl
%*