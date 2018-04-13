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

#include "Game.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Terrain.h"

class Test : public Game
{
    vmath::vec4 bgColor;            // Background color
    Shader renderShader;            // Shader program
    vmath::mat4 projection;         // Projection matrix
    float aspect = 800.0f/600.0f;   // Aspect ratio
    int previousAction = 0;         // GLFW previous keyboard action
    bool wireframe = false;         // Wireframe mode
    Terrain terrain;                // Terrain Digital Elevation Model (DEM)

    // Initialize settings
    void init()
    {
        strcpy(info.title, "Terrain Generator");
        info.windowWidth = 800;
        info.windowHeight = 600;
        info.majorVersion = 4;
        info.minorVersion = 5;
        info.samples = 16;
        info.flags.all = 0;
        info.flags.cursor = 1;
    }

    // Start-up operations
    void startup()
    {
        bgColor = vmath::vec4(0.9f, 0.9f, 0.9f, 1.0f);
        std::string shaderPath = "res/shaders/render.glsl";
        renderShader = Shader(shaderPath);
        
        // Generate terrain DEM
        terrain.GenTerrain(10, 0.7f);

        // Culling and depth testing
        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
    }

    // Render loop
    void render(double currentTime)
    {
        glClear(GL_DEPTH_BUFFER_BIT);
        glClearBufferfv(GL_COLOR, 0, bgColor);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        if(wireframe)
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }

        // Set shader uniforms
        float t = (float)currentTime;
        glUniform1f(3, t);
        glUniform1f(4, terrain.maxElevation);
        glUniform1f(5, terrain.minElevation);
        projection = vmath::perspective(60.0f, aspect, 0.001f, 100.0f) * vmath::translate(vmath::vec3(0.0f, 0.0f, -2.0f)) * vmath::rotate(45.0f, vmath::vec3(-1.0f, 0.0f, 0.0f)) * vmath::rotate(t*5.0f, vmath::vec3(0.0f, 0.0f, -1.0f));
        glUniformMatrix4fv(2, 1, GL_FALSE, projection);
        
        // Render the terrain
        terrain.Render(&renderShader);
    }

    // Clean up
    void shutdown()
    {
        terrain.~Terrain();
    }

    // Resize window callback
    void onResize(int w, int h)
    {
        info.windowWidth = w;
        info.windowHeight = h;
        aspect = (float)w/(float)h;
        glViewport(0, 0, w, h);
    }

    // Key press callback
    void onKey(int key, int action)
    {
        if(key == GLFW_KEY_W && action == GLFW_RELEASE && previousAction == GLFW_PRESS)
        {
            wireframe = !wireframe;
        }
        if(key == GLFW_KEY_R && action == GLFW_RELEASE && previousAction == GLFW_PRESS)
        {
            terrain.GenTerrain(10, 0.7f);
        }
        previousAction = action;
    }
};

// Program entry point
int main(void)
{
    Test* test = new Test();
    test->run(test);
    delete test;
    return 0;
}