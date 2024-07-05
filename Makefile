
#******** c++ config ************
#> source files
sources = Console.cpp dllmain.cpp
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
binsources = main.cpp
#> header files
binheaders = main.hpp
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
files = DllHandle.cs FileSystem.cs 
# *******************************

#********* c# binary config *****
#> name the binary file
binfile = bs
#>compilation mode
binconfig = Release
#>source code files
binfiles = Program.cs DllHandle.cs
# *******************************

ifeq ($(cdebug),1)
cdb = -g
endif
ifeq ($(bpdebug),1)
bpdb = -g
endif
flib = -l$(filename)
fsrc = $(foreach src,$(sources),../src/$(src))
fbsrc = $(foreach bsrc,$(binsources),src/$(bsrc))
objects = $(foreach file,$(sources),obj/$(subst .c,.o,$(subst .cc,.c,$(subst .cpp,.cc,$(file)))))

ifeq ($(shell echo "check_quotes"),"check_quotes")
#windows
prefix = .\\
os_name = win-x64
dllname = "$(name).dll"
libname = "$(filename).dll"
binary = exe
#
else
ifeq ($(findstring NT, $(shell uname -s)),NT)
#cygwin/mingw
prefix = ./
os_name = win-x64
dllname = "$(name).dll"
libname = "$(filename).dll"
binary = exe
#
else
ifeq ($(shell uname -s),Darwin)
#macos
prefix = ./
os_name = osx-x64
dllname = "lib$(name).dylib"
libname = "lib$(filename).dylib"
binary = app
#
else
#linux and similar
prefix = ./
os_name = linux-x64
libname = "lib$(filename).so"
dllname = "lib$(name).so"
binary = bin
#
endif
endif
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
	@cd cplusplus/obj && g++ -w -c -DLIBRARY_EXPORT $(cdb) $(fsrc) -I ../include
	@cd cplusplus && g++ -w -shared -o bin/$(name).dll $(objects) -L ../csharp/bin/lib $(flib)
#
else
ifeq ($(findstring CYGWIN, $(shell uname -s)),CYGWIN)
#mingw [ I think same as windows (?) ]
	@cd cplusplus/obj && g++ -w -c -DLIBRARY_EXPORT $(cdb) $(fsrc) -I ../include
	@cd cplusplus && g++ -w -shared -o bin/$(name).dll $(objects) -L ../csharp/bin/lib $(flib)
#
else
ifeq ($(findstring MINGW, $(shell uname -s)),MINGW)
#cygwin [ I think same as windows (?) ]
	@cd cplusplus/obj && g++ -w -c -DLIBRARY_EXPORT $(cdb) $(fsrc) -I ../include
	@cd cplusplus && g++ -w -shared -o bin/$(name).dll $(objects) -L ../csharp/bin/lib $(flib)
#
else
	@cd cplusplus/obj && g++ -w -c -fpic -DLIBRARY_EXPORT $(cdb) -fvisibility=hidden $(fsrc) -I ../include
ifeq ($(shell uname -s),Darwin)
#macos
	@cd cplusplus && g++ -w -dynamiclib -o bin/lib$(name).dylib $(objects) -L ../csharp/bin/lib $(flib)
#
else
#linux and similar
	@cd cplusplus && g++ -w -shared -o bin/lib$(name).so $(objects) -L ../csharp/bin/lib $(flib)
endif
endif
endif
endif
#

ifeq ($(shell echo "check_quotes"),"check_quotes")
#windows
	@copy cplusplus\bin\$(dllname) binarysharp\bin\exe
	@copy cplusplus\bin\$(dllname) binaryplus\bin
else
#other
	@cp cplusplus/bin/$(dllname) binarysharp/bin/exe
	@cp cplusplus/bin/$(dllname) binaryplus/bin
#
endif

	@echo "Version file. Remove to enable recompile" > $@


cs: $(foreach fl,$(files),csharp/$(fl))
	@cd csharp && dotnet publish -p:NativeLib=Shared -p:SelfContained=true -r $(os_name) -c $(configuration)
