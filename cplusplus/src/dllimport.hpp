#include <stdint.h>

#include <dynamic_library.h>
#include <unicode_conversion.hpp>

namespace csimp {
    
    libimport uniconv::unichar** FileSystem_ImportText(uniconv::unichar* file);
    
    libimport void FileSystem_ExportText(uniconv::unichar* file, uniconv::unichar** content);

} // namespace csimp

