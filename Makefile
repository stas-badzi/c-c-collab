# >>>> Arguments <<<<<<<<
# sudo = [0] -> do copy libs and biraries (requiers sudo permissions)
# forcewin = [1] -> use windows emulator like windows
# debug = [0] -> compile program for debbuging
# msvc = [0] -> force msvc++ compilation


#******* release config *********
#> release name
release = FactoryRush

#********************************

#******** c++ config ************
#> set default compilers
defcompcxx = c++
defcompc = cc
#> source files
sources = Console.cpp dllexport.cpp FileSystem.cpp System.cpp globals.c
#> header files
headers = Console.hpp FileSystem.hpp dllimport.hpp System.hpp System.ipp
#> include files
includes = dynamic_library.h unicode_conversion.hpp getfd.h quick_exit.h control_heap.h operating_system.h quick_exit/defines.h utils/cextern.h utils/dllalloc.h
#> name the dynamic library
name = factoryrushplus
# *******************************

#******** c++ binary config *****
#> source files
binsources = main.cpp Console.cpp FileSystem.cpp System.cpp Control.cpp
#> header files
binheaders = dllimport.hpp Console.hpp FileSystem.hpp System.hpp defines.h Control.hpp
#> include files
binincludes = dynamic_library.h unicode_conversion.hpp control_heap.h utils/cextern.h control_heap.h utils/dllalloc.h
#> name the binary file
binname = cpp-factoryrush
#********************************

#********* c# config ************
#> name the dynamic library
filename = factoryrushsharp
#>source code files
files = DllExport.cs DllImport.cs FileSystem.cs Terminal.cs Console.cs Utility.cs Exec.cs Control.cs
# *******************************

#********* c# binary config *****
#> name the binary file
binfile = cs-factoryrush
#>source code files
binfiles = Program.cs DllImport.cs FileSystem.cs Terminal.cs Utility.cs Exec.cs
# *******************************

#***** application config ****
#> linux root of application path
linuxroot = /usr
#> linux shared library path
linuxlib = /usr/lib
#> linux binary file path
linuxbin = /usr/bin
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

ifeq ($(sudo),1)
copylibs = 1
else
copylibs = 0
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

ifeq ($(cpp-compiler),$(empty))
cpp-compiler = $(defcompcxx)
endif

ifeq ($(c-compiler),$(empty))
c-compiler = $(defcompc)
endif

ifeq ($(msvc),1)
cpp-compiler = cl
c-compiler = cl
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

ifeq ($(debug),1)
configuration = Debug
binconfig = Debug
ifeq ($(msvc),1)
cdb = /MDd /Z7
ldb = /DEBUG /PDB:bin/$(name).pdb
bldb = /DEBUG /PDB:bin/$(binname).pdb
bpdb = /MTd /Z7
else
cdb = -g -D_DEBUG
bpdb = -g -D_DEBUG
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
cdb = -O3
bpdb = -O3
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
admin = sudo
prefix = .\
os_name = win-$(arch)
dllname = "$(name).dll"
libname = "$(filename).dll"
binary = exe
libdir = $(winlib)
bindir = $(winbin)
#
else
ifeq ($(shell uname -s),WINDOWS_NT)
#windows i think
admin = sudo
prefix = .\
os_name = win-$(arch)
dllname = "$(name).dll"
libname = "$(filename).dll"
binary = exe
libdir = $(winlib)
bindir = $(winbin)
#
else
ifeq ($(findstring NT, $(shell uname -s)),NT)
#
ifeq ($(findstring CYGWIN, $(shell uname -s)),CYGWIN)
#cygwin only
libdir = $(cygwinlib)
bindir = $(cygwinbin)
admin = sudo
#
else
# msys mingw and others
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
libdir = $(msyslib)
bindir = $(msysbin)
admin = sudo
#
endif
#all unix emulators on windows
prefix = ./
os_name = win-$(arch)
dllname = "$(name).dll"
libname = "$(filename).dll"
binary = exe
#
else
ifeq ($(shell uname -s),Darwin)
#macos
admin = sudo
prefix = ./
os_name = osx-$(arch)
dllname = "lib$(name).dylib"
libname = "lib$(filename).dylib"
binary = app
libdir = $(macoslib)
bindir = $(macosbin)
#
else
#linux and similar[other]
admin = sudo
prefix = ./
os_name = linux-$(arch)
libname = "lib$(filename).so"
dllname = "lib$(name).so"
binary = bin
libdir = $(linuxlib)
bindir = $(linuxbin)
#
endif
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

