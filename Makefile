# >>>> Arguments <<<<<<<<
# sudo = [0] -> do copy libs and biraries (requiers sudo permissions)
# forcewin = [1] -> use windows emulator like windows
# debug = [0] -> compile program for debbuging


#******* release config *********
#> release name
release = FactoryRush

#********************************

#******** c++ config ************
#> set default compiler
defcomp = c++
#> source files
sources = Console.cpp dllexport.cpp
#> header files
headers = Console.hpp
#> include files
includes = dynamic_library.h unicode_conversion.hpp getfd.h
#> name the dynamic library
name = cplusplus
# *******************************

#******** c++ binary config *****
#> source files
binsources = main.cpp Console.cpp FileSystem.cpp
#> header files
binheaders = dllimport.hpp Console.hpp FileSystem.hpp
#> include files
binincludes = dynamic_library.h unicode_conversion.hpp
#> name the binary file
binname = cpp-consolegame
#********************************

#********* c# config ************
#> name the dynamic library
filename = csharp
#>source code files
files = DllExport.cs DllImport.cs FileSystem.cs Terminal.cs Utility.cs
# *******************************

#********* c# binary config *****
#> name the binary file
binfile = cs-consolegame
#>source code files
binfiles = Program.cs DllImport.cs FileSystem.cs Terminal.cs Utility.cs
# *******************************

#***** application config ****
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


ifeq ($(linker),$(empty))
ldarg = 
else
ldarg = -fuse-ld=$(linker)
endif

ifeq ($(compiler),$(empty))
compiler = $(defcomp)
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
cdb = -g
bpdb = -g
else
configuration = Release
binconfig = Release
cdb = -O3
bpdb = -O3
endif

flibdir = bin
flib = -l$(filename)
fsrc = $(foreach src,$(sources),../src/$(src))
wfsrc = $(foreach src,$(sources),src/$(src))
objects = $(foreach file,$(sources),obj/$(subst .c,.o,$(subst .cc,.c,$(subst .cpp,.cc,$(file)))))
fbsrc = $(foreach bsrc,$(binsources),../src/$(bsrc))
fbobj = $(foreach file,$(binsources),obj/$(subst .c,.o,$(subst .cc,.c,$(subst .cpp,.cc,$(file)))))
os = $(subst $(space),-,$(shell echo $$(uname -s) $$(uname -r).$$(uname -m)))

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

all: dll cppbin csbin
	@echo "Version file. Remove to enable recompile" > $@

dll: cs cpp
	@echo "Version file. Remove to enable recompile" > $@

refresh:
ifeq ($(shell echo "check quotes"),"check quotes")
	@del /f all
	@del /f dll
	@del /f cs
	@del /f csbin
	@del /f cppbin
	@del /f cpp
else
	@rm -f all
	@rm -f dll
	@rm -f cs
	@rm -f csbin
	@rm -f cppbin
	@rm -f cpp
endif

cpprun:
	@cd binaryplus/bin && $(prefix)$(binname).$(binary)

csrun:
	@cd binarysharp/bin/exe && $(prefix)$(binfile).$(binary)

cpp: $(foreach src,$(sources),cplusplus/src/$(src)) $(foreach head,$(headers),cplusplus/src/$(head)) $(foreach inc,$(includes),cplusplus/include/$(inc))


ifneq ($(wildcard cs),cs)
	@$(MAKE) cs sudo=$(sudp) forcewin=$(forcewin) debug=$(debug)
endif
#
ifeq ($(findstring windows32, $(shell uname -s)),windows32)
#windows
	@cd cplusplus/obj && $(compiler) -c -DUNICODE $(cdb) $(fsrc) -I ../include -std=c++20
	@cd cplusplus && $(compiler) -shared -static-libstdc++ -o bin/$(name).dll $(objects) -L$(flibdir) $(flib) $(ldarg)
#
else
ifeq ($(shell uname -s),WINDOWS_NT)
#windows
	@cd cplusplus/obj && $(compiler) -c -DUNICODE $(cdb) $(fsrc) -I ../include -std=c++20
	@cd cplusplus && $(compiler) -shared -static-libstdc++ -o bin/$(name).dll $(objects) -L$(flibdir) $(flib) $(ldarg)
