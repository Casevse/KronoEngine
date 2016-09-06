#version 400

in vec2 TexCoords;

uniform sampler2D Texture;
uniform sampler2D DepthTexture;
uniform int Width;
uniform int Height;

layout (location = 0) out vec4 FragColor;

vec3 OptimizedBlur(float LinearDepth) {
	float DX = (1.5 * LinearDepth) / Width;
	float DY = (1.5 * LinearDepth) / Height;
	
	vec4 Sum = vec4(0.0, 0.0, 0.0, 0.0);
	Sum += texture(Texture, TexCoords + vec2(-DX, -DY));
	Sum += texture(Texture, TexCoords + vec2(-DX, 0.0));
	Sum += texture(Texture, TexCoords + vec2(0.0, -DY));
	Sum += texture(Texture, TexCoords);
	
	return Sum.rgb / 4;
}

void main() {
	float Depth = texture(DepthTexture, TexCoords).x;
	float LinearDepth = 0.0005 / (1.0005 - Depth);
	
	FragColor = vec4(OptimizedBlur(LinearDepth), 1.0) - LinearDepth * 0.075;
}