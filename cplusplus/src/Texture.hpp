#include <vector>
#include <iostream>
#include <utility>
#include <cassert>
#include "Console.hpp"
#include "TextureSystem.hpp"
#pragma once

namespace cpp {

    int wood = 0;

    class Texture
    {
    public:
        std::pair<long long,long long> offset;
    protected:
        const std::vector<std::vector<cpp::Console::Symbol>>* buffer;
    private:
        bool allocated;
    public:
        Texture(void) : offset({0,0}), buffer(nullptr), allocated(false) {}
        Texture(const Texture& copy) : offset({copy.offset.first,copy.offset.second}), buffer(copy.allocated?new auto(*copy.buffer):nullptr), allocated(copy.allocated) {}
        Texture(Texture&& move) : offset({move.offset.first,move.offset.second}), buffer(move.buffer), allocated(move.allocated) {move.allocated=false;}

        Texture(const std::vector<std::vector<cpp::Console::Symbol>>& buffer) : offset({0,0}), buffer(new auto(buffer)), allocated(true) {for (size_t i = 1; i < this->buffer->size(); i++) assert(this->buffer->at(i).size() == this->buffer->at(i-1).size());}
        Texture(const std::vector<std::vector<cpp::Console::Symbol>>& buffer, std::pair<long long,long long> offset) : offset(offset), buffer(new auto(buffer)), allocated(true) {for (size_t i = 1; i < this->buffer->size(); i++) assert(this->buffer->at(i).size() == this->buffer->at(i-1).size());}
        Texture(const std::vector<std::vector<cpp::Console::Symbol>>& buffer, long long x_offset, long long y_offset) : offset({x_offset,y_offset}), buffer(new auto(buffer)), allocated(true) {for (size_t i = 1; i < this->buffer->size(); i++) assert(this->buffer->at(i).size() == this->buffer->at(i-1).size());}
        Texture(const std::vector<std::vector<cpp::Console::Symbol>>* buffer_ptr) : offset({0,0}), buffer(buffer_ptr), allocated(false) {for (size_t i = 1; i < buffer->size(); i++) assert(this->buffer->at(i).size() == this->buffer->at(i-1).size());}
        Texture(const std::vector<std::vector<cpp::Console::Symbol>>* buffer_ptr, std::pair<long long,long long> offset) : offset(offset), buffer(buffer_ptr), allocated(false) {for (size_t i = 1; i < this->buffer->size(); i++) assert(this->buffer->at(i).size() == this->buffer->at(i-1).size());}
        Texture(const std::vector<std::vector<cpp::Console::Symbol>>* buffer_ptr, long long x_offset, long long y_offset) : offset({x_offset,y_offset}), buffer(buffer_ptr), allocated(false) {for (size_t i = 1; i < this->buffer->size(); i++) assert(this->buffer->at(i).size() == this->buffer->at(i-1).size());}
        ~Texture() {if (allocated) delete buffer;}
        bool uptodate;
        virtual void Update(std::vector<std::vector<Texture*>>&, std::pair<int,int>, unsigned long long) {}
        const std::vector<std::vector<cpp::Console::Symbol>>& Buffer() {return *buffer;}
    };
    
    class Animation : public Texture {
    protected:
        const std::vector<std::vector<std::vector<cpp::Console::Symbol>>> frames;
        uint64_t frame_wait;
        uint64_t last_frame_num;
    public:
        Animation(const Animation& copy) : Texture(static_cast<const Texture&>(copy)), frames(copy.frames), frame_wait(copy.frame_wait), last_frame_num(UINT64_MAX) {}
        Animation(Animation&& move) : Texture(static_cast<Texture&&>(move)), frames(move.frames), frame_wait(move.frame_wait), last_frame_num(UINT64_MAX) {}

        Animation(const std::vector<std::vector<std::vector<cpp::Console::Symbol>>>& frames, size_t frame_wait, std::pair<long long,long long> offset) : Texture(&(frames[0]),offset), frames(frames), frame_wait(frame_wait), last_frame_num(UINT64_MAX) {
            assert(frames.size() > 0);
            assert(frame_wait > 0);
        }
        Animation(const std::vector<std::vector<std::vector<cpp::Console::Symbol>>>& frames, size_t frame_wait = 1, long long x_offset = 0, long long y_offset = 0) : Texture(&(frames[0]),x_offset,y_offset), frames(frames), frame_wait(frame_wait), last_frame_num(UINT64_MAX) {
            assert(frames.size() > 0);
            assert(frame_wait > 0);
        }
        Animation(const std::vector<std::vector<std::vector<cpp::Console::Symbol>>>& frames, std::pair<long long,long long> offset) {Animation(frames,1,offset);}
        Animation(const std::vector<std::vector<std::vector<cpp::Console::Symbol>>>& frames, long long x_offset, long long y_offset)  {Animation(frames,1,x_offset,y_offset);}

        virtual void Update(__attribute__((unused)) std::vector<std::vector<Texture*>>& map, __attribute__((unused)) std::pair<int,int> whereami, unsigned long long timestamp) override {
            if ((timestamp / frame_wait) != last_frame_num) this->buffer = &(frames.at((last_frame_num=timestamp/frame_wait)%frames.size()));
        }
    };

