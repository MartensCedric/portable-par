#include <iostream>
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <GL/gl.h>

int main(int argc, char** argv)
{
    std::cout << "Hello World!" << std::endl;
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

    glClearColor(0, 0, 0, 1.0f);

    SDL_UpdateWindowSurface(window);
    SDL_Delay(5000);


    SDL_GL_DeleteContext(main_context);
    SDL_DestroyWindow(window);
    SDL_Quit();
    
    return 0;
}
