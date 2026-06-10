#ifndef VERTICES_HPP
#define VERTICES_HPP

#include "mesh.hpp"
#include <reactphysics3d/reactphysics3d.h>

//Let's add a cube with diffuse map
Vertex cube_vertices[] = {
    // positions          // normals           // texture coords (x,y), 0,0 bottom left, 1,1 top right
    // Back face
    Vertex{glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec2(0.0f, 0.0f)},
    Vertex{glm::vec3( 0.5f, -0.5f, -0.5f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec2(1.0f, 0.0f)},
    Vertex{glm::vec3( 0.5f,  0.5f, -0.5f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec2(1.0f, 1.0f)},
    Vertex{glm::vec3( 0.5f,  0.5f, -0.5f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec2(1.0f, 1.0f)},
    Vertex{glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec2(0.0f, 1.0f)},
    Vertex{glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec2(0.0f, 0.0f)},
    // Front face
    Vertex{glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(0.0f,  0.0f, 1.0f), glm::vec2(0.0f, 0.0f)},
    Vertex{glm::vec3( 0.5f,  0.5f,  0.5f), glm::vec3(0.0f,  0.0f, 1.0f), glm::vec2(1.0f, 1.0f)},
    Vertex{glm::vec3( 0.5f, -0.5f,  0.5f), glm::vec3(0.0f,  0.0f, 1.0f), glm::vec2(1.0f, 0.0f)},
    Vertex{glm::vec3( 0.5f,  0.5f,  0.5f), glm::vec3(0.0f,  0.0f, 1.0f), glm::vec2(1.0f, 1.0f)},
    Vertex{glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(0.0f,  0.0f, 1.0f), glm::vec2(0.0f, 0.0f)},
    Vertex{glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(0.0f,  0.0f, 1.0f), glm::vec2(0.0f, 1.0f)},
    // Left face 
    Vertex{glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec2(1.0f, 1.0f)},
    Vertex{glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec2(0.0f, 0.0f)},
    Vertex{glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec2(0.0f, 1.0f)},
    Vertex{glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec2(0.0f, 0.0f)},
    Vertex{glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec2(1.0f, 1.0f)},
    Vertex{glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec2(1.0f, 0.0f)},
    // Right face 
    Vertex{glm::vec3( 0.5f,  0.5f,  0.5f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec2(1.0f, 0.0f)},
    Vertex{glm::vec3( 0.5f,  0.5f, -0.5f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec2(1.0f, 1.0f)},
    Vertex{glm::vec3( 0.5f, -0.5f, -0.5f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec2(0.0f, 1.0f)},
    Vertex{glm::vec3( 0.5f, -0.5f, -0.5f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec2(0.0f, 1.0f)},
    Vertex{glm::vec3( 0.5f, -0.5f,  0.5f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec2(0.0f, 0.0f)},
    Vertex{glm::vec3( 0.5f,  0.5f,  0.5f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec2(1.0f, 0.0f)},
    // Bottom face
    Vertex{glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec2(1.0f, 1.0f)},
    Vertex{glm::vec3( 0.5f, -0.5f,  0.5f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec2(0.0f, 0.0f)},
    Vertex{glm::vec3( 0.5f, -0.5f, -0.5f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec2(0.0f, 1.0f)},
    Vertex{glm::vec3( 0.5f, -0.5f,  0.5f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec2(0.0f, 0.0f)},
    Vertex{glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec2(1.0f, 1.0f)},
    Vertex{glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec2(1.0f, 0.0f)},
    // Top face
    Vertex{glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec2(0.0f, 1.0f)},
    Vertex{glm::vec3( 0.5f,  0.5f, -0.5f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec2(1.0f, 1.0f)},
    Vertex{glm::vec3( 0.5f,  0.5f,  0.5f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec2(1.0f, 0.0f)},
    Vertex{glm::vec3( 0.5f,  0.5f,  0.5f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec2(1.0f, 0.0f)},
    Vertex{glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec2(0.0f, 0.0f)},  
    Vertex{glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec2(0.0f, 1.0f)}    
};


//Let's add a cube with diffuse map
Vertex cube_vertices_inverted[] = {
    // positions          // normals           // texture coords (x,y), 0,0 bottom left, 1,1 top right
    // Back face
    Vertex{glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.0f,  0.0f, 1.0f), glm::vec2(0.0f, 0.0f)},
    Vertex{glm::vec3( 0.5f, -0.5f, -0.5f), glm::vec3(0.0f,  0.0f, 1.0f), glm::vec2(1.0f, 0.0f)},
    Vertex{glm::vec3( 0.5f,  0.5f, -0.5f), glm::vec3(0.0f,  0.0f, 1.0f), glm::vec2(1.0f, 1.0f)},
    Vertex{glm::vec3( 0.5f,  0.5f, -0.5f), glm::vec3(0.0f,  0.0f, 1.0f), glm::vec2(1.0f, 1.0f)},
    Vertex{glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(0.0f,  0.0f, 1.0f), glm::vec2(0.0f, 1.0f)},
    Vertex{glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.0f,  0.0f, 1.0f), glm::vec2(0.0f, 0.0f)},
    // Front face
    Vertex{glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec2(0.0f, 0.0f)},
    Vertex{glm::vec3( 0.5f,  0.5f,  0.5f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec2(1.0f, 1.0f)},
    Vertex{glm::vec3( 0.5f, -0.5f,  0.5f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec2(1.0f, 0.0f)},  
    Vertex{glm::vec3( 0.5f,  0.5f,  0.5f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec2(1.0f, 1.0f)},
    Vertex{glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec2(0.0f, 0.0f)},
    Vertex{glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec2(0.0f, 1.0f)},   
    // Left face 
    Vertex{glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec2(1.0f, 1.0f)},
    Vertex{glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec2(0.0f, 0.0f)},
    Vertex{glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec2(0.0f, 1.0f)},   
    Vertex{glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec2(0.0f, 0.0f)},
    Vertex{glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec2(1.0f, 1.0f)},
    Vertex{glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec2(1.0f, 0.0f)},
    // Right face 
    Vertex{glm::vec3( 0.5f,  0.5f,  0.5f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec2(1.0f, 0.0f)},
    Vertex{glm::vec3( 0.5f,  0.5f, -0.5f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec2(1.0f, 1.0f)},
    Vertex{glm::vec3( 0.5f, -0.5f, -0.5f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec2(0.0f, 1.0f)},
    Vertex{glm::vec3( 0.5f, -0.5f, -0.5f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec2(0.0f, 1.0f)},
    Vertex{glm::vec3( 0.5f, -0.5f,  0.5f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec2(0.0f, 0.0f)},
    Vertex{glm::vec3( 0.5f,  0.5f,  0.5f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec2(1.0f, 0.0f)},
    // Bottom face
    Vertex{glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.0f, 1.0f,  0.0f), glm::vec2(1.0f, 1.0f)},
    Vertex{glm::vec3( 0.5f, -0.5f,  0.5f), glm::vec3(0.0f, 1.0f,  0.0f), glm::vec2(0.0f, 0.0f)},
    Vertex{glm::vec3( 0.5f, -0.5f, -0.5f), glm::vec3(0.0f, 1.0f,  0.0f), glm::vec2(0.0f, 1.0f)},
    Vertex{glm::vec3( 0.5f, -0.5f,  0.5f), glm::vec3(0.0f, 1.0f,  0.0f), glm::vec2(0.0f, 0.0f)},
    Vertex{glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.0f, 1.0f,  0.0f), glm::vec2(1.0f, 1.0f)},
    Vertex{glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(0.0f, 1.0f,  0.0f), glm::vec2(1.0f, 0.0f)},
    // Top face
    Vertex{glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec2(0.0f, 1.0f)},
    Vertex{glm::vec3( 0.5f,  0.5f, -0.5f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec2(1.0f, 1.0f)},
    Vertex{glm::vec3( 0.5f,  0.5f,  0.5f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec2(1.0f, 0.0f)},
    Vertex{glm::vec3( 0.5f,  0.5f,  0.5f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec2(1.0f, 0.0f)},
    Vertex{glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec2(0.0f, 0.0f)},
    Vertex{glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec2(0.0f, 1.0f)},
};


Vertex cube_side[] = {
    // positions          // normals           // texture coords (x,y), 0,0 bottom left, 1,1 top right
    // Back face
    Vertex{glm::vec3(-0.5f, -0.5f, -0.01f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec2(0.0f, 0.0f)},
    Vertex{glm::vec3( 0.5f, -0.5f, -0.01f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec2(1.0f, 0.0f)},
    Vertex{glm::vec3( 0.5f,  0.5f, -0.01f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec2(1.0f, 1.0f)},
    Vertex{glm::vec3( 0.5f,  0.5f, -0.01f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec2(1.0f, 1.0f)},
    Vertex{glm::vec3(-0.5f,  0.5f, -0.01f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec2(0.0f, 1.0f)},
    Vertex{glm::vec3(-0.5f, -0.5f, -0.01f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec2(0.0f, 0.0f)},
    // Front face
    Vertex{glm::vec3(-0.5f, -0.5f,  0.01f), glm::vec3(0.0f,  0.0f, 1.0f), glm::vec2(0.0f, 0.0f)},
    Vertex{glm::vec3( 0.5f,  0.5f,  0.01f), glm::vec3(0.0f,  0.0f, 1.0f), glm::vec2(1.0f, 1.0f)},
    Vertex{glm::vec3( 0.5f, -0.5f,  0.01f), glm::vec3(0.0f,  0.0f, 1.0f), glm::vec2(1.0f, 0.0f)},
    Vertex{glm::vec3( 0.5f,  0.5f,  0.01f), glm::vec3(0.0f,  0.0f, 1.0f), glm::vec2(1.0f, 1.0f)},
    Vertex{glm::vec3(-0.5f, -0.5f,  0.01f), glm::vec3(0.0f,  0.0f, 1.0f), glm::vec2(0.0f, 0.0f)},
    Vertex{glm::vec3(-0.5f,  0.5f,  0.01f), glm::vec3(0.0f,  0.0f, 1.0f), glm::vec2(0.0f, 1.0f)},
    // Left face 
    Vertex{glm::vec3(-0.5f,  0.5f,  0.01f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec2(1.0f, 1.0f)},
    Vertex{glm::vec3(-0.5f, -0.5f, -0.01f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec2(0.0f, 0.0f)},
    Vertex{glm::vec3(-0.5f,  0.5f, -0.01f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec2(0.0f, 1.0f)},
    Vertex{glm::vec3(-0.5f, -0.5f, -0.01f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec2(0.0f, 0.0f)},
    Vertex{glm::vec3(-0.5f,  0.5f,  0.01f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec2(1.0f, 1.0f)},
    Vertex{glm::vec3(-0.5f, -0.5f,  0.01f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec2(1.0f, 0.0f)},
    // Right face 
    Vertex{glm::vec3( 0.5f,  0.5f,  0.01f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec2(1.0f, 0.0f)},
    Vertex{glm::vec3( 0.5f,  0.5f, -0.01f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec2(1.0f, 1.0f)},
    Vertex{glm::vec3( 0.5f, -0.5f, -0.01f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec2(0.0f, 1.0f)},
    Vertex{glm::vec3( 0.5f, -0.5f, -0.01f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec2(0.0f, 1.0f)},
    Vertex{glm::vec3( 0.5f, -0.5f,  0.01f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec2(0.0f, 0.0f)},
    Vertex{glm::vec3( 0.5f,  0.5f,  0.01f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec2(1.0f, 0.0f)},
    // Bottom face                                                                                
    Vertex{glm::vec3(-0.5f, -0.5f, -0.01f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec2(1.0f, 1.0f)}, 
    Vertex{glm::vec3( 0.5f, -0.5f,  0.01f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec2(0.0f, 0.0f)},
    Vertex{glm::vec3( 0.5f, -0.5f, -0.01f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec2(0.0f, 1.0f)},  
    Vertex{glm::vec3( 0.5f, -0.5f,  0.01f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec2(0.0f, 0.0f)},
    Vertex{glm::vec3(-0.5f, -0.5f, -0.01f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec2(1.0f, 1.0f)},
    Vertex{glm::vec3(-0.5f, -0.5f,  0.01f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec2(1.0f, 0.0f)},
    // Top face
    Vertex{glm::vec3(-0.5f,  0.5f, -0.01f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec2(0.0f, 1.0f)},
    Vertex{glm::vec3( 0.5f,  0.5f, -0.01f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec2(1.0f, 1.0f)},
    Vertex{glm::vec3( 0.5f,  0.5f,  0.01f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec2(1.0f, 0.0f)},
    Vertex{glm::vec3( 0.5f,  0.5f,  0.01f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec2(1.0f, 0.0f)},
    Vertex{glm::vec3(-0.5f,  0.5f,  0.01f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec2(0.0f, 0.0f)},
    Vertex{glm::vec3(-0.5f,  0.5f, -0.01f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec2(0.0f, 1.0f)},
};





unsigned int cubeIndices[] = {
    0, 1, 2,
    3, 4, 5,

    6, 7, 8,
    9, 10, 11,
    
    12, 13, 14,
    15, 16, 17,

    18, 19, 20,
    21, 22, 23,

    24, 25, 26,
    27, 28, 29,

    30, 31, 32,
    33, 34, 35,
};

Vertex plane_vertices[] = {
    Vertex{glm::vec3(-1.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f)}, //bottom left
    Vertex{glm::vec3(1.0f, 0.0f, -1.0f), glm::vec3(0.0f,  1.0f, 0.0f), glm::vec2(10.0f, 0.0f)}, //bottom right
    Vertex{glm::vec3(1.0f, 0.0f,  1.0f), glm::vec3(0.0f,  1.0f, 0.0f), glm::vec2(10.0f, 10.0f)},  //top right
    Vertex{glm::vec3(-1.0f, 0.0f,  1.0f), glm::vec3(0.0f,  1.0f, 0.0f), glm::vec2(0.0f, 10.0f)}, //top left
};

unsigned int planeIndices[] = {
    0, 1, 2,
    2, 3, 0,
};

float skyboxVertices[] =
{
    //   Coordinates
    -1.0f, -1.0f,  1.0f,
    1.0f, -1.0f,  1.0f,
    1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,
    1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f
};

unsigned int skyboxIndices[] =
{
    // Right
    1, 2, 6,
    6, 5, 1,
    // Left
    0, 4, 7,
    7, 3, 0,
    // Top
    4, 5, 6,
    6, 7, 4,
    // Bottom
    0, 3, 2,
    2, 1, 0,
    // Back
    0, 1, 5,
    5, 4, 0,
    // Front
    3, 7, 6,
    6, 2, 3
};

//Crosshair
float crosshairSize = 0.02f;

float crosshairVertices[] = {
    // Horizontal line
    -crosshairSize,  0.0f,
    crosshairSize,  0.0f,
    // Vertical line
    0.0f, -crosshairSize * 1.77f,
    0.0f,  crosshairSize * 1.77f
};

//Conversion for Mesh class
std::vector<Vertex> cubeVertices(
    cube_vertices, 
    cube_vertices + sizeof(cube_vertices) / sizeof(Vertex)
);

std::vector<Vertex> cubeVerticesInvertedNorm(
    cube_vertices_inverted, 
    cube_vertices_inverted + sizeof(cube_vertices_inverted) / sizeof(Vertex)
);

std::vector<unsigned int> cubeIndicesVec(
    cubeIndices,
    cubeIndices + sizeof(cubeIndices) / sizeof(unsigned int)
);

std::vector<Vertex> planeVertices(
    plane_vertices,
    plane_vertices + sizeof(plane_vertices) / sizeof(Vertex)
);

std::vector<unsigned int> planeIndicesVec(
    planeIndices,
    planeIndices + sizeof(planeIndices) / sizeof(unsigned int)
);

std::vector<Vertex> cubeSideVertices(
    cube_side, 
    cube_side + sizeof(cube_side) / sizeof(Vertex)
);


//Cube positions
glm::vec3 cubePositions[] = {
    glm::vec3(0.0f,  1.0f,  2.0f),
    glm::vec3(0.0f,  1.0f,  3.25f),
    glm::vec3(0.0f,  2.0f,  2.75f),

    glm::vec3(0.0f,  1.0f,  -2.0f),
    glm::vec3(0.0f,  1.0f,  -3.25f),
    glm::vec3(0.0f,  2.0f,  -2.75f),

    glm::vec3(2.0f,  1.0f,  0.0f),
    glm::vec3(3.25f,  1.0f,  0.0f),
    glm::vec3(2.75f,  2.0f,  0.0f),

    glm::vec3(-2.0f,  1.0f,  0.0f),
    glm::vec3(-3.25f,  1.0f,  0.0f),
    glm::vec3(-2.75f,  2.0f,  0.0f),

    glm::vec3(2.0f,  1.0f,  -3.0f),
    glm::vec3(2.0f,  2.0f,  -3.0f),
    glm::vec3(3.0f,  1.0f,  -3.0f),

    glm::vec3(0.0f,  0.5f,  0.0f),
};
int numCubes = (int) (sizeof(cubePositions)/ sizeof(glm::vec3));

glm::vec3 emissionCubePositions[] = {
    glm::vec3(0.0f,  -24.0f,  0.0f),
    glm::vec3(0.0f,  -24.0f,  4.0f),
    glm::vec3(0.0f,  -24.0f,  -4.0f),

    glm::vec3(2.0f,  -24.0f,  0.0f),
    glm::vec3(2.0f,  -24.0f,  2.0f),
    glm::vec3(2.0f,  -24.0f,  -2.0f),
    glm::vec3(4.0f,  -24.0f,  0.0f),
    glm::vec3(4.0f,  -24.0f,  4.0f),
    glm::vec3(4.0f,  -24.0f,  -4.0f),

    glm::vec3(-2.0f,  -24.0f,  0.0f),
    glm::vec3(-2.0f,  -24.0f,  2.0f),
    glm::vec3(-2.0f,  -24.0f,  -2.0f),
    glm::vec3(-4.0f,  -24.0f,  0.0f),
    glm::vec3(-4.0f,  -24.0f,  4.0f),
    glm::vec3(-4.0f,  -24.0f,  -4.0f),
};
int numEmissionCubes = (int) (sizeof(emissionCubePositions)/ sizeof(glm::vec3));



//Let's define our light cube pos
glm::vec3 pointLightPositions[] = {
    glm::vec3( 5.0f,  5.0f,  0.0f),
};

glm::vec3 vasePos(10.0f, 1.0f, 0.0f);

std::vector <glm::vec2> fragmentHitBox =  {
    glm::vec2(0.20f, 0.20f),
    glm::vec2(0.2f, 0.2f),
    glm::vec2(0.2f, 0.2f),
    glm::vec2(0.2f, 0.2f),
    glm::vec2(0.2f, 0.2f)
};

glm::mat4 vaseFragmentsModel[] =  
{
    glm::mat4(1.0f),
    glm::mat4(1.0f),
    glm::mat4(1.0f),
    glm::mat4(1.0f),
    glm::mat4(1.0f)
};


glm::vec3 emissionPointLightPositions[] = {
    glm::vec3(0.0f,  -24.0f,  0.0f),
    glm::vec3(0.0f,  -24.0f,  4.0f),
    glm::vec3(0.0f,  -24.0f,  -4.0f),
    glm::vec3(2.0f,  -24.0f,  0.0f),
    glm::vec3(2.0f,  -24.0f,  2.0f),
    glm::vec3(2.0f,  -24.0f,  -2.0f),
    glm::vec3(4.0f,  -24.0f,  0.0f),
    glm::vec3(4.0f,  -24.0f,  4.0f),
    glm::vec3(4.0f,  -24.0f,  -4.0f),
    glm::vec3(-2.0f, -24.0f,  0.0f),
    glm::vec3(-2.0f, -24.0f,  2.0f),
    glm::vec3(-2.0f, -24.0f, -2.0f),
    glm::vec3(-4.0f, -24.0f,  0.0f),
    glm::vec3(-4.0f, -24.0f,  4.0f),
    glm::vec3(-4.0f, -24.0f, -4.0f)
};
const int numEmissionPointLights = sizeof(emissionPointLightPositions) / sizeof(glm::vec3);

// Maoi stuff
glm::vec3 maoiPos(10.0f, 1.0f, 2.0f);

std::vector<reactphysics3d::Vector3> maoiFragmentHitBox = {
    reactphysics3d::Vector3(0.25f, 0.2f, 0.18f),
    reactphysics3d::Vector3(0.22f, 0.23f, 0.4f),
    reactphysics3d::Vector3(0.13f, 0.18f, 0.25f),
    reactphysics3d::Vector3(0.3f, 0.23f, 0.27f),
    reactphysics3d::Vector3(0.22f, 0.23f, 0.5f),
};


const reactphysics3d::Vector3 halfExtets(0.5f, 0.5f, 0.5f);

glm::mat4 maoiFragmentsModels[5];

#endif