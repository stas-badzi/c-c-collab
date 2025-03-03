ls "C:"
echo
ls "C:\Program Files (x86)"
echo
ls "C:\Program Files (x86)\Microsoft Visual Studio"
echo
ls "C:\Program Files (x86)\Microsoft Visual Studio\2022"
echo
ls "C:\Program Files (x86)\Microsoft Visual Studio\2022\Enterprise"
echo
ls "C:\Program Files (x86)\Microsoft Visual Studio\2022\Enterprise\VC"
echo
ls "C:\Program Files (x86)\Microsoft Visual Studio\2022\Enterprise\VC\Auxiliary"
echo
ls "C:\Program Files (x86)\Microsoft Visual Studio\2022\Enterprise\VC\Auxiliary\Build"
echo
ls "C:\Program Files (x86)\Microsoft Visual Studio\2022\Enterprise\VC\Auxiliary\Build\vcvarsall.bat"
echo
call "C:\Program Files (x86)\Microsoft Visual Studio\2022\Enterprise\VC\Auxiliary\Build\vcvarsall.bat" x64
%*