#version 420

// required by GLSL spec Sect 4.5.3 (though nvidia does not, amd does)
precision highp float;

///////////////////////////////////////////////////////////////////////////////
// Material
///////////////////////////////////////////////////////////////////////////////
uniform vec3 material_color;
uniform float material_reflectivity;
uniform float material_metalness;
uniform float material_fresnel;
uniform float material_shininess;
uniform float material_emission;

uniform int has_color_texture;
layout(binding = 0) uniform sampler2D colorMap;
uniform int has_emission_texture;
layout(binding = 5) uniform sampler2D emissiveMap;

///////////////////////////////////////////////////////////////////////////////
// Environment
///////////////////////////////////////////////////////////////////////////////
layout(binding = 6) uniform sampler2D environmentMap;
layout(binding = 7) uniform sampler2D irradianceMap;
layout(binding = 8) uniform sampler2D reflectionMap;
uniform float environment_multiplier;

///////////////////////////////////////////////////////////////////////////////
// Light source
///////////////////////////////////////////////////////////////////////////////
uniform vec3 point_light_color = vec3(1.0, 1.0, 1.0);
uniform float point_light_intensity_multiplier = 50.0;

///////////////////////////////////////////////////////////////////////////////
// Constants
///////////////////////////////////////////////////////////////////////////////
#define PI 3.14159265359

///////////////////////////////////////////////////////////////////////////////
// Input varyings from vertex shader
///////////////////////////////////////////////////////////////////////////////
in vec2 texCoord;
in vec3 viewSpaceNormal;
in vec3 viewSpacePosition;

///////////////////////////////////////////////////////////////////////////////
// Input uniform variables
///////////////////////////////////////////////////////////////////////////////
uniform mat4 viewInverse;
uniform vec3 viewSpaceLightPosition;

///////////////////////////////////////////////////////////////////////////////
// Output color
///////////////////////////////////////////////////////////////////////////////
layout(location = 0) out vec4 fragmentColor;

vec3 getLi() {
	return 
	point_light_intensity_multiplier * 
	point_light_color * 
	(
		1.0f/pow(distance(viewSpaceLightPosition,viewSpacePosition),2)
	);
}
vec3 getWi() {
	return normalize(viewSpaceLightPosition - viewSpacePosition);
}
vec3 getDiffuseTerm(vec3 normal, vec3 wi, vec3 Li) {
	return material_color * 
		(1.0f/PI) * 
		abs(dot(normal,wi))*Li;
}
vec3 getWh(vec3 wo,vec3 wi) {
	return  normalize(wi+wo);
}
float getFresnel(vec3 wi,vec3 wh){
	return material_fresnel+(1-material_fresnel)
		*pow(
			(1.0f-dot(wh,wi)),5
		);
}
float getMicrofacet(vec3 n, vec3 wh) {
	return ((material_shininess+2)/(2*PI))
		*pow(abs(dot(n,wh)),material_shininess);
}
float getShadowingMasking(vec3 n, vec3 wo, vec3 wi, vec3 wh) {
	return min(1, min(
		2*(dot(n,wh)*dot(n,wo)/dot(wo,wh)),
		2*(dot(n,wh)*dot(n,wi)/dot(wo,wh))
	));
}
float getbrdf(float fresnel, float microfacet_function, float masking_function, vec3 n, vec3 wo,vec3 wi) {
	return fresnel * microfacet_function * masking_function / (4*dot(n,wo)*dot(n,wi));
}
vec3 calculateDirectIllumiunation(vec3 wo, vec3 n, vec3 base_color)
{
	vec3 direct_illum = base_color;
	vec3 Li = getLi();
	vec3 wi = getWi();

	if(dot(n, wi) <= 0) {
		return vec3(0);
	}

	vec3 diffuse_term = getDiffuseTerm(n,wi,Li);

	vec3 wh = getWh(wo,wi);

	float fresnel = getFresnel(wi,wh);
	
	float microfacet_function = getMicrofacet(n,wh);
	float masking_function = getShadowingMasking(n,wo,wi,wh);

	float brdf = getbrdf(fresnel, microfacet_function,masking_function, n,wo,wi);

	vec3 dialetic_term = brdf * dot(n, wi)*Li+(1-fresnel)*diffuse_term;
	vec3 metal_term = brdf * material_color * dot(n, wi)*Li;
	vec3 microfacet_term = material_metalness*metal_term+(1-material_metalness)*dialetic_term;

	return material_reflectivity*microfacet_term+(1-material_reflectivity)*diffuse_term;
}

vec2 getSphericalCoords(vec3 dir){
	float theta = acos(max(-1.0f, min(1.0f, dir.y)));
	float phi = atan(dir.z, dir.x);
	if(phi < 0.0f)
	{
		phi = phi + 2.0f * PI;
	}

	// Use these to lookup the color in the environment map
	return vec2(phi / (2.0 * PI), theta / PI);
}
vec3 calculateIndirectIllumination(vec3 wo, vec3 n, vec3 base_color)
{
	vec3 indirect_illum = vec3(0.f);

	vec3 nws = mat3(viewInverse )*n;// nws.xyz
	// Calculate the world-space position of this fragment on the near plane
	vec3 dir = normalize(nws.xyz);

	// Use these to lookup the color in the environment map
	vec2 lookup = getSphericalCoords(dir);
	vec4 irraduance = environment_multiplier * texture(irradianceMap, lookup);

	
	vec3 diffuse_term = material_color * (1.0f/PI) * irraduance.xyz ;

	vec3 wit = mat3(viewInverse) * wo;
	vec3 wi = normalize(reflect(-wo,n));


	// Use these to lookup the color in the environment map
	vec2 lookup2 = getSphericalCoords( normalize(mat3(viewInverse) * wi));

	vec3 wh = normalize(wi+wo);
	float roughness = sqrt(sqrt(2.0f/(material_shininess+2)));

	vec3 Li = environment_multiplier * textureLod(reflectionMap, lookup2, roughness * 7.0f).xyz;
	float fresnel = material_fresnel+(1-material_fresnel)
		*pow(
			(1.0f-dot(wh,wi)),5
		);

	vec3 dialetic_term = fresnel*Li+(1-fresnel)*diffuse_term;
	vec3 metal_term = fresnel*material_color*Li;

	
	vec3 microfacet_term = material_metalness*metal_term+(1-material_metalness)*dialetic_term;

	return material_reflectivity*microfacet_term+(1-material_reflectivity)*diffuse_term;
}


void main()
{
	vec3 wo = -normalize(viewSpacePosition);
	vec3 n = normalize(viewSpaceNormal);

	vec3 base_color = material_color;
	if(has_color_texture == 1)
	{
		base_color *= texture(colorMap, texCoord).xyz;
	}

	// Direct illumination
	vec3 direct_illumination_term = calculateDirectIllumiunation(wo, n, base_color);

	// Indirect illumination
	vec3 indirect_illumination_term = calculateIndirectIllumination(wo, n, base_color);

	///////////////////////////////////////////////////////////////////////////
	// Add emissive term. If emissive texture exists, sample this term.
	///////////////////////////////////////////////////////////////////////////
	vec3 emission_term = material_emission * material_color;
	if(has_emission_texture == 1)
	{
		emission_term *= texture(emissiveMap, texCoord).xyz;
	}

	fragmentColor.xyz = direct_illumination_term + indirect_illumination_term + emission_term;
}