#
else
ifeq ($(findstring CYGWIN, $(shell uname -s)),CYGWIN)
#cygwin [ I think same as windows (?) ]
	@cd cplusplus/obj && $(compiler) -c -DUNICODE $(cdb) $(fsrc) -I ../include -std=c++20
	@cd cplusplus && $(compiler) -shared -static-libstdc++ -o bin/$(name).dll $(objects) -L$(flibdir) $(flib) $(ldarg)
#
else
ifeq ($(findstring MINGW, $(shell uname -s)),MINGW)
#mingw [ I think same as windows (?) ]
	@cd cplusplus/obj && $(compiler) -c -DUNICODE $(cdb) $(fsrc) -I ../include -std=c++20
	@cd cplusplus && $(compiler) -shared -static-libstdc++ -o bin/$(name).dll $(objects) -L$(flibdir) $(flib) $(ldarg)
#
else
ifeq ($(findstring Windows_NT, $(shell uname -s)),Windows_NT)
#msys [ i think older ]
	@cd cplusplus/obj && $(compiler) -c -DUNICODE $(cdb) $(fsrc) -I ../include -std=c++20
	@cd cplusplus && $(compiler) -shared -static-libstdc++ -o bin/$(name).dll $(objects) -L$(flibdir) $(flib) $(ldarg)
#
else
ifeq ($(findstring MSYS, $(shell uname -s)),MSYS)
#msys [ I think same as windows (?) ]
	@cd cplusplus/obj && $(compiler) -c -DUNICODE $(cdb) $(fsrc) -I ../include -std=c++20
	@cd cplusplus && $(compiler) -shared -static-libstdc++ -o bin/$(name).dll $(objects) -L$(flibdir) $(flib) $(ldarg)
#
else
	@cd cplusplus/obj && $(compiler) -c -fpic -DUNICODE $(cdb) -fvisibility=hidden $(fsrc) -I ../include
ifeq ($(shell uname -s),Darwin)
#macos
	@cd cplusplus && $(compiler) -dynamiclib -static-libstdc++ -o bin/lib$(name).dylib $(objects) -L$(flibdir) $(flib) $(ldarg)
#
else
#linux and similar
	@cd cplusplus && $(compiler) -shared -static-libstdc++ -o bin/lib$(name).so $(objects) -L$(flibdir) $(flib) $(ldarg)
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
	@echo "Version file. Remove to enable recompile" > $@


cs: $(foreach fl,$(files),csharp/$(fl))
	@cd csharp && dotnet publish -p:NativeLib=Shared -p:SelfContained=true -r $(os_name) -c $(configuration)
ifeq ($(shell echo "check quotes"),"check quotes")
	@cd csharp/bin/$(configuration)/net8.0/$(os_name)/native/ && echo . > null.exp && echo . > null.lib && echo . > null.pdb && del *.exp && del *.lib && del *.pdb && ren * $(libname)
	@move csharp\bin\$(configuration)\net8.0\$(os_name)\native\$(libname) csharp\bin\lib
ifeq ($(copylibs),1)
	@$(admin) copy cplusplus\bin\$(dllname) $(libdir)
else
	@copy csharp\bin\lib\$(libname) binarysharp\bin\exe
	@copy csharp\bin\lib\$(libname) binaryplus\bin
	@copy csharp\bin\lib\$(libname) cplusplus\bin
	@cd csharp\bin\lib && dir
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
	@cd csharp/bin/lib && ls
endif
endif
	@echo "Version file. Remove to enable recompile" > $@

cppbin: $(foreach src,$(binsources),binaryplus/src/$(src)) $(foreach head,$(binheaders),binaryplus/src/$(head)) $(foreach inc,$(binincludes),binaryplus/include/$(inc))
	
ifneq ($(wildcard cpp),cpp)
	@$(MAKE) cpp sudo=$(sudp) forcewin=$(forcewin) debug=$(debug)
endif

