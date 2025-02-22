#include "Camera.hpp"

#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include "Console.hpp"

using namespace cpp;
using std::vector;

// public

Camera::Camera(int height, int width, Console::Symbol sym) {
    ValidateViewport(height, width);

    this->viewportCenter(floor(height/2), floor(width/2));
	this->buffer = vector<vector<Console::Symbol>>(height, vector<Console::Symbol>(width, sym));
}

Camera::Camera(void* cameraPtr) {
	// Constructor implementation
}

void* Camera::Get() {
	// Method implementation
	return nullptr;
}

void Camera::DrawTextureToCamera(vector<vector<Console::Symbol>> texture, MartixPosition center) {
    float height = (float)texture.size();
    float width = (float)texture[0].size();

    ValidateViewport((int)height, (int)width);

    int startX = max(0, center.jIndex - (int)Math.Floor(height / 2.0));
    int startY = max(0, center.iIndex - (int)Math.Floor(width / 2.0));
    int endX = min(this->buffer.size(), startX + texture.size());
    int endY = min(this->buffer[0].size(), startY + texture[0].size());

    for (int i = startX, ti = 0; i < endX; i++, ti++) {
        for (int j = startY, tj = 0; j < endY; j++, tj++) {
            this->buffer[i][j] = texture[ti][tj];
        }
    }
}

// private

Camera::Camera(vector<vector<Console::Symbol>> buffer, MartixPosition viewportCenter) {
	this->buffer = buffer;
    this->viewportCenter = viewportCenter;
}

static void ValidateViewport(int height, int width) {
    if (width % 2 == 0 || height % 2 == 0) {
		throw std::invalid_argument("Width and height must be odd numbers.");
	}
}