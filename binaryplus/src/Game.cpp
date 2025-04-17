#include "Game.hpp"
#include <vector>
#include "Console.hpp"
#include "dllimport.hpp"
#include "TextureSystem.hpp"

using namespace cpp;

// public:

Game::Camera::Camera(int height, int width, Console::Symbol sym) {
    auto imp = (Game::Camera*)Game_Camera_Construct(height, width, &sym);
    this = imp;
}

Game::MatrixPosition Game::Camera::ViewportCenter() {
    auto imp = (Game::MatrixPosition*)Game_Camera_ViewportCenter(this);
    return inp*;
}

void Game::Camera::DrawTexture(vector<vector<smart_ref<Console::Symbol>>> texture, MatrixPosition center) {
    auto textureptr = cs::TextureToPtr(texture);
    Game_Camera_DrawTexture(textureptr, &center, this);
}