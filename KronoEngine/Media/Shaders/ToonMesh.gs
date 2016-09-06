#version 400

layout(triangles_adjacency) in;
layout(triangle_strip, max_vertices = 15) out;

out vec3 GNormal;
out vec3 GPosition;
out vec2 GTexture;
out vec4 GShadowCoordinate;

in vec3 VNormal[];
in vec3 VPosition[];
in vec2 VTexture[];
in vec4 VShadowCoordinate[];

void main() {
	GNormal = VNormal[0];
	GPosition = VPosition[0];
	GTexture = VTexture[0];
	GShadowCoordinate = VShadowCoordinate[0];
	gl_Position = gl_in[0].gl_Position;
	EmitVertex();
	
	GNormal = VNormal[2];
	GPosition = VPosition[2];
	GTexture = VTexture[2];
	GShadowCoordinate = VShadowCoordinate[2];
	gl_Position = gl_in[2].gl_Position;
	EmitVertex();
	
	GNormal = VNormal[4];
	GPosition = VPosition[4];
	GTexture = VTexture[4];
	GShadowCoordinate = VShadowCoordinate[4];
	gl_Position = gl_in[4].gl_Position;
	EmitVertex();
	
	EndPrimitive();
}