#version 130

in vec4 fragmentColor;
in vec2 fragmentUV;

out vec4 color;

uniform sampler2D samplerUniform;

void main()
{    
	vec4 textureColor = texture(samplerUniform, fragmentUV);    
	color = fragmentColor * textureColor;
}