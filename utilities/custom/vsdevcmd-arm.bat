call "C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Auxiliary\Build\vcvarsall.bat" amd64_arm 

where clang-cl
where link

echo WindowsSdkDir=%WindowsSdkDir%
echo LIB=%LIB%

dir "%WindowsSdkDir%Lib\10.0.26100.0\um\arm"

%*