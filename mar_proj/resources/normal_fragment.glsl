varying vec3 normal;

void main()
{
  gl_FragColor.rgb = normalize((normal+vec3(1.,1.,1.))/2.); /* Packing normals in the 0;1 range */
  gl_FragColor.a   = 1.;
}