#include "mar_proj/utils.matrix.h"

#include <cmath>
#include <algorithm>
#include <iomanip>

#include "mar_proj/vector.h"
#include "mar_proj/utils.vector.h"
#include "mar_proj/platform.h"

// Reminders:
//
// - OpenGL stores matrixes in column-first linear order:
//   |  0 |  4 |  8 | 12 |
//   |  1 |  5 |  9 | 13 |
//   |  2 |  6 | 10 | 14 |
//   |  3 |  7 | 11 | 15 |
//
// - OpenGL uses column-vectors:
//   | 0 |
//   | 1 |
//   | 2 |
//   | 3 |

namespace mar
{
  unsigned int at( unsigned int r, unsigned int c )
  {
    return 4*c+r;
  }
  
  void identity( float* out )
  {
    std::fill( out, out+16, 0 );

    out[at(0,0)] = 1;
    out[at(1,1)] = 1;
    out[at(2,2)] = 1;
    out[at(3,3)] = 1;
  }
  
  void zero( float* out )
  {
    std::fill( out, out+16, 0 );
  }
  
  void copy( const float* in, float* out )
  {
    std::copy( in, in+16, out );
  }
  
  void multiply( const float* lhs_in, const float* rhs_in, float* out )
  {
    // Sanity check, just in case someone tried an inplace matrix multiplication...
    const float* lhs = lhs_in;
    const float* rhs = rhs_in;
    if ( lhs_in==out )
    {
      float* tmp = new float[16];
      copy( lhs_in, tmp );
      lhs = tmp;
    }
    if ( lhs_in==out )
    {
      float* tmp = new float[16];
      copy( rhs_in, tmp );
      rhs = tmp;
    }
    
    // Ref: http://www.kerrywong.com/2009/03/07/matrix-multiplication-performance-in-c/
    for ( int r=0 ; r<4; ++r )
    {
      for ( int c=0;  c<4; ++c )
      {
        out[at(r,c)] = 0;
        for ( int k=0; k<4; ++k )
          out[at(r,c)] += lhs[at(r,k)] * rhs[at(k,c)];
      }
    }
    
    if ( lhs_in==out )
      delete [] lhs;
    if ( lhs_in==out )
      delete [] rhs;
  }
  
  void rotate( float angle, const Vector<3>& vector, float* out )
  {
    // Ref: http://www.gamedev.net/reference/articles/article1199.asp
    float rotation_matrix[16];
    Vector<3> v(vector);
    mar::normalize(v);
    
    float c = std::cos(angle/180.*M_PI);
    float s = std::sin(angle/180.*M_PI);
    float t = 1. - std::cos(angle/180.*M_PI);
    
    // Row 1
    rotation_matrix[ 0] = t*v.x*v.x + c;
    rotation_matrix[ 1] = t*v.x*v.y + s*v.z;
    rotation_matrix[ 2] = t*v.x*v.z - s*v.y;
    rotation_matrix[ 3] = 0;
    
    // Row 2
    rotation_matrix[ 4] = t*v.x*v.y - s*v.z;
    rotation_matrix[ 5] = t*v.y*v.y + c;
    rotation_matrix[ 6] = t*v.y*v.z + s*v.x;
    rotation_matrix[ 7] = 0;
    
    // Row 3
    rotation_matrix[ 8] = t*v.x*v.z + s*v.y;
    rotation_matrix[ 9] = t*v.y*v.z - s*v.x;
    rotation_matrix[10] = t*v.z*v.z + c;
    rotation_matrix[11] = 0;
    
    // Row 4
    rotation_matrix[12] = 0;
    rotation_matrix[13] = 0;
    rotation_matrix[14] = 0;
    rotation_matrix[15] = 1;
    
    multiply( out, rotation_matrix, out );
  }
  
  void translate( const Vector<3>& v, float* out )
  {
    // Ref: http://en.wikipedia.org/wiki/Translation_(geometry)
    float translation_matrix[16];
    
    zero( translation_matrix );
    
    translation_matrix[at(0,0)] = 1;
    translation_matrix[at(1,1)] = 1;
    translation_matrix[at(2,2)] = 1;
    translation_matrix[at(3,3)] = 1;
    
    translation_matrix[at(0,3)] = v.x;
    translation_matrix[at(1,3)] = v.y;
    translation_matrix[at(2,3)] = v.z;
    
    multiply( out, translation_matrix, out );
  }
  
  void scale( const Vector<3>& v, float* out )
  {
    float scaling_matrix[16];
    
    zero( scaling_matrix );
    
    scaling_matrix[at(0,0)] = v.x;
    scaling_matrix[at(1,1)] = v.y;
    scaling_matrix[at(2,2)] = v.z;
    scaling_matrix[at(3,3)] = 1;
    
    multiply( out, scaling_matrix, out );
  }
  
  void print_matrix( const float* matrix )
  {
    std::cout.precision(2);
    for ( int i=0; i<16; ++i )
    {
      if ( i%4==0 )
        std::cout << std::endl << "| ";
      std::cout << std::setw(6) << std::fixed << matrix[i] << " |";
    }
  }
}
