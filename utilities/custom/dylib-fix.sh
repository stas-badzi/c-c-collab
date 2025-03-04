#!/bin/sh
# usage: dylib-fix.sh [executable/dynamiclib-path] [dylib-name]
install_name_tool -add_rpath "@loader_path" "$1"
install_name_tool -add_rpath "@executable_path/../lib" "$1"
install_name_tool -change $(otool -l "$1" | grep "$2" | sed 's/ (offset 24)//' | sed 's/         name //') "@rpath/lib$2.dylib" "$1"