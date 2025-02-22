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

Camera::Camera(void* cameraPtr) { // work in progress
	return;

    int offset = 0;
    auto int32_size = sizeof(int);
    auto symptr_size = sizeof(void*); // <---

    vector<vector<Console::Symbol>> ret_buffer;
    MartixPosition ret_viewportCenter;

    int height = &(int*)(cameraPtr+offset);
    offset += int32_size;

    buffer.resize(height);

    for (int i = 0; i < height; i++) {
        int width = &(int*)(cameraPtr+offset);
        offset += int32_size;

        buffer[i].resize(width);
        for (int j = 0; j < width; j++) {
            // buffer[i][j] = Console::Symbol(&(void*)(cameraPtr+offset)) <---
            offset += symptr_size;
        }
    }

    ret_viewportCenter = MartixPosition(&(int*)(cameraPtr+offset), &(int*)(cameraPtr+offset+int32_size));
    ValidateViewport(ret_viewportCenter);

    this->buffer = ret_buffer;
    this->viewportCenter = ret_viewportCenter;

    free(cameraPtr);
}

void* Camera::Get() { // work in progress
	return nullptr;

	int alloc = 0;
    auto int32_size = sizeof(int);
    auto symptr_size = sizeof(void*); // <---

    alloc += 3 * int32_size; // buffer.Count + viewportCenter
    for (i = 0; i < this->buffer.size(); i++) {
        alloc += int32_size + buffer[i].size() * symptr_size; // buffer[i].size() + buffer[i]
    }

    int offset = 0;
    void* ret = malloc(alloc);

    *(ret+offset) = this->buffer.size();
    offset += int32_size;
    
    for (int i = 0; i < this->buffer.size(); i++) {
        auto row = buffer[i];
        *(ret+offset) = row.size();
        offset += int32_size;
        for (int j = 0; j < row.size(); j++) {
            // *(ret+offset) = row[j].Get(); <---
            offset += symptr_size;
        }
    }

    *(ret+offset) = this->viewportCenter.iIndex;
    offset += int32_size;

    *(ret+offset) = this->viewportCenter.jIndex;
    offset += int32_size;

    return ret;
}

void Camera::DrawTextureToCamera(vector<vector<Console::Symbol>> texture, MartixPosition center) {
    float height = (float)texture.size();
    float width = (float)texture[0].size();

    ValidateViewport(center);

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

// private:

Camera::Camera(vector<vector<Console::Symbol>> buffer, MartixPosition viewportCenter) {
	this->buffer = buffer;
    this->viewportCenter = viewportCenter;
}

static void ValidateViewport(MartixPosition vpc) {
    if (vpc.iIndex % 2 == 0 || vpc.jIndex % 2 == 0) {
		throw std::invalid_argument("Width and height must be odd numbers.");
	}
}