#version 400

struct Material{
    sampler2D diffuse1;
    sampler2D specular1;
};

struct PointLight{
    vec3 position ,ambient ,diffuse ,specular;
    float constant ,linear ,quadratic;
};

uniform Material material;
uniform PointLight pointLight;
uniform vec3 cameraPosition;

in VS_OUT{
    vec3 normal ,fragPos;
    vec2 textureCoords;
} fs_in;

out vec4 fragColor;

void main() {
    vec3 texel = texture(material.diffuse1 ,fs_in.textureCoords).rgb;
    vec3 specularTexel = texture(material.specular1 ,fs_in.textureCoords).rgb;
    vec3 normal = normalize(fs_in.normal);
    vec3 lightDirection = normalize(pointLight.position - fs_in.fragPos);
    vec3 cameraDirection = normalize(cameraPosition - fs_in.fragPos);
    vec3 halfwayVec = normalize(cameraDirection+lightDirection);
    float lightDistance = distance(pointLight.position ,fs_in.fragPos);
    float attenuation = 1.0/(pointLight.constant+pointLight.linear*lightDistance+pointLight.quadratic*(lightDistance*lightDistance));

    vec3 ambient = pointLight.ambient * texel*attenuation;

    float diff = max(dot(lightDirection ,normal) ,0.0);
    vec3 diffuse = diff * pointLight.diffuse * texel*attenuation;

    float spec = pow(max(dot(halfwayVec ,normal),0.0),32.0);
    diff<0.1?spec=0:0;
    vec3 specular = spec * pointLight.specular * specularTexel*attenuation;

    vec3 result = ambient+diffuse+specular;

    fragColor = vec4(result ,1.0);
}
