#include "game/game.hpp"

static void rand_asteroid_properties(Asteroid& asteroid)
{
    int site = rand() % 2800;

    auto& t = *asteroid.game_object.get<transform>();

    if (site < 800)
    {
        t.location_x = static_cast<float>(site - 400);
        t.location_y = 300 + 50;
        t.rotation = static_cast<float>(rand() % 180 + 180);
    }
    else if (site < 1400)
    {
        t.location_x = 400 + 50;
        t.location_y = static_cast<float>(site - 1100);
        t.rotation = static_cast<float>(rand() % 180 + 90);
    }
    else if (site < 2200)
    {
        t.location_x = static_cast<float>(site - 1800);
        t.location_y = -300 - 50;
        t.rotation = static_cast<float>(rand() % 180);
    }
    else
    {
        t.location_x = -400 - 50;
        t.location_y = static_cast<float>(site - 2500);
        t.rotation = static_cast<float>(rand() % 180 - 90);
    }
    asteroid.current_speed =
        static_cast<float>(Asteroid::base_speed * (rand() % 3 + 1));
}

static Asteroid create_asteroid(transform& asteroid_transform,
                                color& asteroid_color,
                                mesh& asteroid_mesh)
{

    asteroid_color.rgb = 1.0f;
    asteroid_mesh.lines = {{+50.0f, -10.0f},
                           {+20.0f, -50.0f},
                           {-5.0f, -50.0f},
                           {-5.0f, -25.0f},
                           {-30.0f, -50.0f},
                           {-50.0f, -10.0f},
                           {-25.0f, 0.0f},
                           {-50.0f, +10.0f},
                           {-15.0f, +45.0f},
                           {+20.0f, +45.0f}};
    Asteroid asteroid{};
    asteroid.game_object =
        game_entity{asteroid_transform, asteroid_color, asteroid_mesh};
    return asteroid;
}

static Ship create_racket(transform& racket_transform,
                          direction& racket_direction,
                          color& racket_color,
                          mesh& racket_mesh)
{
    racket_direction.forward = direction::Forward{true};
    racket_color.rgb = 1.0f;
    racket_mesh.lines = {
        {+15.0f, 0.0f}, {-15.0f, -10.0f}, {-5.0f, 0.0f}, {-15.0f, +10.0}};

    Ship ship{};
    ship.game_object = game_entity{
        racket_transform, racket_direction, racket_color, racket_mesh};
    return ship;
}

template <typename... Args>
static void reserve_memory(const std::size_t size, Args&... args)
{
    (args.reserve(size), ...);
}

Game::Game(ui::window& w, ui::keyboard& k) : window{w}, keyboard{k}
{
    reserve_memory(2, transforms, directions, colors, meshs);
    racket = create_racket(transforms.emplace_back(),
                           directions.emplace_back(),
                           colors.emplace_back(),
                           meshs.emplace_back());
    asteroid = create_asteroid(transforms.emplace_back(),
                               colors.emplace_back(),
                               meshs.emplace_back());
    rand_asteroid_properties(asteroid);
}

static void rotate_left(transform& transform, const float rotation_step)
{
    transform.rotation += rotation_step;

    if (transform.rotation > 360.0f)
    {
        transform.rotation -= 360.0f;
    }
}

static void rotate_right(transform& transform, const float rotation_step)
{
    if (transform.rotation <= 0.0f)
    {
        transform.rotation = 360.0f - rotation_step;
    }
    else
    {
        transform.rotation -= rotation_step;
    }
}

void Game::on_pressed(const ui::keyboard_key key)
{
    switch (key)
    {
    case ui::keyboard_key::left:
        rotate_left(*racket.game_object.get<transform>(), Ship::rotation_step);
        break;
    case ui::keyboard_key::right:
        rotate_right(*racket.game_object.get<transform>(),
                     Ship::rotation_step);
        break;
    case ui::keyboard_key::up:
        racket.game_object.get<direction>()->forward = true;
        break;
    case ui::keyboard_key::down:
        racket.game_object.get<direction>()->forward = false;
        break;
    case ui::keyboard_key::space:
        rand_asteroid_properties(asteroid);
        break;
    case ui::keyboard_key::unknown:
        break;
    }
}

void Game::on_released(const ui::keyboard_key)
{
}

