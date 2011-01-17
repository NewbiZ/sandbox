#include <iostream>
#include <cstdlib>

#include <GL/glew.h> // glew before gl.h
#include <GL/glut.h>

#include "mar_proj/scene.h"

static const unsigned int width  = 1920;
static const unsigned int height = 1200;

mar::Scene scene;

void keyboard_func( unsigned char key, int x, int y )
{
  switch (key)
  {
    case 27: // ESC
      scene.toggle_console();
      break;
    default:
      if ( scene.console_is_open() )
        scene.send_keyboard( key );
      break;
  }
}

void special_func( int key, int x, int y )
{
  if ( scene.console_is_open() )  // --- console opened
  {
    scene.send_special( key );
  }
  else                           // --- console closed
  {
    switch (key)
    {
      case GLUT_KEY_LEFT:
        scene.turn_left();
        break;
      case GLUT_KEY_RIGHT:
        scene.turn_right();
        break;
      case GLUT_KEY_UP:
        scene.move_forward();
        break;
      case GLUT_KEY_DOWN:
        scene.move_backward();
        break;
    }
  }
}

void display_func()
{
  scene.render();
  glutSwapBuffers();
}

void setup_glut()
{
  int   argc = 1;
  char* argv = "mar_proj";
  glutInit( &argc, &argv );
  glutInitWindowSize( width, height );
  glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
  glutCreateWindow( "mar_proj" );
  glutFullScreen();
}

void setup_glew()
{
  glewInit();
  
  if ( !glewIsSupported("GL_VERSION_2_0") )
  {
    std::cerr << "[Fatal error] GLEW detected OpenGL<2.0. GLSL not available." << std::endl;
    exit( EXIT_SUCCESS );
  }
}

void setup_scene()
{
  scene.setup(width, height);
}

int main( int, char** )
{
  setup_glut();
  setup_glew();
  setup_scene();

  glutSpecialFunc ( special_func  );
  glutKeyboardFunc( keyboard_func );
  glutIdleFunc    ( display_func  );
  glutDisplayFunc ( display_func  );
  
  glutMainLoop();
  
  return EXIT_SUCCESS;
}
