#include <iostream>
#include <cstdlib>

#include "mar_tp1/vector.h"
#include "mar_tp1/model.h"
#include "mar_tp1/camera.h"
#include "mar_tp1/transform.h"
#include "mar_tp1/color.h"
#include "mar_tp1/utils.matrix.h"
#include "mar_tp1/utils.vector.h"
#include "mar_tp1/scene_graph.h"

#include <GL/glfw.h>

using namespace mar;

static const unsigned int width  = 1920;
static const unsigned int height = 1200;

void initialize_glfw()
{
  glfwInit();
  
  glfwOpenWindowHint( GLFW_FSAA_SAMPLES, 4 ); // fullscreen antialiasing
  glfwOpenWindow( width, height, 8, 8, 8, 8, 24, 0, GLFW_FULLSCREEN );
  glfwSetWindowTitle( "MAR TP1" );
  glfwSwapInterval(1); // vsync enabled
}

void initialize_opengl()
{
  glEnable( GL_LIGHTING );
  glEnable( GL_LIGHT0 );
  glEnable( GL_TEXTURE_2D );
  glEnable( GL_BLEND );
  
  glEnable( GL_DEPTH_TEST );
  
  glShadeModel( GL_SMOOTH );
  glEnable( GL_CULL_FACE );
  
  glClearColor( 1, 1, 1, 1 );
}

void setup_normal_view()
{
  glViewport( 0, 0, width, height );
  glMatrixMode( GL_PROJECTION );
  glLoadIdentity();
  gluPerspective( 60, (float)width/(float)height, 1, 10000. );
  glMatrixMode( GL_MODELVIEW );
  glLoadIdentity();
}

void setup_minimap_view()
{
  glViewport( 0, 0, (float)width/5, (float)height/5. );
  glMatrixMode( GL_PROJECTION );
  glLoadIdentity();
  glOrtho( -(float)width/3, (float)width/3, -(float)height/3, (float)height/3, -300, 300);
  glMatrixMode( GL_MODELVIEW );
  glLoadIdentity();
}

void release_glfw()
{
  glfwTerminate();
}

int main ( int argc, char const** argv)
{
  initialize_glfw();
  initialize_opengl();
  
  setup_normal_view();
  
  Model scene;
  Model tree;
  
  Camera camera( Vector<3>(0,160,800), 90 );
  
  scene.load( "resources/", "scene.obj", "scene.mtl" );
  tree.load( "resources/", "tree.obj", "tree.mtl" );
  
  camera.loadSkybox( "resources/skybox_top.tga",
                     "resources/skybox_front.tga",
                     "resources/skybox_back.tga",
                     "resources/skybox_left.tga",
                     "resources/skybox_right.tga" );
  
  SceneGraph graph;
  
  while (1)
  {
    setup_normal_view();
    
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    
    if ( glfwGetKey( GLFW_KEY_ESC ) || !glfwGetWindowParam( GLFW_OPENED ) )
      break;
    
    double deltaTime = glfwGetTime()*100;
    glfwSetTime(0);
    
    std::cout << deltaTime << std::endl;
    
    if ( glfwGetKey( GLFW_KEY_RIGHT ) )
      camera.rotate( -deltaTime );
    if ( glfwGetKey( GLFW_KEY_LEFT ) )
      camera.rotate( deltaTime );
    if ( glfwGetKey( GLFW_KEY_UP ) )
      camera.forward( 5*deltaTime );
    if ( glfwGetKey( GLFW_KEY_DOWN ) )
      camera.backward( 5*deltaTime );

    camera.transform_and_render();
    
    static float position[] = { 200.0, 200.0, 0.0, 1.0 };
    glLightfv( GL_LIGHT0, GL_POSITION, position );
    
    scene.render();

    glTranslatef( 200,0,0 );
    tree.render(true);
    
    glTranslatef( 200,0,0 );
    tree.render( true );
    
    glTranslatef( 200,0,0 );
    tree.render( true );
    
    //----------------------- MINIMAP ---------------------------
    glDisable( GL_LIGHTING );
    glShadeModel( GL_FLAT );
    glDisable( GL_TEXTURE_2D );
    
    setup_minimap_view();
    
    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    glColor4f(0,0,0,0.4);
    glBegin( GL_QUADS );
      glVertex3f(-(float)width/3, -(float)height/3, -200);
      glVertex3f( (float)width/3, -(float)height/3, -200);
      glVertex3f( (float)width/3,  (float)height/3, -200);
      glVertex3f(-(float)width/3,  (float)height/3, -200);
    glEnd();
    glDisable( GL_BLEND );
    
    glLineWidth( 2 );
    glColor3f(0,0,0);
    glBegin( GL_LINES );
      glVertex3f(-(float)width/3, -(float)height/3, 100);
      glVertex3f( (float)width/3, -(float)height/3, 100);
      
      glVertex3f( (float)width/3, -(float)height/3, 100);
      glVertex3f( (float)width/3,  (float)height/3, 100);
      
      glVertex3f( (float)width/3,  (float)height/3, 100);
      glVertex3f(-(float)width/3,  (float)height/3, 100);
      
      glVertex3f(-(float)width/3,  (float)height/3, 100);
      glVertex3f(-(float)width/3, -(float)height/3, 100);
    glEnd();
    glLineWidth( 1 );
    
    glScalef( 0.5, 0.5, 0.5 );
    glRotatef( 90, 1, 0, 0 );
    
    camera.render_fov();
    
    scene.render();
    glTranslatef( 200,0,0 );
    tree.render();
    
    glTranslatef( 200,0,0 );
    tree.render();
    
    glTranslatef( 200,0,0 );
    tree.render();
    
    glEnable( GL_LIGHTING );
    glShadeModel( GL_SMOOTH );
    glEnable( GL_TEXTURE_2D );
    //-----------------------------------------------------------
    glfwSwapBuffers();
  }
  
  release_glfw();
  
  return EXIT_SUCCESS;
}
