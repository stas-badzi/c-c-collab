# >>>> Arguments <<<<<<<<
# sudo = [0] -> do copy libs and biraries (requiers [sudo/doas/su] permissions)
# forcewin = [1] -> use windows emulator like windows
# debug = [0] -> compile program for debbuging
# tgarch = [host arch] -> set target architecture (for dotnet publish)
# msvc = [0] -> force msvc++ compilation (windows only, must be run in VS developer command prompt)
# give-ctrl = [1] -> give control over the keyboard to the binary (macOS only, requires sudo permissions)
# c-compiler = [$defcompc] -> set compiler loacation
# cpp-compiler = [$defcompcxx] -> set compiler loacation


#******* release config *********
#> release name
release = FactoryRush

#********************************

#******** c++ config ************
#> set default compilers
defcompcxx = c++
defcompc = cc
cflags = -Wno-dollar-in-identifier-extension -Wno-unused-command-line-argument
cxxflags = -Wno-dollar-in-identifier-extension -Wno-unused-command-line-argument
#> source files
sources = Console.cpp TextureSystem.cpp System.cpp Game.cpp dllexport.cpp
#> header files
headers = Console.hpp TextureSystem.hpp TextureSystem.ipp Game.hpp dllimport.hpp System.hpp System.ipp smart_ref.hpp smart_ref.ipp
#> include files
includes = dynamic_library.h unicode_conversion.hpp linux/getfd.h windows/quick_exit.h control_heap.h operating_system.h windows/quick_exit/defines.h utils/cextern.h utils/dllalloc.h linux/key.hpp windows/key.hpp apple/key.hpp apple/keyboard.h apple/openfile.h linux/ledctrl.h linux/mousefd.h
#> name the dynamic library
name = factoryrushplus
# *******************************

#******** c++ binary config *****
#> source files
binsources = main.cpp Console.cpp TextureSystem.cpp System.cpp Control.cpp dllexport.cpp
#> header files
binheaders = dllimport.hpp Console.hpp TextureSystem.hpp System.hpp defines.h Control.hpp
#> include files
binincludes = dynamic_library.h unicode_conversion.hpp control_heap.h utils/cextern.h control_heap.h utils/dllalloc.h linux/key.hpp windows/key.hpp apple/key.hpp
#> name the binary file
binname = cpp-factoryrush
#********************************

#********* c# config ************
#> name the dynamic library
filename = factoryrushsharp
#>source code files
files = DllExport.cs DllImport.cs TextureSystem.cs Terminal.cs Console.cs Utility.cs Exec.cs Control.cs Key.cs linux_keyboard.cs apple_event.cs windows_vkcodes.cs SoundSystem.cs
# *******************************

#********* c# binary config *****
#> name the binary file
binfile = cs-factoryrush
#>source code files
binfiles = Program.cs DllImport.cs TextureSystem.cs Terminal.cs Utility.cs Exec.cs Key.cs linux_keyboard.cs apple_event.cs windows_vkcodes.cs SoundSystem.cs
# *******************************

#***** application config ****
#> linux root of application path
linuxroot = /usr
#> linux shared library path
linuxlib = /usr/lib
#> linux binary file path
linuxbin = /usr/bin
#> macos root of application path
macosroot = /usr/local
#> macos shared library path
macoslib = /usr/local/lib
#> macos binary file path
macosbin = /usr/local/bin
#> windows shared library path
winlib = %SystemRoot%
#> windows binary file path
winbin = %SystemRoot%
#> mysys(mingw) shared library path
msyslib = /usr/lib
#> mysys(mingw) binary filepath
msysbin = /usr/bin
#>cygwin shared library path
cygwinlib = /usr/lib
#>cygwin shared library path
cygwinbin = /usr/bin
#>cygwin dll path to windows
symcyglib = /cygdrive/c/Windows
#>cygwin exe path to windows
symcygbin = /cygdrive/c/Windows
#>msys2(mingw) dll path to windows
symsyslib = /c/Windows
#>msys2(mingw) exe path to windows
symsysbin = /c/Windows
 
#*********************************

# chmod 666 /usr/bin/../temp/initialized.dat
# (linux only)

ifeq ($(findstring MSYS, $(shell uname -s)),MSYS)
static-libc = -static-libgcc -Wl,-Bstatic -lstdc++ -lpthread -Wl,-Bdynamic
static-libc++ = -static-libstdc++
else
ifeq ($(findstring MINGW, $(shell uname -s)),MINGW)
static-libc = -static-libgcc -Wl,-Bstatic -lstdc++ -lpthread -Wl,-Bdynamic
static-libc++ = -static-libstdc++
else
static-libc = -static-libgcc
static-libc++ = -static-libstdc++
endif
endif

ifeq ($(sudo),1)
copylibs = 1
else
copylibs = 0
endif

ifeq ($(shell echo "check quotes"),"check quotes")
copy = copy
else
copy = cp
endif

empty =
space = $(empty) $(empty)

ifeq ($(findstring arm, $(shell uname -m)),arm)
arch = arm64
else
ifeq ($(findstring aarch, $(shell uname -m)),aarch)
arch = arm64
else
arch = x64
endif
endif

ifeq ($(shell uname -s),Darwin)
ifeq ($(shell arch), x86_64)
arch = x64
endif
endif

ifeq ($(shell echo "check quotes"),"check quotes")
movefl = move
dir = $(shell cd)
else
movefl = mv
dir = $(shell pwd)
endif

