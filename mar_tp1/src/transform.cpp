#include "mar_tp1/transform.h"

#include <GL/glfw.h>
#include "mar_tp1/utils.matrix.h"

namespace mar
{
  Transform::Transform()
  {
    identity( matrix_ );
  }
  
  Transform::~Transform()
  {
  }
  
  const float* Transform::matrix() const
  {
    return matrix_;
  }
  
  void Transform::combine( float* transform )
  {
    mar::multiply( matrix_, transform, matrix_ );
  }
  
  void Transform::rotate( float angle, Vector<3> vector )
  {
    mar::rotate( angle, vector, matrix_ );
  }
  
  void Transform::translate( Vector<3> v )
  {
    mar::translate( v, matrix_ );
  }
  
  void Transform::scale( Vector<3> v )
  {
    mar::scale( v, matrix_ );
  }
  
  void Transform::apply() const
  {
    float modelview[16];
    
    glGetFloatv( GL_MODELVIEW_MATRIX , modelview );
    
    mar::multiply( modelview, matrix_, modelview );
    
    glLoadMatrixf( modelview );
  }
}
