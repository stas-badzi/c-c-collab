#******* release config *********
#> release name
release = Consolegame

#********************************

#******** c++ config ************
#> set compiler
compiler = g++
#> source files
sources = Console.cpp dllexport.cpp
#> header files
headers = Console.hpp
#> include files
includes = dynamic_library.h
#> name the dynamic library
name = cplusplus
#> Debug?
cdebug = 1
# *******************************

#******** c++ binary config *****
#> source files
binsources = main.cpp Console.cpp FileSystem.cpp
#> header files
binheaders = dllimport.hpp Console.hpp FileSystem.hpp
#> include files
binincludes = dynamic_library.h
#> name the binary file
binname = cpp-consolegame
#> Debug?
bpdebug = 1
#********************************

#********* c# config ************
#> name the dynamic library
filename = csharp
#> compilation mode
configuration = Release
#>source code files
files = DllExport.cs FileSystem.cs
# *******************************

#********* c# binary config *****
#> name the binary file
binfile = cs-consolegame
#>compilation mode
binconfig = Release
#>source code files
binfiles = Program.cs DllImport.cs FileSystem.cs Console.cs
# *******************************

#***** shared library config ****
#> linux shared library path
linuxlib = /usr/lib
#> macos shared library path
macoslib = /usr/local/lib
#> windows shared library path
winlib = %SystemRoot%
#> mysys(mingw) shared library path
msyslib = /usr/lib
#>cygwin shared library path
cygwinlib = /usr/lib
#>cygwin path to windows
symcyglib = /cygdrive/c/Windows
#>msys2(mingw) path to windows
symsyslib = /c/Windows
#*********************************

ifeq ($(sudo),1) 
copylibs = 1
else
copylibs = 0
endif

ifeq ($(findstring arm, $(shell uname -m)),arm)
arch = arm64
else
ifeq ($(findstring aarch, $(shell uname -m)),aarch)
arch = arm64
else
arch = x64
endif
endif

ifeq ($(force-win),1) 
genwin = 1
else
genwin = 0
endif

ifeq ($(cdebug),1)
cdb = -g
endif
ifeq ($(bpdebug),1)
bpdb = -g
endif
flibdir = bin
flib = -l$(filename)
fsrc = $(foreach src,$(sources),../src/$(src))
wfsrc = $(foreach src,$(sources),src/$(src))
fbsrc = $(foreach bsrc,$(binsources),src/$(bsrc))
objects = $(foreach file,$(sources),obj/$(subst .c,.o,$(subst .cc,.c,$(subst .cpp,.cc,$(file)))))
os = $(shell echo $$(uname -s)-$$(uname -m))

ifeq ($(findstring indows, $(shell uname -s)),indows)
#windows
admin = sudo
prefix = .\
os_name = win-$(arch)
dllname = "$(name).dll"
libname = "$(filename).dll"
binary = exe
libdir = $(winlib)
#
else
ifeq ($(findstring NT, $(shell uname -s)),NT)
#
ifeq ($(findstring CYGWIN, $(shell uname -s)),CYGWIN)
#cygwin only
libdir = $(cygwinlib)
admin = sudo
#
else
# msys mingw and others
ifeq ($(findstring MSYS, $(shell uname -s)),MSYS)
#msys
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
#non-cygwin 'NT'
libdir = $(msyslib)
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
#
endif
endif
endif

ifeq ($(genwin),1)
ifeq ($(findstring CYGWIN, $(shell uname -s)),CYGWIN)
libdir = $(symcyglib)
else
ifeq ($(findstring NT, $(shell uname -s)),NT)
libdir = $(symsyslib)
endif
endif
endif

ifeq ($(copylibs),1)
flibdir = $(libdir)
endif

release: all
ifeq ($(findstring indows, $(shell uname -s)),indows)
	@mkdir bin\cpp
	@mkdir bin\cs
	@copy binaryplus\bin\$(binname).$(binary) bin\cpp
	@copy cplusplus\bin\$(dllname) bin\cpp
	@copy csharp\bin\lib\$(libname) bin\cpp

	@copy binarysharp\bin\exe\$(binfile).$(binary) bin\cs
	@copy cplusplus\bin\$(dllname) bin\cs
	@copy csharp\bin\lib\$(libname) bin\cs

	@cd bin && ren cpp C++-$(release)-$(os)
	@cd bin && ren cs C\#-$(release)-$(os)

	@cd bin && zip -r C++-$(release)-$(os).zip C++-$(release)-$(os)
	@cd bin && zip -r C\#-$(release)-$(os).zip C\#-$(release)-$(os)

