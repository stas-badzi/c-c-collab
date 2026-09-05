#include "Game.hpp"
#include <vector>
#include "Console.hpp"
#include "dllimport.hpp"
#include "TextureSystem.hpp"
#include "System.hpp"
using namespace cpp;

// public:

Game::MatrixPosition::MatrixPosition(int iIndex, int jIndex) {
    auto imp = (Game::MatrixPosition*)cppimp::Game_MartixPosition_Construct(iIndex, jIndex);

    this->iIndex = imp->iIndex;
    this->jIndex = imp->jIndex;
}

Game::Camera::Camera(int height, int width, Console::Symbol sym) {
    internal = cppimp::Game_Camera_Construct(height, width, sym.Get());
}

void Game::Camera::DrawTexture(int x, int y, std::vector<std::vector<Console::Symbol>>& texture) {
    auto textureptr = util::TextureToPtr(texture);
    cppimp::Game_Camera_DrawTexture(x, y, textureptr, internal);
}

void Game::Camera::DrawToScreen(int x, int y, std::vector<std::vector<Console::Symbol>>& screen) {
    auto screenptr = util::TextureToPtr(screen);
    cppimp::Game_Camera_DrawToScreen(x, y, screenptr, internal);
}