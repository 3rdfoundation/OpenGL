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
uniform sampler2D u_Texture_Channel_0;

// Channel 1
uniform sampler2D u_Texture_Channel_1;

void main() {
	float v_channel = 1;
	if (v_channel == 0) {
		vec4 texColor = texture(u_Texture_Channel_0, v_TexCoord);
		color = texColor;
	}
	else {
		vec4 texColor = texture(u_Texture_Channel_1, v_TexCoord);
		color = texColor;
	}
};