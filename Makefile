#******** c++ config ************
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
binname = bpp
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
binfile = bs
#>compilation mode
binconfig = Release
#>source code files
binfiles = Program.cs DllImport.cs FileSystem.cs Console.cs
# *******************************

#***** shared library config ****
#> do copy shred libraries
copylibs = 1
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
#>windows running for symulators generation
genwin = 1
#>cygwin path to windows
symcyglib = /cygdrive/c/Windows
#>msys2(mingw) path to windows
symsyslib = /c/Windows
#*********************************

ifeq ($(cdebug),1)
cdb = -g
endif
ifeq ($(bpdebug),1)
bpdb = -g
endif
flibdir = bin
flib = -l$(filename)
fsrc = $(foreach src,$(sources),../src/$(src))
fbsrc = $(foreach bsrc,$(binsources),src/$(bsrc))
objects = $(foreach file,$(sources),obj/$(subst .c,.o,$(subst .cc,.c,$(subst .cpp,.cc,$(file)))))

ifeq ($(shell echo "check_quotes"),"check_quotes")
#windows
admin = sudo
prefix = .\
os_name = win-x64
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
os_name = win-x64
dllname = "$(name).dll"
libname = "$(filename).dll"
binary = exe
#
else
ifeq ($(shell uname -s),Darwin)
#macos
admin = sudo
prefix = ./
os_name = osx-x64
dllname = "lib$(name).dylib"
libname = "lib$(filename).dylib"
binary = app
libdir = $(macoslib)
#
else
#linux and similar
admin = sudo
prefix = ./
os_name = linux-x64
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

all:
	@$(MAKE) dll
	@$(MAKE) cppbin
	@$(MAKE) csbin

dll:
	@$(MAKE) cs
	@$(MAKE) cpp


cpprun:
	@cd binaryplus/bin && $(prefix)$(binname).$(binary)

csrun:
	@cd binarysharp/bin/exe && $(prefix)$(binfile).$(binary)

cpp: $(foreach src,$(sources),cplusplus/src/$(src)) $(foreach head,$(headers),cplusplus/src/$(head)) $(foreach inc,$(includes),cplusplus/include/$(inc))


ifneq ($(wildcard cs),cs)
	@$(MAKE) cs
endif
#
ifeq ($(shell echo "check_quotes"),"check_quotes")
#windows
	@cd cplusplus/obj && g++ -c -DUNICODE $(cdb) $(fsrc) -I ../include
	@cd cplusplus && g++ -shared -o bin/$(name).dll $(objects) -L$(flibdir) $(flib)
#
else
ifeq ($(findstring CYGWIN, $(shell uname -s)),CYGWIN)
#cygwin [ I think same as windows (?) ]
	@cd cplusplus/obj && g++ -c -DUNICODE $(cdb) $(fsrc) -I ../include
	@cd cplusplus && g++ -shared -o bin/$(name).dll $(objects) -L$(flibdir) $(flib)
#
else
ifeq ($(findstring MINGW, $(shell uname -s)),MINGW)
#mingw [ I think same as windows (?) ]
	@cd cplusplus/obj && g++ -c -DUNICODE $(cdb) $(fsrc) -I ../include
	@cd cplusplus && g++ -shared -o bin/$(name).dll $(objects) -L$(flibdir) $(flib)
#
else
ifeq ($(findstring MSYS, $(shell uname -s)),MSYS)
#msys [ I think same as windows (?) ]
	@cd cplusplus/obj && g++ -c -DUNICODE $(cdb) $(fsrc) -I ../include
	@cd cplusplus && g++ -shared -o bin/$(name).dll $(objects) -L$(flibdir) $(flib)
#
else
	@cd cplusplus/obj && g++ -c -fpic -DUNICODE $(cdb) -fvisibility=hidden $(fsrc) -I ../include
ifeq ($(shell uname -s),Darwin)
#macos
	@cd cplusplus && g++ -dynamiclib -o bin/lib$(name).dylib $(objects) -L$(flibdir) $(flib)
#
else
#linux and similar
	@cd cplusplus && g++ -shared -o bin/lib$(name).so $(objects) -L$(flibdir) $(flib)
endif
endif
endif
endif
endif
#

ifeq ($(shell echo "check_quotes"),"check_quotes")
#windows
ifeq ($(copylibs),1)
	@$(admin) copy cplusplus\bin\$(dllname) $(libdir)
else
	@copy cplusplus\bin\$(dllname) binarysharp\bin\exe
	@copy cplusplus\bin\$(dllname) binaryplus\bin
endif
else
#other
ifeq ($(copylibs),1)
	@$(admin) cp cplusplus/bin/$(dllname) $(libdir)
else
	@cp cplusplus/bin/$(dllname) binarysharp/bin/exe
	@cp cplusplus/bin/$(dllname) binaryplus/bin
endif
endif

	@echo "Version file. Remove to enable recompile" > $@


cs: $(foreach fl,$(files),csharp/$(fl))
	@cd csharp && dotnet publish -p:NativeLib=Shared -p:SelfContained=true -r $(os_name) -c $(configuration)
