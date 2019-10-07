#include "graphics/opengl_old_pipeline.hpp"

#include <windows.h>

#include <gl/GLU.h>

namespace graphics
{
opengl_old_pipeline::opengl_old_pipeline(const std::size_t number_of_objects)
{
    rendering_targets.reserve(number_of_objects);
}

component::rendering_target& opengl_old_pipeline::create_rendering_target()
{
    return rendering_targets.emplace_back();
}

static void render_object(const component::rendering_target& renderer)
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glPushMatrix();

    glTranslatef(renderer.location_x, renderer.location_y, 0);
    glRotatef(renderer.rotation, 0, 0, 1);

    glColor3f(renderer.rgb, renderer.rgb, renderer.rgb);

    glBegin(GL_POLYGON);
    for (const auto& line : renderer.lines)
    {
        glVertex2f(line.first, line.second);
    }
    glEnd();

    glBegin(GL_POINTS);
    for (const auto& point : renderer.points)
    {
        glVertex2f(point.first, point.second);
    }
    glEnd();

    glPopMatrix();
}

void opengl_old_pipeline::render()
{
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glOrtho(-400, 400, -300, 300, -1, 1);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    for (const auto& renderer : rendering_targets)
    {
        if (renderer.is_visible)
        {
            render_object(renderer);
        }
    }
}

}
