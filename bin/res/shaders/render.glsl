#shader vertex
#version 450

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 normal;
layout(location = 2) uniform mat4 projection;
out float c;
out vec3 vs_Position;
out vec3 vs_Normal;
out float time;
layout(location = 3) uniform float t;
layout(location = 4) uniform float maxH;
layout(location = 5) uniform float minH;

void main()
{
    vec4 newPosition = vec4(position.xyz, 1.0);
    gl_Position = projection * newPosition;
    c = clamp( (newPosition.z-minH)/(maxH-minH) , 0.0, 1.0);
    // TODO: separate modelview and projection matrices
    vs_Position = newPosition.xyz;
    vs_Normal = normal;
    time = t;
}

#shader fragment
#version 450
// Modified blinn phone shader from Wikipedia
in float c;
in float time;
in vec3 vs_Position;
in vec3 vs_Normal;
out vec4 color;

uniform int mode;

const vec3 blue = vec3(0, 0.180, 0.341);
const vec3 tanC = vec3(0.803, 0.450, 0.196);
const vec3 green = vec3(0.190, 0.472, 0.064);
const vec3 brown = vec3(0.301, 0.129, 0.015);


const vec3 lightPos = vec3(10.0*cos(time),10.0*sin(time),10.0);

const float lightPower = 150.0;

vec3 diffuseColor = vec3(0.5, 0.5, 0.5);
const vec3 specColor = vec3(1.0, 1.0, 1.0);
const float shininess = 2.0;
const float screenGamma = 2.2;

void main()
{
  vec3 ambientColor = vec3(0.0, 0.0, 0.0);
  if(c >= 0.0 && c < 1.0/3.0)
  {
    ambientColor = (-3.0*c + 1.0)*blue + (3.0*c)*tanC; 
  }
  else if(c >= 1.0/3.0 && c < 2.0/3.0)
  {
    ambientColor = (-3.0*c + 2.0)*tanC + (3.0*c - 1.0)*green; 
  }
  else if(c >= 2.0/3.0 && c < 1.0)
  {
    ambientColor = (-3.0*c + 3.0)*green + (3.0*c - 2.0)*brown; 
  }
  vec3 lightColor = ambientColor;
  diffuseColor = ambientColor*0.5;
  ambientColor = ambientColor*0.1;
  vec3 normal = normalize(vs_Normal);
  vec3 lightDir = lightPos - vs_Position;
  float distance = length(lightDir);
  distance = distance * distance;
  lightDir = normalize(lightDir);

  float lambertian = max(dot(lightDir,normal), 0.0);
  float specular = 0.0;

  if(lambertian > 0.0) {

    vec3 viewDir = normalize(-vs_Position);

    // blinn phong
    vec3 halfDir = normalize(lightDir + viewDir);
    float specAngle = max(dot(halfDir, normal), 0.0);
    specular = pow(specAngle, shininess);
  }
  vec3 colorLinear = ambientColor +
                     diffuseColor * lambertian * lightColor * lightPower / distance +
                     specColor * specular * lightColor * lightPower / distance;

  vec3 colorGammaCorrected = pow(colorLinear, vec3(1.0/screenGamma));
  color = vec4(colorLinear, 1.0);

  color = vec4(colorGammaCorrected, 1.0);
}