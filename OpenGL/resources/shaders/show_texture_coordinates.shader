# -----------------------------------------------------------------------------
# SHADER VERTEX
# -----------------------------------------------------------------------------
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;

out vec2 v_TexCoord;

uniform mat4 u_MVP;

void main() {
	gl_Position = u_MVP * position;
	v_TexCoord = texCoord;
};

# -----------------------------------------------------------------------------
# SHADER FRAGMENT
# -----------------------------------------------------------------------------
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;

void main() {
	color = vec4(v_TexCoord, 0, 1);
};