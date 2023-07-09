#include <iostream>
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include "shader.h"
#include "texture.h"
#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <random>
#include "shader_program.h"
#include "model.h"
#include "terrain.h"

float apply_friction(float value, float friction)
{
    if(value < 0)
        return std::min(0.0f, value + friction);
    return std::max(0.0f, value - friction);
}

std::mt19937 gen(0xcedbeef);

glm::vec2 get_random_velocity(float scale)
{
    std::uniform_real_distribution<float> x_dist(-scale, scale);
    std::uniform_real_distribution<float> z_dist(-scale, scale);

    return {x_dist(gen), z_dist(gen)};
}

glm::vec3 move_hole(glm::vec3 hole, glm::vec2 xz,Terrain& terrain)
{
    hole.x += xz.x;
    hole.z += xz.y;
    return hole;
}

int main(int argc, char** argv)
{
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cerr << "Failed to initialize SDL" << std::endl;
        return -1;
    }
    
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    SDL_Window* window = SDL_CreateWindow("notcodegolf - GMTK2023", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1280, 720, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    if(!window)
    {
        std::cerr << "Failed to initialize window" << std::endl;
        return -1;
    }

    SDL_GLContext main_context = SDL_GL_CreateContext(window);

    SDL_GL_SetSwapInterval(1);
    if(glewInit() != GLEW_OK)
    {
        std::cerr << "Glew NOT ok" << std::endl;
        return -1;
    }

    glViewport(0, 0, 1280, 720);

    glm::vec3 camera_position = glm::vec3(0.0f, 10.0f, 20.0f);
    glm::vec3 camera_target = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 camera_direction = glm::normalize(camera_position - camera_target);

    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 camera_right = glm::normalize(glm::cross(up, camera_direction));

    glm::vec3 camera_up = glm::normalize(glm::cross(camera_direction, camera_right));

    glm::mat4 proj = glm::perspective(glm::radians(45.f), (float)1280.f/(float)720.f, 0.1f, 100.f);

    glm::mat4 view = glm::mat4(1.0f);
    view = glm::lookAt(camera_position,camera_target, camera_up);

    glm::vec3 light_pos = glm::vec3(5.0, 10.0, 2.0);
    float near_plane = 1.0f, far_plane = 7.5f;
    glm::mat4 light_projection = glm::ortho(-10.f, 10.f, -10.f, 10.f, near_plane, far_plane);
    glm::mat4 light_view = glm::lookAt(light_pos, glm::vec3(0.f, 0.0f, 0.f), glm::vec3(0.0f, 1.0f, 0.0f));

    glm::mat4 light_space_matrix = light_projection * light_view;

    glm::vec3 ball_pos = glm::vec3(3.0, 0.0, 0.0);
    glm::vec3 hole_pos = glm::vec3(0, 0, 0);
    glm::vec3 ball_velocity = glm::vec3(0, 0.0, 0.0);
    glm::vec2 random_vel = get_random_velocity(6.0f);
    glm::vec3 target_velocity = glm::vec3(random_vel.x, 0.0f, random_vel.y);

    Model* ball_model = new Model("assets/mesh/ball.obj");
    Model* light_model = new Model("assets/mesh/cube.obj");
    Model* map_model = new Model("assets/mesh/map1.obj");
    Model* hole_model = new Model("assets/mesh/hole.obj");
    Model* flag_pole_model = new Model("assets/mesh/flag_pole.obj");
    Model* flag_top_model = new Model("assets/mesh/flag_top.obj");

    std::vector<Model*> models = {ball_model, map_model, hole_model, flag_top_model, flag_pole_model};

    Shader* v_passthrough = new Shader("assets/shaders/passthrough.vs", shader_type::vertex);
    Shader* f_passthrough = new Shader("assets/shaders/passthrough.fs", shader_type::fragment);
    Shader* f_phong = new Shader("assets/shaders/phong.fs", shader_type::fragment);
    Shader* v_shadow = new Shader("assets/shaders/shadow.vs", shader_type::vertex);
    Shader* f_shadow = new Shader("assets/shaders/shadow.fs", shader_type::fragment);

    Shader* dots_v_passthrough = new Shader("assets/shaders/dots.vs", shader_type::vertex);
    Shader* dots_f_passthrough = new Shader("assets/shaders/dots.fs", shader_type::fragment);

    if(!v_passthrough->compile()) return -1;
    if(!f_passthrough->compile()) return -1;
    if(!f_phong->compile()) return -1;
    if(!dots_v_passthrough->compile()) return -1;
    if(!dots_f_passthrough->compile()) return -1;
    if(!v_shadow->compile()) return -1;
    if(!f_shadow->compile()) return -1;

    ShaderProgram* phong_shader = new ShaderProgram(v_passthrough, f_phong);
//    ShaderProgram* texture_shader = new ShaderProgram(v_passthrough, f_passthrough);
    ShaderProgram* dots_shader = new ShaderProgram(dots_v_passthrough, dots_f_passthrough);
    ShaderProgram* shadow_shader = new ShaderProgram(v_shadow, f_shadow);


    ball_model->set_shader(phong_shader);
    hole_model->set_shader(phong_shader);
    map_model->set_shader(phong_shader);
    flag_pole_model->set_shader(phong_shader);
    flag_top_model->set_shader(phong_shader);

    delete v_passthrough;
    delete f_passthrough;
    delete f_phong;
    delete dots_f_passthrough;
    delete dots_v_passthrough;

    Terrain terrain("assets/map/gray.png", "assets/map/gradient.png");
    Texture tex_checkerboard;
    tex_checkerboard.load("assets/sprites/checkerboard.png");

    Texture white_texture;
    white_texture.load("assets/sprites/white.png");

    Texture green_texture;
    green_texture.load("assets/sprites/green.png");

    Texture red_texture;
    red_texture.load("assets/sprites/red.png");

    ball_model->set_texture(&white_texture);
    hole_model->set_texture(&white_texture);
    map_model->set_texture(&green_texture);
    flag_pole_model->set_texture(&white_texture);
    flag_top_model->set_texture(&red_texture);

    ball_model->model = glm::mat4(1);
    ball_model->model = glm::scale(ball_model->model, glm::vec3(0.5, 0.5, 0.5));

    map_model->model = glm::scale(map_model->model, 10.0f * glm::vec3(1.0, 1.0, 1.0));

    bool is_running = true;
    bool wireframe_mode = false;
    bool cull_faces = true;

    glClearColor(77.f / 255.f, 166.f/255.f, 166.f/225.f, 1.0f);
    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    uint32_t last_time_ms = SDL_GetTicks();
    uint32_t ms_per_tick = 16;
    uint64_t accumulator = 0;
    float accumulator_f = 0.0f;
    uint32_t delta = 16;
    float delta_t = static_cast<float>(delta) / 1000.f;


    bool render_points = true;
    bool ball_launched = false;
    std::vector<float> points = {
            0.0f, 3.0f, 0.0f,
            0.0f, 0.25f, 0.0f,
            0.0f, 0.5f, 0.0f,
            0.0f, 0.75f, 0.0f,
            0.0f, 1.0f, 0.0f,
    };

    uint32_t points_vao;
    glGenVertexArrays(1, &points_vao);
    glBindVertexArray(points_vao);

    uint32_t points_vbo;
    glGenBuffers(1, &points_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
    glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(float) * points.size(), points.data(), GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), nullptr);
    glEnableVertexAttribArray(0);

    uint32_t depth_map_fbo;
    glGenFramebuffers(1, &depth_map_fbo);

    const uint32_t shadow_width = 1024, shadow_height = 1024;
    uint32_t depth_map;
    glGenTextures(1, &depth_map);
    glBindTexture(GL_TEXTURE_2D, depth_map);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadow_width, shadow_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT,
                 nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,  GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,  GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,  GL_TEXTURE_WRAP_T, GL_REPEAT);

    glBindFramebuffer(GL_FRAMEBUFFER, depth_map_fbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth_map, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    double camera_around_angle = 0.0;

    glPointSize(5.f);
    while(is_running)
    {
        SDL_Event e;
        glm::vec2 vel = get_random_velocity(7.0f);
        while(SDL_PollEvent(&e) > 0)
        {
            switch(e.type)
            {
                case SDL_QUIT:
                    is_running = false;
                    break;
                case SDL_KEYDOWN:
                    switch(e.key.keysym.sym)
                    {
                        case SDL_KeyCode::SDLK_p:
                            wireframe_mode = !wireframe_mode;
                            std::cout << "Wireframe mode: " << wireframe_mode << std::endl;
                            break;
                        case SDL_KeyCode::SDLK_c:
                            cull_faces = !cull_faces;
                            std::cout << "cull face mode: " << cull_faces << std::endl;
                            break;
                        case SDL_KeyCode::SDLK_UP:
                            camera_position += glm::vec3(0, 1, 0);
                            break;
                        case SDL_KeyCode::SDLK_DOWN:
                            camera_position += glm::vec3(0, -1, 0);
                            break;
                        case SDL_KeyCode::SDLK_LEFT:
                            camera_around_angle += 0.15;
                            break;
                        case SDL_KeyCode::SDLK_RIGHT:
                            camera_around_angle -= 0.15;
                            break;
                        case SDL_KeyCode::SDLK_SPACE:
                            ball_launched = true;
                            ball_velocity = target_velocity;
                            target_velocity = glm::vec3(vel.x, 0.0f, vel.y);
                            render_points = false;
                            break;
                        case SDL_KeyCode::SDLK_a:
                            hole_pos = move_hole(hole_pos, glm::vec2(0.0f, 0.2f), terrain);
                            break;
                        case SDL_KeyCode::SDLK_d:
                            hole_pos = move_hole(hole_pos, glm::vec2(0.0f, -0.2f), terrain);
                            break;
                        case SDL_KeyCode::SDLK_w:
                            hole_pos = move_hole(hole_pos, glm::vec2(-0.2f, 0.0f), terrain);
                            break;
                        case SDL_KeyCode::SDLK_s:
                            hole_pos = move_hole(hole_pos, glm::vec2(0.2f, 0.0f), terrain);
                            break;

                    }
            }

            camera_position = glm::vec3(20 * cos(camera_around_angle), camera_position.y, 20 * sin(camera_around_angle));
            SDL_UpdateWindowSurface(window);
        }

        if(wireframe_mode)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        if(cull_faces)
        {
            glEnable(GL_CULL_FACE);
        }
        else
        {
            glDisable(GL_CULL_FACE);
        }

        light_pos = camera_position  + glm::vec3(0.0, 0.0, 10.0f);

        ball_model->model = glm::mat4(1);

        float ball_friction = 0.05f;
        ball_velocity.x = apply_friction(ball_velocity.x, ball_friction);
        ball_velocity.z = apply_friction(ball_velocity.z, ball_friction);

        glm::vec3 gradient = terrain.get_gradient(ball_pos.x, ball_pos.z);
        ball_velocity += gradient;

        if(glm::length(ball_velocity) < 0.05f)
        {
            ball_velocity = glm::vec3(0, 0, 0);
            render_points = true;
            ball_launched = false;
        }
//        std::cout << "gradient: " << gradient[0] << " " << gradient[2] << std::endl;
//        std::cout << "vel: " << ball_velocity[0] << " " << ball_velocity[2] << std::endl;
//        std::cout << "pos: " << ball_pos[0] << " " << ball_pos[2] << std::endl;
        ball_pos += ball_velocity * delta_t;

        float ball_x = ball_pos.x;
        float ball_z = ball_pos.z;
        ball_model->model = glm::translate(ball_model->model, glm::vec3( ball_x, -1.f + terrain.get_height(ball_x, ball_z) * 10.f,  ball_z));
        ball_model->model = glm::scale(ball_model->model, glm::vec3(0.2f, 0.2f, 0.2f));

        hole_model->model = glm::mat4(1);
        hole_pos.y = -1.f + terrain.get_height(hole_pos.x, hole_pos.z) * 10.f;
        hole_model->model = glm::translate(hole_model->model, hole_pos);
        hole_model->model = glm::scale(hole_model->model, glm::vec3(0.2f, 0.2f, 0.2f));

        flag_top_model->model = glm::mat4(1);

        flag_top_model->model = glm::translate(flag_top_model->model, glm::vec3(hole_pos.x,   0.25f +  hole_pos.y, hole_pos.z));
        flag_top_model->model = glm::rotate(flag_top_model->model, glm::radians(90.f), glm::vec3(0.0, 1.0, 0.0));

        flag_pole_model->model = glm::mat4(1);
        flag_pole_model->model = glm::translate(flag_pole_model->model, glm::vec3(hole_pos.x,  0.25f + hole_pos.y, hole_pos.z));

        camera_target = glm::vec3(0.0f, 0.0f, 0.0f);
        camera_direction = glm::normalize(camera_position - camera_target);

        up = glm::vec3(0.0f, 1.0f, 0.0f);
        camera_right = glm::normalize(glm::cross(up, camera_direction));

        camera_up = glm::normalize(glm::cross(camera_direction, camera_right));
        view = glm::lookAt(camera_position,camera_target, camera_up);

        shadow_shader->use();

        int light_space_matrix_loc = glGetUniformLocation(shadow_shader->get_id(), "light_space_matrix");
        glUniformMatrix4fv(light_space_matrix_loc, 1, GL_FALSE, glm::value_ptr(light_space_matrix));

        glViewport(0, 0, shadow_width, shadow_height);
        glBindFramebuffer(GL_FRAMEBUFFER, depth_map_fbo);
        glClear(GL_DEPTH_BUFFER_BIT);

        for(auto model : models)
        {
            model->render(shadow_shader);
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);


        glViewport(0, 0, 1280, 720);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        for(auto model : models)
        {

            ShaderProgram* current_shader = model->get_shader();
            current_shader->use();

            light_space_matrix_loc = glGetUniformLocation(current_shader->get_id(), "light_space_matrix");
            glUniformMatrix4fv(light_space_matrix_loc, 1, GL_FALSE, glm::value_ptr(light_space_matrix));

            int texture_uniform_location = glGetUniformLocation(current_shader->get_id(), "current_texture");
            glUniform1i(texture_uniform_location, 0);

            int view_location = glGetUniformLocation(current_shader->get_id(), "view");
            glUniformMatrix4fv(view_location, 1, GL_FALSE, glm::value_ptr(view));

            int projection_location = glGetUniformLocation(current_shader->get_id(), "projection");
            glUniformMatrix4fv(projection_location, 1, GL_FALSE, glm::value_ptr(proj));

            int light_pos_location = glGetUniformLocation(current_shader->get_id(), "light_position");
            glUniform3fv(light_pos_location, 1, &light_pos[0]);

            int view_pos_location = glGetUniformLocation(current_shader->get_id(), "view_position");
            glUniform3fv(view_pos_location, 1, &camera_position[0]);

            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, depth_map);
            model->render();
        }

        if(render_points)
        {
            // simulate 30 frames
            glm::vec3 points_velocity = target_velocity;
            glm::vec3 points_pos = ball_pos;

            if(!ball_launched)
            {
                for(int i = 1; i <= 30; i++)
                {
                    points_velocity.x = apply_friction(points_velocity.x, ball_friction);
                    points_velocity.z = apply_friction(points_velocity.z, ball_friction);

                    glm::vec3 gradient = terrain.get_gradient(points_pos.x, points_pos.z);
                    points_velocity += gradient;

                    if(glm::length(points_velocity) < 0.005f)
                    {
                        points_velocity = glm::vec3(0, 0, 0);
                    }

                    points_pos += points_velocity * 0.016f;

                    if(i % 6 == 0)
                    {
                        int offset = 3*((i/6) - 1);

                        points[offset] = points_pos[0];
                        points[offset + 1] = -0.75f + terrain.get_height(points_pos[0], points_pos[2]) * 10.f;
                        points[offset + 2] =  points_pos[2];
                    }
                }
            }

            glBindVertexArray(points_vao);
            glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
            glBufferSubData(GL_ARRAY_BUFFER,0, points.size() * sizeof(float), points.data());
            glBindBuffer(GL_ARRAY_BUFFER, 0);

            dots_shader->use();
            int view_location = glGetUniformLocation(dots_shader->get_id(), "view");
            glUniformMatrix4fv(view_location, 1, GL_FALSE, glm::value_ptr(view));

            int projection_location = glGetUniformLocation(dots_shader->get_id(), "projection");
            glUniformMatrix4fv(projection_location, 1, GL_FALSE, glm::value_ptr(proj));
            glBindVertexArray(points_vao);
            glDrawArrays(GL_POINTS, 0, points.size() / 3);
            glBindVertexArray(0);
        }

        uint32_t current_time_ms = SDL_GetTicks();
        delta = current_time_ms -  last_time_ms;
        delta_t = static_cast<float>(delta) / 1000.f;
        accumulator += delta;
        accumulator_f += delta_t;
        last_time_ms = current_time_ms;

        if(delta < ms_per_tick)
            SDL_Delay(ms_per_tick - delta);

        SDL_GL_SwapWindow(window);
    }

    SDL_GL_DeleteContext(main_context);
    SDL_DestroyWindow(window);
    SDL_Quit();
    
    return 0;
}