void Game::run()
{
    frame_clock clock{};

    while (window.is_open())
    {
        window.read_events();
        update_game_logic(clock.delta());
        render_game();
        window.swap_buffers();
    }
}

static void draw(const game_entity& game_object)
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glPushMatrix();

    if (const auto* local_transform = game_object.get<transform>())
    {
        glTranslatef(
            local_transform->location_x, local_transform->location_y, 0);
        glRotatef(local_transform->rotation, 0, 0, 1);
    }

    if (const auto* object_color = game_object.get<color>())
    {
        glColor3f(object_color->rgb, object_color->rgb, object_color->rgb);
    }

    if (const auto* object_mesh = game_object.get<mesh>())
    {
        glBegin(GL_POLYGON);
        for (const auto& point : object_mesh->lines)
        {
            glVertex2f(point.first, point.second);
        }
        glEnd();

        glBegin(GL_POINTS);
        glVertex2f(0.0f, 0.0f);
        glEnd();
    }
    glPopMatrix();
}

void Game::render_game()
{
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glOrtho(-400, 400, -300, 300, -1, 1);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    draw(racket.game_object);

    if (asteroid_visible)
    {
        draw(asteroid.game_object);
    }
}

static bool is_object_visible(const game_entity& game_object)
{
    if (game_object.get<transform>()->location_x > 400 + 50 ||
        game_object.get<transform>()->location_x < -400 - 50 ||
        game_object.get<transform>()->location_y > 300 + 50 ||
        game_object.get<transform>()->location_y < -300 - 50)
    {
        return false;
    }
    return true;
}

static void move_object(transform& local_transform,
                        const int move_speed,
                        const int rotation_speed,
                        const float delta)
{
    local_transform.location_x +=
        cos(local_transform.rotation * M_PI / 180.0f) * move_speed * delta;
    local_transform.location_y +=
        sin(local_transform.rotation * M_PI / 180.0f) * move_speed * delta;

    local_transform.rotation += rotation_speed * delta;

    if (local_transform.rotation > 360.0f)
        local_transform.rotation -= 360.0f;
}

void Game::play_death_animation(Ship& ship, const float delta)
{
    ship_slower += delta;
    if (ship_slower > 0.2)
    {
        ship_destroyed = true;
        if (ship.game_object.get<color>()->rgb > 0)
        {
            ship.game_object.get<color>()->rgb -= 0.1f;
        }
        else
        {
            ship_destroyed = false;
            ship.game_object.get<transform>()->rotation = 0;
            ship.game_object.get<transform>()->location_x = 0;
            ship.game_object.get<transform>()->location_y = 0;
            ship.game_object.get<color>()->rgb = 1.0f;
            ship_deaths = 0;
        }
        ship_slower = 0;
    }
}

// TODO: move to collision system
static bool is_collided(const transform& first, const transform& second)
{
    const auto x = first.location_x - second.location_x;
    const auto y = first.location_y - second.location_y;
    const auto distance = sqrt((x * x) + (y * y));
    return distance < 54.0f;
}

void Game::update_game_logic(float delta)
{
    if (!ship_destroyed && is_collided(*racket.game_object.get<transform>(),
                                       *asteroid.game_object.get<transform>()))
    {
        ship_destroyed = true;
    }

    if (ship_destroyed)
    {
        play_death_animation(racket, delta);
    }
    else
    {
        const int move_speed = racket.game_object.get<direction>()->forward ?
                                   Ship::forward_speed :
                                   Ship::backward_speed;
        auto rotation_speed = 0;

        auto& local_transform = *racket.game_object.get<transform>();
        move_object(local_transform, move_speed, rotation_speed, delta);

        if (local_transform.location_x > 400 ||
            local_transform.location_x < -400)
            local_transform.location_x *= -1;

        if (local_transform.location_y > 300 ||
            local_transform.location_y < -300)
            local_transform.location_y *= -1;
    }

    if (asteroid_visible = is_object_visible(asteroid.game_object))
    {
        move_object(*asteroid.game_object.get<transform>(),
                    asteroid.current_speed,
                    1,
                    delta);
    }
    else
    {
        if (asteroidBuffer >= 2)
        {
            asteroidBuffer = 0;
            rand_asteroid_properties(asteroid);
            asteroid_visible = true;
        }
        else
        {
            asteroidBuffer += delta;
        }
    }
}
