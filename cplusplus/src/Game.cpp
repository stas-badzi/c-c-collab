#include "Game.hpp"

#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include "Console.hpp"
#include "TextureSystem.hpp"

#ifdef max
#undef max
#endif
#ifdef min
#undef min
#endif

using namespace cpp;
using namespace cs;
using std::vector;

// public:

Game::MatrixPosition::MatrixPosition(int iIndex, int jIndex) {
    this->iIndex = iIndex;
    this->jIndex = jIndex;
}

Game::Camera::Camera(int height, int width, Console::Symbol sym) {
	this->buffer_ = vector<vector<Console::Symbol>>(height, vector<Console::Symbol>(width, sym));
}

Game::MatrixPosition Game::Camera::ViewportCenter() {
    return {static_cast<int>(this->buffer.size() / 2), static_cast<int>(this->buffer[0].size() / 2)};
}

void Game::Camera::DrawTexture(int x, int y, vector<vector<Console::Symbol>> texture) {
    TextureSystem::DrawTextureToScreen(x, y, texture, this->buffer_);
}

void Game::Camera::DrawToScreen(int x, int y, vector<vector<Console::Symbol>> screen) {
    TextureSystem::DrawTextureToScreen(x, y, this->buffer_, screen);
}

vector<vector<Console::Symbol>> Game::Camera::buffer() {
    return this->buffer_;
}

// private:

void Game::ValidateViewport(MatrixPosition vpc) {
    return;
}