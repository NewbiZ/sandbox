#include <iostream>
#include <cstdlib>

#include <GL/glew.h>
#include <GL/glut.h>
#include <glui.h>

#include "mar_proj/scene.h"

static const unsigned int width  = 1024;
static const unsigned int height = 768;

static int window_handle;
static GLUI* glui;

// Pass 1 options
int   pass1_model = 0;
// Pass 2 options
float pass2_radius    = 0.005;
float pass2_precision = 1;
int   pass2_nbrays    = 8;
int   pass2_power     = 12;
// Pass 3 options
int   pass3_blend = 1;
int   pass3_blur  = 1;

mar::Scene scene( pass2_radius,
                  pass2_precision,
                  pass2_nbrays,
                  pass2_power,
                  pass3_blend,
                  pass3_blur );

void change_model( int id )
{
  if ( pass1_model==0 )
  {
    scene.load_model( "resources/dragon.ply" );
  }
  else if ( pass1_model==1 )
  {
    scene.load_model( "resources/bunny.ply" );
  }
  else if ( pass1_model==2 )
  {
    scene.load_model( "resources/buddha.ply" );
  }
}

void keyboard_func( unsigned char key, int x, int y )
{
  switch (key)
  {
    case 27: // ESC
      exit(EXIT_SUCCESS);
      break;
  }
}

void special_func( int key, int x, int y )
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

void display_func()
{
  scene.render();
  glutSwapBuffers();
}

void idle_func()
{
  if ( glutGetWindow()!=window_handle )
    glutSetWindow(window_handle);
  
  glutPostRedisplay();
}

void setup_glut()
{
  int   argc = 1;
  char* argv = "mar_proj";
  glutInit( &argc, &argv );
  glutInitWindowSize( width, height );
  glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
  window_handle = glutCreateWindow( "SSAO" );
  
  glutSpecialFunc ( special_func  );
  glutKeyboardFunc( keyboard_func );
  glutIdleFunc    ( idle_func     );
  glutDisplayFunc ( display_func  );
}

void setup_glui()
{
  glui = GLUI_Master.create_glui( "SSAO controls" );
  
  GLUI_Panel* panel_pass1 = glui->add_panel( "Pass 1" );
  GLUI_Panel* panel_pass2 = glui->add_panel( "Pass 2" );
  GLUI_Panel* panel_pass3 = glui->add_panel( "Pass 3" );
  
  panel_pass1->set_w(300);
  panel_pass2->set_w(300);
  panel_pass3->set_w(300);
  
  GLUI_Listbox* model_list = glui->add_listbox_to_panel( panel_pass1, "Model:", &pass1_model, 0, change_model );
  model_list->add_item( 0, "Dragon" );
  model_list->add_item( 1, "Bunny"  );
  model_list->add_item( 2, "Buddha" );
  
  glui->add_spinner_to_panel( panel_pass2, "Radius:", GLUI_SPINNER_FLOAT, &pass2_radius )
    ->set_float_limits( 0.00001, 0.5, GLUI_LIMIT_CLAMP );
  
  glui->add_spinner_to_panel( panel_pass2, "Precision:", GLUI_SPINNER_FLOAT, &pass2_precision )
    ->set_float_limits( 1., 10., GLUI_LIMIT_CLAMP );
  
  glui->add_spinner_to_panel( panel_pass2, "Nb rays:", GLUI_SPINNER_INT, &pass2_nbrays )
    ->set_int_limits( 1, 128, GLUI_LIMIT_CLAMP );
  
  glui->add_spinner_to_panel( panel_pass2, "Power:", GLUI_SPINNER_INT, &pass2_power )
    ->set_int_limits( 1, 30, GLUI_LIMIT_CLAMP );
  
  glui->add_checkbox_to_panel( panel_pass3, "Blend", &pass3_blend );
  glui->add_checkbox_to_panel( panel_pass3, "Blur",  &pass3_blur  );
  
  glui->set_main_gfx_window( window_handle );
  
  GLUI_Master.set_glutIdleFunc    ( idle_func     );
  GLUI_Master.set_glutKeyboardFunc( keyboard_func );
  GLUI_Master.set_glutSpecialFunc ( special_func  );
  
  glui->add_separator();
  
  glui->add_button( "Quit", 0, (GLUI_Update_CB)exit );
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
  setup_glui();
  setup_scene();
  
  glutMainLoop();
  
  return EXIT_SUCCESS;
}
