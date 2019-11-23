#pragma once

#include <vector>

#include "game/asteroid.hpp"
#include "game/ship.hpp"
#include "game/keyboard_control_system.hpp"
#include "graphics/opengl_old_pipeline.hpp"
#include "ui/keyboard.hpp"
#include "ui/window.hpp"

class asteroid_game
{
public:
    asteroid_game(ui::window& w, ui::keyboard& k);

    void on_pressed(const ui::keyboard_key key);
    void on_released(const ui::keyboard_key);
    void run();

private:
    void render_game();
    void update_game_logic(double delta);
    void play_death_animation(Ship&, double delta);
    void update_rendering_target_components();

    const std::size_t number_of_game_objects{2};

    std::vector<component::transform> transforms{};
    std::vector<component::direction> directions{};
    std::vector<game_entity> game_objects{};

    Ship racket{};
    bool ship_destroyed{false};
    int ship_deaths{0};

    Asteroid asteroid{};

    ui::window& window;
    ui::keyboard& keyboard;

    graphics::opengl_old_pipeline game_rendering_pipeline{
        number_of_game_objects};
    keyboard_control_system player_controller;
};
