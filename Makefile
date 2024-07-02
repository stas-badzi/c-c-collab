
#******** c++ config ************
#> source files
sources = class1.cpp class2.cpp class3.cpp class4.cpp
#> header files
headers = class1.hpp class2.hpp class3.hpp class4.hpp
#> include files
includes = operating_system.h dynamic_library.h
#> additional libraries (including the c# one)
libs = csharp
#> name the dynamic library
name = cplusplus
# *******************************

#********* c# config ************
#> compilation mode
configuration = Release
#> generated file (project name)
filename = csharp
#>source code files
files = Class1.cs Class3.cs DllHandle.cs
# *******************************

#********* c# binary config ************
#>source code files
binfiles = Program.cs DllHandle.cs
# *******************************


flib = $(foreach lib,$(libs),-l$(lib))
fsrc = $(foreach src,$(sources),../src/$(src))
objects = $(foreach file,$(sources),obj/$(subst .c,.o,$(subst .cc,.c,$(subst .cpp,.cc,$(file)))))

ifeq ($(shell echo "check_quotes"),"check_quotes")
#windows
os_name = win-x64
dllname = "$(name).dll"
libname = "$(filename).dll"
binary = exe
#
else
ifeq ($(findstring NT, $(shell uname -s)),NT)
#cygwin/mingw
os_name = win-x64
dllname = "$(name).dll"
libname = "$(filename).dll"
binary = exe
#
else
ifeq ($(shell uname -s),Darwin)
#macos
os_name = osx-x64
dllname = "lib$(name).dylib"
libname = "lib$(filename).dylib"
binary = app
#
else
#linux and similar
os_name = linux-x64
libname = "lib$(filename).so"
dllname = "lib$(name).so"
binary = bin
#
endif
endif
endif

all:
	@$(MAKE) cs > null.log
	@$(MAKE) cpp > null.log
	@$(MAKE) bin > null.log
ifeq ($(shell echo "check_quotes"),"check_quotes")
	@del null.log
else
	@rm null.log
endif

run:
	@cd cplusplus/bin && ./main.$(binary)

cpp: $(foreach src,$(sources),cplusplus/src/$(src)) $(foreach head,$(headers),cplusplus/src/$(head)) $(foreach inc,$(includes),cplusplus/include/$(inc))

#
ifeq ($(shell echo "check_quotes"),"check_quotes")
#windows
	@cd cplusplus/obj && g++ -w -c -DLIBRARY_EXPORT $(fsrc) -I ../include
	@cd cplusplus && g++ -w -shared -o bin/lib/$(name).dll $(objects) -L bin/lib $(flib)
#
else
ifeq ($(findstring CYGWIN, $(shell uname -s)),CYGWIN)
#mingw [ I think same as windows (?) ]
	@cd cplusplus/obj && g++ -w -c -DLIBRARY_EXPORT $(fsrc) -I ../include
	@cd cplusplus && g++ -w -shared -o bin/lib/$(name).dll $(objects) -L bin/lib $(flib)
#
else
ifeq ($(findstring MINGW, $(shell uname -s)),MINGW)
#cygwin [ I think same as windows (?) ]
	@cd cplusplus/obj && g++ -w -c -DLIBRARY_EXPORT $(fsrc) -I ../include
	@cd cplusplus && g++ -w -shared -o bin/lib/$(name).dll $(objects) -L bin/lib $(flib)
#
else
	@cd cplusplus/obj && g++ -w -c -fpic -DLIBRARY_EXPORT -fvisibility=hidden $(fsrc) -I ../include
ifeq ($(shell uname -s),Darwin)
#macos
	@cd cplusplus && g++ -w -dynamiclib -o bin/lib/lib$(name).dylib $(objects) -L bin/lib $(flib)
#
else
#linux and similar
	@cd cplusplus && g++ -w -shared -o bin/lib/lib$(name).so $(objects) -L bin/lib $(flib)
endif
endif
endif
endif
#
	@echo "Version file. Remove to enable recompile" > $@


cs: $(foreach fl,$(files),csharp/$(fl))
	@cd csharp && dotnet publish /p:NativeLib=Shared /p:SelfContained=true -r $(os_name) -c $(configuration)
ifeq ($(shell echo "check_quotes"),"check_quotes")
	@cd csharp/bin/$(configuration)/net8.0/$(os_name)/native/ && echo . > null.exp && echo . > null.lib && echo . > null.pdb && del *.exp && del *.lib && del *.pdb && ren * $(libname)
	@move csharp\bin\$(configuration)\net8.0\$(os_name)\native\$(libname) csharp/bin/lib
