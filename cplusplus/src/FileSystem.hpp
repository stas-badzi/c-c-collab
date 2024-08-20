#pragma once

#include <vector>
#include <unicode_conversion.hpp>

namespace cs {
    class FileSystem {
    public:
        static std::vector<uniconv::utfstr> ImportText(uniconv::utfstr filename);
        static void ExportText(uniconv::utfstr file, std::vector<uniconv::utfstr> lines);
    };
}