    std::vector<std::vector<cpp::Console::Symbol>> __WoodChopperTexture0;
    auto& WoodChopperTexture0() {if(__WoodChopperTexture0.empty()) __WoodChopperTexture0=io::TextureSystem::TextureFromFile(u"/home/stas/sand.1.tux");return __WoodChopperTexture0;}

    class WoodChopper : public Animation {
    private:
        uint64_t last_earn_timestamp;
        static constexpr uint64_t earn_interval = 2000000;
    public:
        WoodChopper(uint64_t this_timestamp) : Animation(std::vector<std::vector<std::vector<Console::Symbol>>>{WoodChopperTexture0()},(earn_interval>>1),{0,0}), last_earn_timestamp(this_timestamp) {}
        virtual void Update(std::vector<std::vector<Texture*>>& map, std::pair<int,int> whereami, unsigned long long timestamp) override {
            this->Animation::Update(map,whereami,timestamp-last_earn_timestamp);
            if ((timestamp-last_earn_timestamp)/earn_interval > 0) {
                wood+=(timestamp-last_earn_timestamp)/earn_interval;
                last_earn_timestamp += ((timestamp-last_earn_timestamp)/earn_interval)*earn_interval;
            }
        }
    };
}

/*
using namespace std;
using namespace cpp;
using namespace io;
vector<cpp::Texture*> vec;
// we want STDIN to be still open when we exit
int App(); 
int main() {
    Console::Init();
    int ret = App();
    //Console::Fin(); 
    return ret;
}

int App() {
    Console::HideCursor();
    vector<vector<vector<Console::Symbol>>> anim;
    Texture sand(TextureSystem::TextureFromFile(u"/home/stas/sand.1.tux"));
    anim.push_back(TextureSystem::TextureFromFile(u"/home/stas/grass1.tux"));
    anim.push_back(TextureSystem::TextureFromFile(u"/home/stas/grass2.tux"));
    anim.push_back(TextureSystem::TextureFromFile(u"/home/stas/grass3.tux"));
    anim.push_back(TextureSystem::TextureFromFile(u"/home/stas/grass4.tux"));
    anim.push_back(TextureSystem::TextureFromFile(u"/home/stas/grass5.tux"));
    anim.push_back(TextureSystem::TextureFromFile(u"/home/stas/grass6.tux"));
    Animation grass(anim, 500000);
    WoodChopper woodchp(chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now().time_since_epoch()).count());
    vector<vector<Animation>> screen;
    vector<vector<WoodChopper>> screen2;
    vector<vector<Texture*>> texture_map;
    size_t columns = 10;
    size_t rows = 11;
    srand(time(0));
    for (size_t i=0;i<rows;++i) {
        screen.push_back({});screen2.push_back({});
        for (size_t j=0;j<columns-(i&1);++j)
            {screen.back().push_back(grass);screen2.back().push_back(woodchp);}
    }
    for (size_t i=0;i<rows;++i) {
        texture_map.push_back({});
        for (size_t j=0;j<columns-(i&1);++j)
            if (random()&1)
                texture_map.back().push_back(&(screen.at(i).at(j)));
            else texture_map.back().push_back(&(screen2.at(i).at(j)));
    }
            
    int xoffset=0, yoffset=0;

    while (1) {
        uint64_t time = chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now().time_since_epoch()).count();
        auto width = Console::GetWindowWidth(), height = Console::GetWindowHeight();
        vector<vector<Console::Symbol>> display(height,vector<Console::Symbol>(width,Console::Symbol("*",15,4)));
        int offsetx, offsety=0; bool push=false;
        for (auto&& row : texture_map) {
            if (!row.size()) break;
            offsetx = push ? (grass.Buffer().front().size()/2) : 0;
            for (auto&& cell : row) {
                cell->Update(texture_map,{0,0},time);
                TextureSystem::DrawTextureToScreen(offsetx+xoffset,offsety+yoffset,cell->Buffer(),display);
                offsetx += cell->Buffer().front().size();
            }
            offsety += (row.front()->Buffer().size()/2) + 1;
            push = !push;
        }
        for (int i=0; i<to_string(wood).size();++i) {
            display.at(max(0ul,display.size()-2)).at(max(0ul,display.at(max(0ul,display.size()-3)).size()-to_string(wood).size()-2+i)).character = to_string(wood).at(i);
        }
        Console::FillScreen(display);
        Console::HandleKeyboard();
        switch (Console::KeyPressed())
        {
        case Key::Enum::w:
            ++yoffset;
            break;
        case Key::Enum::s:
            --yoffset;
            break;
        case Key::Enum::a:
            ++xoffset;
            break;
        case Key::Enum::d:
            --xoffset;
            break;
        case Key::Enum::q:
            goto finish;
        }
        Console::HandleMouseAndFocus();
        if (Console::MouseButtonClicked().first == MOUSE_BUTTON_PRIMARY) goto finish;
    }
finish:
    return 0;
}*/