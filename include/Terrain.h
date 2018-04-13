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

#ifndef __TERRAIN__
#define __TERRAIN__

#include "Mesh.h"
#include "vmath.h"

class Terrain : public Mesh
{
private:

    float** genMap(int length)
    {
    	const int n = length;

    	float** map = new float *[n];
    	for(int i = 0; i < n; i++)
    	{
    		map[i] = new float[n];
    		for(int j = 0; j < n; j++)
            {
                map[i][j] = 0.0f;
            }
    	}
    	return map;
    }

    double fRand(float fMin, float fMax)
    {
    	float f = (float)rand() / RAND_MAX;
    	return fMin + f * (fMax - fMin);
    }

public:
    Terrain();
    ~Terrain();
    float minElevation;
    float maxElevation;

    void GenTerrain(unsigned char detailLevel, float range);
};

#endif//__TERRAIN__