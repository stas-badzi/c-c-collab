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
                    Camera(void* cameraPtr); // don't know if it is useful at all
                    void* Get(); //
                    void DrawTexture(std::vector<std::vector<Console::Symbol>> texture, MatrixPosition center);
                private:
                    std::vector<std::vector<cpp::Console::Symbol>> buffer;
                    //Camera(std::vector<std::vector<Console::Symbol>> buffer, MatrixPosition viewportCenter);
            };
        private:
            static void ValidateViewport(MatrixPosition vpc);
    };
}