#include "Game.hpp"
#include <vector>
#include "Console.hpp"
#include "dllimport.hpp"
#include "TextureSystem.hpp"
#include "System.hpp"

using namespace cpp;

// public:

Game::MatrixPosition::MatrixPosition(int iIndex, int jIndex) {
    auto imp = (Game::MatrixPosition*)(cppimp::Game_MartixPosition_Construct(iIndex, jIndex));
    return imp*;
}

Game::Camera::Camera(int height, int width, Console::Symbol sym) {
    //auto imp = ((Game::Camera*)cppimp::Game_Camera_Construct(height, width, &sym))*;
    // chuj wie co dalej
}

Game::MatrixPosition Game::Camera::ViewportCenter() {
    auto imp = (Game::MatrixPosition*)(cppimp::Game_Camera_ViewportCenter(this));
    return imp*;
}

void Game::Camera::DrawTexture(int x, int y, std::vector<std::vector<Console::Symbol>> texture) {
    auto textureptr = cs::TextureToPtr(texture);
    cppimp::Game_Camera_DrawTexture(x, y, textureptr, this);
}

void Game::Camera::DrawToScreen(int x, int y, std::vector<std::vector<Console::Symbol>> screen) {
    auto screenptr = cs::TextureToPtr(screen);
    cppimp::Game_Camera_DrawToScreen(x, y, screenptr, this);
}