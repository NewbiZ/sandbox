#include "mar_proj/utils.font.h"

#include <GL/glut.h>
#include <algorithm>

namespace mar
{
  void render_text( int x, int y, const std::string& text )
  {
    glRasterPos3i( x, y, 1 );
    std::string::const_iterator it = text.begin();
    for (; it!=text.end(); ++it )
      glutBitmapCharacter( GLUT_BITMAP_9_BY_15, *it );
  }
}