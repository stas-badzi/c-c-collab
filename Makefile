# >>>> Arguments <<<<<<<<
# sudo = [0] -> do copy libs and binaries (requires [sudo/doas/su] permissions)
# forcewin = [1] -> use windows emulator like windows
# debug = [0] -> compile program for debbuging
# tgarch = [host arch] -> set target architecture (for dotnet publish)
# msvc = [0] -> force msvc++ compilation (windows only, must be run in VS developer command prompt)
# give-ctrl = [1] -> give control over the keyboard to the binary (macOS only, requires sudo permissions)
# universal2 = [0] -> compile for both arm64 and x64 (macOS only) 
# c-compiler = [$defcompc] -> set compiler location
# cpp-compiler = [$defcompcxx] -> set compiler location


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
sources = Console.cpp TextureSystem.cpp System.cpp Game.cpp dllexport.cpp SoundSystem.cpp
#> header files
headers = Console.hpp TextureSystem.hpp TextureSystem.ipp Game.hpp dllimport.hpp System.hpp System.ipp smart_ref.hpp smart_ref.ipp SoundSystem.hpp Texture.hpp
#> include files
includes = dynamic_library.h unicode_conversion.hpp linux/getfd.h quick_exit.h operating_system.h quick_exit/defines.h utils/cextern.h linux/key.hpp windows/key.hpp apple/key.hpp apple/keyboard.h apple/openfile.h linux/ledctrl.h linux/mousefd.h windows/thread_safe/queue windows/thread_safe/vector promise.hpp
#> name of the dynamic library
name = factoryrushplus
# *******************************

#******** c++ binary config *****
#> source files
binsources = main.cpp Console.cpp TextureSystem.cpp System.cpp dllexport.cpp SoundSystem.cpp Game.cpp
#> header files
binheaders = dllimport.hpp Console.hpp TextureSystem.hpp System.hpp defines.h SoundSystem.hpp Game.hpp Texture.hpp
#> include files
binincludes = dynamic_library.h unicode_conversion.hpp utils/cextern.h linux/key.hpp windows/key.hpp apple/key.hpp
#> name of the binary file
binname = cpp-factoryrush
#********************************

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

sanitze = 
ifeq ($(findstring arm, $(shell uname -m)),arm)
arch = arm64
else
ifeq ($(findstring aarch, $(shell uname -m)),aarch)
arch = arm64
else
sanitize = -fsanitize=address,undefined
arch = x64
endif
endif

ifneq ($(tgarch),$(empty))
	arch = $(tgarch)
endif
arch2 = $(arch)

_cflags = $(cflags)
_cxxflags = $(cxxflags)
archif =
ifeq ($(shell uname -s),Darwin)
macver = -D_MAC_VER=$(shell sw_vers -productVersion | cut -d '.' -f 1)
ifeq ($(shell arch), x86_64)
arch = x64
endif
ifeq ($(universal2),1)
arch = x86_64
archif = -arch $(arch)
_cflags = -arch $(arch) $(cflags)
_cxxflags = -arch $(arch) $(cxxflags)
arch2 = universal2
endif
macver =
endif

ifeq ($(shell echo "check quotes"),"check quotes")
movefl = move
dir = $(shell cd)
else
movefl = mv
dir = $(shell pwd)
endif

ifeq ($(offline),1)
restore = --no-restore
else
restore =
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

ifeq ($(msvc),1)
ifeq ($(findstring clang, $(c-compiler)),clang)
ifeq ($(arch),x86)
	archarg = 
	archarg = 
endif
	clstd = /std:clatest $(archarg)
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
bpdb = /MDd /Z7
else
cdb = -g -Og -pg -D_DEBUG $(sanitze)
bpdb = -g -Og -pg -D_DEBUG $(sanitze)
clnk = $(sanitze)
endif
else
configuration = Release
binconfig = Release
ifeq ($(msvc),1)
cdb = /MD /O3
ldb = /CGTHREADS:8
bldb = /CGTHREADS:8
bpdb = /MD /O3
else
cdb = -s -Ofast
bpdb = -s -Ofast
clnk =
endif
endif

