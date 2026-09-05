#include "Game.hpp"
#include "Texture.hpp"

#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <vector>
#include <iostream>
#include <utility>
#include <cassert>
#include <chrono>
#include "Console.hpp"
#include "TextureSystem.hpp"

#ifdef max
#undef max
#endif
#ifdef min
#undef min
#endif

using namespace cpp;
using namespace util;
using std::vector;

// public:

Game::MatrixPosition::MatrixPosition(int iIndex, int jIndex) {
    this->iIndex = iIndex;
    this->jIndex = jIndex;
}

Game::Camera::Camera(int height, int width, Console::Symbol sym) {
	this->buffer_ = vector<vector<Console::Symbol>>(height, vector<Console::Symbol>(width, sym));
}

vector<vector<Console::Symbol>> Game::Camera::buffer() {
    return this->buffer_;
}