ifeq ($(linker),$(empty))
ldarg = 
else
ldarg = -fuse-ld=$(linker)
endif

ifeq ($(msvc),1)
defcompcxx = cl
defcompc = cl
objsuf = obj
else
objsuf = o
endif

ifeq ($(cpp-compiler),$(empty))
cpp-compiler = $(defcompcxx)
endif

ifeq ($(c-compiler),$(empty))
c-compiler = $(defcompc)
endif

ifeq ($(shell uname -s),Darwin)
static-libc = 
static-libc++ = 
endif

ifeq ($(force-win),0) 
genwin = 0
else
ifeq ($(force-win),$(empty)) 
genwin = 0
else
genwin = 1
endif
endif

ifeq ($(nodep),1)
cppdep =
resdep =
else
cppdep = cpp
resdep = resources
endif

ifneq ($(tgarch),$(empty))
	arch = $(tgarch)
endif

ifeq ($(msvc),1)
ifeq ($(findstring clang, $(c-compiler)),clang)
ifeq ($(arch),x86)
	archarg = -m32
	archarg = -m32
endif
	clstd = /std:c17 $(archarg)
	clstdpp = /std:c++latest $(archarg)

else
	clstd = /std:clatest
	clstdpp = /std:c++latest
endif
endif

ifeq ($(debug),1)
configuration = Debug
binconfig = Debug
ifeq ($(msvc),1)
cdb = /MDd /Z7
ldb = /DEBUG /PDB:bin/$(name).pdb
bldb = /DEBUG /PDB:bin/$(binname).pdb
bpdb = /MTd /Z7
else
cdb = -g -Og
bpdb = -g -Og
endif
else
configuration = Release
binconfig = Release
ifeq ($(msvc),1)
cdb = /MD /O2
ldb = /CGTHREADS:8
bldb = /CGTHREADS:8
bpdb = /MT /O2
else
cdb = -s -Ofast
bpdb = -s -Ofast
endif
endif

flibdir = bin
wfsrc = $(foreach src,$(sources),src/$(src))
fbsrc = $(foreach bsrc,$(binsources),binaryplus/src/$(bsrc))
os = $(subst $(space),-,$(shell echo $$(uname -s) $$(uname -r).$$(uname -m)))
ifeq ($(msvc),1)
flib = ../csharp/bin/lib/$(filename).lib
fsrc = $(foreach src,$(sources),cplusplus\\src\\$(src))
objects = $(foreach file,$(sources),obj/$(subst .c,.obj,$(subst .cc,.c,$(subst .cpp,.cc,$(file)))))
fbobj = $(foreach file,$(binsources),obj/$(subst .c,.obj,$(subst .cc,.c,$(subst .cpp,.cc,$(file)))))
else
flib = -l$(filename)
fsrc = $(foreach src,$(sources),cplusplus/src/$(src))
objects = $(foreach file,$(sources),obj/$(subst .c,.o,$(subst .cc,.c,$(subst .cpp,.cc,$(file)))))
fbobj = $(foreach file,$(binsources),obj/$(subst .c,.o,$(subst .cc,.c,$(subst .cpp,.cc,$(file)))))
endif

ifeq ($(findstring MSYS, $(shell uname -s)),MSYS)
os = $(subst $(space),-,$(shell echo $$(uname -s)_$$(uname -r)))
else
ifeq ($(findstring MINGW, $(shell uname -s)),MINGW)
os = $(subst $(space),-,$(shell echo $$(uname -s)_$$(uname -r)))
else
ifeq ($(findstring CYGWIN, $(shell uname -s)),CYGWIN)
os = $(subst $(space),-,$(shell echo $$(uname -s)_$$(uname -r)))
endif
endif
endif



ifeq ($(shell uname -s),Darwin)
ifeq ($(findstring arm, $(shell uname -m)),arm)
ifeq ($(findstring aarch, $(shell uname -m)),aarch)
ifeq ($(shell arch), x86_64)
os = $(shell echo $$(uname -s)-$$(arch))
endif
endif
endif
endif


