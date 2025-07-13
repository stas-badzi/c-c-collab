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
                    MatrixPosition ViewportCenter();
                    void DrawTexture(int x, int y, const std::vector<std::vector<Console::Symbol>>& texture) {cs::TextureSystem::DrawTextureToScreen(x, y, texture, this->buffer_);}
                    inline void DrawToScreen(int x, int y, std::vector<std::vector<Console::Symbol>>& screen) {cs::TextureSystem::DrawTextureToScreen(x, y, this->buffer_, screen);}
                    void DrawToScreen(int x, int y, std::vector<std::vector<smart_ref<Console::Symbol>>>& screen) {cs::TextureSystem::DrawTextureToScreen(x, y, this->buffer_, screen);}
                    std::vector<std::vector<cpp::Console::Symbol>> buffer();
                private:
                    std::vector<std::vector<cpp::Console::Symbol>> buffer_;
            };
        private:
            static void ValidateViewport(MatrixPosition vpc);
    };
}