ifeq ($(shell echo "check_quotes"),"check_quotes")
	@cd csharp/bin/$(configuration)/net8.0/$(os_name)/native/ && echo . > null.exp && echo . > null.lib && echo . > null.pdb && del *.exp && del *.lib && del *.pdb && ren * $(libname)
	@move csharp\bin\$(configuration)\net8.0\$(os_name)\native\$(libname) csharp\bin\lib
	@copy csharp\bin\lib\$(libname) binarysharp\bin\exe
	@copy csharp\bin\lib\$(libname) binaryplus\bin
else
	@cd csharp/bin/$(configuration)/net8.0/$(os_name)/native/ && mkdir null.dSYM && touch null.dSYM/null.null && rm *.dSYM/* && rmdir *.dSYM && touch null.dbg && touch null.exp && touch null.lib && touch null.pdb && rm *.dbg && rm *.exp && rm *.lib && rm *.pdb
	@mv -f csharp/bin/$(configuration)/net8.0/$(os_name)/native/* csharp/bin/lib/$(libname)
	@cp csharp/bin/lib/$(libname) binarysharp/bin/exe
	@cp csharp/bin/lib/$(libname) binaryplus/bin
endif
	@echo "Version file. Remove to enable recompile" > $@

cppbin: $(foreach src,$(binsources),binaryplus/src/$(src)) $(foreach head,$(binheaders),binaryplus/src/$(head)) $(foreach inc,$(binincludes),binaryplus/include/$(inc))
	
ifneq ($(wildcard cpp),cpp)
	@$(MAKE) cpp
endif

ifeq ($(shell echo "check_quotes"),"check_quotes")
#windows
	@cd binaryplus && g++ -w $(bpdb) -o bin/$(binname).$(binary) $(fbsrc) -I include -Lbin -l$(filename) -l$(name)
#
else
ifeq ($(findstring CYGWIN, $(shell uname -s)),CYGWIN)
#cygwin [ I think same as windows (?) ]
	@cd binaryplus && g++ -w $(bpdb) -o bin/$(binname).$(binary) $(fbsrc) -I include -Lbin -l$(filename) -l$(name)
#
else
ifeq ($(findstring MINGW, $(shell uname -s)),MINGW)
#cygwin [ I think same as windows (?) ]
	@cd binaryplus && g++ -w $(bpdb) -o bin/$(binname).$(binary) $(fbsrc) -I include -Lbin -l$(filename) -l$(name)
#
else
#	@cd binaryplus/obj && g++ -w -c -fpic -DLIBRARY_EXPORT -fvisibility=hidden $(fsrc) -I ../include
ifeq ($(shell uname -s),Darwin)
#macos
	@cd binaryplus && g++ -w $(bpdb) -o bin/$(binname).$(binary) $(fbsrc) -I include -Lbin -l$(filename) -l$(name)
#
else
#linux and similar
	@cd binaryplus && g++ -w $(bpdb) -o bin/$(binname).$(binary) $(fbsrc) -I include -Lbin -l$(filename) -l$(name)
endif
endif
endif
endif
	@echo "Version file. Remove to enable recompile" > $@

csbin: $(foreach bfl,$(binfiles),binarysharp/$(bfl))
	@cd binarysharp && dotnet publish -p:SelfContained=true -r $(os_name) -c $(binconfig)
ifeq ($(shell echo "check_quotes"),"check_quotes")
	@cd binarysharp/bin/$(binconfig)/net8.0/$(os_name)/native/ && echo . > null.exp && echo . > null.lib && echo . > null.pdb && del *.exp && del *.lib && del *.pdb && ren * $(binfile).$(binary)
	@move binarysharp\bin\$(binconfig)\net8.0\$(os_name)\native\$(binfile).$(binary) binarysharp\bin\exe
else
	@cd binarysharp/bin/$(binconfig)/net8.0/$(os_name)/native/ && mkdir null.dSYM && touch null.dSYM/null.null && rm *.dSYM/* && rmdir *.dSYM && touch null.dbg && touch null.exp && touch null.lib && touch null.pdb && rm *.dbg && rm *.exp && rm *.lib && rm *.pdb
	@mv -f binarysharp/bin/$(binconfig)/net8.0/$(os_name)/native/* binarysharp/bin/exe/$(binfile).$(binary)
endif
	@echo "Version file. Remove to enable recompile" > $@