ifeq ($(shell echo "check_quotes"),"check_quotes")
	@cd csharp/bin/$(configuration)/net8.0/$(os_name)/native/ && echo . > null.exp && echo . > null.lib && echo . > null.pdb && del *.exp && del *.lib && del *.pdb && ren * $(libname)
	@move csharp\bin\$(configuration)\net8.0\$(os_name)\native\$(libname) csharp\bin\lib
ifeq ($(copylibs),1)
	@$(admin) copy cplusplus\bin\$(dllname) $(libdir)
else
	@copy csharp\bin\lib\$(libname) binarysharp\bin\exe
	@copy csharp\bin\lib\$(libname) binaryplus\bin
endif
else
	@cd csharp/bin/$(configuration)/net8.0/$(os_name)/native/ && mkdir null.dSYM && touch null.dSYM/null.null && rm *.dSYM/* && rmdir *.dSYM && touch null.dbg && touch null.exp && touch null.lib && touch null.pdb && rm *.dbg && rm *.exp && rm *.lib && rm *.pdb
	@mv -f csharp/bin/$(configuration)/net8.0/$(os_name)/native/* csharp/bin/lib/$(libname)
ifeq ($(copylibs),1)
	@$(admin) cp csharp/bin/lib/$(libname) $(libdir)
else
	@cp csharp/bin/lib/$(libname) binarysharp/bin/exe
	@cp csharp/bin/lib/$(libname) binaryplus/bin
endif
endif
	@echo "Version file. Remove to enable recompile" > $@

cppbin: $(foreach src,$(binsources),binaryplus/src/$(src)) $(foreach head,$(binheaders),binaryplus/src/$(head)) $(foreach inc,$(binincludes),binaryplus/include/$(inc))
	
ifneq ($(wildcard cpp),cpp)
	@$(MAKE) cpp
endif

ifeq ($(shell echo "check_quotes"),"check_quotes")
#windows
	@cd binaryplus && g++ $(bpdb) -o bin/$(binname).$(binary) $(fbsrc) -I include -L$(flibdir) -l$(filename) -l$(name)
#
else
ifeq ($(findstring CYGWIN, $(shell uname -s)),CYGWIN)
#cygwin [ I think same as windows (?) ]
	@cd binaryplus && g++ $(bpdb) -o bin/$(binname).$(binary) $(fbsrc) -I include -L$(flibdir) -l$(filename) -l$(name)
#
else
ifeq ($(findstring MINGW, $(shell uname -s)),MINGW)
#mingw [ I think same as windows (?) ]
	@cd binaryplus && g++ $(bpdb) -o bin/$(binname).$(binary) $(fbsrc) -I include -L$(flibdir) -l$(filename) -l$(name)
#
else
ifeq ($(findstring MSYS, $(shell uname -s)),MSYS)
#msys [ I think same as windows (?) ]
	@cd binaryplus && g++ $(bpdb) -o bin/$(binname).$(binary) $(fbsrc) -I include -L$(flibdir) -l$(filename) -l$(name)
#
else
#	@cd binaryplus/obj && g++ -c -fpic -DUNICODE -fvisibility=hidden $(fsrc) -I ../include
ifeq ($(shell uname -s),Darwin)
#macos
	@cd binaryplus && g++ $(bpdb) -o bin/$(binname).$(binary) $(fbsrc) -I include -L$(flibdir) -l$(filename) -l$(name)
#
else
#linux and similar
	@cd binaryplus && g++ $(bpdb) -o bin/$(binname).$(binary) $(fbsrc) -I include -L$(flibdir) -l$(filename) -l$(name)
endif
endif
endif
endif
endif
	@echo "Version file. Remove to enable recompile" > $@

csbin: $(foreach bfl,$(binfiles),binarysharp/$(bfl))
	@cd binarysharp && dotnet publish -p:SelfContained=true -r $(os_name) -c $(binconfig)
ifeq ($(shell echo "check_quotes"),"check_quotes")
	@cd binarysharp/bin/$(binconfig)/net8.0/$(os_name)/native/ && echo . > null.exp && echo . > null.lib && echo . > null.pdb && del *.exp && del *.lib && del *.pdb && ren * $(binfile).$(binary)
	@copy binarysharp\bin\$(binconfig)\net8.0\$(os_name)\native\$(binfile).$(binary) binarysharp\bin\exe
	@del binarysharp\bin\$(binconfig)\net8.0\$(os_name)\native\$(binfile).$(binary)
else
	@cd binarysharp/bin/$(binconfig)/net8.0/$(os_name)/native/ && mkdir null.dSYM && touch null.dSYM/null.null && rm *.dSYM/* && rmdir *.dSYM && touch null.dbg && touch null.exp && touch null.lib && touch null.pdb && rm *.dbg && rm *.exp && rm *.lib && rm *.pdb
	@cp -f binarysharp/bin/$(binconfig)/net8.0/$(os_name)/native/* binarysharp/bin/exe/$(binfile).$(binary)
	@rm -f binarysharp/bin/$(binconfig)/net8.0/$(os_name)/native/*
endif
	@echo "Version file. Remove to enable recompile" > $@
