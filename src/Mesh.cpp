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

#include "Mesh.h"

Mesh::Mesh()
    : mVboPtr(nullptr), mIboPtr(nullptr), mVao(0)
{
    
}

Mesh::~Mesh()
{
    GLCall( glDeleteVertexArrays(1, &mVao) );
}

void Mesh::SetVerticies(VertexBuffer *vertexBuffer, unsigned int position)
{
    mVboPtr = vertexBuffer;
    if(mVao == 0)
    {
        GLCall( glGenVertexArrays(1, &mVao) );
    }
    GLCall( glBindVertexArray(mVao) );
    mVboPtr[0].Bind();
    GLCall( glEnableVertexAttribArray(position) );
    GLCall( glVertexAttribPointer(position, 3, GL_FLOAT, GL_FALSE, 0, 0) );
}

void Mesh::SetIndices(IndexBuffer *indexBuffer)
{
    mIboPtr = indexBuffer;
    if(mVao == 0)
    {
        GLCall( glGenVertexArrays(1, &mVao) );
    }
    GLCall( glBindVertexArray(mVao) );
    mIboPtr[0].Bind();
}

void Mesh::Render(Shader* shader)
{
    if(mVboPtr == nullptr && mIboPtr == nullptr)
    {
        std::cout << "Nothing to draw..." << std::endl;
        return;
    }
    else if(mIboPtr == nullptr && mVboPtr != nullptr)
    {
        shader[0].Bind();
        GLCall( glBindVertexArray(mVao) );
        glDrawArrays(GL_TRIANGLES, 0, mVboPtr[0].GetCount());
        return;
    }
    else if(mIboPtr != nullptr && mVboPtr != nullptr)
    {
        shader[0].Bind();
        GLCall( glBindVertexArray(mVao) );
        glDrawElements(GL_TRIANGLES, mIboPtr[0].GetCount(), GL_UNSIGNED_INT, nullptr);
        return;
    }
}
