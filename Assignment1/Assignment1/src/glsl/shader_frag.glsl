#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 vertexColor;
//for lights
in vec3 FragPos;  
in vec3 Normal;

in vec4 FragPosLightSpace;

uniform sampler2D diffuseTexture;
uniform sampler2D shadowMap;

uniform vec3 viewPos;

// texture sampler
uniform sampler2D texture1;
uniform vec3 lightPosition;  
uniform vec3 lightDirection;
uniform vec3 lightColour;
uniform float constant;
uniform float linear;
uniform float quadratic;
uniform float ambientStrength;
uniform bool isShadow;

float ShadowCalculation(vec4 fragPosLightSpace)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
    float shadow = currentDepth > closestDepth  ? 1.0 : 0.0;

    return shadow;
}  

void main()
{
    // ambient
    vec3 ambient = ambientStrength * lightColour;
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPosition - FragPos);
    float diff = max(dot(norm, lightDirection), 0.0);
    vec3 diffuse = diff * lightColour;

    //Specular
    float specularStrength = 3.0;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDirection, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColour;  

    vec4 textureColor = texture(texture1, TexCoord);
    
    vec3 projCoords = FragPosLightSpace.xyz / FragPosLightSpace.w;

    float shadow = ShadowCalculation(FragPosLightSpace);
    if(!isShadow){
        shadow = 0;
    }
    
    
    shadow = 0;
    vec3 result = (ambient + (1.0 - shadow) * (diffuse + specular)) ;
    FragColor = vec4(result, 1.0) * textureColor ;

	//FragColor = textureColor * vec4(vertexColor.r, vertexColor.b, vertexColor.g, 1.0f);
}