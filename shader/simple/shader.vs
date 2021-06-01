#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 3) in mat4 aInstanceMatrix;

out vec4 ourColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec4 color;

void main()
{
    vec4 coordtrans = projection * view * aInstanceMatrix * vec4(aPos, 1.0);
    gl_Position = vec4(coordtrans.x, -coordtrans.y, coordtrans.z, coordtrans.w);
    ourColor = aColor;
}
