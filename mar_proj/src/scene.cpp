#include "mar_proj/scene.h"

#include <GL/glut.h>
#include <iostream>

namespace mar
{
  Scene::Scene()
  {
  }
  
  Scene::~Scene()
  {
  }
  
  void Scene::setup()
  {
    setup_opengl();
    setup_models();
    setup_view();
    
    shaderNormal_.load( "resources/normal_vertex.glsl", "resources/normal_fragment.glsl" );
    shaderNormal_.disable();
    
    shaderSSAO_.load( "resources/ssao_vertex.glsl", "resources/ssao_fragment.glsl" );
    shaderSSAO_.disable();
    
    framebuffer1_.setup();
    framebuffer2_.setup();
  }
  
  void Scene::setup_opengl()
  {
    // Viewport & projections
    glViewport( 0, 0, 1024, 768 );
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluPerspective( 60, 1024./768., 1, 10. );
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
    static float light_position[] = { 50.0, 50.0, 50.0, 1.0 };

    static float light_ambient [] = { 1.0, 1.0, 1.0, 1.0 };
    static float light_diffuse [] = { 1.0, 1.0, 1.0, 1.0 };
    static float light_specular[] = { 1.0, 1.0, 1.0, 1.0 };

    static float material_bronze_ambient [] = { 0.212500, 0.127500, 0.054000, 1.0 };
    static float material_bronze_diffuse [] = { 0.714000, 0.428400, 0.181440, 1.0 };
    static float material_bronze_specular[] = { 0.393548, 0.271906, 0.166721, 1.0 };
    static float material_bronze_shininess  = 25;

    static float material_chrome_ambient [] = { 0.250000, 0.250000, 0.250000, 1.0 };
    static float material_chrome_diffuse [] = { 0.400000, 0.400000, 0.400000, 1.0 };
    static float material_chrome_specular[] = { 0.774597, 0.774597, 0.774597, 1.0 };
    static float material_chrome_shininess  = 25;
    
    //========================================================================== PASS 1 : GET Z BUFFER TO TEXTURE
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluPerspective( 60, 1024./768., 8., 60. );
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    
    framebuffer1_.enable();
    
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glLoadIdentity();

    gluLookAt( distance_, distance_, distance_,
                0,  5,  0,
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
    
    shaderNormal_.enable();
      glPushMatrix();
      glScalef( 50, 50, 50 );
      model_.render();
      glPopMatrix();
      
      glBegin( GL_QUADS );
        glNormal3f  ( 0, 1 ,0 );
        glVertex3f( -10, 2.7, -10 );
        glVertex3f(  10, 2.7, -10 );
        glVertex3f(  10, 2.7,  10 );
        glVertex3f( -10, 2.7,  10 );
      glEnd();
    shaderNormal_.disable();
    
    framebuffer1_.disable();
    
    //========================================================================== PASS 2 : COMPUTE SCREEN SPACE OCCLUSION
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glOrtho( 0, 1024, 0, 768, -5, 5);
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    
    framebuffer2_.enable();
    
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    
    glActiveTexture(GL_TEXTURE0);
    glEnable( GL_TEXTURE_2D );
    glBindTexture( GL_TEXTURE_2D, framebuffer1_.depth_buffer() );
    
    glActiveTexture(GL_TEXTURE1);
    glEnable( GL_TEXTURE_2D );
    glBindTexture( GL_TEXTURE_2D, framebuffer1_.color_buffer() );
    
    shaderSSAO_.enable();
    GLuint depthMap = shaderSSAO_.uniformLocation( "depthMap" );
    glUniform1i( depthMap, 0 );
    GLuint normalMap = shaderSSAO_.uniformLocation( "normalMap" );
    glUniform1i( normalMap, 1 );
    
    glBegin( GL_QUADS );
      glTexCoord2f(0,0); glVertex3f(    0,   0, 0 );
      glTexCoord2f(1,0); glVertex3f( 1024,   0, 0 );
      glTexCoord2f(1,1); glVertex3f( 1024, 768, 0 );
      glTexCoord2f(0,1); glVertex3f(    0, 768, 0 );
    glEnd();
    
    shaderSSAO_.disable();
    
    framebuffer2_.disable();
    
    glActiveTexture(GL_TEXTURE0);
    
    //========================================================================== DIPLAY
    glDisable( GL_LIGHTING );
    
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glOrtho( 0, 1024, 0, 768, 0, 1);
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    
    glEnable( GL_TEXTURE_2D );
    
    glBindTexture( GL_TEXTURE_2D, framebuffer1_.depth_buffer() );
    glBegin( GL_QUADS );
      glTexCoord2f(0,0); glVertex3f(       0,  768./2, 0 );
      glTexCoord2f(1,0); glVertex3f( 1024./2,  768./2, 0 );
      glTexCoord2f(1,1); glVertex3f( 1024./2,     768, 0 );
      glTexCoord2f(0,1); glVertex3f(       0,     768, 0 );
    glEnd();
    
    glBindTexture( GL_TEXTURE_2D, framebuffer1_.color_buffer() );
    glBegin( GL_QUADS );
      glTexCoord2f(0,0); glVertex3f( 1024./2, 768./2, 0 );
      glTexCoord2f(1,0); glVertex3f(    1024, 768./2, 0 );
      glTexCoord2f(1,1); glVertex3f(    1024,    768, 0 );
      glTexCoord2f(0,1); glVertex3f( 1024./2,    768, 0 );
    glEnd();
    
    glBindTexture( GL_TEXTURE_2D, framebuffer2_.depth_buffer() );
    glBegin( GL_QUADS );
      glTexCoord2f(0,0); glVertex3f(       0,  0, 0 );
      glTexCoord2f(1,0); glVertex3f( 1024./2,  0, 0 );
      glTexCoord2f(1,1); glVertex3f( 1024./2,  768./2, 0 );
      glTexCoord2f(0,1); glVertex3f(       0,  768./2, 0 );
    glEnd();
    
    glBindTexture( GL_TEXTURE_2D, framebuffer2_.color_buffer() );
    glBegin( GL_QUADS );
      glTexCoord2f(0,0); glVertex3f( 1024./2, 0, 0 );
      glTexCoord2f(1,0); glVertex3f(    1024, 0, 0 );
      glTexCoord2f(1,1); glVertex3f(    1024, 768./2, 0 );
      glTexCoord2f(0,1); glVertex3f( 1024./2, 768./2, 0 );
    glEnd();
  }
}