flibdir = bin
wfsrc = $(foreach src,$(sources),src/$(src))
fbsrc = $(foreach bsrc,$(binsources),binaryplus/src/$(bsrc))
os = $(subst $(space),-,$(shell echo $$(uname -s) $$(uname -r).$$(uname -m)))
ifeq ($(msvc),1)
fsrc = $(foreach src,$(sources),cplusplus\\src\\$(src))
objects = $(foreach file,$(sources),obj/$(arch)/$(subst .c,.obj,$(subst .cc,.c,$(subst .cpp,.cc,$(file)))))
fbobj = $(foreach file,$(binsources),obj/$(arch)/$(subst .c,.obj,$(subst .cc,.c,$(subst .cpp,.cc,$(file)))))
else
fsrc = $(foreach src,$(sources),cplusplus/src/$(src))
objects = $(foreach file,$(sources),obj/$(arch)/$(subst .c,.o,$(subst .cc,.c,$(subst .cpp,.cc,$(file)))))
fbobj = $(foreach file,$(binsources),obj/$(arch)/$(subst .c,.o,$(subst .cc,.c,$(subst .cpp,.cc,$(file)))))
endif

ifeq ($(findstring MSYS, $(shell uname -s)),MSYS)
os1 = $(subst $(space),-,$(shell echo $$(uname -s)_$$(uname -r)))
ifeq ($(tgarch),i686)
os = $(shell echo $(os1) | sed 's/x86_64/i686/g')
else
os = $(os1)
endif
else
ifeq ($(findstring MINGW, $(shell uname -s)),MINGW)
os1 = $(subst $(space),-,$(shell echo $$(uname -s)_$$(uname -r)))
ifeq ($(tgarch),i686)
os = $(shell echo $(os1) | sed 's/x86_64/i686/g')
else
os = $(os1)
endif
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
static = a
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
#
else
ifeq ($(shell uname -s),Darwin)
#macos
nulldir =  /dev/null
binflags = -lutfchar -L../assets/$(arch2)
admin = sudo$(space)
adminend =
staticgen = libtool -static -o$(space)
run = ./
os_name = osx-$(subst 86_,,$(arch))
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
binflags = -pg
ifneq ($(shell which sudo 2> /dev/null),$(empty))
	admin = sudo$(space)
	adminend =
else
ifneq ($(shell which doas 2> /dev/null),$(empty))
	admin = doas$(space)
	adminend =
else
ifneq ($(shell which su 2> /dev/null),$(empty))
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
endif
endif
endif

i686cygwin = 
ifeq ($(findstring CYGWIN, $(shell uname -s)),CYGWIN)
ifeq ($(arch),i686)
i686cygwin = 1
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

ifeq ($(msvc),1)
ifeq ($(debug),1)
build-type = Debug-MSVC-$(arch)
else
build-type = Release-MSVC-$(arch)
endif
else
ifeq ($(debug),1)
build-type = Debug-$(arch)
else
build-type = Release-$(arch)
endif
endif

ifeq ($(archchk),0)
check_arch =
else
check_arch = check-arch
endif

old_arch = $(shell cat __arch.dat 2> /dev/null || echo > __arch.dat)
ifneq ($(old_arch),$(build-type))
archfile = $(shell echo $(old_arch) > __oldarch.dat && echo $(build-type) > __arch.dat && echo __arch.dat)
else
archfile =
endif

package: release

check-arch: $(archfile)
	@echo "Build type changed from $(shell cat __oldarch.dat) to $(build-type) - Cleaning"
	-@rm __oldarch.dat
	@$(MAKE) clean debug=$(debug) tgarch=$(tgarch) msvc=$(msvc)
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

	@copy binaryplus\bin\$(binname).$(binary) bin\cpp
	@copy cplusplus\bin\$(dllname) bin\cpp

	@cd bin && ren cpp Cpp.$(release)

	@cd bin && powershell Invoke-WebRequest -Uri "https://github.com/leok7v/gnuwin32.mirror/raw/master/bin/zip.exe" -OutFile "zip.exe" -Verbose

	@zip -r bin/Cpp.$(release).$(os).zip bin/Cpp.$(release)

else
ifeq ($(wildcard release),release)
	@rm -rf bin
endif
	@mkdir -p bin/cpp
	@cp binaryplus/bin/$(binname).$(binary) bin/cpp
	@cp cplusplus/bin/$(dllname) bin/cpp

	@cd bin && mv cpp Cpp.$(release)

ifeq ($(findstring windows32, $(shell uname -s)),windows32)
	zip -r bin/Cpp.$(release).$(os).zip bin/Cpp.$(release)
else
ifeq ($(findstring CYGWIN, $(shell uname -s)),CYGWIN)
	zip -r bin/Cpp.$(release).$(os).zip bin/Cpp.$(release)
else
ifeq ($(findstring NT, $(shell uname -s)),NT)
	zip -r bin/Cpp.$(release).$(os).zip bin/Cpp.$(release)
else
ifeq ($(shell uname -s),Darwin)
	cd bin && tar -czvf Cpp.$(release).$(os).tgz Cpp.$(release)
else
	cd bin && tar -czvf Cpp.$(release).$(os).tar.gz Cpp.$(release)
