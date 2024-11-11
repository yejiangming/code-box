#version 330 core
in vec3 ourColor;
in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D texture1;
uniform sampler2D texture2;

uniform float mixValue;

void main() {

    vec2 texCoord = vec2(1-TexCoord.s, TexCoord.t);
    FragColor = mix(texture(texture1, texCoord),texture(texture2, texCoord), mixValue);
}