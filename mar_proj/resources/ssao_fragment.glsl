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
uniform sampler2D normalMap;

const float width  = 1024.;
const float height =  768.;

const float znear =  8.;
const float zfar  = 60.;

const float radius    = 0.01;
const float precision = 1.;

float linearDepth( vec2 uv )
{
  /* TODO: linearize z */
  float z = texture2D(depthMap, uv).x;
  return z;
}

vec3 viewspaceNormal2( vec2 uv )
{
  return (texture2D(normalMap, uv).rgb)*2.-vec3(1.,1.,1.); /* Unpack normals */
}

vec3 viewspaceNormal( vec2 uv )
{
  /* TODO: use sobel filter here */
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
  return vec3( uv.s, uv.t, linearDepth(uv) );
}

bool in_hemisphere( vec3 center, float radius, vec3 point )
{
  return distance(center,point)<=radius; /* Is in the surround sphere */
         /* dot(point,viewspaceNormal(vec2(center.s,center.t)))<0.; /* Is in the normal-oriented hemisphere */
}

float accessibility( vec2 uv )
{
  vec2 current = uv;
  vec2 i;
  float horizon;
  float totalHorizon = 0.;
  
  /* Marching in +x */
  i = current+dFdx(i);
  horizon = 0.;
  while ( distance(current,i)<=radius )
  {
    float c = dot(viewspaceNormal(current), normalize(coords(i)-coords(current)) );
    if ( c>0. && c>horizon && (distance(coords(current),coords(i))<=radius) )
      horizon = c;
    i += precision*dFdx(i);
  }
  totalHorizon += horizon;

  return 1.-totalHorizon;
}

void main()
{
  float acc = accessibility(gl_TexCoord[0].st );
  gl_FragColor = vec4( acc, acc, acc, 1. );
}