ifeq ($(findstring windows32, $(shell uname -s)),windows32)
#windows
nulldir = nul
binflags = 
admin = sudo$(space)
adminend =
staticgen = lib /OUT:
run = .\
os_name = win-$(arch)
binary = exe
static = lib
dynamic = dll
prefix = $(empty)
dllname = '$(name).$(dynamic)'
libname = '$(filename).$(dynamic)'
libdir = $(winlib)
bindir = $(winbin)
#
else
ifeq ($(shell uname -s),WINDOWS_NT)
#windows i think
nulldir = nul
binflags = 
admin = sudo$(space)
adminend =
staticgen = lib /OUT:
run = .\
os_name = win-$(arch)
binary = exe
static = lib
prefix = $(empty)
dynamic = dll
dllname = '$(name).$(dynamic)'
libname = '$(filename).$(dynamic)'
libdir = $(winlib)
bindir = $(winbin)
#
else
ifeq ($(findstring NT, $(shell uname -s)),NT)
#
ifeq ($(findstring CYGWIN, $(shell uname -s)),CYGWIN)
#cygwin only
binary = exe
static = a
prefix = $(empty)
dynamic = dll
binflags = 
libdir = $(cygwinlib)
bindir = $(cygwinbin)
admin = sudo$(space)
adminend =
#
else
# msys mingw and others
binflags =
ifeq ($(findstring MSYS, $(shell uname -s)),MSYS)
#msys
exec = $(shell cygpath -w /msys2.exe)
#
else
ifeq ($(findstring Windows_NT, $(shell uname -s)),Windows_NT)
#msys (older version i think)
exec = $(shell cygpath -w /msys2.exe)
#
else
#mingw [ond others]
ifeq ($(findstring MINGW64, $(shell uname -s)),MINGW64)
# x64 mingw
exec = $(shell cygpath -w /mingw64.exe)
#
else
# x32 mingw (and others)
exec = $(shell cygpath -w /mingw32.exe)
#
endif
endif
endif
#non-cygwin 'NT'
binary = exe
static = lib
prefix = $(empty)
dynamic = dll
libdir = $(msyslib)
bindir = $(msysbin)
admin = sudo$(space)
adminend =
#
endif
#all unix emulators on windows
run = ./
nulldir = nul
staticgen = ar -rcs$(space)
os_name = win-$(arch)
dllname = '$(name).$(dynamic)'
libname = '$(filename).$(dynamic)'
#
else
ifeq ($(shell uname -s),Darwin)
#macos
nulldir =  /dev/null
binflags = -lapplecuchar -L../assets
admin = sudo$(space)
adminend =
staticgen = ar -rcs$(space)
run = ./
os_name = osx-$(arch)
binary = app
static = a
prefix = lib
dynamic = dylib
libdir = $(macoslib)
bindir = $(macosbin)
#
else
#linux and similar[other]
nulldir = /dev/null
binflags = 
ifneq ($(shell which sudo),$(empty))
	admin = sudo$(space)
	adminend =
else
ifneq ($(shell which doas),$(empty))
	admin = doas$(space)
	adminend =
else
ifneq ($(shell which su),$(empty))
	admin = su -c "
	adminend = "
else
	$(error "neither sudo, doas or su not found")
endif
endif
endif
staticgen = ar rcs$(space)
run = ./
os_name = linux-$(arch)
libname = 'lib$(filename).so'
dllname = 'lib$(name).so'
binary = bin
static = a
prefix = lib
dynamic = so
libdir = $(linuxlib)
bindir = $(linuxbin)
#
endif
dllname = "lib$(name).$(dynamic)"
libname = "lib$(filename).$(dynamic)"
endif
endif
endif

ifeq ($(genwin),1)
ifeq ($(findstring CYGWIN, $(shell uname -s)),CYGWIN)
libdir = $(symcyglib)
bindir = $(symcygbin)
else
ifeq ($(findstring NT, $(shell uname -s)),NT)
libdir = $(symsyslib)
bindir = $(symsysbin)
endif
endif
endif

ifeq ($(copylibs),1)
flibdir = $(libdir)
endif

ifeq ($(archchk),0)
check_arch =
else
check_arch = check-arch
endif

old_arch = $(shell cat __arch.dat 2> /dev/null || echo > __arch.dat)
ifneq ($(old_arch),$(arch))
archfile = $(shell echo $(old_arch) > __oldarch.dat && echo $(arch) > __arch.dat && echo __arch.dat)
else
archfile = __arch.dat
endif

package: release

check-arch: $(archfile)
	@echo "Architecture changed from $(shell cat __oldarch.dat) to $(arch) - Cleaning"
	@rm __oldarch.dat
	@$(MAKE) clean
	@echo "Version file. Remove to enable recompile" > $@

release: all
ifeq ($(shell echo "check quotes"),"check quotes")

