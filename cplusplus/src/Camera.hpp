#include <iostream>
#include <vector>
#include "Console.hpp"

namespace cpp {
    class Camera {
        public:
            Camera(int width, int height, Symbol sym);
            Camera(void* cameraPtr);
            void* Get();
            void DrawTextureToCamera(std::vector<std::vector<Symbol>> texture, MartixPosition center);
        private:
            Camera(std::vector<std::vector<Symbol>> buffer, MartixPosition viewportCenter);
            std::vector<std::vector<Symbol>> buffer;
            MartixPosition viewportCenter;
    }

    struct MartixPosition {
        int iIndex;
        int jIndex;
    }
}