package: release

release: all
ifeq ($(shell echo "check quotes"),"check quotes")

ifneq ($(wildcard release),release)
	@del /f bin/*/*
	@del /f bin/*.zip
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
	@cd bin && zip -r Cpp.$(release).$(os).zip Cpp.$(release)
	@cd bin && zip -r Cs.$(release).$(os).zip Cs.$(release)
else
ifeq ($(findstring CYGWIN, $(shell uname -s)),CYGWIN)
	@cd bin && zip -r Cpp.$(release).$(os).zip Cpp.$(release)
	@cd bin && zip -r Cs.$(release).$(os).zip Cs.$(release)
else
ifeq ($(findstring NT, $(shell uname -s)),NT)
	@cd bin && zip -r Cpp.$(release).$(os).zip Cpp.$(release)
	@cd bin && zip -r Cs.$(release).$(os).zip Cs.$(release)
else
ifeq ($(shell uname -s),Darwin)
	@cd bin && tar -czvf Cpp.$(release).$(os).tgz Cpp.$(release)
	@cd bin && tar -czvf Cs.$(release).$(os).tgz Cs.$(release)
else
	@cd bin && tar -czvf Cpp.$(release).$(os).tar.gz Cpp.$(release)
	@cd bin && tar -czvf Cs.$(release).$(os).tar.gz Cs.$(release)
endif
endif
endif
endif
endif
	@echo "Version file. Remove to enable recompile" > $@

all: dll cppbin csbin resources
	@echo "Version file. Remove to enable recompile" > $@

dll: cs cpp
	@echo "Version file. Remove to enable recompile" > $@

refresh:
ifeq ($(shell echo "check quotes"),"check quotes")
	@del /f cs
	@del /f csbin
	@del /f cppbin
	@del /f cpp
	@del /f resources
else
	@rm -f cs
	@rm -f csbin
	@rm -f cppbin
	@rm -f cpp
	@rm -f resources
endif

resources: source/getfd.h source/setkbdmode.c assets/a.tux
ifeq ($(shell uname -s),Linux)
	@$(c-compiler) -o assets/setkbdmode.$(binary) $(cdb) source/setkbdmode.c -Isource -Wimplicit-function-declaration -std=c2x -static-libgcc
ifeq ($(copylibs),1)
	@echo "$(linuxroot)/share/factoryrush/bin"
	@$(admin) mkdir -p $(linuxroot)/share/factoryrush/bin
	@$(admin) cp assets/setkbdmode.$(binary) $(linuxroot)/share/factoryrush/bin
else
	@mkdir -p binaryplus/bin/../share/factoryrush/bin
	@cp assets/setkbdmode.$(binary) binaryplus/share/factoryrush/bin
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
	wt -f "$(dir)\binaryplus\bin\$(binname).$(binary)"
else
	@cd binaryplus/bin && $(prefix)$(binname).$(binary)
endif

csrun:
	@cd binarysharp/bin/exe && $(prefix)$(binfile).$(binary)

cpp: $(foreach src,$(sources),cplusplus/src/$(src)) $(foreach head,$(headers),cplusplus/src/$(head)) $(foreach inc,$(includes),cplusplus/include/$(inc))


ifneq ($(wildcard cs),cs)
	@$(MAKE) cs sudo=$(sudp) forcewin=$(forcewin) debug=$(debug)
endif

ifeq ($(msvc),1)
	@echo "$(cpp-compiler) /EHsc /c /DUNICODE /D_MSVC $(cdb) $(fsrc) /Icplusplus\include /std:c++latest" > run.bat
	@run.bat
	@echo $(foreach obj,$(subst obj/,$(empty),$(objects)),&& $(movefl) -f $(obj) cplusplus/obj$(space))
	@echo "cd cplusplus && link /OUT:bin/$(name).dll $(ldb) /DLL $(flib) $(objects) USER32.lib Gdi32.lib" > run.bat
	@run.bat
	@rm run.bat
ifeq ($(debug),1)
	@cp cplusplus/bin/$(filename).pdb binarysharp/bin/exe
	@cp cplusplus/bin/$(filename).pdb binaryplus/bin
	@cp cplusplus/bin/$(filename).pdb csharp/bin/lib
endif

ifeq ($(copylibs),1)
	@$(admin) cp cplusplus/bin/$(dllname) $(libdir)
else
	@cp cplusplus/bin/$(dllname) binaryplus/bin
	@cp cplusplus/bin/$(dllname) binarysharp/bin/exe
	@cp cplusplus/bin/$(dllname) csharp/bin/lib
endif

else
#
ifeq ($(findstring windows32, $(shell uname -s)),windows32)
#windows
	@$(cpp-compiler) -c -Wall -DUNICODE $(cdb) $(fsrc) -I cplusplus/include -std=c++2b $(foreach obj,$(subst obj/,$(empty),$(objects)),&& $(movefl) -f $(obj) cplusplus/obj$(space))
	@cd cplusplus && $(cpp-compiler) -shared -o bin/$(name).dll $(objects) -L$(flibdir) $(flib) -static-libstdc++ -static-libgcc -lGdi32 $(ldarg)
#
else
ifeq ($(shell uname -s),WINDOWS_NT)
#windows
	@$(cpp-compiler) -c -Wall -DUNICODE $(cdb) $(fsrc) -I cplusplus/include -std=c++2b $(foreach obj,$(subst obj/,$(empty),$(objects)),&& $(movefl) -f $(obj) cplusplus/obj$(space))
	@cd cplusplus && $(cpp-compiler) -shared -o bin/$(name).dll $(objects) -L$(flibdir) $(flib) -static-libstdc++ -static-libgcc -lGdi32 $(ldarg)
#
else
ifeq ($(findstring CYGWIN, $(shell uname -s)),CYGWIN)
#cygwin [ I think same as windows (?) ]
	@$(cpp-compiler) -c -Wall -fPIC -DUNICODE $(cdb) $(fsrc) -I cplusplus/include -std=c++2b $(foreach obj,$(subst obj/,$(empty),$(objects)),&& $(movefl) -f $(obj) cplusplus/obj$(space))
	@cd cplusplus && $(cpp-compiler) -shared -o bin/$(name).dll $(objects) -L$(flibdir) $(flib) -static-libstdc++ -static-libgcc -lGdi32 $(ldarg)
#
else
ifeq ($(findstring MINGW, $(shell uname -s)),MINGW)
#mingw [ I think same as windows (?) ]
	@$(cpp-compiler) -c -Wall -fPIC -DUNICODE $(cdb) $(fsrc) -I cplusplus/include -std=c++2b $(foreach obj,$(subst obj/,$(empty),$(objects)),&& $(movefl) -f $(obj) cplusplus/obj$(space))
	@cd cplusplus && ls && cd obj && ls && cd .. && $(cpp-compiler) -shared -o bin/$(name).dll $(objects) -L$(flibdir) $(flib) -static-libstdc++ -static-libgcc -lGdi32 $(ldarg)
#
else
ifeq ($(findstring Windows_NT, $(shell uname -s)),Windows_NT)
#msys [ i think older ]
	@$(cpp-compiler) -c -Wall -fPIC -DUNICODE $(cdb) $(fsrc) -I cplusplus/include -std=c++2b $(foreach obj,$(subst obj/,$(empty),$(objects)),&& $(movefl) -f $(obj) cplusplus/obj$(space))
	@cd cplusplus && $(cpp-compiler) -shared -o bin/$(name).dll $(objects) -L$(flibdir) $(flib) -static-libstdc++ -static-libgcc -lGdi32 $(ldarg)
#
else
ifeq ($(findstring MSYS, $(shell uname -s)),MSYS)
#msys [ I think same as windows (?) ]
	@$(cpp-compiler) -c -Wall -fPIC -DUNICODE $(cdb) $(fsrc) -I cplusplus/include -std=c++2b $(foreach obj,$(subst obj/,$(empty),$(objects)),&& $(movefl) -f $(obj) cplusplus/obj$(space))
	@cd cplusplus && ls && cd obj && ls && cd .. && $(cpp-compiler) -shared -o bin/$(name).dll $(objects) -L$(flibdir) $(flib) -static-libstdc++ -static-libgcc -lGdi32 $(ldarg)
#
else
# not windows
#	@cd cplusplus/obj && $(cpp-compiler) -c -Wall -fPIC -DUNICODE $(cdb) -fvisibility=hidden $(fsrc) -I ../include -std=c++2b
	@$(cpp-compiler) -c -Wall -fPIC -DUNICODE $(cdb) -fvisibility=hidden $(fsrc) -I cplusplus/include -std=c++2b $(foreach obj,$(subst obj/,$(empty),$(objects)),&& $(movefl) -f $(obj) cplusplus/obj$(space))
ifeq ($(shell uname -s),Darwin)
#macos
	@cd cplusplus && $(cpp-compiler) -dynamiclib -o bin/lib$(name).dylib $(objects) -L$(flibdir) $(flib) -static-libstdc++ -static-libgcc $(ldarg)
#
else
#linux and similar
	@cd cplusplus && $(cpp-compiler) -shared -o bin/lib$(name).so $(objects) -L$(flibdir) $(flib) -static-libstdc++ -static-libgcc $(ldarg)
endif
endif
endif
endif
endif
endif
endif
#

ifeq ($(shell echo "check quotes"),"check quotes")
#windows
ifeq ($(copylibs),1)
	@$(admin) copy cplusplus\bin\$(dllname) $(libdir)
else
	@copy cplusplus\bin\$(dllname) binaryplus\bin
	@copy cplusplus\bin\$(dllname) binarysharp\bin\exe
	@copy cplusplus\bin\$(dllname) csharp\bin\lib
endif
else
#other
ifeq ($(copylibs),1)
	@$(admin) cp cplusplus/bin/$(dllname) $(libdir)
else
	@cp cplusplus/bin/$(dllname) binaryplus/bin
	@cp cplusplus/bin/$(dllname) binarysharp/bin/exe
	@cp cplusplus/bin/$(dllname) csharp/bin/lib
endif
endif
endif
	@echo "Version file. Remove to enable recompile" > $@


cs: $(foreach fl,$(files),csharp/$(fl))

ifneq ($(wildcard resources),resources)
	@$(MAKE) resources sudo=$(sudp) forcewin=$(forcewin) debug=$(debug)
endif
	@cd csharp && dotnet publish -p:NativeLib=Shared -p:SelfContained=true -r $(os_name) -c $(configuration)

ifeq ($(msvc),1)
	@cd csharp/bin/$(arch)/$(configuration)/net8.0/$(os_name)/native/ && for i in *.exp; do if [ ! "$$i" = '$(filename).exp' ]; then mv $$i $(filename).exp; fi; done && for i in *.lib; do if [ ! "$$i" = '$(filename).lib' ]; then mv $$i $(filename).lib; fi; done && for i in *.pdb; do if [ ! "$$i" = '$(filename).pdb' ]; then mv $$i $(filename).pdb; fi; done && for i in *.dll; do if [ ! "$$i" = '$(filename).dll' ]; then mv $$i $(filename).dll; fi; done 
	@mv csharp/bin/$(arch)/$(configuration)/net8.0/$(os_name)/native/* csharp/bin/lib
ifeq ($(debug),1)
	@cp csharp/bin/lib/$(filename).pdb binarysharp/bin/exe
	@cp csharp/bin/lib/$(filename).pdb binaryplus/bin
	@cp csharp/bin/lib/$(filename).pdb cplusplus/bin
endif
ifeq ($(copylibs),1)
	@$(admin) cp csharp/bin/lib/$(libname) $(libdir)
else
	@cp csharp/bin/lib/$(libname) binarysharp/bin/exe
	@cp csharp/bin/lib/$(libname) binaryplus/bin
	@cp csharp/bin/lib/$(libname) cplusplus/bin
endif
else
ifeq ($(shell echo "check quotes"),"check quotes")
	@cd csharp/bin/$(configuration)/net8.0/$(os_name)/native/ && echo . > null.exp && echo . > null.lib && echo . > null.pdb && del *.exp && del *.lib && del *.pdb && ren * $(libname)
	@move csharp\bin\$(configuration)\net8.0\$(os_name)\native\$(libname) csharp\bin\lib
ifeq ($(copylibs),1)
	@$(admin) copy csharp\bin\lib\$(libname) $(libdir)
else
	@copy csharp\bin\lib\$(libname) binarysharp\bin\exe
	@copy csharp\bin\lib\$(libname) binaryplus\bin
	@copy csharp\bin\lib\$(libname) cplusplus\bin
endif
else
	@cd csharp/bin/$(configuration)/net8.0/$(os_name)/native/ && mkdir null.dSYM && touch null.dSYM/null.null && rm *.dSYM/* && rmdir *.dSYM && touch null.dbg && touch null.exp && touch null.lib && touch null.pdb && rm *.dbg && rm *.exp && rm *.lib && rm *.pdb
	@mv -f csharp/bin/$(configuration)/net8.0/$(os_name)/native/* csharp/bin/lib/$(libname)
ifeq ($(copylibs),1)
	@$(admin) cp csharp/bin/lib/$(libname) $(libdir)
else
	@cp csharp/bin/lib/$(libname) binarysharp/bin/exe
	@cp csharp/bin/lib/$(libname) binaryplus/bin
	@cp csharp/bin/lib/$(libname) cplusplus/bin
endif
endif
endif
	@echo "Version file. Remove to enable recompile" > $@

cppbin: $(foreach src,$(binsources),binaryplus/src/$(src)) $(foreach head,$(binheaders),binaryplus/src/$(head)) $(foreach inc,$(binincludes),binaryplus/include/$(inc))
	
ifneq ($(wildcard cpp),cpp)
	@$(MAKE) cpp sudo=$(sudp) forcewin=$(forcewin) debug=$(debug)
endif

ifeq ($(msvc),1)
	@echo "$(cpp-compiler) /EHsc /c $(bpdb) $(fbsrc) /Ibinaryplus\include /std:c++latest" > run.bat
	@run.bat
	@echo $(foreach obj,$(subst obj/,$(empty),$(fbobj)),&& $(movefl) -f $(obj) binaryplus/obj$(space))
	@echo "cd binaryplus && link /OUT:bin/$(binname).$(binary) $(bldb) $(flib) ../cplusplus/bin/$(name).lib ../csharp/bin/lib/$(filename).lib $(fbobj) USER32.lib" > run.bat
	@run.bat
	@rm run.bat
else
#all
	@$(cpp-compiler) -c -Wall $(bpdb) $(fbsrc) -I binaryplus/include -std=c++2b $(foreach obj,$(subst obj/,$(empty),$(fbobj)),&& $(movefl) -f $(obj) binaryplus/obj$(space))
	@cd binaryplus && $(cpp-compiler) -o bin/$(binname).$(binary) $(fbobj) -L$(flibdir) -l$(name) $(flib) -static-libstdc++ -static-libgcc $(ldarg)
#

ifeq ($(shell uname -s),Darwin)
#macos
	@cd binaryplus/bin && install_name_tool -change $$(otool -l $(binname).$(binary) | grep $(dllname) | sed 's/ (offset 24)//' | sed 's/         name //') @loader_path/$(dllname) $(binname).$(binary)
	@cd binaryplus/bin && install_name_tool -change $$(otool -l $(binname).$(binary) | grep $(filename).dylib | sed 's/ (offset 24)//' | sed 's/         name //') @loader_path/$(libname) $(binname).$(binary)
#
endif
endif

ifeq ($(shell echo "check quotes"),"check quotes")
#windows
ifeq ($(copylibs),1)
	@$(admin) copy binaryplus\bin\$(binname).$(binary) $(bindir)
else
	@cd binaryplus\bin && dir
endif
else
#other
ifeq ($(copylibs),1)
	@$(admin) cp binaryplus/bin/$(binname).$(binary) $(bindir)
else
	@cd binaryplus/bin && ls
endif
endif
	@echo "Version file. Remove to enable recompile" > $@

csbin: $(foreach bfl,$(binfiles),binarysharp/$(bfl))
	@cd binarysharp && dotnet publish -p:SelfContained=true -r $(os_name) -c $(binconfig)
ifeq ($(msvc),1)
	@cd binarysharp/bin/$(arch)/$(configuration)/net8.0/$(os_name)/native/ && for i in *.$(binary); do if [ ! "$$i" = '$(binname).$(binary)' ]; then mv $$i $(binname).$(binary); fi; done 
	@mv binarysharp/bin/$(arch)/$(configuration)/net8.0/$(os_name)/native/* binarysharp/bin/exe
ifeq ($(copylibs),1)
	@$(admin) cp binarysharp/bin/exe/$(binfile).$(binary) $(bindir)
else
endif
else

ifeq ($(shell echo "check quotes"),"check quotes")
	@cd binarysharp/bin/$(binconfig)/net8.0/$(os_name)/native/ && echo . > null.exp && echo . > null.lib && echo . > null.pdb && del *.exp && del *.lib && del *.pdb && ren * $(binfile).$(binary)
	@copy binarysharp\bin\$(binconfig)\net8.0\$(os_name)\native\$(binfile).$(binary) binarysharp\bin\exe
	@del binarysharp\bin\$(binconfig)\net8.0\$(os_name)\native\$(binfile).$(binary)
else
	@cd binarysharp/bin/$(binconfig)/net8.0/$(os_name)/native/ && mkdir null.dSYM && touch null.dSYM/null.null && rm *.dSYM/* && rmdir *.dSYM && touch null.dbg && touch null.exp && touch null.lib && touch null.pdb && rm *.dbg && rm *.exp && rm *.lib && rm *.pdb
	@cp -f binarysharp/bin/$(binconfig)/net8.0/$(os_name)/native/* binarysharp/bin/exe/$(binfile).$(binary)
	@rm -f binarysharp/bin/$(binconfig)/net8.0/$(os_name)/native/*
endif

ifeq ($(shell echo "check quotes"),"check quotes")
#windows
ifeq ($(copylibs),1)
	@$(admin) copy binarysharp\bin\exe\$(binfile).$(binary) $(bindir)
else
	@cd binarysharp\bin\exe && dir
endif
else
#other
ifeq ($(copylibs),1)
	@$(admin) cp binarysharp/bin/exe/$(binfile).$(binary) $(bindir)
else
	@cd binarysharp/bin/exe && ls
endif
endif
endif
	@echo "Version file. Remove to enable recompile" > $@
