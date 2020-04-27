#version 400

layout(location=0) in vec3 position;
layout(location=1) in vec3 normal;
layout(location=2) in vec2 textureCoords;
layout(location=3) in mat4 model;

uniform mat4 projection;
uniform mat4 view;

out VS_OUT{
    vec3 normal,fragPos;
    vec2 textureCoords;
} vs_out;

void main() {
    gl_Position = projection * view * model * vec4(position ,1.0);
    vs_out.textureCoords = textureCoords;
    vs_out.fragPos = vec3(model * vec4(position ,1.0));
    vs_out.normal = mat3(transpose(inverse(model))) * normal;
}
