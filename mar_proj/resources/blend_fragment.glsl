uniform sampler2D ppp_map;
uniform sampler2D ssao_map;
uniform sampler2D bloom_map;
uniform int apply_ambient;
uniform int apply_blur;

vec4 blur( vec2 uv )
{
  /*
  float kernel[5*5];
  
  kernel[0]  =  1./273.;
  kernel[1]  =  4./273.;
  kernel[2]  =  7./273.;
  kernel[3]  =  4./273.;
  kernel[4]  =  1./273.;
  
  kernel[5]  =  4./273.;
  kernel[6]  = 16./273.;
  kernel[7]  = 26./273.;
  kernel[8]  = 16./273.;
  kernel[9]  =  4./273.;
  
  kernel[10] =  7./273.;
  kernel[11] = 26./273.;
  kernel[12] = 41./273.;
  kernel[13] = 26./273.;
  kernel[14] =  7./273.;
  
  kernel[15] =  4./273.;
  kernel[16] = 16./273.;
  kernel[17] = 26./273.;
  kernel[18] = 16./273.;
  kernel[19] =  4./273.;
  
  kernel[20] =  1./273.;
  kernel[21] =  4./273.;
  kernel[22] =  7./273.;
  kernel[23] =  4./273.;
  kernel[24] =  1./273.;
  */
  vec4 t;
  
  for ( float x=-6.; x<6.; ++x )
    for ( float y=-6.; y<6.; ++y )
      t += texture2D(bloom_map, gl_TexCoord[0].st + (x*dFdx(gl_TexCoord[0].st)+y*dFdy(gl_TexCoord[0].st)) );
  
  return t/(6.*6.)/2.;
}

void main()
{
  vec4 occlusionValue;
  vec4 phongValue;
  vec4 specularValue;
  
  occlusionValue = vec4(texture2D(ssao_map, gl_TexCoord[0].st));
  phongValue     = texture2D(ppp_map,gl_TexCoord[0].st);
  specularValue  = blur(gl_TexCoord[0].st);
  
  gl_FragColor = occlusionValue * (phongValue+specularValue);
}
