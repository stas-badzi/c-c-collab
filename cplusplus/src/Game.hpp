#pragma once

#include <iostream>
#include <vector>
#include "Console.hpp"

namespace cpp {
    class Game {
        public: 
            struct MatrixPosition {
                int iIndex;
                int jIndex;
            };
            struct Camera {
                public:
                    Camera(int height, int width, cpp::Console::Symbol sym);
                    MatrixPosition ViewportCenter();
                    void DrawTexture(std::vector<std::vector<Console::Symbol>> texture, MatrixPosition center);
                private:
                    std::vector<std::vector<cpp::Console::Symbol>> buffer;
            };
        private:
            static void ValidateViewport(MatrixPosition vpc);
    };
}
