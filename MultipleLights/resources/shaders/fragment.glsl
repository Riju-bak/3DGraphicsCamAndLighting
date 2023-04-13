#version 330 core

#define NR_POINT_LIGHTS 4 

struct Material
{
	vec3 ambient;
	sampler2D diffuse;	//Basically the texture unit
	sampler2D specular; //affects which part of the material will shine due to specular lighting.(e.g metal bar but not wood)
	sampler2D emission;
	float shininess;	//controls how big the specular region will be
};

struct SpotLight
{
	vec3 position; // not needed for directional light
	vec3 direction; //for directional light (e.g. sunlight) and spotDir for spot light 
	vec3 ambient;	
	vec3 diffuse;	//diffuse color
	vec3 specular;	//color of specular bright spot

	//Attenuation terms
	float constant;
	float linear;
	float quadratic;

	float cutoff; //The cutoff angle for a light
	float outerCutoff; //The outercutoff for realistic light that fades at the edges.
};

struct DirLight
{
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};


struct PointLight
{
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	//Attenutation terms
	float constant;
	float linear;
	float quadratic;
};

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;
in vec3 ViewPos;

uniform Material material;
uniform SpotLight spotLight;
uniform DirLight dirLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];

out vec4 FragColor;

//TODO: Refactor
vec3 CalcSpotLight(SpotLight light)
{
	vec3 LightPos = light.position;
	float theta = dot(normalize(LightPos - FragPos),normalize(-light.direction));
	float spotlightIntensity = clamp((theta - light.outerCutoff)/(light.cutoff - light.outerCutoff), 0.0, 1.0); 
	vec3 result;

	//NOTE: theta is cosine, therefore the following equality sign
	if(theta > light.outerCutoff)
	{
		//Ambient lighting
		vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoord));

		//Diffuse lighting
		vec3 normal = normalize(Normal);
		vec3 lightDir = normalize(LightPos - FragPos);
		float diff = max(dot(normal, lightDir), 0.0);
		vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoord));

		//Blinn Specular lighting
		vec3 viewDir = normalize(ViewPos - FragPos); //  In world space, viewDir = normalize(ViewPos - FragPos); In view-space ViewPos(i.e. CameraPos) will be vec3(0.0)
		vec3 blinnMidDir = normalize(lightDir + viewDir);
		float spec = pow(max(dot(normal, blinnMidDir),0.0), material.shininess);
		vec3 specular = light.specular * texture(material.specular, TexCoord).rgb * spec;
		// vec3 specular = light.specular * texture(material.diffuse, TexCoord).rgb * spec; //del later

		//Emission with metal edges masked
		vec3 emissionMask = floor(vec3(1.0) - texture(material.specular, TexCoord).rgb);
		vec3 emission = texture(material.emission, TexCoord).rgb * emissionMask;

		//Attenuation
		float dist = length(LightPos - FragPos);
		float attenuation = 1.0/(light.constant + light.linear * dist + light.quadratic * dist * dist);
		// ambient *= attenuation;	//We could get rid of this one
		diffuse *= attenuation * spotlightIntensity;
		specular *= attenuation * spotlightIntensity;

		// vec3 result = (ambient + diffuse + specular + emission);
		result = (diffuse + specular); // del later
	}
	else
	{
		//If the fragment is outside the range of flashlight lighting area, then only handle Ambient lighting
		result = light.ambient * texture(material.diffuse, TexCoord).rgb;
	}
	return result;
}

vec3 CalcDirLight(DirLight light)
{
	//Ambient lighting
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoord));
	
	//Diffuse lighting
	vec3 normal = normalize(Normal);
	vec3 lightDir = normalize(-light.direction);
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoord));
	
	//Blinn Specular lighting
	vec3 viewDir = normalize(ViewPos - FragPos); //  In world space, viewDir = normalize(ViewPos - FragPos); In view-space ViewPos(i.e. CameraPos) will be vec3(0.0)
	vec3 blinnMidDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(normal, blinnMidDir),0.0), material.shininess);
	vec3 specular = light.specular * texture(material.specular, TexCoord).rgb * spec;
	// vec3 specular = light.specular * texture(material.diffuse, TexCoord).rgb * spec; //del later
	
	//Emission with metal edges masked
	vec3 emissionMask = floor(vec3(1.0) - texture(material.specular, TexCoord).rgb);
	vec3 emission = texture(material.emission, TexCoord).rgb * emissionMask;
	
	vec3 result = (ambient + diffuse + specular); // del later
	return result;
}

vec3 CalcPointLight(PointLight light)
{
		vec3 LightPos = light.position;
		
		//Ambient lighting
		vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoord));

		//Diffuse lighting
		vec3 normal = normalize(Normal);
		vec3 lightDir = normalize(LightPos - FragPos);
		float diff = max(dot(normal, lightDir), 0.0);
		vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoord));

		//Blinn Specular lighting
		vec3 viewDir = normalize(ViewPos - FragPos); //  In world space, viewDir = normalize(ViewPos - FragPos); In view-space ViewPos(i.e. CameraPos) will be vec3(0.0)
		vec3 blinnMidDir = normalize(lightDir + viewDir);
		float spec = pow(max(dot(normal, blinnMidDir),0.0), material.shininess);
		vec3 specular = light.specular * texture(material.specular, TexCoord).rgb * spec;
		// vec3 specular = light.specular * texture(material.diffuse, TexCoord).rgb * spec; //del later

		//Emission with metal edges masked
		vec3 emissionMask = floor(vec3(1.0) - texture(material.specular, TexCoord).rgb);
		vec3 emission = texture(material.emission, TexCoord).rgb * emissionMask;

		//Attenuation
		float dist = length(LightPos - FragPos);
		float attenuation = 1.0/(light.constant + light.linear * dist + light.quadratic * dist * dist);
		// ambient *= attenuation;	//We could get rid of this one
		diffuse *= attenuation;
		specular *= attenuation;

		// vec3 result = (ambient + diffuse + specular + emission);
		vec3 result = (diffuse + specular); // del later
		return result;
}

void main()
{
	vec3 result = CalcSpotLight(spotLight);
	// vec3 result;
	result += CalcDirLight(dirLight);
	for(int i=0; i<NR_POINT_LIGHTS; i++)
	{
		result += CalcPointLight(pointLights[i]);
	}
	FragColor = vec4(result, 1.0);
}