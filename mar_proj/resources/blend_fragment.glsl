uniform sampler2D ppp_map;
uniform sampler2D ssao_map;
uniform int apply_ambient;
uniform int apply_blur;

float blur( vec2 uv )
{
  /* Gaussian blur */
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
  
  float x = -2.0;
  float y = -2.0;
  float t =  0.0;
  
  for ( x=-2.0; x<=2.0; ++x )
    for ( y=-2.0; y<=2.0; ++y )
      t += texture2D(ssao_map, gl_TexCoord[0].st + (x*dFdx(gl_TexCoord[0].st)+y*dFdy(gl_TexCoord[0].st)) ).r;
  
  return t/25.;
}

void main()
{
  vec4 occlusionValue;
  vec4 lightingValue;
  
  if ( apply_blur==1 )
    occlusionValue = vec4(blur(gl_TexCoord[0].st));
  else
    occlusionValue = vec4(texture2D(ssao_map, gl_TexCoord[0].st));
  
  if ( apply_ambient==1 )
    lightingValue = texture2D(ppp_map,gl_TexCoord[0].st);
  else
    lightingValue = vec4(1.);
  
  gl_FragColor = occlusionValue * lightingValue;
}
