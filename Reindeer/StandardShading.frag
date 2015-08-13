#version 120

//reference http://en.wikibooks.org/wiki/GLSL_Programming/GLUT/Multiple_Lights
//          https://code.google.com/p/opengl-tutorial-org/source/browse/tutorial16_shadowmaps/ShadowMapping_SimpleVersion.fragmentshader

varying vec3 Position_worldspace;
varying vec3 Normal_cameraspace;
varying vec3 Normal_modelspace;
varying vec2 UV;
varying vec4 ShadowCoord;

uniform vec3 LightPosition_worldspace;
uniform mat4 M;
uniform mat4 V;
uniform mat4 V_inv;
uniform mat3 N;
uniform sampler2D texture;
uniform int useTexture;
uniform sampler2DShadow shadowMap;
uniform sampler2D shadowMap2;

struct Light
{
	vec3 position;
	vec4 diffuse;
	vec4 specular;
	float constantAttenuation;
	float linearAttenuation;
	float quadraticAttenuation;
};

const int lightCount = 1;
Light lights[lightCount];

Light light0 = Light(
	LightPosition_worldspace,
	vec4(1.0, 1.0, 1.0, 1.0),
	vec4(1.0, 1.0, 1.0, 1.0),
	0.0,
	0.125,
	0.0
);

void main() {
	lights[0] = light0;

	gl_MaterialParameters frontMaterial;
	frontMaterial.specular = vec4(1.0, 1.0, 1.0, 1.0);
	frontMaterial.shininess = 128;

	vec4 sceneAmbient = vec4(0.2, 0.2, 0.2, 1.0);

	if (useTexture == 1)
		frontMaterial.diffuse = frontMaterial.ambient = texture2D(texture, UV);
	else
		frontMaterial.diffuse = frontMaterial.ambient = vec4(1.0, 1.0, 1.0, 1.0);

	vec3 viewDirection = normalize(vec3(V_inv * vec4(0.0, 0.0, 0.0, 1.0)) - Position_worldspace);
	vec3 n = normalize(Normal_cameraspace);

	vec3 totalLighting = vec3(0);
	for (int i = 0; i < lightCount; i++) {
		vec3 lightDirection = lights[i].position - Position_worldspace;
		float dist = length( lightDirection );
		vec3 l = normalize(lightDirection);
		float cosTheta = clamp(dot(n, l), 0.1, 1.0);

		float attenuation = 1 / (lights[i].constantAttenuation +
								 lights[i].linearAttenuation * dist +
								 lights[i].quadraticAttenuation * dist * dist);

		vec3 diffuseReflection = attenuation *
								 vec3(lights[i].diffuse) *
								 vec3(frontMaterial.diffuse) *
								 cosTheta;

		vec3 halfV = normalize(Position_worldspace + lightDirection);
		vec3 specularReflection = attenuation *
								  vec3(lights[i].specular) *
								  vec3(frontMaterial.specular) *
								  pow(max(dot(n, halfV), 0.0), frontMaterial.shininess);

		float bias = 0.005;
		//float bias = 0.0005*tan(acos(cosTheta));
		//bias = clamp(bias, 0, 0.0005);
		//bias = clamp(bias, 0, 0.01);

		//float visibility = shadow2D(shadowMap, vec3(ShadowCoord.xy, (ShadowCoord.z - bias) / ShadowCoord.w)).r;
		//float visibility = shadow2D(shadowMap, vec3(ShadowCoord.xy, (ShadowCoord.z) / ShadowCoord.w)).r < ShadowCoord.z - bias ? 0.5 : 1.0;
		float visibility = (texture2D(shadowMap2, ShadowCoord.xy).z < ShadowCoord.z - bias) ? 0.2 : 1.0;
		//float visibility = 0.0;
		//visibility += shadow2D(shadowMap, vec3(ShadowCoord.xy, (ShadowCoord.z - bias) / ShadowCoord.w)).r;
		//visibility += shadow2D(shadowMap, vec3(ShadowCoord.xy, (ShadowCoord.z - bias) / ShadowCoord.w)).r;
		//visibility += shadow2D(shadowMap, vec3(ShadowCoord.xy, (ShadowCoord.z - bias) / ShadowCoord.w)).r;

		//visibility = 1.0;

		totalLighting += (visibility * (diffuseReflection + specularReflection));
	}

	vec4 ambient = sceneAmbient * frontMaterial.ambient;

	gl_FragColor = ambient + vec4(totalLighting, 1.0);
}
