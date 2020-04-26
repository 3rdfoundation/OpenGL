# -----------------------------------------------------------------------------
# SHADER VERTEX
# -----------------------------------------------------------------------------
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec4 vertexColor;

out vec4 v_VertexColor;

uniform mat4 u_MVP;

void main() {
	gl_Position = u_MVP * position;
	v_VertexColor = vertexColor;
};

# -----------------------------------------------------------------------------
# SHADER FRAGMENT
# -----------------------------------------------------------------------------
#version 330 core

layout(location = 0) out vec4 color;

in vec4 v_VertexColor;

void main() {
	color = v_VertexColor;
};