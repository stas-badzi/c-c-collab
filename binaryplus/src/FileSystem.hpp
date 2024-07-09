#pragma once
#include <vector>
#include <string>

namespace cs {
    class FileSystem {
    public:
        static std::vector<std::wstring> ImportText(std::wstring filename);
        //static void ExportText(std::wstring file, std::vector<std::wstring> contents);
    };
}