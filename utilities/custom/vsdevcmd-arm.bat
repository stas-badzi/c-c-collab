call "C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Auxiliary\Build\vcvarsall.bat" amd64_arm 

where clang-cl
where link

echo WindowsSdkDir=%WindowsSdkDir%
echo LIB=%LIB%

dir "%WindowsSdkDir%Lib\*\um\arm\USER32.lib"
if errorlevel 1 (
    echo ARM32 USER32.lib was not found
    exit /b 1
)

%*