else
	@mkdir -p bin/cpp
	@mkdir -p bin/cs
	@cp binaryplus/bin/$(binname).$(binary) bin/cpp
	@cp cplusplus/bin/$(dllname) bin/cpp
	@cp csharp/bin/lib/$(libname) bin/cpp

	@cp binarysharp/bin/exe/$(binfile).$(binary) bin/cs
	@cp cplusplus/bin/$(dllname) bin/cs
	@cp csharp/bin/lib/$(libname) bin/cs

	@mv bin/cpp bin/C++-$(release)-$(os)
	@mv bin/cs bin/C\#-$(release)-$(os)

ifeq ($(findstring NT, $(shell uname -s)),NT)
	@cd bin && zip -r C++-$(release)-$(os).zip C++-$(release)-$(os)
	@cd bin && zip -r C\#-$(release)-$(os).zip C\#-$(release)-$(os)
else
ifeq ($(shell uname -s),Darwin)
	@cd bin && tar -czvf C++-$(release)-$(os).tgz C++-$(release)-$(os)
	@cd bin && tar -czvf C\#-$(release)-$(os).tgz C\#-$(release)-$(os)
else
	@cd bin && tar -czvf C++-$(release)-$(os).tar.gz C++-$(release)-$(os)
	@cd bin && tar -czvf C\#-$(release)-$(os).tar.gz C\#-$(release)-$(os)
endif
endif
endif
	@echo "Version file. Remove to enable recompile" > $@

all: dll cppbin csbin
	@echo "Version file. Remove to enable recompile" > $@

dll: cs cpp
	@echo "Version file. Remove to enable recompile" > $@

refresh:
ifeq ($(findstring indows, $(shell uname -s)),indows)
	@del /f all
	@del /f dll
	@del /f release
	@del /f cs
	@del /f csbin
	@del /f cppbin
	@del /f cpp
else
	@rm -f all
	@rm -f dll
	@rm -f release
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
	@$(MAKE) cs
endif
#
ifeq ($(findstring indows, $(shell uname -s)),indows)
#windows
	@cd cplusplus && $(compiler) -shared -o bin/$(name).dll -DUNICODE $(cdb) $(wfsrc) -I include -std=c++20 -L$(flibdir) $(flib)
#
else
ifeq ($(findstring CYGWIN, $(shell uname -s)),CYGWIN)
#cygwin [ I think same as windows (?) ]
	@cd cplusplus/obj && $(compiler) -c -DUNICODE $(cdb) $(fsrc) -I ../include -std=c++20
	@cd cplusplus && $(compiler) -shared -o bin/$(name).dll $(objects) -L$(flibdir) $(flib)
#
else
ifeq ($(findstring MINGW, $(shell uname -s)),MINGW)
#mingw [ I think same as windows (?) ]
	@cd cplusplus/obj && $(compiler) -c -DUNICODE $(cdb) $(fsrc) -I ../include -std=c++20
	@cd cplusplus && $(compiler) -shared -o bin/$(name).dll $(objects) -L$(flibdir) $(flib)
#
else
ifeq ($(findstring MSYS, $(shell uname -s)),MSYS)
#msys [ I think same as windows (?) ]
	@cd cplusplus/obj && $(compiler) -c -DUNICODE $(cdb) $(fsrc) -I ../include -std=c++20
	@cd cplusplus && $(compiler) -shared -o bin/$(name).dll $(objects) -L$(flibdir) $(flib)
#
else
	@cd cplusplus/obj && $(compiler) -c -fpic -DUNICODE $(cdb) -fvisibility=hidden $(fsrc) -I ../include
ifeq ($(shell uname -s),Darwin)
#macos
	@cd cplusplus && $(compiler) -dynamiclib -o bin/lib$(name).dylib $(objects) -L$(flibdir) $(flib)
#
else
#linux and similar
	@cd cplusplus && $(compiler) -shared -o bin/lib$(name).so $(objects) -L$(flibdir) $(flib)
endif
endif
endif
endif
endif
#