ifeq ($(findstring windows32, $(shell uname -s)),windows32)
#windows
	@cd binaryplus/obj && $(compiler) -c $(bpdb) $(fbsrc) -I ../include -std=c++20
	@cd binaryplus && $(compiler) -static-libstdc++ -o bin/$(binname).$(binary) $(fbobj) -I include -L$(flibdir) -l$(filename) -l$(name) $(ldarg)
#
else
ifeq ($(shell uname -s),WINDOWS_NT)
#windows i think
	@cd binaryplus/obj && $(compiler) -c $(bpdb) $(fbsrc) -I ../include -std=c++20
	@cd binaryplus && $(compiler) -static-libstdc++ -o bin/$(binname).$(binary) $(fbobj) -I include -L$(flibdir) -l$(filename) -l$(name) $(ldarg)
#
else
ifeq ($(findstring CYGWIN, $(shell uname -s)),CYGWIN)
#cygwin [ I think same as windows (?) ]
	@cd binaryplus/obj && $(compiler) -c $(bpdb) $(fbsrc) -I ../include -std=c++20
	@cd binaryplus && $(compiler) -static-libstdc++ -o bin/$(binname).$(binary) $(fbobj) -I include -L$(flibdir) -l$(filename) -l$(name) $(ldarg)
#
else
ifeq ($(findstring Windows_NT, $(shell uname -s)),Windows_NT)
#msys2 [ I think same as windows (?) ]
	@cd binaryplus/obj && $(compiler) -c $(bpdb) $(fbsrc) -I ../include -std=c++20
	@cd binaryplus && $(compiler) -static-libstdc++ -o bin/$(binname).$(binary) $(fbobj) -I include -L$(flibdir) -l$(filename) -l$(name) $(ldarg)
else
ifeq ($(findstring MINGW, $(shell uname -s)),MINGW)
#mingw [ I think same as windows (?) ]
	@cd binaryplus/obj && $(compiler) -c $(bpdb) $(fbsrc) -I ../include -std=c++20
	@cd binaryplus && $(compiler) -static-libstdc++ -o bin/$(binname).$(binary) $(fbobj) -I include -L$(flibdir) -l$(filename) -l$(name) $(ldarg)
#
else
ifeq ($(findstring MSYS, $(shell uname -s)),MSYS)
#msys [ I think same as windows (?) ]
	@cd binaryplus/obj && $(compiler) -c $(bpdb) $(fbsrc) -I ../include -std=c++20
	@cd binaryplus && $(compiler) -static-libstdc++ -o bin/$(binname).$(binary) $(fbobj) -I include -L$(flibdir) -l$(filename) -l$(name) $(ldarg)
#
else
#	@cd binaryplus/obj && $(compiler) -c -fpic -DUNICODE -fvisibility=hidden $(fsrc) -I ../include
ifeq ($(shell uname -s),Darwin)
#macos
	@cd binaryplus/obj && $(compiler) -c $(bpdb) $(fbsrc) -I ../include -std=c++20
	@cd binaryplus && $(compiler) -static-libstdc++ -o bin/$(binname).$(binary) $(fbobj) -I include -L$(flibdir) -l$(filename) -l$(name) $(ldarg)
	@cd binaryplus/bin && install_name_tool -change $$(otool -l $(binname).$(binary) | grep $(dllname) | sed 's/ (offset 24)//' | sed 's/         name //') @loader_path/$(dllname) $(binname).$(binary)
	@cd binaryplus/bin && install_name_tool -change $$(otool -l $(binname).$(binary) | grep $(filename).dylib | sed 's/ (offset 24)//' | sed 's/         name //') @loader_path/$(libname) $(binname).$(binary)
#
else
#linux and similar
	@cd binaryplus/obj && $(compiler) -c $(bpdb) $(fbsrc) -I ../include -std=c++20
	@cd binaryplus && $(compiler) -static-libstdc++ -o bin/$(binname).$(binary) $(fbobj) -I include -L$(flibdir) -l$(filename) -l$(name) $(ldarg)
endif
endif
endif
endif
endif
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
	@echo "Version file. Remove to enable recompile" > $@
