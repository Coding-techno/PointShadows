#version 400
uniform sampler2D inputTexture;

in VS_OUT{
    vec2 textureCoords;
} fs_in;

out vec4 fragColor;
void main() {
    fragColor = texture(inputTexture ,fs_in.textureCoords);

    float gamma = 2.2;
    fragColor.rgb = pow(fragColor.rgb ,vec3(1.0/gamma));
}
