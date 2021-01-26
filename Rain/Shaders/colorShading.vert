#version 130

// Inputs

in vec2 vertexPosition;  // 2D vec.
in vec4 vertexColor;
//in vec2 vertexUV;

out vec2 fragmentPosition;
out vec4 fragmentColor;
//out vec2 fragmentUV;

uniform float time;

void main() {
	gl_Position.xy = vertexPosition;
	gl_Position.z = 0.0;
	gl_Position.w = 1.0;  // Default.
	
	fragmentPosition = vertexPosition;
	
	fragmentColor = vertexColor;
	
	//fragmentUV = vec2(vertexUV.x, 1.0 - vertexUV.y);
	
}