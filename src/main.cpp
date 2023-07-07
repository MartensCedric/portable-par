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
#include "shader_program.h"
#include "model.h"


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

    SDL_Window* window = SDL_CreateWindow("GMTK-2023", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1280, 720, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
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

    glm::mat4 proj = glm::perspective(glm::radians(45.f), (float)1280.f/(float)720.f, 0.1f, 100.f);
    glm::mat4 model = glm::mat4(1.0f);

    glm::mat4 view = glm::mat4(1.0f);
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -5.0f));

    glm::vec3 light_pos = glm::vec3(5.0, 10.0, 2.0);

    Model* current_model = new Model("assets/mesh/test.obj");
    Shader* v_passthrough = new Shader("assets/shaders/passthrough.vs", shader_type::vertex);
    Shader* f_passthrough = new Shader("assets/shaders/passthrough.fs", shader_type::fragment);
    Shader* f_phong = new Shader("assets/shaders/phong.fs", shader_type::fragment);

    if(!v_passthrough->compile()) return -1;
    if(!f_passthrough->compile()) return -1;
    if(!f_phong->compile()) return -1;

    ShaderProgram* current_shader = new ShaderProgram(v_passthrough, f_phong);

    delete v_passthrough;
    delete f_passthrough;
    delete f_phong;

    Texture tex_checkerboard;
    tex_checkerboard.load("assets/sprites/checkerboard.png");
    tex_checkerboard.use();

    int texture_uniform_location = glGetUniformLocation(current_shader->get_id(), "current_texture");
    current_shader->use();
    glUniform1i(texture_uniform_location, 0);

    bool is_running = true;
    bool wireframe_mode = false;
    bool cull_faces = false;

    glClearColor(0, 0, 0, 1.0f);
    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    uint32_t last_time_ms = SDL_GetTicks();
    uint32_t ms_per_tick = 16;
    uint64_t accumulator = 0;
    uint32_t delta = 16;
    float delta_t = static_cast<float>(delta) / 1000.f;
    while(is_running)
    {
        SDL_Event e;
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
                    }
            }
            
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


        model = glm::rotate(model, glm::radians(-15.f * delta_t), glm::vec3(1.0f, -0.35f, 0.0f));


        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        current_shader->use();

        int model_location = glGetUniformLocation(current_shader->get_id(), "model");
        glUniformMatrix4fv(model_location, 1, GL_FALSE, glm::value_ptr(model));

        int view_location = glGetUniformLocation(current_shader->get_id(), "view");
        glUniformMatrix4fv(view_location, 1, GL_FALSE, glm::value_ptr(view));

        int projection_location = glGetUniformLocation(current_shader->get_id(), "projection");
        glUniformMatrix4fv(projection_location, 1, GL_FALSE, glm::value_ptr(proj));

        int light_pos_location = glGetUniformLocation(current_shader->get_id(), "light_position");
        glUniform3fv(light_pos_location, 1, &light_pos[0]);


        glActiveTexture(GL_TEXTURE0);
        tex_checkerboard.use();
        current_shader->use();
        current_model->render();

        uint32_t current_time_ms = SDL_GetTicks();
        delta = current_time_ms -  last_time_ms;
        delta_t = static_cast<float>(delta) / 1000.f;
        accumulator += delta;
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
