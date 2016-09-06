#version 400

in float Transp;
uniform sampler2D ParticleTexture;
uniform vec4 Color;

layout ( location = 0 ) out vec4 FragColor;

void main() {
	vec4 ColorAux = texture(ParticleTexture, gl_PointCoord) * Color;
	
	if (ColorAux.a < 0.5) {
		discard;
	}
	
	FragColor = ColorAux;
}