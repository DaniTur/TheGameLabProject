#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec2 TexCoords;
in vec3 FragPos;

// texture samplers
//uniform sampler2D ourtexture;
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_normal1;

//struct Material {
//    sampler2D diffuse;
//    sampler2D specular;
//    sampler2D normal;
//    float shininess;
//};

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular; // specular color strenght, (1,1,1) is pure white light
};

uniform Light light;
uniform vec3 viewPos;

void main()
{
	// Base material color (Albedo)
    vec3 albedo = texture(texture_diffuse1, TexCoords).rgb;

	// Ambient lightning
    vec3 ambient = light.ambient * albedo;
	
	// Normalize for each fragment, due to vertex interpolation modifing the vector lenght
    vec3 normal = normalize(Normal);

	// Vector pointing to the light source position
    vec3 lightDir = normalize(light.position - FragPos);

	// Diffuse lightning
	// Amount of light that aligns with the normal vector, hence the light power focusing that normal
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = light.diffuse * (diff * albedo);

	// Specular lightning (Phong)
	vec3 viewDir = normalize(viewPos - FragPos); // Direction between the fragment and the camera
	vec3 reflectDir = reflect(-lightDir, normal);	// Light reflection on the fragment surface
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = light.specular * (spec * texture(texture_specular1, TexCoords).rgb);

	vec3 result = ambient + diffuse + specular;
	FragColor = vec4(result, 1.0);
}