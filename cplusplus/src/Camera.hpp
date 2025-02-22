#include <iostream>
#include <vector>
#include "Console.hpp"

namespace cpp {
    struct MartixPosition {
        int iIndex;
        int jIndex;
    };
    class Camera {
        public:
            Camera(int width, int height, Console::Symbol sym);
            Camera(void* cameraPtr);
            void* Get();
            void DrawTextureToCamera(std::vector<std::vector<Console::Symbol>> texture, MartixPosition center);
        private:
            Camera(std::vector<std::vector<Console::Symbol>> buffer, MartixPosition viewportCenter);
            std::vector<std::vector<Console::Symbol>> buffer;
            MartixPosition viewportCenter;
    };
}