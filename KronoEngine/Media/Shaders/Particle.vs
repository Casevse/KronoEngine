#version 400

layout (location = 0) in vec3 InitialVelocity;
layout (location = 1) in float StartTime;
layout (location = 2) in vec3 InitialPosition;

out float Transp;

uniform float Time;
uniform vec3 Acceleration;
uniform float ParticleLifetime;
uniform mat4 MVP;
uniform float Size;

void main() {
	vec3 Position = InitialPosition;
    Transp = 0.0;
	
	// Calculamos el número de la iteración. Esto permite sostener las particulas a lo largo del tiempo y que no solo se muevan en la primera iteración.
	float Iteration = floor((Time - StartTime) / ParticleLifetime);

	// Si el tiempo es mayor que el tiempo de nacimiento (teniendo en cuenta la iteración) dibujamos algo...
    if (Time > StartTime + Iteration * ParticleLifetime) {
        float t = Time - (StartTime + Iteration * ParticleLifetime);

		// Si el tiempo (de esta iteracion) es menor que la vida de la partícula, calculamos su posición.
        if (t < ParticleLifetime) {
            Position += InitialVelocity * t + Acceleration * t * t;
            Transp = 1.0 - t / ParticleLifetime;	// Desvanecemos la partícula conforme se acerca a su fín.
        }
    }
	
    gl_Position = MVP * vec4(Position, 1.0);
	
	// Esto permite reducir el tamaño de la particula según la distancia a la que se encuentre.
	vec3 Ndc = gl_Position.xyz / gl_Position.w;
	float PointScale = 1.0 - Ndc.z;
	
	gl_PointSize = Size * Transp * PointScale;
}