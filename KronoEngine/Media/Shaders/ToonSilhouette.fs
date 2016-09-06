#version 400

layout(location = 0) out vec4 FragColor;

uniform float Alpha;

void main() {
	FragColor = vec4(0.0, 0.0, 0.0, Alpha);		// Emitimos una línea negra.
}