endif
endif
endif
endif
endif
	@echo "Version file. Remove to enable recompile" > $@

all: resources dll cppbin fixmintty-cygwin fixmintty-msys2
	@echo "Version file. Remove to enable recompile" > $@

dll: cpp
	@echo "Version file. Remove to enable recompile" > $@

clean: cppclean

cppclean:
ifeq ($(shell echo "check quotes"),"check quotes")
	@del /f cppbin
	@del /f compile-cppbin
	@del /f cpp
	@del /f resources
	@del /f cplusplus\obj\* 
	@del /f binaryplus\obj\*
	@del /f objects\*
else
	@rm -f compile-cppbin
	@rm -f cppbin
	@rm -f cpp
	@rm -f resources
	@rm -rf cplusplus/obj/*
	@rm -rf binaryplus/obj/*
	@rm -rf objects/*
endif

fixmintty-cygwin: source/fixmintty.c
ifeq ($(findstring CYGWIN, $(shell uname -s)),CYGWIN)
	gcc -o binaryplus/bin/fixmintty/cygwin.exe source/fixmintty.c
	@echo "Version file. Remove to enable recompile" > $@
endif

fixmintty-msys2: source/fixmintty.c
ifeq ($(findstring MSYS, $(shell uname -s)),MSYS)
	gcc -o binaryplus/bin/fixmintty/msys2.exe source/fixmintty.c
	@echo "Version file. Remove to enable recompile" > $@
endif

resources: $(check_arch) source/setkbdmode.c source/killterm.c source/getfd.c source/getfd.h source/keyboard.h source/keyboard.m source/openfile.h source/openfile.m source/globals.c assets/a.tux source/ledctrl.c source/ledctrl.h source/utfchar.c source/mousefd.c source/mousefd.h source/killwindow.c source/resources.rc assets/images/icon.ico source/beep.c source/startprogram.c
	@echo MAKE RESOURCES
	
	-@mkdir objects/$(arch)/ assets/$(arch) cplusplus/obj/$(arch)/

ifneq ($(msvc),1)
ifeq ($(binary),exe)
	$(c-compiler) -c source/killwindow.c source/beep.c -pedantic -Wall -Wall -Wextra -Wpedantic -DUNICODE $(_cflags) $(cdb) -std=c2x && mv *.o objects/$(arch)/
	$(c-compiler) -o binaryplus/bin/killwindow.exe objects/$(arch)/killwindow.o $(clnk)
	$(c-compiler) -o binaryplus/bin/beep.exe objects/$(arch)/beep.o $(clnk)
endif
	$(c-compiler) -c source/globals.c -pedantic -Wall -Wall -Wextra -Wpedantic $(_cflags) $(cdb) -Isource -Icplusplus/include -std=c2x && mv *.o objects/$(arch)/
	$(staticgen)assets/$(arch)/libglobals.$(static) objects/$(arch)/globals.o
ifeq ($(universal2),1)
	-@mkdir objects/arm64/ objects/universal2/ assets/arm64/ assets/universal2/ cplusplus/obj/arm64/ cplusplus/obj/universal2/
	$(c-compiler) -c source/globals.c -pedantic -Wall -Wextra -Wpedantic $(cflags) -arch arm64 $(cdb) -Isource -Icplusplus/include -std=c2x && mv *.o objects/arm64/
	$(staticgen)assets/arm64/libglobals.$(static) objects/arm64/globals.o
endif
else
	@echo "rc /nologo /fo objects\$(arch)\resources.res source\resources.rc" > run.bat
	@cmd.exe /c run.bat

	@echo "$(c-compiler) /c /DUNICODE /D_MSVC /D_CRT_SECURE_NO_DEPRECATE $(cdb) source/killwindow.c source/beep.c $(clstd)" > run.bat
	@cmd.exe /c run.bat
	@mv *.obj objects/$(arch)/

	@echo "link /OUT:binaryplus/bin/killwindow.exe objects/$(arch)/killwindow.obj USER32.lib" > run.bat
	@cmd.exe /c run.bat

	@echo "link /OUT:binaryplus/bin/beep.exe objects/$(arch)/beep.obj USER32.lib" > run.bat
	@cmd.exe /c run.bat

	@echo "$(cpp-compiler) /c /DUNICODE /D_MSVC $(cdb) source/globals.c /Icplusplus\include $(clstd)" > run.bat
	@cmd.exe /c run.bat
	@dir
	@mv *.obj objects/$(arch)/
	@echo "lib /OUT:assets/$(arch)/globals.lib objects/$(arch)/globals.obj" > run.bat
	@cmd.exe /c run.bat
	@rm run.bat
endif

#	echo $(c-compiler) -v -o $(prefix)std.$(dynamic) -pedantic -Wall -Wall -Wextra -Wpedantic -shared -fPIC -lm -static-libgcc 2>&1 | grep ld | sed s/-lc/'$$(find -O3 /usr/lib -name libc.a 2>&1 | grep $$(uname -m) | sed 1q)' | sed s/-lm/'$$(find -O3 /usr/lib -name libm.a 2>&1 | grep $$(uname -m) | sed 1q)'/g | sed s/-o/-Bsymbolic\ -o/g > temp.sh
#	@chmod +x temp.sh
#	./temp.sh
#	@rm temp.sh

ifeq ($(shell uname -s),Linux)
	-@rm *.o 2> $(nulldir)
	$(c-compiler) -c source/setkbdmode.c source/startprogram.c source/getfd.c source/ledctrl.c source/mousefd.c -pedantic -Wall -Wall -Wextra -Wpedantic $(_cflags) $(cdb) -Isource -Icplusplus/include -std=c2x && mv *.o objects/$(arch)/
	ar rcs assets/$(arch)/liblinuxctrl.$(static) objects/$(arch)/getfd.o objects/$(arch)/ledctrl.o objects/$(arch)/mousefd.o objects/$(arch)/setkbdmode.o
	$(c-compiler) -o assets/$(arch)/setkbdmode objects/$(arch)/setkbdmode.o -Lassets/$(arch) -llinuxctrl $(static-libc)
	$(c-compiler) -o assets/$(arch)/startprogram.bin objects/$(arch)/startprogram.o $(static-libc) -D_GNU_SOURCE
ifneq ($(offline),1)
	git submodule update --init --rebase --recursive --remote utilities/doas-keepenv
endif
ifeq ($(copylibs),1)
	@echo "$(linuxroot)/share/factoryrush/bin"
	$(admin)mkdir -p $(linuxroot)/share/factoryrush/bin$(adminend)
	$(admin)cp assets/$(arch2)/setkbdmode $(linuxroot)/share/factoryrush/bin$(adminend)
	$(admin)cp assets/$(arch2)/startprogram.bin $(linuxroot)/share/factoryrush/bin$(adminend)
	$(admin)cp utilities/doas-keepenv/doas-keepenv $(linuxroot)/share/factoryrush/bin/doas-keepenv.sh$(adminend)
	$(admin)cp utilities/doas-keepenv/doas-keepenv $(linuxbin)/doas-keepenv.sh$(adminend)

	$(admin)mkdir -p $(linuxroot)/share/factoryrush/assets$(adminend)
	$(admin)cp README.md $(linuxroot)/share/factoryrush/assets$(adminend)
else
	@mkdir -p binaryplus/bin/../share/factoryrush/bin
	@cp assets/$(arch2)/setkbdmode binaryplus/share/factoryrush/bin
	@cp utilities/doas-keepenv/doas-keepenv binaryplus/share/factoryrush/bin/doas-keepenv.sh
	@cp utilities/doas-keepenv/doas-keepenv binaryplus/bin/doas-keepenv.sh

	@mkdir -p binaryplus/bin/../share/factoryrush/assets
	@cp README.md binaryplus/share/factoryrush/assets
endif
else
ifeq ($(i686cygwin),1)
	$(c-compiler) -c source/utfchar.c -pedantic -Wall -Wextra -Wpedantic $(_cflags) $(cdb) -Isource && mv *.o objects/$(arch)/
	$(staticgen)assets/$(arch)/libutfchar.a objects/$(arch)/utfchar.o
endif

ifeq ($(shell uname -s),Darwin)
	$(c-compiler) -c source/utfchar.c -pedantic -Wall -Wextra -Wpedantic $(_cflags) $(cdb) -Isource  && mv *.o objects/$(arch)/
	$(staticgen)assets/$(arch)/libutfchar.a objects/$(arch)/utfchar.o
	$(c-compiler) -c source/keyboard.m source/openfile.m -framework CoreGraphics -framework CoreServices -pedantic -Wall -Wall -Wextra -Wpedantic $(_cflags) $(cdb) -Isource -Icplusplus/include -std=c2x && mv *.o objects/$(arch)/
	$(c-compiler) -dynamiclib $(archif) -o assets/$(arch)/libapplectrl.dylib objects/$(arch)/keyboard.o objects/$(arch)/openfile.o -framework CoreGraphics -framework CoreServices

	$(c-compiler) -c source/killterm.c -pedantic -Wall -Wextra -Wpedantic $(_cflags) $(cdb) -Isource -Icplusplus/include -std=c2x && mv *.o objects/$(arch)/
	$(c-compiler) $(archif) -o assets/$(arch)/killterm objects/$(arch)/killterm.o

ifeq ($(universal2),1)
	$(c-compiler) -c source/utfchar.c -pedantic -Wall -Wextra -Wpedantic $(_cflags) -arch arm64 $(cdb) -Isource && mv *.o objects/arm64/
	$(staticgen)assets/arm64/libutfchar.a objects/arm64/utfchar.o

	$(c-compiler) -c source/keyboard.m source/openfile.m -framework CoreGraphics -framework CoreServices -pedantic -Wall -Wall -Wextra -Wpedantic $(cflags) -arch arm64 $(cdb) -Isource -Icplusplus/include -std=c2x && mv *.o objects/arm64/
	$(c-compiler) -dynamiclib -arch arm64 -o assets/arm64/libapplectrl.dylib objects/arm64/keyboard.o objects/arm64/openfile.o -framework CoreGraphics -framework CoreServices
	lipo -create assets/$(arch)/libapplectrl.dylib assets/arm64/libapplectrl.dylib -output assets/universal2/libapplectrl.dylib

	$(c-compiler) -c source/killterm.c -pedantic -Wall -Wextra -Wpedantic $(cflags) -arch arm64 $(cdb) -Isource -Icplusplus/include -std=c2x && mv *.o objects/arm64/
	$(c-compiler) -arch arm64 -o assets/arm64/killterm objects/arm64/killterm.o
	lipo -create assets/$(arch)/killterm assets/arm64/killterm -output assets/universal2/killterm
endif
ifneq ($(offline),1)
	git submodule update --init --rebase --recursive --remote utilities/give-control
endif
ifeq ($(copylibs),1)
	$(admin)mkdir -p $(macosroot)/share/factoryrush/lib $(macosroot)/share/factoryrush/bin$(adminend)
	$(admin)cp assets/$(arch2)/libapplectrl.dylib $(macosroot)/share/factoryrush/lib$(adminend)
	$(admin)cp assets/$(arch2)/libapplectrl.dylib $(macoslib)$(adminend)
	$(admin)cp assets/$(arch2)/killterm $(macosroot)/share/factoryrush/bin$(adminend)
else
	@mkdir -p binaryplus/share/factoryrush/lib binaryplus/share/factoryrush/bin
	@cp assets/$(arch2)/libapplectrl.dylib binaryplus/share/factoryrush/lib
	@cp assets/$(arch2)/libapplectrl.dylib binaryplus/bin
	@cp assets/$(arch2)/killterm binaryplus/share/factoryrush/bin
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

cpp: $(resdep) $(foreach obj,$(objects),cplusplus/$(obj)) $(foreach head,$(headers),cplusplus/src/$(head)) $(foreach inc,$(includes),cplusplus/include/$(inc))
	@echo MAKE CPP

ifeq ($(msvc),1)
	echo "cd cplusplus && link /OUT:bin/$(name).dll $(ldb) /DLL $(objects) ../assets/$(arch)/globals.lib USER32.lib Gdi32.lib Shell32.lib Shlwapi.lib Dbghelp.lib" > run.bat
	@cmd.exe /c run.bat
	@rm run.bat
ifeq ($(debug),1)
	@cp cplusplus/bin/$(name).pdb binarysharp/bin/exe
	@cp cplusplus/bin/$(name).pdb binaryplus/bin
endif

ifeq ($(copylibs),1)
	$(admin)cp cplusplus/bin/$(dllname) $(libdir)$(adminend)
else
	@cp cplusplus/bin/$(dllname) binaryplus/bin
	@cp cplusplus/bin/$(dllname) binarysharp/bin/exe
endif

else
#
ifeq ($(binary),exe)
#windows
ifeq ($(shell uname -s),Windows_NT)
	cd cplusplus && $(cpp-compiler) -shared -o bin/$(name).dll $(objects) -L../assets/$(arch) -lglobals -ldbghelp -lshlwapi -lshell32 $(static-libc++) $(static-libc) $(ldarg)
else
ifeq ($(shell uname -s),windows32)
	cd cplusplus && $(cpp-compiler) -shared -o bin/$(name).dll $(objects) -L../assets/$(arch) -lglobals -ldbghelp -lshlwapi -lshell32 $(static-libc++) $(static-libc) $(ldarg)
else
ifeq ($(i686cygwin),1)
	cd cplusplus && $(cpp-compiler) -shared -o bin/$(name).dll $(objects) -L../assets/$(arch) -lglobals -ldbghelp -lshlwapi -lshell32 -lutfchar $(static-libc++) $(static-libc) $(ldarg)
else
	cd cplusplus && $(cpp-compiler) -shared -o bin/$(name).dll $(objects) -L../assets/$(arch) -lglobals -ldbghelp -lshlwapi -lshell32 $(static-libc++) $(static-libc) $(ldarg)
endif
endif
endif
#
else
ifeq ($(shell uname -s),Darwin)
#macos
	cd cplusplus && $(cpp-compiler) -dynamiclib $(archif) -o bin/lib$(name).dylib $(objects) -L../assets/$(arch2) -L../assets/$(arch) -L$(flibdir) -lutfchar -lapplectrl -lglobals $(static-libc++) $(static-libc) $(ldarg)
	utilities/custom/dylib-fix.sh "cplusplus/bin/lib$(name).dylib" "applectrl"

ifeq ($(universal2),1)
	cd cplusplus && $(cpp-compiler) -dynamiclib -arch arm64 -o bin/lib$(name).arm64.dylib $(foreach file,$(sources),obj/arm64/$(subst .c,.o,$(subst .cc,.c,$(subst .cpp,.cc,$(file))))) -L../assets/$(arch2) -L../assets/arm64 -L$(flibdir) -lutfchar -lapplectrl -lglobals $(static-libc++) $(static-libc) $(ldarg)
	utilities/custom/dylib-fix.sh "cplusplus/bin/lib$(name).arm64.dylib" "applectrl"

	lipo -create cplusplus/bin/lib$(name).dylib cplusplus/bin/lib$(name).arm64.dylib -output cplusplus/bin/lib$(name).dylib
endif
#
else
#linux and similar
#	cd cplusplus && $(cpp-compiler) -v -shared -o bin/lib$(name).so $(objects) -L../assets/$(arch) -L$(flibdir) -lglobals -llinuxctrl $(static-libc++) $(static-libc) $(ldarg) 2>&1 | grep ld | sed 's/-lc/$$(find -O3 \/usr\/lib -name libc.a 2>&1 | grep $$(uname -m) | sed 1q)'/g | sed 's/-lm/$$(find -O3 \/usr\/lib -name libm.a 2>&1 | grep $$(uname -m) | sed 1q)'/g | sed s/-o/-Bsymbolic\ -o/g > temp.sh && chmod +x temp.sh && ./temp.sh && rm temp.sh
	cd cplusplus && $(cpp-compiler) -shared -o bin/lib$(name).so $(objects) -L../assets/$(arch) -L$(flibdir) -lglobals -llinuxctrl $(static-libc++) $(static-libc) $(ldarg)
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
endif
else
#other
ifeq ($(copylibs),1)
	$(admin)cp cplusplus/bin/$(dllname) $(libdir)$(adminend)
else
	@cp cplusplus/bin/$(dllname) binaryplus/bin
	@cp cplusplus/bin/$(dllname) binarysharp/bin/exe
endif
endif
endif
	@echo "Version file. Remove to enable recompile" > $@

ifeq ($(msvc),1)
resources = objects/$(arch)/resources.res
else
resources =
endif

compile-cppbin: $(foreach src,$(binsources),binaryplus/src/$(src)) $(foreach head,$(binheaders),binaryplus/src/$(head)) $(foreach inc,$(binincludes),binaryplus/include/$(inc)) $(resources)
	@echo MAKE COMPILE CPPBIN

	-@mkdir binaryplus/obj/$(arch)/
ifeq ($(msvc),1)
	echo "$(cpp-compiler) /EHsc /c /DUNICODE $(bpdb) source/launcher.cpp $(clstdpp)" > run.bat
	@cmd.exe /c run.bat
	@$(movefl) -f launcher.obj objects
	echo "link /OUT:binaryplus/launcher.exe /CGTHREADS:8 objects/launcher.obj objects/$(arch)/resources.res" > run.bat
	@cmd.exe /c run.bat
	@rm run.bat

	echo "$(cpp-compiler) /EHsc /c $(bpdb) $(fbsrc) /Ibinaryplus\include $(clstdpp)" > run.bat
	@cmd.exe /c run.bat
	@$(movefl) -f $(subst obj/$(arch)/,$(empty),$(fbobj)) binaryplus/obj/$(arch)/
else
#all
	$(cpp-compiler) -c -pedantic -Wall -Wall -Wextra -Wpedantic $(_cxxflags) -DUNICODE -D_GNU_SOURCE -D_GNU_SOURCE $(bpdb) source/launcher.cpp -std=c++2b
	@$(movefl) -f launcher.o objects/$(arch)/
	$(cpp-compiler) $(archif) -o binaryplus/launcher.$(binary) objects/$(arch)/launcher.o $(static-libc++) $(static-libc) $(ldarg)

	$(cpp-compiler) -c -pedantic -Wall -Wextra -Wpedantic $(_cxxflags) $(bpdb) $(fbsrc) -I binaryplus/include -std=c++2b -D_GNU_SOURCE
	@$(movefl) -f $(subst obj/$(arch)/,$(empty),$(fbobj)) binaryplus/obj/$(arch)/

ifeq ($(universal2),1)
	mkdir binaryplus/obj/arm64/

	$(cpp-compiler) -c -pedantic -Wall -Wextra -Wpedantic $(cxxflags) -arch arm64 -DUNICODE -D_GNU_SOURCE $(bpdb) source/launcher.cpp -std=c++2b
	@$(movefl) -f launcher.o objects/arm64/
	$(cpp-compiler) -arch arm64 -o binaryplus/launcher.arm64.$(binary) objects/arm64/launcher.o $(static-libc++) $(static-libc) $(ldarg)
	lipo -create binaryplus/launcher.$(binary) binaryplus/launcher.arm64.$(binary) -output binaryplus/launcher.$(binary)

	$(cpp-compiler) -c -pedantic -Wall -Wall -Wextra -Wpedantic $(cxxflags) -arch arm64 $(bpdb) $(fbsrc) -I binaryplus/include -std=c++2b -D_GNU_SOURCE
	@$(movefl) -f $(subst obj/$(arch)/,$(empty),$(fbobj)) binaryplus/obj/arm64/
endif
endif
	@echo "Version file. Remove to enable recompile" > $@
	

cppbin: cpp compile-cppbin
	@echo MAKE CPPBIN
ifeq ($(msvc),1)
	echo "cd binaryplus && link /OUT:bin/$(binname).$(binary) $(bldb) ../cplusplus/bin/$(name).lib $(fbobj) USER32.lib ../objects/$(arch)/resources.res" > run.bat
	@cmd.exe /c run.bat
	@rm run.bat
else
	cd binaryplus && $(cpp-compiler) $(archif) -o bin/$(binname).$(binary) -L../assets/$(arch) $(fbobj) -L$(flibdir) -l$(name) $(static-libc++) $(static-libc) $(ldarg)
ifeq ($(universal2),1)
	cd binaryplus && $(cpp-compiler) -arch arm64 -o bin/$(binname).arm64.$(binary) $(binflags) -L../assets/arm64 $(foreach file,$(binsources),obj/arm64/$(subst .c,.o,$(subst .cc,.c,$(subst .cpp,.cc,$(file))))) -L$(flibdir) -l$(name) $(static-libc++) $(static-libc) $(ldarg) $(binflags)
endif
#

ifeq ($(shell uname -s),Darwin)
#macos
	utilities/custom/dylib-fix.sh "binaryplus/bin/$(binname).$(binary)" "$(name)"
ifeq ($(universal2),1)
	utilities/custom/dylib-fix.sh "binaryplus/bin/$(binname).arm64.$(binary)" "$(name)"
	lipo -create binaryplus/bin/$(binname).$(binary) binaryplus/bin/$(binname).arm64.$(binary) -output binaryplus/bin/$(binname).$(binary)
endif
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
	cd binaryplus\bin
endif
else
#other
ifeq ($(copylibs),1)
	$(admin)cp binaryplus/bin/$(binname).$(binary) $(bindir)$(adminend)
	$(admin)chown root $(bindir)/$(binname).$(binary) && $(admin)chmod u+s $(bindir)/$(binname).$(binary)$(adminend)
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


# .cpp
cplusplus/obj/$(arch)/%.$(objsuf): cplusplus/src/%.cpp $(foreach head,$(headers),cplusplus/src/$(head)) $(foreach inc,$(includes),cplusplus/include/$(inc))
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
	$(cpp-compiler) -c -pedantic -Wall -Wall -Wextra -Wpedantic $(_cxxflags) -DUNICODE $(cdb) $< -I cplusplus/include -std=c++2b
#
else
ifeq ($(shell uname -s),WINDOWS_NT)
#windows
	$(cpp-compiler) -c -pedantic -Wall -Wall -Wextra -Wpedantic $(_cxxflags) -DUNICODE $(cdb) $< -I cplusplus/include -std=c++2b
#
else
ifeq ($(findstring CYGWIN, $(shell uname -s)),CYGWIN)
#cygwin [ I think same as windows (?) ]
	$(cpp-compiler) -c -pedantic -Wall -Wall -Wextra -Wpedantic $(_cxxflags) -fPIC -DUNICODE -D_GNU_SOURCE $(cdb) $< -I cplusplus/include -std=c++2b
#
else
ifeq ($(findstring MINGW, $(shell uname -s)),MINGW)
#mingw [ I think same as windows (?) ]
	$(cpp-compiler) -c -pedantic -Wall -Wall -Wextra -Wpedantic $(_cxxflags) -fPIC -DUNICODE $(cdb) $< -I cplusplus/include -std=c++2b
#
else
ifeq ($(findstring Windows_NT, $(shell uname -s)),Windows_NT)
#msys [ i think older ]
	$(cpp-compiler) -c -pedantic -Wall -Wall -Wextra -Wpedantic $(_cxxflags) -fPIC -DUNICODE $(cdb) $< -I cplusplus/include -std=c++2b
else
ifeq ($(findstring MSYS, $(shell uname -s)),MSYS)
#msys [ I think same as windows (?) ]
	$(cpp-compiler) -c -pedantic -Wall -Wall -Wextra -Wpedantic $(_cxxflags) -fPIC -DUNICODE $(cdb) $< -I cplusplus/include -std=c++2b
else
# not windows
	$(cpp-compiler) -c -pedantic -Wall -Wall -Wextra -Wpedantic $(_cxxflags) -fPIC -DUNICODE $(macver) $(cdb) -fvisibility=hidden $< -I cplusplus/include -std=c++2b
endif
endif
endif
endif
endif
endif
endif
	@$(movefl) -f $(subst cplusplus/obj/$(arch)/,$(empty),$@) cplusplus/obj/$(arch)/
ifeq ($(universal2),1)
	$(cpp-compiler) -c -pedantic -Wall -Wextra -Wpedantic $(cxxflags) -arch arm64 -fPIC -DUNICODE $(macver) $(cdb) -fvisibility=hidden $< -I cplusplus/include -std=c++2b
	@$(movefl) -f $(subst cplusplus/obj/$(arch)/,$(empty),$@) cplusplus/obj/arm64/
endif
endif

# .c
cplusplus/obj/$(arch)/%.$(objsuf): cplusplus/src/%.c $(foreach head,$(headers),cplusplus/src/$(head)) $(foreach inc,$(includes),cplusplus/include/$(inc))
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
	$(c-compiler) -c -pedantic -Wall -Wall -Wextra -Wpedantic $(_cflags) -DUNICODE $(cdb) $< -I cplusplus/include -std=c2x
#
else
ifeq ($(shell uname -s),WINDOWS_NT)
#windows
	$(c-compiler) -c -pedantic -Wall -Wall -Wextra -Wpedantic $(_cflags) -DUNICODE $(cdb) $< -I cplusplus/include -std=c2x
#
else
ifeq ($(findstring CYGWIN, $(shell uname -s)),CYGWIN)
#cygwin [ I think same as windows (?) ]
	$(c-compiler) -c -pedantic -Wall -Wall -Wextra -Wpedantic $(_cflags) -fPIC -DUNICODE -D_GNU_SOURCE $(cdb) $< -I cplusplus/include -std=c2x
#
else
ifeq ($(findstring MINGW, $(shell uname -s)),MINGW)
#mingw [ I think same as windows (?) ]
	$(c-compiler) -c -pedantic -Wall -Wall -Wextra -Wpedantic $(_cflags) -fPIC -DUNICODE $(cdb) $< -I cplusplus/include -std=c2x
#
else
ifeq ($(findstring Windows_NT, $(shell uname -s)),Windows_NT)
#msys [ i think older ]
	$(c-compiler) -c -pedantic -Wall -Wall -Wextra -Wpedantic $(_cflags) -fPIC -DUNICODE $(cdb) $< -I cplusplus/include -std=c2x
else
ifeq ($(findstring MSYS, $(shell uname -s)),MSYS)
#msys [ I think same as windows (?) ]
	$(c-compiler) -c -pedantic -Wall -Wall -Wextra -Wpedantic $(_cflags) -fPIC -DUNICODE $(cdb) $< -I cplusplus/include -std=c2x
else
# not windows
	$(c-compiler) -c -pedantic -Wall -Wall -Wextra -Wpedantic $(_cflags) -fPIC -DUNICODE $(macver) $(cdb) -fvisibility=hidden $< -I cplusplus/include -std=c2x
endif
endif
endif
endif
endif
endif
endif
	@$(movefl) -f $(subst cplusplus/obj/$(arch)/,$(empty),$@) cplusplus/obj/$(arch)/
ifeq ($(universal2),1)
	$(c-compiler) -c -pedantic -Wall -Wextra -Wpedantic $(cflags) -arch arm64 -fPIC -DUNICODE $(macver) $(cdb) $< -I cplusplus/include -std=c2x
	@$(movefl) -f $(subst cplusplus/obj/$(arch)/,$(empty),$@) cplusplus/obj/arm64/
endif
endif
