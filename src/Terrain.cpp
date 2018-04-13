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

#include "Terrain.h"

Terrain::Terrain()
{

}

Terrain::~Terrain()
{

}

void Terrain::GenTerrain(const unsigned char detailLevel, float range)
{
    const int n = pow(2,detailLevel) + 1;   // DEM length
    const int m = 6*pow(n-1,2);             // Index count
    minElevation = 0.0f;                    // DEM min elevation
    maxElevation = 0.0f;                    // DEM max elevation
    srand(time(NULL));                      // Seed random number generator
    float** map = genMap(n);                // Initialize heightmap
    
    // Set corners
    map[0][0] = 0.0f;
    map[0][n-1] = 0.0f;
    map[n-1][0] = 0.0f;
    map[n-1][n-1] = 0.0f;

    // Generate heightmap using diamond-square alogrithm
    // https://en.wikipedia.org/wiki/Diamond-square_algorithm
    for(int sideLength = n-1; sideLength >= 2; sideLength /= 2, range /= 2)
	{
		int halfSide = sideLength/2;
        // Diamond step
		for(int x = 0; x < n-1; x += sideLength)
	    {
	    	for(int y = 0; y < n-1; y += sideLength)
	    	{
	    		double avg = map[x][y] + map[x+sideLength][y] + map[x][y+sideLength] + map[x+sideLength][y+sideLength];
	    		avg /= 4.0;
	    		map[x+halfSide][y+halfSide] = avg + fRand(-range, range);
                if(map[x+halfSide][y+halfSide] < minElevation)
                {
                    minElevation = map[x+halfSide][y+halfSide];
                }
                else if(map[x+halfSide][y+halfSide] > maxElevation)
                {
                    maxElevation = map[x+halfSide][y+halfSide];
                }
	    	}
        }
        // Square step
		for(int x = 0; x < n-1; x += halfSide)
	    {
	    	for(int y = (x+halfSide)%sideLength; y < n -1; y +=sideLength)
	    	{
	    		double avg = map[(x-halfSide+n-1)%(n-1)][y] +
	    			map[(x+halfSide)%(n-1)][y] +
	    			map[x][(y+halfSide)%(n-1)] +
	    			map[x][(y-halfSide+n-1)%(n-1)];
	    		avg /= 4.0 + fRand(-range, range);
	    		map[x][y] = avg;

	    		if(x == 0) map[n-1][y] = avg;
	    		if(y == 0) map[x][n-1] = avg;

                if(map[x][y] < minElevation)
                {
                    minElevation = map[x][y];
                }
                else if(map[x][y] > maxElevation)
                {
                    maxElevation = map[x][y];
                }
	    	}
        }
    }
    
    // Generate vertices
    vmath::vec3* vertices = new vmath::vec3[n*n];
    for(int i = 0; i < n; i++)
    {
        float y = -(2.0f*((float)i/(float)(n - 1)) - 1.0f);
        for(int j = 0; j < n; j++)
        {
            float x = 2.0f*((float)j/(float)(n - 1)) - 1.0f;
            vertices[n*i + j] = vmath::vec3(x, y, map[j][i]);
        }
    }

    // Order to render vertices
    unsigned int* indices = new unsigned int[m];
    for(int i = 0; i < n-1; i++)
    {
        for(int j = 0; j < n-1; j++)
        {
            indices[6*(n-1)*i+6*j+0] = n + j + i*n;
            indices[6*(n-1)*i+6*j+1] = 0 + j + i*n;
            indices[6*(n-1)*i+6*j+2] = 1 + j + i*n;
            indices[6*(n-1)*i+6*j+3] = 1 + j + i*n;
            indices[6*(n-1)*i+6*j+4] = n+1 + j + i*n;
            indices[6*(n-1)*i+6*j+5] = n + j + i*n;
        }
    }

    // Normal vectors for lighting
    vmath::vec3* faceNormals = new vmath::vec3[m/3];
    for(int i = 0; i < m/3; i++)
    {
        faceNormals[i] = vmath::cross(vertices[indices[3*i+2]] - vertices[indices[3*i]], vertices[indices[3*i+1]] - vertices[indices[3*i]]);
    }
    // TODO: average face normals per vertex
    vmath::vec3* vertexNormals = new vmath::vec3[n*n];
    for(int i = 0; i < m/3; i++)
    {
        vertexNormals[indices[3*i]] = faceNormals[i];
        vertexNormals[indices[3*i+1]] = faceNormals[i];
        vertexNormals[indices[3*i+2]] = faceNormals[i];
    }

    // Buffer data
    mVbo.CreateBuffer(vertices, n*n, sizeof(vmath::vec3));
    mNbo.CreateBuffer(vertexNormals, n*n, sizeof(vmath::vec3));
    mIbo.CreateBuffer(indices, m);
    // Clean up
    delete[] vertices;
    delete[] faceNormals;
    delete[] vertexNormals;
    delete[] indices;
    for(int i = 0; i < n; i++)
    {
        delete[] map[i];
    }
    // Create vertex array object
    SetVerticies(&mVbo, 0);
    SetVerticies(&mNbo, 1);
    SetIndices(&mIbo);
}
