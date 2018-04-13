/*
 * Copyright (c) 2018 Brendan Barnes
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#ifndef __GAME__
#define __GAME__

#define GLEW_STATIC

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include <iostream>
#include <string.h>
#include <vmath.h>
#include "Shader.h"

class Game
{
public:
    Game() {}
    virtual ~Game() {}
    void run(Game* the_game)
    {
        game = the_game;
        bool running = true;

        if (!glfwInit())
        {
            std::cout << "Failed to initialize GLFW!" << std::endl;
            return;
        }

        init();

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, info.majorVersion);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, info.minorVersion);


        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_SAMPLES, info.samples);

        window = glfwCreateWindow(info.windowWidth, info.windowHeight, info.title, info.flags.fullscreen ? glfwGetPrimaryMonitor() : NULL, NULL);
        if (!window)
        {
            std::cout << "Failed to open window!" << std::endl;
            return;
        }

        glfwMakeContextCurrent(window);

        glfwSetWindowSizeCallback(window, glfw_onResize);
        glfwSetKeyCallback(window, glfw_onKey);
        glfwSetMouseButtonCallback(window, glfw_onMouseButton);
        glfwSetCursorPosCallback(window, glfw_onMouseMove);
        glfwSetScrollCallback(window, glfw_onMouseWheel);

        if (!info.flags.cursor)
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
        }

        if(glewInit() != GLEW_OK)
        {
            std::cout << "GLEW failed to initialize" << std::endl;
        }

        startup();

        do
        {
            render(glfwGetTime());

            glfwSwapBuffers(window);
            glfwPollEvents();

            running &= (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_RELEASE);
            running &= (glfwWindowShouldClose(window) != GL_TRUE);
        } while (running);

        shutdown();

        glfwDestroyWindow(window);
        glfwTerminate();

    }

    virtual void init()
    {
        strcpy(info.title, "Untitled");
        info.windowWidth = 800;
        info.windowHeight = 600;
        info.majorVersion = 4;
        info.minorVersion = 5;
        info.samples = 0;
        info.flags.all = 0;
        info.flags.cursor = 1;
    }

    virtual void startup()
    {

    }

    virtual void render(double currentTime)
    {

    }

    virtual void shutdown()
    {

    }

    void setWindowTitle(const char * title)
    {
        glfwSetWindowTitle(window, title);
    }

    virtual void onResize(int w, int h)
    {
        info.windowWidth = w;
        info.windowHeight = h;
    }

    virtual void onKey(int key, int action)
    {

    }

    virtual void onMouseButton(int button, int action)
    {

    }

    virtual void onMouseMove(int x, int y)
    {

    }

    virtual void onMouseWheel(int pos)
    {

    }

    void getMousePosition(int& x, int& y)
    {
        double dx, dy;
        glfwGetCursorPos(window, &dx, &dy);

        x = static_cast<int>(floor(dx));
        y = static_cast<int>(floor(dy));
    }

    struct GameInfo
    {
        char title[128];
        int windowWidth;
        int windowHeight;
        int majorVersion;
        int minorVersion;
        int samples;
        union
        {
            struct
            {
                unsigned int    fullscreen  : 1;
                unsigned int    vsync       : 1;
                unsigned int    cursor      : 1;
            };
            unsigned int        all;
        } flags;
    };

protected:
    static Game* game;
    GLFWwindow* window;
    GameInfo info;

    static void glfw_onResize(GLFWwindow* window, int w, int h)
    {
        game->onResize(w, h);
    }

    static void glfw_onKey(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        game->onKey(key, action);
    }

    static void glfw_onMouseButton(GLFWwindow* window, int button, int action, int mods)
    {
        game->onMouseButton(button, action);
    }

    static void glfw_onMouseMove(GLFWwindow* window, double x, double y)
    {
        game->onMouseMove(static_cast<int>(x), static_cast<int>(y));
    }

    static void glfw_onMouseWheel(GLFWwindow* window, double xoffset, double yoffset)
    {
        game->onMouseWheel(static_cast<int>(yoffset));
    }

};

Game* Game::game = 0;

#endif//__GAME