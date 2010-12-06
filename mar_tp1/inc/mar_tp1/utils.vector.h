/**
 * \file
 * \brief Utilities for handling vectors
 */

#ifndef UTILS_VECTOR_H
#define UTILS_VECTOR_H

#include <iosfwd>

namespace mar
{
  // --- Some forwards
  template <unsigned int> class Vector;
  template <unsigned int> class Point;
  
  /**
   * \brief vector*scalar
   */
  template <class PointT>
  PointT operator*( const PointT& v, float s );
  
  /**
   * \brief scalar*vector
   */
  template <class PointT>
  PointT operator*( float s, const PointT& v );
  
  /**
   * vvector/scalar
   */
  template <class PointT>
  PointT operator/( const PointT& v, float s );
  
  /**
   * \brief scalar/vector, Not very meaningful tho... equivalent to vector/scalar
   */
  template <class PointT>
  PointT operator/( float s, const PointT& v );
  
  /**
   * \brief -vector
   */
  template <class PointT>
  void negate( PointT& v );
  
  /**
   * \brief adjust size to unit length
   */
  template <class PointT>
  void normalize( PointT& v );
  
  /**
   * \brief dot (inner) product
   */
  template <class PointT>
  float dot( const PointT& v1, const PointT& v2 );
  
  /**
   * vcross (outter) product
   */
  inline Vector<3> cross( const Vector<3>& v1, const Vector<3>& v2 );
  
  /**
   * \brief vector length
   */
  template <class PointT>
  float length( PointT& v );
}

/**
 * \brief Prints out a vector on a stream.
 */
template <unsigned int D>
std::ostream& operator<<( std::ostream& o, const mar::Point<D>& p );

// --- Inline implementation
#include "mar_tp1/utils.vector.hxx"

#endif // UTILS_VECTOR_H
