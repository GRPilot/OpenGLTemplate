#version 330 core

in vec4 FragColor;
in vec2 TexCoord;

uniform float mix_value;

uniform int tex_id = 0;

uniform sampler2D sample_0;
uniform sampler2D sample_1;

out vec4 ResultColor;

void main() {
    vec4 activeTexture;
    switch(texId) {
        case 0:
            activeTexture = texture(sample_0, TexCoord);
            break;
        case 1:
            activeTexture = texture(sample_1, TexCoord);
            break;
    }
    ResultColor = mix(FragColor, activeTexture, mix_value);
}
