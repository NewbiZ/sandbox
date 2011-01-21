#include "mar_proj/scene.h"

#include <GL/glew.h>
#include <iostream>
#include "mar_proj/utils.font.h"

namespace mar
{
  Scene::Scene( float& radius, float& precision, int& nbrays, int& power, int& blend, int& blur )
    :  width_         ( 800       ),
       height_        ( 600       ),
       pass2Radius_   ( radius    ),
       pass2Precision_( precision ),
       pass2Nbrays_   ( nbrays    ),
       pass2Power_    ( power     ),
       pass3Blend_    ( blend     ),
       pass3Blur_     ( blur      ),
       framebuffer1_  ( 2         ) // 2 color buffers
  {
  }
  
  Scene::~Scene()
  {
  }
  
  void Scene::setup( unsigned int width, unsigned int height )
  {
    width_  = width;
    height_ = height;
    
    setup_opengl();
    setup_models();
    setup_view();
    
    shaderPPP_.load( "resources/ppp_vertex.glsl", "resources/ppp_fragment.glsl" );
    shaderPPP_.disable();
    
    shaderSSAO_.load( "resources/ssao_vertex.glsl", "resources/ssao_fragment.glsl" );
    shaderSSAO_.disable();
    
    shaderBlend_.load( "resources/blend_vertex.glsl", "resources/blend_fragment.glsl" );
    shaderBlend_.disable();
    
    framebuffer1_.setup(width_, height_);
    framebuffer2_.setup(width_, height_);
    framebuffer3_.setup(width_, height_);
  }
  
  void Scene::setup_opengl()
  {
    // Viewport & projections
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluPerspective( 60, width_/height_, 1, 10. );
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    
    // Misc
    glEnable( GL_DEPTH );
    glEnable( GL_DEPTH_TEST );
    
    // Lighting
    glShadeModel( GL_SMOOTH );
    glEnable( GL_LIGHTING );
    glEnable( GL_LIGHT0 );
  }
  
  void Scene::setup_models()
  {
    model_.load( "resources/dragon.ply" );
  }
  
  void Scene::setup_view()
  {
    rotation_ = 0;
    distance_ = 10.;
  }
  
  void Scene::load_model( const std::string& filename )
  {
    model_.load( filename );
  }
  
  void Scene::turn_left()
  {
    rotation_ -= 2;
  }
  
  void Scene::turn_right()
  {
    rotation_ += 2;
  }
  
  void Scene::move_forward()
  {
    distance_ -= 0.5;
  }
  
  void Scene::move_backward()
  {
    distance_ += 0.5;
  }
  
