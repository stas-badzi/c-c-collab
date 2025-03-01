#include "Camera.hpp"

#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include "Console.hpp"

using namespace cpp;
using std::vector;

// public:

Camera::Camera(int height, int width, Console::Symbol sym) {
    MartixPosition ret_viewportCenter(floor(height/2), floor(width/2));
    ValidateViewport(ret_viewportCenter);

    this->viewportCenter = ret_viewportCenter;
	this->buffer = vector<vector<Console::Symbol>>(height, vector<Console::Symbol>(width, sym));
}

Camera::Camera(void* cameraPtr) {
    int offset = 0;
    auto int32_size = sizeof(int);
    auto symptr_size = sizeof(Console::Symbol*);

    vector<vector<Console::Symbol>> ret_buffer;
    MartixPosition ret_viewportCenter;

    int height = *((int*)cameraPtr+offset);
    offset += int32_size;

    buffer.resize(height);

    for (int i = 0; i < height; i++) {
        int width = *((int*)cameraPtr+offset);
        offset += int32_size;

        buffer[i].resize(width);
        for (int j = 0; j < width; j++) {
            buffer[i][j] = *((Console::Symbol*)cameraPtr+offset);
            offset += symptr_size;
        }
    }

    ret_viewportCenter = MartixPosition(*((int*)cameraPtr+offset), *((int*)cameraPtr+offset+int32_size));
    ValidateViewport(ret_viewportCenter);

    this->buffer = ret_buffer;
    this->viewportCenter = ret_viewportCenter;

    free(cameraPtr);
}

void* Camera::Get() {
	int alloc = 0;
    auto int32_size = sizeof(int);
    auto symptr_size = sizeof(Console::Symbol*);

    alloc += 3 * int32_size; // buffer.Count + viewportCenter
    for (size_t i = 0; i < this->buffer.size(); i++) {
        alloc += int32_size + buffer[i].size() * symptr_size; // buffer[i].size() + buffer[i]
    }

    int offset = 0;
    void* ret = malloc(alloc);

    *((int*)ret+offset) = this->buffer.size();
    offset += int32_size;
    
    for (size_t i = 0; i < this->buffer.size(); i++) {
        auto row = buffer[i];
        *((int*)ret+offset) = row.size();
        offset += int32_size;
        for (size_t j = 0; j < row.size(); j++) {
            *((Console::Symbol*)ret+offset) = row[j];
            offset += symptr_size;
        }
    }

    *((int*)ret+offset) = this->viewportCenter.iIndex;
    offset += int32_size;

    *((int*)ret+offset) = this->viewportCenter.jIndex;
    offset += int32_size;

    return ret;
}

void Camera::DrawTextureToCamera(vector<vector<Console::Symbol>> texture, MartixPosition center) {
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

Camera::Camera(vector<vector<Console::Symbol>> buffer, MartixPosition viewportCenter) {
	this->buffer = buffer;
    this->viewportCenter = viewportCenter;
}

void Camera::ValidateViewport(MartixPosition vpc) {
    if (vpc.iIndex % 2 == 0 || vpc.jIndex % 2 == 0) {
		throw std::invalid_argument("Width and height must be odd numbers.");
	}
}