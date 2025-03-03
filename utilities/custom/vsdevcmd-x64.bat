ls "C:"
echo _
ls "C:\Program Files"
echo _
ls "C:\Program Files\Microsoft Visual Studio"
echo _
ls "C:\Program Files\Microsoft Visual Studio\2022"
echo _
ls "C:\Program Files\Microsoft Visual Studio\2022\Enterprise"
echo _
ls "C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC"
echo _
ls "C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Auxiliary"
echo _
ls "C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Auxiliary\Build"
echo _
ls "C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Auxiliary\Build\vcvarsall.bat"
echo _
call "C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Auxiliary\Build\vcvarsall.bat" x64
%*