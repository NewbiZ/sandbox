/**
 * \file
 * \warning This file contains template definitions of utils.vector.h
 *          It is NOT intended to be included directly. Use utils.vector.h
 *          instead.
 */
#ifndef UTILS_VECTOR_HXX
#define UTILS_VECTOR_HXX

#include <algorithm>
#include <numeric>
#include <functional>
#include <cmath>

#include "mar_tp1/vector.h"
#include "mar_tp1/point.h"

namespace mar
{
  template <class PointT>
  PointT operator*( const PointT& v, float s )
  {
    PointT result;
    std::transform( v.components, v.components+PointT::Dimension, result.components, std::bind2nd(std::multiplies<float>(),s) );
    return result;
  }
  
  template <class PointT>
  PointT operator*( float s, const PointT& v )
  {
    return v*s;
  }
  
  template <class PointT>
  PointT operator/( const PointT& v, float s )
  {
    PointT result;
    std::transform( v.components, v.components+PointT::Dimension, result.components, std::bind2nd(std::divides<float>(),s) );
    return result;
  }
  
  template <class PointT>
  PointT operator/( float s, const PointT& v )
  {
    return v/s;
  }
  
  template <class PointT>
  void negate( PointT& v )
  {
    std::transform( v.components, v.components+PointT::Dimension, v.components, std::negate<float>() );
  }
  
  template <class PointT>
  void normalize( PointT& v )
  {
    v = v/length(v);
  }
  
  template <class PointT>
  float dot( const PointT& v1, const PointT& v2 )
  {
    return std::inner_product( v1.components, v1.components+PointT::Dimension, v2.components, 0 );
  }
  
  Vector<3> cross( const Vector<3>& v1, const Vector<3>& v2 )
  {
    return Vector<3>( v1.y*v2.z-v1.z*v2.y,
                      v1.z*v2.x-v1.x*v2.z,
                      v1.x*v2.y-v1.y*v2.x );
  }
  
  template <class PointT>
  float length( PointT& v )
  {
    return std::sqrt( dot(v,v) );
  }
}

template <unsigned int D>
std::ostream& operator<<( std::ostream& o, const mar::Point<D>& p )
{
  o << "[ ";
  std::copy( p.components, p.components+D, std::ostream_iterator<float>(std::cout," ") );
  o << "]";
  return o;
}

#endif // UTILS_VECTOR_HXX
