#include "Camera.hpp"

#include <iostream>
#include <vector>
#include "Console.hpp"

using std::vector;

// public
Camera::Camera(int width, int height, Symbol sym);
Camera::Camera(void* cameraPtr);
void* Camera::Get();
void Camera::DrawTextureToCamera(vector<vector<Symbol>> texture, MartixPosition center);
// private
Camera::Camera(vector<vector<Symbol>> buffer, MartixPosition viewportCenter);