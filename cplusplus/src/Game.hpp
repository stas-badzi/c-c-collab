#pragma once

#include <iostream>
#include <vector>
#include "Console.hpp"

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
                    void DrawTexture(int x, int y, std::vector<std::vector<Console::Symbol>> texture);
                    void DrawToScreen(int x, int y, std::vector<std::vector<Console::Symbol>> screen);
                    std::vector<std::vector<cpp::Console::Symbol>> buffer();
                private:
                    std::vector<std::vector<cpp::Console::Symbol>> buffer_;
            };
        private:
            static void ValidateViewport(MatrixPosition vpc);
    };
}
