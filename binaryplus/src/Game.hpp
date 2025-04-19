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
                    Camera(int height, int width, cpp::Console::Symbol sym);
                    MatrixPosition ViewportCenter();
                    void DrawTexture(int x, int y, std::vector<std::vector<Console::Symbol>> texture);
                    void DrawToScreen(int x, int y, std::vector<std::vector<Console::Symbol>> screen);
            };
    };
}