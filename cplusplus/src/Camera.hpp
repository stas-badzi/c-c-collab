#include <iostream>
#include <vector>
#include "Console.hpp"

namespace cpp {
    struct MartixPosition {
        int iIndex;
        int jIndex;
    };
    struct Camera {
        public:
            Camera(int height, int width, Console::Symbol sym);
            Camera(void* cameraPtr);
            void* Get();
            void DrawTextureToCamera(std::vector<std::vector<Console::Symbol>> texture, MartixPosition center);
        private:
            std::vector<std::vector<Console::Symbol>> buffer;
            MartixPosition viewportCenter;
            Camera(std::vector<std::vector<Console::Symbol>> buffer, MartixPosition viewportCenter);
            static void ValidateViewport(MartixPosition vpc);
    };
}