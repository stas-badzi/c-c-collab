#include "Game.hpp"

#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include "Console.hpp"

#ifdef max
#undef max
#endif
#ifdef min
#undef min
#endif

using namespace cpp;
using std::vector;

// public:

Game::Camera::Camera(int height, int width, Console::Symbol sym) {
	this->buffer = vector<vector<Console::Symbol>>(height, vector<Console::Symbol>(width, sym));

    ValidateViewport(this->ViewportCenter());
}

Game::MatrixPosition Game::Camera::ViewportCenter() {
    return {static_cast<int>(this->buffer.size() / 2), static_cast<int>(this->buffer[0].size() / 2)};
}

void Game::Camera::DrawTexture(vector<vector<Console::Symbol>> texture, MatrixPosition center) {
    float height = (float)texture.size();
    float width = (float)texture[0].size();

    ValidateViewport(center);

    int startX = std::max(0, center.jIndex - static_cast<int>(std::floor(height / 2.0)));
    int startY = std::max(0, center.iIndex - static_cast<int>(std::floor(width / 2.0)));
    int endX = std::min(static_cast<int>(this->buffer.size()), startX + static_cast<int>(texture.size()));
    int endY = std::min(static_cast<int>(this->buffer[0].size()), startY + static_cast<int>(texture[0].size()));

    for (int i = startX, ti = 0; i < endX; i++, ti++) {
        for (int j = startY, tj = 0; j < endY; j++, tj++) {
            this->buffer[i][j] = texture[ti][tj];
        }
    }
}

// private:

void Game::ValidateViewport(MatrixPosition vpc) {
    if (vpc.iIndex % 2 == 0 || vpc.jIndex % 2 == 0) {
		throw std::invalid_argument("Width and height must be odd numbers.");
	}
}