ifneq ($(wildcard release),release)
	del /f bin/*/*
	del /f bin/*.zip
	@rmdir /f bin/*
	@rmdir /f bin
endif
	@mkdir bin\cpp
	@mkdir bin\cs

	@copy binaryplus\bin\$(binname).$(binary) bin\cpp
	@copy cplusplus\bin\$(dllname) bin\cpp
	@copy csharp\bin\lib\$(libname) bin\cpp

	@copy binarysharp\bin\exe\$(binfile).$(binary) bin\cs
	@copy cplusplus\bin\$(dllname) bin\cs
	@copy csharp\bin\lib\$(libname) bin\cs

	@cd bin && ren cpp Cpp.$(release)
	@cd bin && ren cs Cs.$(release)

	@cd bin && powershell Invoke-WebRequest -Uri "https://github.com/leok7v/gnuwin32.mirror/raw/master/bin/zip.exe" -OutFile "zip.exe" -Verbose

	@cd bin && zip -r Cpp.$(release).$(os).zip Cpp.$(release)
	@cd bin && zip -r Cs.$(release).$(os).zip Cs.$(release)

else
ifeq ($(wildcard release),release)
	@rm -rf bin
endif
	@mkdir -p bin/cpp
	@mkdir -p bin/cs
	@cp binaryplus/bin/$(binname).$(binary) bin/cpp
	@cp cplusplus/bin/$(dllname) bin/cpp
	@cp csharp/bin/lib/$(libname) bin/cpp

	@cp binarysharp/bin/exe/$(binfile).$(binary) bin/cs
	@cp cplusplus/bin/$(dllname) bin/cs
	@cp csharp/bin/lib/$(libname) bin/cs

	@cd bin && mv cpp Cpp.$(release)
	@cd bin && mv cs Cs.$(release)

ifeq ($(findstring windows32, $(shell uname -s)),windows32)
	cd bin && zip -r Cpp.$(release).$(os).zip Cpp.$(release)
	cd bin && zip -r Cs.$(release).$(os).zip Cs.$(release)
else
ifeq ($(findstring CYGWIN, $(shell uname -s)),CYGWIN)
	cd bin && zip -r Cpp.$(release).$(os).zip Cpp.$(release)
	cd bin && zip -r Cs.$(release).$(os).zip Cs.$(release)
else
ifeq ($(findstring NT, $(shell uname -s)),NT)
	cd bin && zip -r Cpp.$(release).$(os).zip Cpp.$(release)
	cd bin && zip -r Cs.$(release).$(os).zip Cs.$(release)
else
ifeq ($(shell uname -s),Darwin)
	cd bin && tar -czvf Cpp.$(release).$(os).tgz Cpp.$(release)
	cd bin && tar -czvf Cs.$(release).$(os).tgz Cs.$(release)
else
	cd bin && tar -czvf Cpp.$(release).$(os).tar.gz Cpp.$(release)
	cd bin && tar -czvf Cs.$(release).$(os).tar.gz Cs.$(release)
endif
endif
endif
endif
endif
	@echo "Version file. Remove to enable recompile" > $@

all: dll cppbin csbin
	@echo "Version file. Remove to enable recompile" > $@

dll: cs cpp
	@echo "Version file. Remove to enable recompile" > $@

clean: cppclean csclean

csclean:
ifeq ($(shell echo "check quotes"),"check quotes")
	@del /f cs
	@del /f csbin
else
	@rm -f cs
	@rm -f csbin
endif

cppclean:
ifeq ($(shell echo "check quotes"),"check quotes")
	@del /f cppbin
	@del /f cpp
	@del /f resources
	@del /f cplusplus\obj\*
	@del /f binaryplus\obj\*
	@del /f objects\*
else
	@rm -f cppbin
	@rm -f cpp
	@rm -f resources
	@rm -f cplusplus/obj/*
	@rm -f binaryplus/obj/*
	@rm -f objects/*
endif

resources: $(check_arch) source/setkbdmode.c source/killterm.c source/getfd.c source/getfd.h source/keyboard.h source/keyboard.m source/openfile.h source/openfile.m source/globals.c assets/a.tux source/ledctrl.c source/ledctrl.h source/cuchar.cpp source/mousefd.c source/mousefd.h
	@echo MAKE RESOURCES

ifneq ($(msvc),1)
	$(c-compiler) -c source/globals.c -pedantic -Wextra $(cflags) $(cdb) -Isource -Icplusplus/include -std=c2x && mv *.o objects/
	$(staticgen)assets/$(prefix)globals.$(static) objects/globals.o
else
	@echo "$(cpp-compiler) /c /DUNICODE /D_MSVC $(cdb) source/globals.c /Icplusplus\include $(clstd)" > run.bat
	@cmd.exe /c run.bat
	@dir
	@mv *.obj objects/
	@echo "lib /OUT:assets/globals.lib objects/globals.obj" > run.bat
	@cmd.exe /c run.bat
	@rm run.bat
endif

#	echo $(c-compiler) -v -o $(prefix)std.$(dynamic) -pedantic -Wextra -shared -fPIC -lm -static-libgcc 2>&1 | grep ld | sed s/-lc/'$$(find -O3 /usr/lib -name libc.a 2>&1 | grep $$(uname -m) | sed 1q)' | sed s/-lm/'$$(find -O3 /usr/lib -name libm.a 2>&1 | grep $$(uname -m) | sed 1q)'/g | sed s/-o/-Bsymbolic\ -o/g > temp.sh
#	@chmod +x temp.sh
#	./temp.sh
#	@rm temp.sh

ifeq ($(shell uname -s),Linux)
	-@rm *.o 2> $(nulldir)
	$(c-compiler) -c source/setkbdmode.c source/getfd.c source/ledctrl.c source/mousefd.c -pedantic -Wextra $(cflags) $(cdb) -Isource -Icplusplus/include -std=c2x && mv *.o objects/
	ar rcs assets/$(prefix)linuxctrl.$(static) objects/getfd.o objects/ledctrl.o objects/mousefd.o objects/setkbdmode.o
	$(c-compiler) -o assets/setkbdmode objects/setkbdmode.o -Lassets -llinuxctrl $(static-libc)
	git submodule update --init --recursive --remote utilities/doas-keepenv
ifeq ($(copylibs),1)
	@echo "$(linuxroot)/share/factoryrush/bin"
	$(admin)mkdir -p $(linuxroot)/share/factoryrush/bin$(adminend)
	$(admin)cp assets/setkbdmode $(linuxroot)/share/factoryrush/bin$(adminend)
	$(admin)cp utilities/doas-keepenv/doas-keepenv $(linuxroot)/share/factoryrush/bin/doas-keepenv.sh$(adminend)
	$(admin)cp utilities/doas-keepenv/doas-keepenv $(linuxbin)/doas-keepenv.sh$(adminend)

	$(admin)mkdir -p $(linuxroot)/share/factoryrush/assets$(adminend)
	$(admin)cp README.md $(linuxroot)/share/factoryrush/assets$(adminend)
else
	@mkdir -p binaryplus/bin/../share/factoryrush/bin
	@cp assets/setkbdmode binaryplus/share/factoryrush/bin
	@cp utilities/doas-keepenv/doas-keepenv binaryplus/share/factoryrush/bin/doas-keepenv.sh
	@cp utilities/doas-keepenv/doas-keepenv binaryplus/bin/doas-keepenv.sh

	@mkdir -p binaryplus/bin/../share/factoryrush/assets
	@cp README.md binaryplus/share/factoryrush/assets
endif
else
ifeq ($(shell uname -s),Darwin)
	$(cpp-compiler) -c source/cuchar.cpp -pedantic -Wextra $(cxxflags) $(cdb) -Isource -std=c++2b && mv *.o objects/
	ar rcs assets/libapplecuchar.a objects/cuchar.o
	$(c-compiler) -c source/keyboard.m source/openfile.m source/killterm.c -pedantic -Wextra $(cflags) $(cdb) -Isource -Icplusplus/include -std=c2x && mv *.o objects/
	$(c-compiler) -dynamiclib -o assets/libapplectrl.dylib objects/keyboard.o objects/openfile.o -framework CoreGraphics -framework CoreServices
	$(c-compiler) -o assets/killterm objects/killterm.o
	git submodule update --init --recursive --remote utilities/give-control
ifeq ($(copylibs),1)
	$(admin)mkdir -p $(macosroot)/share/factoryrush/lib $(macosroot)/share/factoryrush/bin$(adminend)
	$(admin)cp assets/libapplectrl.dylib $(macosroot)/share/factoryrush/lib$(adminend)
	$(admin)cp assets/libapplectrl.dylib $(macoslib)$(adminend)
	$(admin)cp assets/killterm $(macosroot)/share/factoryrush/bin$(adminend)
else
	@mkdir -p binaryplus/share/factoryrush/lib
	@cp assets/libapplectrl.dylib binaryplus/share/factoryrush/lib
	@cp assets/libapplectrl.dylib binaryplus/bin
endif
else
endif
endif

ifeq ($(shell echo "check quotes"),"check quotes")
	@copy assets\a.tux binaryplus\bin
else
	@cp assets/a.tux binaryplus/bin
endif
	@echo "Version file. Remove to enable recompile" > $@

cpprun:
ifeq ($(binary),exe)
	-$(run)binaryplus\bin\$(binname).$(binary)
else
ifeq ($(sudo),1)
	$(bindir)/$(binname).$(binary)
else
	-cd binaryplus/bin && $(run)$(binname).$(binary)
endif
endif

csrun:
	-cd binarysharp/bin/exe && $(run)$(binfile).$(binary)

cpp: cs  $(foreach obj,$(objects),cplusplus/$(obj)) $(foreach head,$(headers),cplusplus/src/$(head)) $(foreach inc,$(includes),cplusplus/include/$(inc))
	@echo MAKE CPP

ifeq ($(msvc),1)
	echo "cd cplusplus && link /OUT:bin/$(name).dll $(ldb) /DLL $(flib) $(objects) ../assets/globals.lib USER32.lib Gdi32.lib Shell32.lib Shlwapi.lib Dbghelp.lib" > run.bat
	@cmd.exe /c run.bat
	@rm run.bat
ifeq ($(debug),1)
	@cp cplusplus/bin/$(filename).pdb binarysharp/bin/exe
	@cp cplusplus/bin/$(filename).pdb binaryplus/bin
	@cp cplusplus/bin/$(filename).pdb csharp/bin/lib
endif

ifeq ($(copylibs),1)
	$(admin)cp cplusplus/bin/$(dllname) $(libdir)$(adminend)
else
	@cp cplusplus/bin/$(dllname) binaryplus/bin
	@cp cplusplus/bin/$(dllname) binarysharp/bin/exe
	@cp cplusplus/bin/$(dllname) csharp/bin/lib
endif

else
#
ifeq ($(binary),exe)
#windows
ifeq ($(shell uname -s),Windows_NT)
	cd cplusplus && $(cpp-compiler) -shared -o bin/$(name).dll $(objects) -L../assets -L$(flibdir) -lglobals -ldbghelp -lshlwapi -lshell32 $(flib) $(static-libc++) $(static-libc) $(ldarg)
else
ifeq ($(shell uname -s),windows32)
	cd cplusplus && $(cpp-compiler) -shared -o bin/$(name).dll $(objects) -L../assets -L$(flibdir) -lglobals -ldbghelp -lshlwapi -lshell32 $(flib) $(static-libc++) $(static-libc) $(ldarg)
else
	cd cplusplus && $(cpp-compiler) -shared -o bin/$(name).dll $(objects) -L../assets -L$(flibdir) -lglobals -ldbghelp -lshlwapi -lshell32 $(flib) $(static-libc++) $(static-libc) $(ldarg)
endif
endif
#
else
ifeq ($(shell uname -s),Darwin)
#macos
	cd cplusplus && $(cpp-compiler) -dynamiclib -o bin/lib$(name).dylib $(objects) -L../assets -L$(flibdir) -lapplecuchar -lapplectrl -lglobals $(flib) $(static-libc++) $(static-libc) $(ldarg)
	utilities/custom/dylib-fix.sh "cplusplus/bin/lib$(name).dylib" "applectrl"
#
else
#linux and similar
#	cd cplusplus && $(cpp-compiler) -v -shared -o bin/lib$(name).so $(objects) -L../assets -L$(flibdir) -lglobals -llinuxctrl $(flib) $(static-libc++) $(static-libc) $(ldarg) 2>&1 | grep ld | sed 's/-lc/$$(find -O3 \/usr\/lib -name libc.a 2>&1 | grep $$(uname -m) | sed 1q)'/g | sed 's/-lm/$$(find -O3 \/usr\/lib -name libm.a 2>&1 | grep $$(uname -m) | sed 1q)'/g | sed s/-o/-Bsymbolic\ -o/g > temp.sh && chmod +x temp.sh && ./temp.sh && rm temp.sh
	cd cplusplus && $(cpp-compiler) -shared -o bin/lib$(name).so $(objects) -L../assets -L$(flibdir) -lglobals -llinuxctrl $(flib) $(static-libc++) $(static-libc) $(ldarg)
endif
endif
#

ifeq ($(shell echo "check quotes"),"check quotes")
#windows
ifeq ($(copylibs),1)
	$(admin)copy cplusplus\bin\$(dllname) $(libdir)$(adminend)
else
	@copy cplusplus\bin\$(dllname) binaryplus\bin
	@copy cplusplus\bin\$(dllname) binarysharp\bin\exe
	@copy cplusplus\bin\$(dllname) csharp\bin\lib
endif
else
#other
ifeq ($(copylibs),1)
	$(admin)cp cplusplus/bin/$(dllname) $(libdir)$(adminend)
else
	@cp cplusplus/bin/$(dllname) binaryplus/bin
	@cp cplusplus/bin/$(dllname) binarysharp/bin/exe
	@cp cplusplus/bin/$(dllname) csharp/bin/lib
endif
endif
endif
	@echo "Version file. Remove to enable recompile" > $@


cs: $(resdep) $(foreach fl,$(files),csharp/$(fl))
	@echo MAKE CS
	cd csharp && dotnet publish -p:NativeLib=Shared -p:SelfContained=true -r $(os_name) -c $(configuration)

	-@mkdir -p csharp/bin/$(configuration)/net9.0/$(os_name)/native/
	-@mv csharp/bin/$(arch)/$(configuration)/net9.0/$(os_name)/native/* csharp/bin/$(configuration)/net9.0/$(os_name)/native/
ifeq ($(msvc),1)
	@cd csharp/bin/$(configuration)/net9.0/$(os_name)/native/ && for i in *.exp; do if [ ! "$$i" = '$(filename).exp' ]; then mv $$i $(filename).exp; fi; done && for i in *.lib; do if [ ! "$$i" = '$(filename).lib' ]; then mv $$i $(filename).lib; fi; done && for i in *.pdb; do if [ ! "$$i" = '$(filename).pdb' ]; then mv $$i $(filename).pdb; fi; done && for i in *.dll; do if [ ! "$$i" = '$(filename).dll' ]; then mv $$i $(filename).dll; fi; done 
	@mv csharp/bin/$(configuration)/net9.0/$(os_name)/native/* csharp/bin/lib
ifeq ($(debug),1)
	@cp csharp/bin/lib/$(filename).pdb binarysharp/bin/exe
	@cp csharp/bin/lib/$(filename).pdb binaryplus/bin
	@cp csharp/bin/lib/$(filename).pdb cplusplus/bin
endif
ifeq ($(copylibs),1)
	$(admin)cp csharp/bin/lib/$(libname) $(libdir)$(adminend)
else
	@cp csharp/bin/lib/$(libname) binarysharp/bin/exe
	@cp csharp/bin/lib/$(libname) binaryplus/bin
	@cp csharp/bin/lib/$(libname) cplusplus/bin
endif
else
ifeq ($(shell echo "check quotes"),"check quotes")
	@cd csharp/bin/$(configuration)/net9.0/$(os_name)/native/ && @echo . > null.exp && @echo . > null.lib && @echo . > null.pdb && del *.exp && del *.lib && del *.pdb && ren * $(libname)
	@move csharp\bin\$(configuration)\net9.0\$(os_name)\native\$(libname) csharp\bin\lib
ifeq ($(copylibs),1)
	$(admin)copy csharp\bin\lib\$(libname) $(libdir)$(adminend)
else
	@copy csharp\bin\lib\$(libname) binarysharp\bin\exe
	@copy csharp\bin\lib\$(libname) binaryplus\bin
	@copy csharp\bin\lib\$(libname) cplusplus\bin
endif
else
	@cd csharp/bin/$(configuration)/net9.0/$(os_name)/native/ && mkdir null.dSYM && touch null.dSYM/null.null && rm *.dSYM/* && rmdir *.dSYM && touch null.dbg && touch null.exp && touch null.lib && touch null.pdb && rm *.dbg && rm *.exp && rm *.lib && rm *.pdb
	@mv -f csharp/bin/$(configuration)/net9.0/$(os_name)/native/* csharp/bin/lib/$(libname)
ifeq ($(copylibs),1)
	$(admin)cp csharp/bin/lib/$(libname) $(libdir)$(adminend)
else
	@cp csharp/bin/lib/$(libname) binarysharp/bin/exe
	@cp csharp/bin/lib/$(libname) binaryplus/bin
	@cp csharp/bin/lib/$(libname) cplusplus/bin
endif
endif
endif
	@echo "Version file. Remove to enable recompile" > $@

cppbin: cpp $(foreach src,$(binsources),binaryplus/src/$(src)) $(foreach head,$(binheaders),binaryplus/src/$(head)) $(foreach inc,$(binincludes),binaryplus/include/$(inc))
	@echo MAKE CPPBIN

ifeq ($(msvc),1)
	echo "$(cpp-compiler) /EHsc /c $(bpdb) $(fbsrc) /Ibinaryplus\include $(clstdpp)" > run.bat
	@cmd.exe /c run.bat
	@$(movefl) -f $(subst obj/,$(empty),$(fbobj)) binaryplus/obj
	echo "cd binaryplus && link /OUT:bin/$(binname).$(binary) $(bldb) $(flib) ../cplusplus/bin/$(name).lib ../csharp/bin/lib/$(filename).lib $(fbobj) USER32.lib" > run.bat
	@cmd.exe /c run.bat
	@rm run.bat
else
#all
	$(cpp-compiler) -c -pedantic -Wextra $(cxxflags) $(bpdb) $(fbsrc) -I binaryplus/include -std=c++2b
	@$(movefl) -f $(subst obj/,$(empty),$(fbobj)) binaryplus/obj
	cd binaryplus && $(cpp-compiler) -o bin/$(binname).$(binary) $(binflags) $(fbobj) -L$(flibdir) -l$(name) $(flib) $(static-libc++) $(static-libc) $(ldarg)
#

ifeq ($(shell uname -s),Darwin)
#macos
	utilities/custom/dylib-fix.sh "binaryplus/bin/$(binname).$(binary)" "$(name)"
	utilities/custom/dylib-fix.sh "binaryplus/bin/$(binname).$(binary)" "$(filename)"
ifneq ($(give-ctrl),0)
	utilities/give-control/give-control "binaryplus/bin/$(binname).$(binary)"
endif
#
endif
ifeq ($(shell uname -s),Linux)
#linux
	cd binaryplus/bin && $(admin)chown root $(binname).$(binary) && $(admin)chmod u+s $(binname).$(binary)$(adminend)
#
endif

ifeq ($(shell echo "check quotes"),"check quotes")
#windows
ifeq ($(copylibs),1)
	$(admin)copy binaryplus\bin\$(binname).$(binary) $(bindir)$(adminend)
	cd binaryplus\bin && dir
endif
else
#other
ifeq ($(copylibs),1)
	$(admin)cp binaryplus/bin/$(binname).$(binary) $(bindir)$(adminend)
ifeq ($(shell uname -s), Darwin)
ifneq ($(give-ctrl),0)
	$(admin)utilities/give-control/give-control '$(bindir)/$(binname).$(binary)'$(adminend)
endif
endif
else
endif
endif
endif
	@echo "Version file. Remove to enable recompile" > $@

csbin: $(cppdep) $(foreach bfl,$(binfiles),binarysharp/$(bfl))
	@echo MAKE CSBIN
	cd binarysharp && dotnet publish -p:SelfContained=true -r $(os_name) -c $(binconfig)

	-@mkdir -p binarysharp/bin/$(configuration)/net9.0/$(os_name)/native/
	-@mv binarysharp/bin/$(arch)/$(configuration)/net9.0/$(os_name)/native/* binarysharp/bin/$(configuration)/net9.0/$(os_name)/native/
ifeq ($(msvc),1)
	@cd binarysharp/bin/$(configuration)/net9.0/$(os_name)/native/ && for i in *.$(binary); do if [ ! "$$i" = '$(binname).$(binary)' ]; then mv $$i $(binname).$(binary); fi; done 
	@mv binarysharp/bin/$(configuration)/net9.0/$(os_name)/native/* binarysharp/bin/exe
ifeq ($(copylibs),1)
	$(admin)cp binarysharp/bin/exe/$(binfile).$(binary) $(bindir)$(adminend)
else
endif
else

ifeq ($(shell echo "check quotes"),"check quotes")
	@cd binarysharp/bin/$(binconfig)/net9.0/$(os_name)/native/ && @echo . > null.exp && @echo . > null.lib && @echo . > null.pdb && del *.exp && del *.lib && del *.pdb && ren * $(binfile).$(binary)
	@copy binarysharp\bin\$(binconfig)\net9.0\$(os_name)\native\$(binfile).$(binary) binarysharp\bin\exe
	del binarysharp\bin\$(binconfig)\net9.0\$(os_name)\native\$(binfile).$(binary)
else
	@cd binarysharp/bin/$(binconfig)/net9.0/$(os_name)/native/ && mkdir null.dSYM && touch null.dSYM/null.null && rm *.dSYM/* && rmdir *.dSYM && touch null.dbg && touch null.exp && touch null.lib && touch null.pdb && rm *.dbg && rm *.exp && rm *.lib && rm *.pdb
	@cp -f binarysharp/bin/$(binconfig)/net9.0/$(os_name)/native/* binarysharp/bin/exe/$(binfile).$(binary)
	@rm -f binarysharp/bin/$(binconfig)/net9.0/$(os_name)/native/*
endif

ifeq ($(shell echo "check quotes"),"check quotes")
#windows
ifeq ($(copylibs),1)
	$(admin)copy binarysharp\bin\exe\$(binfile).$(binary) $(bindir)$(adminend)
else
	@cd binarysharp\bin\exe && dir
endif
else
#other
ifeq ($(copylibs),1)
	$(admin)cp binarysharp/bin/exe/$(binfile).$(binary) $(bindir)$(adminend)
else
	@cd binarysharp/bin/exe && ls
endif
endif
endif
	@echo "Version file. Remove to enable recompile" > $@

# .cpp
cplusplus/obj/%.$(objsuf): cplusplus/src/%.cpp $(foreach head,$(headers),cplusplus/src/$(head)) $(foreach inc,$(includes),cplusplus/include/$(inc))
ifeq ($(findstring $(subst cplusplus/src/,$(empty),$<),$(sources)),$(subst cplusplus/src/,$(empty),$<))

ifeq ($(msvc),1)
#msvc
	@echo "$(cpp-compiler) /EHsc /c /DUNICODE /D_CRT_SECURE_NO_DEPRECATE /D_MSVC $(cdb) $< /Icplusplus\include $(clstdpp)" > run.bat
####@type run.bat
	@cmd.exe /c run.bat
	@rm run.bat
#
else
ifeq ($(findstring windows32, $(shell uname -s)),windows32)
#windows
	$(cpp-compiler) -c -pedantic -Wextra $(cxxflags) -DUNICODE $(cdb) $< -I cplusplus/include -std=c++2b
#
else
ifeq ($(shell uname -s),WINDOWS_NT)
#windows
	$(cpp-compiler) -c -pedantic -Wextra $(cxxflags) -DUNICODE $(cdb) $< -I cplusplus/include -std=c++2b
#
else
ifeq ($(findstring CYGWIN, $(shell uname -s)),CYGWIN)
#cygwin [ I think same as windows (?) ]
	$(cpp-compiler) -c -pedantic -Wextra $(cxxflags) -fPIC -DUNICODE $(cdb) $< -I cplusplus/include -std=c++2b
#
else
ifeq ($(findstring MINGW, $(shell uname -s)),MINGW)
#mingw [ I think same as windows (?) ]
	$(cpp-compiler) -c -pedantic -Wextra $(cxxflags) -fPIC -DUNICODE $(cdb) $< -I cplusplus/include -std=c++2b
#
else
ifeq ($(findstring Windows_NT, $(shell uname -s)),Windows_NT)
#msys [ i think older ]
	$(cpp-compiler) -c -pedantic -Wextra $(cxxflags) -fPIC -DUNICODE $(cdb) $< -I cplusplus/include -std=c++2b
else
ifeq ($(findstring MSYS, $(shell uname -s)),MSYS)
#msys [ I think same as windows (?) ]
	$(cpp-compiler) -c -pedantic -Wextra $(cxxflags) -fPIC -DUNICODE $(cdb) $< -I cplusplus/include -std=c++2b
else
# not windows
	$(cpp-compiler) -c -pedantic -Wextra $(cxxflags) -fPIC -DUNICODE $(cdb) -fvisibility=hidden $< -I cplusplus/include -std=c++2b
endif
endif
endif
endif
endif
endif
endif
	@$(movefl) -f $(subst cplusplus/obj/,$(empty),$@) cplusplus/obj
endif

# .c
cplusplus/obj/%.$(objsuf): cplusplus/src/%.c $(foreach head,$(headers),cplusplus/src/$(head)) $(foreach inc,$(includes),cplusplus/include/$(inc))
ifeq ($(findstring $(subst cplusplus/src/,$(empty),$<),$(sources)),$(subst cplusplus/src/,$(empty),$<))

ifeq ($(msvc),1)
#msvc
	@echo "$(cpp-compiler) /c /DUNICODE /D_MSVC $(cdb) $< /Icplusplus\include $(clstd)" > run.bat
####@type run.bat
	@cmd.exe /c run.bat
	@rm run.bat
#
else
ifeq ($(findstring windows32, $(shell uname -s)),windows32)
#windows
	$(c-compiler) -c -pedantic -Wextra $(cxxflags) -DUNICODE $(cdb) $< -I cplusplus/include -std=c2x
#
else
ifeq ($(shell uname -s),WINDOWS_NT)
#windows
	$(c-compiler) -c -pedantic -Wextra $(cxxflags) -DUNICODE $(cdb) $< -I cplusplus/include -std=c2x
#
else
ifeq ($(findstring CYGWIN, $(shell uname -s)),CYGWIN)
#cygwin [ I think same as windows (?) ]
	$(c-compiler) -c -pedantic -Wextra $(cxxflags) -fPIC -DUNICODE $(cdb) $< -I cplusplus/include -std=c2x
#
else
ifeq ($(findstring MINGW, $(shell uname -s)),MINGW)
#mingw [ I think same as windows (?) ]
	$(c-compiler) -c -pedantic -Wextra $(cxxflags) -fPIC -DUNICODE $(cdb) $< -I cplusplus/include -std=c2x
#
else
ifeq ($(findstring Windows_NT, $(shell uname -s)),Windows_NT)
#msys [ i think older ]
	$(c-compiler) -c -pedantic -Wextra $(cxxflags) -fPIC -DUNICODE $(cdb) $< -I cplusplus/include -std=c2x
else
ifeq ($(findstring MSYS, $(shell uname -s)),MSYS)
#msys [ I think same as windows (?) ]
	$(c-compiler) -c -pedantic -Wextra $(cxxflags) -fPIC -DUNICODE $(cdb) $< -I cplusplus/include -std=c2x
else
# not windows
	$(c-compiler) -c -pedantic -Wextra $(cxxflags) -fPIC -DUNICODE $(cdb) -fvisibility=hidden $< -I cplusplus/include -std=c2x
endif
endif
endif
endif
endif
endif
endif
	@$(movefl) -f $(subst cplusplus/obj/,$(empty),$@) cplusplus/obj
endif