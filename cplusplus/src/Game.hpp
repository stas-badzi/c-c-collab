#include <iostream>
#include <vector>
#include "Console.hpp"
#include "smart_ref.hpp"

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
                    void DrawTexture(std::vector<std::vector<smart_ref<cpp::Console::Symbol>>> texture, MatrixPosition center);
                private:
                    std::vector<std::vector<smart_ref<cpp::Console::Symbol>>> buffer;
            };
        private:
            static void ValidateViewport(MatrixPosition vpc);
    };
}