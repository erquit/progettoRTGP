#version 330 core

void main()
{
    //We can simply use an empty fragment shader, since we disabled the Read and Write buffers
    //In reality the following line is what's happening when we leave it blank

    // gl_FragDepth = gl_FragCoord.z;
}