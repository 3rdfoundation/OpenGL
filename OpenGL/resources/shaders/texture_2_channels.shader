# -----------------------------------------------------------------------------
# SHADER VERTEX
# -----------------------------------------------------------------------------
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in float channel;

out vec2 v_TexCoord;
out float v_Channel;

uniform mat4 u_MVP;

void main() {
	gl_Position = u_MVP * position;
	v_TexCoord = texCoord;
	v_Channel = channel;
};

# -----------------------------------------------------------------------------
# SHADER FRAGMENT
# -----------------------------------------------------------------------------
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;
in float v_Channel;

// Channel 0
uniform sampler2D u_Texture[2];

void main() {
	int channel = int(v_Channel);
	vec4 texColor = texture(u_Texture[channel], v_TexCoord); // vec4(v_Channel, v_Channel, v_Channel, 0);
	color = texColor;
};