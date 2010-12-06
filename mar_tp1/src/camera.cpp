#include "mar_tp1/camera.h"

#include <cmath>
#include <GL/glfw.h>
#include "mar_tp1/utils.vector.h"
#include "mar_tp1/platform.h"

namespace mar
{
  Camera::Camera( Vector<3> position, float angle )
  {
    position_ = position;
    angle_    = angle;
  }
  
  void Camera::forward( float distance )
  {
    position_.x += std::cos(angle_/180*M_PI)*distance;
    position_.z -= std::sin(angle_/180*M_PI)*distance;
  }
  
  void Camera::backward( float distance )
  {
    position_.x -= std::cos(angle_/180*M_PI)*distance;
    position_.z += std::sin(angle_/180*M_PI)*distance;
  }
  
  void Camera::rotate( float angle )
  {
    angle_ += angle;
  }
  
  void Camera::renderSkybox() const
  {
    // We need a huge cube
    glScalef( 500, 500, 500 );
    
    // Disable lighting, that would reveal the cube
    glDisable( GL_LIGHTING );
    
    glBindTexture( GL_TEXTURE_2D, sky_front );
    glBegin( GL_QUADS );
      // Front face
      glTexCoord2f(0,0); glVertex3f( -5, -5, -5 );
      glTexCoord2f(1,0); glVertex3f(  5, -5, -5 );
      glTexCoord2f(1,1); glVertex3f(  5,  5, -5 );
      glTexCoord2f(0,1); glVertex3f( -5,  5, -5 );
    glEnd();
    
    glBindTexture( GL_TEXTURE_2D, sky_back );
    glBegin( GL_QUADS );
      // Back face
      glTexCoord2f(1,0); glVertex3f( -5, -5, 5 );
      glTexCoord2f(1,1); glVertex3f( -5,  5, 5 );
      glTexCoord2f(0,1); glVertex3f(  5,  5, 5 );
      glTexCoord2f(0,0); glVertex3f(  5, -5, 5 );
    glEnd();
    
    glBindTexture( GL_TEXTURE_2D, sky_top );
    glBegin( GL_QUADS );
      // Top face
      glTexCoord2f(0,0); glVertex3f( -5, 5, -5 );
      glTexCoord2f(1,0); glVertex3f(  5, 5, -5 );
      glTexCoord2f(1,1); glVertex3f(  5, 5,  5 );
      glTexCoord2f(0,1); glVertex3f( -5, 5,  5 );
    glEnd();
    
    glBindTexture( GL_TEXTURE_2D, sky_left );
    glBegin( GL_QUADS );
      // Left face
      glTexCoord2f(1,0); glVertex3f( -5, -5, -5 );
      glTexCoord2f(1,1); glVertex3f( -5,  5, -5 );
      glTexCoord2f(0,1); glVertex3f( -5,  5,  5 );
      glTexCoord2f(0,0); glVertex3f( -5, -5,  5 );
    glEnd();
    
    glBindTexture( GL_TEXTURE_2D, sky_right );
    glBegin( GL_QUADS );
      // Right face
      glTexCoord2f(0,0); glVertex3f( 5, -5, -5 );
      glTexCoord2f(1,0); glVertex3f( 5, -5,  5 );
      glTexCoord2f(1,1); glVertex3f( 5,  5,  5 );
      glTexCoord2f(0,1); glVertex3f( 5,  5, -5 );
    glEnd();
    
    // Scale back to normal size
    glScalef( 1./500, 1./500, 1./500 );
    
    // Re-enable lighting
    glEnable( GL_LIGHTING );
  }
  
  const Vector<3>& Camera::position() const
  {
    return position_;
  }
  
  float Camera::angle() const
  {
    return angle_;
  }
  
  void Camera::render_fov() const
  {
    glColor3f(1,0,0);
    glBegin( GL_TRIANGLES );
      glVertex3f(   0, position_.y,  0 );
      glVertex3f( -50, position_.y, 75 );
      glVertex3f(  50, position_.y, 75 );
    glEnd();
    
    glColor4f(1,1,1,1);
    
    glRotatef( 90-angle_, 0, 1, 0 );
    
    glTranslatef( -position_.x, -position_.y, -position_.z );
  }
  
  void Camera::transform_and_render() const
  {
    // We want the skybox to move as the user do, but it should not rotate
    // at all! Thus we interleave the skybox rendering between the
    // rotate and the translate
    
    glRotatef( 90-angle_, 0, 1, 0 );
    
    renderSkybox();
    
    glTranslatef( -position_.x, -position_.y, -position_.z );
  }
  
  void Camera::loadSkybox( const std::string& top,
                           const std::string& front,
                           const std::string& back,
                           const std::string& left,
                           const std::string& right )
  {
    resourceManager_.load_texture( top,   true );
    resourceManager_.load_texture( front, true );
    resourceManager_.load_texture( back,  true );
    resourceManager_.load_texture( left,  true );
    resourceManager_.load_texture( right, true );
    
    sky_top   = resourceManager_.get_texture( top   );
    sky_front = resourceManager_.get_texture( front );
    sky_back  = resourceManager_.get_texture( back  );
    sky_left  = resourceManager_.get_texture( left  );
    sky_right = resourceManager_.get_texture( right );
  }
}