varying vec4 diffuse;
varying vec4 ambient;
varying vec3 normal;
varying vec3 lightDir;
varying vec3 halfVector;

void main()
{
  normal = normalize(gl_NormalMatrix * gl_Normal);
 
  lightDir = normalize(vec3(gl_LightSource[0].position));
 
  halfVector = normalize(gl_LightSource[0].halfVector.xyz);
 
  diffuse = gl_FrontMaterial.diffuse * gl_LightSource[0].diffuse;
  ambient = gl_FrontMaterial.ambient * gl_LightSource[0].ambient;
  ambient += gl_LightModel.ambient * gl_FrontMaterial.ambient;
  
  gl_Position = ftransform();
}