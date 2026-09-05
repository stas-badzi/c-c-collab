#pragma once

#include <iostream>
#include <vector>
#include "Console.hpp"
#include "smart_ref.hpp"
#include "TextureSystem.hpp"

namespace cpp {
    class Game {
        public: 
            struct MatrixPosition {
                public:
                    int iIndex;
                    int jIndex;

                    MatrixPosition(int iIndex, int jIndex);
            };
            struct Camera {
                public:
                    Camera(int height, int width, Console::Symbol sym);
                    void DrawTexture(int x, int y, const std::vector<std::vector<Console::Symbol>>& texture) {io::TextureSystem::DrawTextureToScreen(x, y, texture, this->buffer_);}
                    void DrawToScreen(int x, int y, std::vector<std::vector<Console::Symbol>>& screen) {io::TextureSystem::DrawTextureToScreen(x, y, this->buffer_, screen);}
                    void DrawToScreen(int x, int y, std::vector<std::vector<smart_ref<Console::Symbol>>>& screen) {io::TextureSystem::DrawTextureToScreen(x, y, this->buffer_, screen);}
                    std::vector<std::vector<cpp::Console::Symbol>> buffer();
                private:
                    std::vector<std::vector<cpp::Console::Symbol>> buffer_;
            };
            //static int Main(int argc, char_t* argv[]);
    };
}
