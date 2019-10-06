#pragma once

#define _USE_MATH_DEFINES

#include <windows.h>

#include <chrono>
#include <cmath>
#include <ctime>
#include <exception>
#include <functional>
#include <gl/GLU.h>
#include <iostream>
#include <stdlib.h>
#include <vector>

#include "game/asteroid.hpp"
#include "game/ship.hpp"
#include "ui/keyboard.hpp"
#include "ui/window_win32.hpp"
#include "world/frame_clock.hpp"

class Game
{
public:
    Game(ui::window& w, ui::keyboard& k);

    void on_pressed(const ui::keyboard_key key);
    void on_released(const ui::keyboard_key);
    void run();

private:
    void update_surface();
    void update_logic(float delta);
    void play_death_animation(Ship&, float delta);

    float asteroidBuffer = 0;
    std::vector<transform> transforms{};
    std::vector<direction> directions{};
    std::vector<color> colors{};
    std::vector<mesh> meshs{};

    Ship racket{};
    bool ship_destroyed{false};
    int ship_deaths{0};
    float ship_slower{0}; // TODO: what the slower is?

    Asteroid asteroid{};
    bool asteroid_visible{false};

    ui::window& window;
    ui::keyboard& keyboard;
};
