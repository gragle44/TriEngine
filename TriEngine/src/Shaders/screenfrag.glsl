#version 450 core
out vec4 FragColor;
  
in vec2 v_TexCoords;

uniform sampler2D u_ScreenTexture;

void main()
{ 
    FragColor = texture(u_ScreenTexture, v_TexCoords);
}