#include "Camera.hpp"

#include <iostream>
#include <vector>
#include "Console.hpp"

using std::vector;
using Console::Symbol;

// public

Camera::Camera(int width, int height, Symbol sym) {
	// Constructor implementation
}

Camera::Camera(void* cameraPtr) {
	// Constructor implementation
}

void* Camera::Get() {
	// Method implementation
	return nullptr;
}

void Camera::DrawTextureToCamera(vector<vector<Symbol>> texture, MartixPosition center) {
	// Method implementation
}

// private

Camera::Camera(vector<vector<Symbol>> buffer, MartixPosition viewportCenter) {
	// Constructor implementation
}