else
	@cd csharp/bin/$(configuration)/net8.0/$(os_name)/native/ && mkdir null.dSYM && touch null.dSYM/null.null && rm *.dSYM/* && rmdir *.dSYM && touch null.dbg && touch null.exp && touch null.lib && touch null.pdb && rm *.dbg && rm *.exp && rm *.lib && rm *.pdb
	@mv csharp/bin/$(configuration)/net8.0/$(os_name)/native/* csharp/bin/lib
endif
	@echo "Version file. Remove to enable recompile" > $@

cppbin: binaryplus/src/main.cpp binaryplus/src/main.hpp

ifeq ($(shell echo "check_quotes"),"check_quotes")
	@copy csharp\bin\lib\$(libname) binaryplus\bin
	@copy cplusplus\bin\lib\$(dllname) binaryplus\bin
else
	@cp csharp/bin/lib/$(libname) binaryplus/bin
	@cp cplusplus/bin/lib/$(dllname) binaryplus/bin
endif

ifeq ($(shell echo "check_quotes"),"check_quotes")
#windows
#$(error windows c++ binary not yet suppoted)
	@cd binaryplus && g++ -w -o bin/main.$(binary) src/main.cpp -I include -Lbin -l$(filename) -l$(name)
#
else
ifeq ($(findstring CYGWIN, $(shell uname -s)),CYGWIN)
#cygwin [ I think same as windows (?) ]
#	@cd binaryplus/obj && g++ -w -c -DLIBRARY_EXPORT $(fsrc) -I ../include
	@cd binaryplus && g++ -w -o bin/main.$(binary) src/main.cpp -I include -Lbin -l$(filename) -l$(name)
#
else
ifeq ($(findstring MINGW, $(shell uname -s)),MINGW)
#cygwin [ I think same as windows (?) ]
#	@cd binaryplus/obj && g++ -w -c -DLIBRARY_EXPORT $(fsrc) -I ../include
	@cd binaryplus && g++ -w -o bin/main.$(binary) src/main.cpp -I include -Lbin -l$(filename) -l$(name)
$(error mingw c++ binary not yet suppoted)
#
else
#	@cd binaryplus/obj && g++ -w -c -fpic -DLIBRARY_EXPORT -fvisibility=hidden $(fsrc) -I ../include
ifeq ($(shell uname -s),Darwin)
#macos
	@cd binaryplus && g++ -w -o bin/main.$(binary) src/main.cpp -I include -Lbin -l$(filename) -l$(name)
#
else
#linux and similar
	@cd binaryplus && g++ -w -o bin/main.$(binary) src/main.cpp -I include -Lbin -l$(filename) -l$(name)
endif
endif
endif
endif
#
#g++
#	@cd binaryplus && g++ -w -o bin/main.$(binary) src/main.cpp -I include -Lbin -l$(filename) -l$(name)
#fix this ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
	@echo "Version file. Remove to enable recompile" > $@

csbin: $(foreach bfl,$(binfiles),binarysharp/$(bfl))
	@cd binarysharp && dotnet publish /p:PublishSingleFile=true /p:SelfContained=true -r $(os_name) -c $(configuration)
ifeq ($(shell echo "check_quotes"),"check_quotes")
	@cd binarysharp/bin/$(configuration)/net8.0/$(os_name) && del * && cd publish && del *.pdb
	@copy csharp\bin\lib\$(libname) binarysharp\bin\$(configuration)\net8.0\$(os_name)\publish
	@copy cplusplus\bin\lib\$(dllname) binarysharp\bin\$(configuration)\net8.0\$(os_name)\publish
else
	@cd binarysharp/bin/$(configuration)/net8.0/$(os_name) && rm *.dll && touch binarysharp.null && touch createdump.null && rm binarysharp* && rm createdump* && cd publish && mkdir null.dSYM && touch null.dSYM/null.null && rm *.dSYM/* && rmdir *.dSYM && touch null.pdb && rm *.pdb
	@cp csharp/bin/lib/$(libname) binarysharp/bin/$(configuration)/net8.0/$(os_name)/publish
	@cp cplusplus/bin/lib/$(dllname) binarysharp/bin/$(configuration)/net8.0/$(os_name)/publish
endif
	@echo "Version file. Remove to enable recompile" > $@