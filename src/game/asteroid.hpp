#pragma once

#include <cassert>

#include "game/game_entity.hpp"

static bool is_object_inside_screen(const component::transform& transform)
{
    if (transform.position.x > 400 + 50 || transform.position.x < -400 - 50 ||
        transform.position.y > 300 + 50 || transform.position.y < -300 - 50)
    {
        return false;
    }
    return true;
}

class Actor
{
public:
    virtual ~Actor() = default;
    virtual void update(double delta) = 0;
};

struct Asteroid : public Actor
{
    game_entity* game_object;

    // TODO: replace by movement component
    float current_speed{1};
    static const int base_speed{30};
    // ...

    float asteroidBuffer = 0;

    Asteroid() = default;
    Asteroid(std::vector<game_entity>& game_objects,
             component::transform& asteroid_transform,
             component::direction& asteroid_direction,
             component::rendering_target& asteroid_renderer)
    {
        asteroid_direction.is_forward = component::direction::forward{true};
        asteroid_direction.direction_x = 1;
        asteroid_direction.direction_y = -1;
        asteroid_renderer.is_visible = true;
        asteroid_renderer.color.r = 1.0f;
        asteroid_renderer.color.g = 1.0f;
        asteroid_renderer.color.b = 1.0f;
        asteroid_renderer.lines = {{+50.0f, -10.0f},
                                   {+20.0f, -50.0f},
                                   {-5.0f, -50.0f},
                                   {-5.0f, -25.0f},
                                   {-30.0f, -50.0f},
                                   {-50.0f, -10.0f},
                                   {-25.0f, 0.0f},
                                   {-50.0f, +10.0f},
                                   {-15.0f, +45.0f},
                                   {+20.0f, +45.0f}};

        asteroid_renderer.points = {{0.0f, 0.0f}};

        game_objects.push_back(game_entity{
            asteroid_transform, asteroid_renderer, asteroid_direction});
        game_object = &game_objects.back();

        move_to_new_position();
    }

    void update(const double delta) override
    {
        // if asteroid go on out of screen wait some time before show up in the
        // new position
        if (!is_object_inside_screen(
                *game_object->get<component::transform>()))
        {
            asteroidBuffer += delta;

            if (asteroidBuffer >= 2)
            {
                asteroidBuffer = 0;
                move_to_new_position();
            }
        }
    }

    void move_to_new_position()
    {
        int site = rand() % 2800;
        int alfa = 0;

        assert(game_object->get<component::transform>() != nullptr);
        assert(game_object->get<component::direction>() !=
               nullptr); // FIXME: better way to be sure
                         // that is not null

        auto& t = *game_object->get<component::transform>();
        auto& d = *game_object->get<component::direction>();

        if (site < 800)
        {
            t.position.x = static_cast<float>(site - 400);
            t.position.y = 300 + 50;
            t.rotation = static_cast<float>(rand() % 180 + 180);
            alfa = rand() % 180 + 180;
        }
        else if (site < 1400)
        {
            t.position.x = 400 + 50;
            t.position.y = static_cast<float>(site - 1100);
            t.rotation = static_cast<float>(rand() % 180 + 90);
            alfa = rand() % 180 + 90;
        }
        else if (site < 2200)
        {
            t.position.x = static_cast<float>(site - 1800);
            t.position.y = -300 - 50;
            t.rotation = static_cast<float>(rand() % 180);
            alfa = rand() % 180;
        }
        else
        {
            t.position.x = -400 - 50;
            t.position.y = static_cast<float>(site - 2500);
            t.rotation = static_cast<float>(rand() % 180 - 90);
            alfa = rand() % 180 - 90;
        }

        d.direction_x = cos(alfa * M_PI / 180.0f);
        d.direction_y = sin(alfa * M_PI / 180.0f);

        current_speed =
            static_cast<float>(Asteroid::base_speed * (rand() % 3 + 1));
    }
};