ifeq ($(findstring indows, $(shell uname -s)),indows)
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
ifeq ($(findstring indows, $(shell uname -s)),indows)
	@cd csharp/bin/$(configuration)/net8.0/$(os_name)/native/ && echo . > null.exp && echo . > null.lib && echo . > null.pdb && del *.exp && del *.lib && del *.pdb && ren * $(libname)
	@move csharp\bin\$(configuration)\net8.0\$(os_name)\native\$(libname) csharp\bin\lib
ifeq ($(copylibs),1)
	@$(admin) copy cplusplus\bin\$(dllname) $(libdir)
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
	@echo "Version file. Remove to enable recompile" > $@

cppbin: $(foreach src,$(binsources),binaryplus/src/$(src)) $(foreach head,$(binheaders),binaryplus/src/$(head)) $(foreach inc,$(binincludes),binaryplus/include/$(inc))
	
ifneq ($(wildcard cpp),cpp)
	@$(MAKE) cpp
endif

ifeq ($(findstring indows, $(shell uname -s)),indows)
#windows
	@cd binaryplus && $(compiler) $(bpdb) -o bin/$(binname).$(binary) $(fbsrc) -I include -L$(flibdir) -l$(filename) -l$(name) -std=c++20
#
else
ifeq ($(findstring CYGWIN, $(shell uname -s)),CYGWIN)
#cygwin [ I think same as windows (?) ]
	@cd binaryplus && $(compiler) $(bpdb) -o bin/$(binname).$(binary) $(fbsrc) -I include -L$(flibdir) -l$(filename) -l$(name) -std=c++20
#
else
ifeq ($(findstring MINGW, $(shell uname -s)),MINGW)
#mingw [ I think same as windows (?) ]
	@cd binaryplus && $(compiler) $(bpdb) -o bin/$(binname).$(binary) $(fbsrc) -I include -L$(flibdir) -l$(filename) -l$(name) -std=c++20
#
else
ifeq ($(findstring MSYS, $(shell uname -s)),MSYS)
#msys [ I think same as windows (?) ]
	@cd binaryplus && $(compiler) $(bpdb) -o bin/$(binname).$(binary) $(fbsrc) -I include -L$(flibdir) -l$(filename) -l$(name) -std=c++20
#
else
#	@cd binaryplus/obj && $(compiler) -c -fpic -DUNICODE -fvisibility=hidden $(fsrc) -I ../include
ifeq ($(shell uname -s),Darwin)
#macos
	@cd binaryplus && $(compiler) $(bpdb) -o bin/$(binname).$(binary) $(fbsrc) -I include -L$(flibdir) -l$(filename) -l$(name) -std=c++20
#
else
#linux and similar
	@cd binaryplus && $(compiler) $(bpdb) -o bin/$(binname).$(binary) $(fbsrc) -I include -L$(flibdir) -l$(filename) -l$(name) -std=c++20
endif
endif
endif
endif
endif
	@echo "Version file. Remove to enable recompile" > $@

csbin: $(foreach bfl,$(binfiles),binarysharp/$(bfl))
	@cd binarysharp && dotnet publish -p:SelfContained=true -r $(os_name) -c $(binconfig)
ifeq ($(findstring indows, $(shell uname -s)),indows)
	@cd binarysharp/bin/$(binconfig)/net8.0/$(os_name)/native/ && echo . > null.exp && echo . > null.lib && echo . > null.pdb && del *.exp && del *.lib && del *.pdb && ren * $(binfile).$(binary)
	@copy binarysharp\bin\$(binconfig)\net8.0\$(os_name)\native\$(binfile).$(binary) binarysharp\bin\exe
	@del binarysharp\bin\$(binconfig)\net8.0\$(os_name)\native\$(binfile).$(binary)
else
	@cd binarysharp/bin/$(binconfig)/net8.0/$(os_name)/native/ && mkdir null.dSYM && touch null.dSYM/null.null && rm *.dSYM/* && rmdir *.dSYM && touch null.dbg && touch null.exp && touch null.lib && touch null.pdb && rm *.dbg && rm *.exp && rm *.lib && rm *.pdb
	@cp -f binarysharp/bin/$(binconfig)/net8.0/$(os_name)/native/* binarysharp/bin/exe/$(binfile).$(binary)
	@rm -f binarysharp/bin/$(binconfig)/net8.0/$(os_name)/native/*
endif
	@echo "Version file. Remove to enable recompile" > $@
