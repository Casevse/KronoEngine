#version 400

in vec3 GPosition;
in vec3 GNormal;
in vec2 GTexture;
in vec4 GShadowCoordinate;

layout(location = 0) out vec4 FragColor;

const int Levels = 2;
const float ScaleFactor = 1.0 / Levels;

uniform vec4 VertexColor;
uniform float HasTexture;
uniform sampler2D Texture;
uniform float HasShadowMap;
uniform sampler2DShadow ShadowMap;

const int MAX_POINT_LIGHTS = 4;

struct PointLight {
	vec3 Position;
	vec3 Color;
	float AmbientIntensity;
	float DiffuseIntensity;
	float Attenuation;
};

uniform int NumPointLights;
uniform PointLight PointLights[MAX_POINT_LIGHTS];
uniform vec4 SunPosition;
uniform float SunIntensity;
uniform vec3 SunColor;

vec3 SunLight() {
	vec3 S = normalize(vec3(SunPosition));
	vec3 Ambient = SunColor;
	vec3 Diffuse = SunColor * max(dot(S, GNormal), 0.0);

	if (HasShadowMap > 0.5) {
		float Shadow = 1.0;
		if (GShadowCoordinate.w > 0.0) {
			vec4 TmpShadow = GShadowCoordinate;
			TmpShadow.z -= 0.0003;
			
			float Sum = 0.0;
			
			// PCF, smooth shadow borders.
			Sum += textureProjOffset(ShadowMap, TmpShadow, ivec2(-1, -1));
			Sum += textureProjOffset(ShadowMap, TmpShadow, ivec2(-1,  1));
			Sum += textureProjOffset(ShadowMap, TmpShadow, ivec2( 1,  1));
			Sum += textureProjOffset(ShadowMap, TmpShadow, ivec2( 1, -1));

			Shadow = Sum * 0.25;
			
		}
		return SunIntensity * (Ambient + Diffuse * Shadow);
	} else {
		return SunIntensity * (Ambient + Diffuse);
	}
}

vec3 Point(int Index) {
	vec3 S = PointLights[Index].Position - GPosition;
	float Distance = length(S);
	S = normalize(S);
	
	vec3 Ambient = PointLights[Index].Color * PointLights[Index].AmbientIntensity;
	vec3 Diffuse = PointLights[Index].Color * max(dot(S, GNormal), 0.0) * PointLights[Index].DiffuseIntensity;		// SIN TOON
	
	float Attenuation = PointLights[Index].Attenuation * Distance;
	
	return (Ambient + Diffuse) / Attenuation;
}

void main() {
	vec4 Toon = vec4(SunLight(), 1.0);
	
	for (int i = 0; i < NumPointLights; i++) {
		Toon += vec4(Point(i), 0.0);
	}
	
	if (HasTexture > 0.5) {
		FragColor = texture(Texture, GTexture) * Toon * VertexColor;
	}
	else {
		FragColor = VertexColor * Toon;
	}
}