  void Scene::render() const
  {
    static float light_position[] = { 20.0, 100.0, 20.0, 1.0 };

    static float light_ambient [] = { 1.0, 1.0, 1.0, 1.0 };
    static float light_diffuse [] = { 1.0, 1.0, 1.0, 1.0 };
    static float light_specular[] = { 1.0, 1.0, 1.0, 1.0 };

    static float material_bronze_ambient [] = { 0.212500, 0.127500, 0.054000, 1.0 };
    static float material_bronze_diffuse [] = { 0.714000, 0.428400, 0.181440, 1.0 };
    static float material_bronze_specular[] = { 0.793548, 0.671906, 0.566721, 1.0 };
    static float material_bronze_shininess  = 10;

    static float material_chrome_ambient [] = { 0.250000, 0.250000, 0.250000, 1.0 };
    static float material_chrome_diffuse [] = { 0.400000, 0.400000, 0.400000, 1.0 };
    static float material_chrome_specular[] = { 0.774597, 0.774597, 0.774597, 1.0 };
    static float material_chrome_shininess  = 25;
    
    //========================================================================== PASS 1 : GET Z BUFFER TO TEXTURE
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluPerspective( 60, width_/height_, 5., 60. );
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    
    framebuffer1_.enable();
    
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glLoadIdentity();

    gluLookAt( distance_, distance_, distance_,
                0,  8,  0,
                0,  1,  0 );
    
    glLightfv( GL_LIGHT0, GL_POSITION, light_position );
    glLightfv( GL_LIGHT0, GL_AMBIENT,  light_ambient  );
    glLightfv( GL_LIGHT0, GL_DIFFUSE,  light_diffuse  );
    glLightfv( GL_LIGHT0, GL_SPECULAR, light_specular );

    glMaterialfv( GL_FRONT, GL_AMBIENT,   material_bronze_ambient   );
    glMaterialfv( GL_FRONT, GL_DIFFUSE,   material_bronze_diffuse   );
    glMaterialfv( GL_FRONT, GL_SPECULAR,  material_bronze_specular  );
    glMaterialf ( GL_FRONT, GL_SHININESS, material_bronze_shininess );

    glRotatef( 80+rotation_, 0, 1, 0 );

    shaderPPP_.enable();
    glPushMatrix();
    glScalef( 50, 50, 50 );
    model_.render();
    glPopMatrix();

    shaderPPP_.disable();

    framebuffer1_.disable();
    
    //========================================================================== PASS 2 : COMPUTE SCREEN SPACE OCCLUSION
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glOrtho( 0, width_, 0, height_, -5, 5);
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    framebuffer2_.enable();
    
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    
    glActiveTexture(GL_TEXTURE0);
    glEnable( GL_TEXTURE_2D );
    glBindTexture( GL_TEXTURE_2D, framebuffer1_.depth_buffer() );
    
    shaderSSAO_.enable();
    
    shaderSSAO_.setUniform1i( "depthMap",  0              );
    shaderSSAO_.setUniform1f( "width",     width_     );
    shaderSSAO_.setUniform1f( "height",    height_    );
    shaderSSAO_.setUniform1f( "radius",    pass2Radius_    );
    shaderSSAO_.setUniform1f( "precision", pass2Precision_ );
    shaderSSAO_.setUniform1f( "nbRays",    pass2Nbrays_    );
    shaderSSAO_.setUniform1f( "power",     pass2Power_     );
    
    glBegin( GL_QUADS );
      glTexCoord2f(0,0); glVertex3f(      0,       0, 0 );
      glTexCoord2f(1,0); glVertex3f( width_,       0, 0 );
      glTexCoord2f(1,1); glVertex3f( width_, height_, 0 );
      glTexCoord2f(0,1); glVertex3f(      0, height_, 0 );
    glEnd();
    
    shaderSSAO_.disable();
    
    framebuffer2_.disable();
    
    glActiveTexture(GL_TEXTURE0);
    
    //========================================================================== PASS 3 : BLUR OCCLUSION MAP
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glOrtho( 0, width_, 0, height_, -5, 5);
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    
    framebuffer3_.enable();
    
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    
    glActiveTexture(GL_TEXTURE0);
    glEnable( GL_TEXTURE_2D );
    glBindTexture( GL_TEXTURE_2D, framebuffer1_.color_buffer() );
    
    glActiveTexture(GL_TEXTURE1);
    glEnable( GL_TEXTURE_2D );
    glBindTexture( GL_TEXTURE_2D, framebuffer2_.color_buffer() );
    
    glActiveTexture(GL_TEXTURE2);
    glEnable( GL_TEXTURE_2D );
    glBindTexture( GL_TEXTURE_2D, framebuffer1_.color_buffer(1) );
    
    shaderBlend_.enable();
    
    shaderBlend_.setUniform1i( "apply_ambient",  pass3Blend_ );
    shaderBlend_.setUniform1i( "apply_blur",     pass3Blur_  );
    shaderBlend_.setUniform1i( "ppp_map",        0           );
    shaderBlend_.setUniform1i( "ssao_map",       1           );
    shaderBlend_.setUniform1i( "bloom_map",      2           );
    
    glBegin( GL_QUADS );
      glTexCoord2f(0,0); glVertex3f(      0,       0, 0 );
      glTexCoord2f(1,0); glVertex3f( width_,       0, 0 );
      glTexCoord2f(1,1); glVertex3f( width_, height_, 0 );
      glTexCoord2f(0,1); glVertex3f(      0, height_, 0 );
    glEnd();
    
    shaderBlend_.disable();
    
    framebuffer3_.disable();
    
    glActiveTexture(GL_TEXTURE0);
    
    //========================================================================== DISPLAY
    glDisable( GL_LIGHTING );
    glDisable( GL_TEXTURE_2D );
    
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glOrtho( 0, width_, 0, height_, -1, 1);
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    
    glEnable( GL_TEXTURE_2D );
    
    glBindTexture( GL_TEXTURE_2D, framebuffer1_.color_buffer(0) );
    glBegin( GL_QUADS );
      glTexCoord2f(0,0); glVertex3f(        0, height_/2, 0 );
      glTexCoord2f(1,0); glVertex3f( width_/2, height_/2, 0 );
      glTexCoord2f(1,1); glVertex3f( width_/2,   height_, 0 );
      glTexCoord2f(0,1); glVertex3f(        0,   height_, 0 );
    glEnd();
    
    glBindTexture( GL_TEXTURE_2D, framebuffer1_.color_buffer(1) );
    glBegin( GL_QUADS );
      glTexCoord2f(0,0); glVertex3f( width_/2, height_/2, 0 );
      glTexCoord2f(1,0); glVertex3f(   width_, height_/2, 0 );
      glTexCoord2f(1,1); glVertex3f(   width_,   height_, 0 );
      glTexCoord2f(0,1); glVertex3f( width_/2,   height_, 0 );
    glEnd();
    
    glBindTexture( GL_TEXTURE_2D, framebuffer2_.color_buffer() );
    glBegin( GL_QUADS );
      glTexCoord2f(0,0); glVertex3f(        0,         0, 0 );
      glTexCoord2f(1,0); glVertex3f( width_/2,         0, 0 );
      glTexCoord2f(1,1); glVertex3f( width_/2, height_/2, 0 );
      glTexCoord2f(0,1); glVertex3f(        0, height_/2, 0 );
    glEnd();
    
    glBindTexture( GL_TEXTURE_2D, framebuffer3_.color_buffer() );
    glBegin( GL_QUADS );
      glTexCoord2f(0,0); glVertex3f( width_/2,         0, 0 );
      glTexCoord2f(1,0); glVertex3f(   width_,         0, 0 );
      glTexCoord2f(1,1); glVertex3f(   width_, height_/2, 0 );
      glTexCoord2f(0,1); glVertex3f( width_/2, height_/2, 0 );
    glEnd();
    
    glDisable( GL_TEXTURE_2D );
    
    glColor3f( 0, 0, 1 );
    render_text(          10, height_/2+10, "Pass 1: Depth buffer" );
    render_text( width_/2+10, height_/2+10, "Pass 1: Color buffer" );
    render_text(          10,           10, "Pass 2: Color buffer" );
    render_text( width_/2+10,           10, "Pass 3: Color buffer" );
    
    //==========================================================================
    glColor4f(1.0,1.0,1.0,1.0);
    
    rotation_ += 2;
  }
}