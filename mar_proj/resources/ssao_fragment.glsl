/*uniform sampler2D depthMap;
uniform sampler2D normalMap;

void main()
{
  ============================================= SCREENSHOT 1 =============================================
  float occlusion = texture2D(depthMap, gl_TexCoord[0].st).r;
  
  for ( float x=-64.; x<=64.; x+=8. )
  {
    for ( float y=-64.; y<=64.; y+=8. )
    {
      occlusion += texture2D(depthMap, gl_TexCoord[0].st + vec2( x*(gl_TexCoord[0].s/1024.), y*(gl_TexCoord[0].t/768.) )).r;
    }
  }
  
  occlusion /= 256.;
  occlusion /= texture2D(depthMap, gl_TexCoord[0].st).r;
  
  gl_FragColor.rgb  = vec3(occlusion);
  gl_FragColor.a    = 1.;*/
  
  /*============================================= SCREENSHOT 2 =============================================
  float currentOcclusion = texture2D(depthMap, gl_TexCoord[0].st).r;
  float occlusion        = currentOcclusion;
  float cpt              = 1.0;
  
  for ( float x=-64.; x<=64.; x+=8. )
  {
    for ( float y=-64.; y<=64.; y+=8. )
    {
      float localOcclusion   = texture2D(depthMap, gl_TexCoord[0].st + vec2( x*(gl_TexCoord[0].s/1024.), y*(gl_TexCoord[0].t/768.) )).r;
      
      if (     x!=0. && y!=0.
            && localOcclusion<=currentOcclusion
            && (1.-localOcclusion/currentOcclusion<0.2)
         )
      {
        cpt++;
        occlusion += localOcclusion;
      }
    }
  }
  
  occlusion /= cpt;
  
  occlusion /= currentOcclusion;
  
  occlusion = 1.- ((1.-occlusion) / 0.2);
  
  gl_FragColor.rgb  = vec3(occlusion);
  gl_FragColor.a    = 1.;
}*/



uniform sampler2D depthMap;

uniform float width;
uniform float height;

uniform float radius;
uniform float precision;
uniform float nbRays;
uniform float power;

const float znear =  8.0;
const float zfar  = 60.0;

const float pi = 3.1415;

float counter = 0.;
int error = 0;

float linearDepth( vec2 uv )
{
  float z = texture2D(depthMap, uv).x;
  return z;
  /*return (2.0 * znear) / (znear + zfar - z * (zfar - znear));*/
}

vec3 viewspaceNormal( vec2 uv )
{
  float c   = linearDepth(uv);
  float xm1 = linearDepth(uv-dFdx(uv));
  float xp1 = linearDepth(uv+dFdx(uv));
  float ym1 = linearDepth(uv-dFdy(uv));
  float yp1 = linearDepth(uv+dFdy(uv));

  vec3 v1 = vec3( 1., 0., (xm1-xp1)*400. );
  vec3 v2 = vec3( 0., 1., (ym1-yp1)*400. );

  return normalize(cross(v1,v2));
}

vec3 coords( vec2 uv )
{
  return vec3( uv.s, uv.t, 1.-linearDepth(uv) );
}

float horizonAt( vec2 uv, float angle )
{
  vec2 i        = uv;
  float horizon = 0.0;
  counter = 0.;
  while ( distance(i,uv) <= radius )
  { 
    float c = dot( viewspaceNormal(uv), normalize(coords(i)-coords(uv)) );
    if ( c>0. && c>horizon && (distance(coords(uv),coords(i))<=radius) )
    {
      horizon = c;
    }
    /* Sanity check in case of 0-displacement */
    vec2 displacement = precision*(cos(angle)*dFdx(i) + sin(angle)*dFdy(i));
    if ( distance(i,displacement)==0. || displacement==vec2(0.,0.) )
    {
      error = 1;
      displacement = dFdx(i)+dFdy(i);
    }
    i += displacement;
    counter++;
    if ( counter>10. )
    {
      error = 1;
      break;
    }
  }
  
  return horizon;
}

float accessibility( vec2 uv )
{
  float totalHorizon = 0.0;
  float angle = 0.0;
  
  for ( angle=0.; angle<2.0*pi; angle+=(2.0*pi)/nbRays )
  {
    totalHorizon += horizonAt( uv, angle );
  }
  
  return 1. - totalHorizon/nbRays;
}

void main()
{
  if ( error==1 )
  {
    gl_FragColor.rgb = vec3(1.,0.,0.);
  }
  else
  {
    float access = accessibility(gl_TexCoord[0].st);
    gl_FragColor.rgb = vec3( pow(access,power) );
  }
}