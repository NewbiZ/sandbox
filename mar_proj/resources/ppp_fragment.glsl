varying vec4 diffuse;
varying vec4 ambient;
varying vec3 normal;
varying vec3 lightDir;
varying vec3 halfVector;

void main()
{
  vec3 n;
  vec3 halfV;
  
  float NdotL;
  float NdotHV;

  vec4 color = ambient;

  n = normalize(normal);

  NdotL = max( dot(n,lightDir), 0.0 );
  
  if ( NdotL > 0.0 )
  {
    color += diffuse * NdotL;
    halfV  = normalize(halfVector);
    NdotHV = max(dot(n,halfV),0.0);
    /*color += gl_FrontMaterial.specular * gl_LightSource[0].specular * pow(NdotHV, gl_FrontMaterial.shininess);*/
  }
  
  /* Color buffer 1: the per pixel phong without specularity */
  gl_FragData[0] = color;
  
  /* Color buffer 2: the phong specularity only */
  gl_FragData[1] = gl_FrontMaterial.specular * gl_LightSource[0].specular * 3.*pow(NdotHV, gl_FrontMaterial.shininess*2.);
}