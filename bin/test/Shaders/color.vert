#version 130

in vec2 vertexPosition;
in vec4 vertexColor;
in vec2 vertexUV;

uniform mat4 projectionMatrix;

out vec4 fragmentColor;
out vec2 fragmentUV;

void main()
{
	//Set position on the screen
	gl_Position.xy = (projectionMatrix * vec4(vertexPosition, 0.0, 1.0)).xy;	
	gl_Position.z = 0.0; // the z position is zero since we are in 2D
	gl_Position.w = 1.0; // Indicate that the coordinates are normalized

	// out
	fragmentColor = vertexColor;
	fragmentUV = vec2(vertexUV.x, 1.0 - vertexUV.y);
}