/**
 * \file
 * \brief Utilities for handling matrices
 */

#ifndef UTILS_MATRIX_H
#define UTILS_MATRIX_H

#include <iosfwd>

namespace mar
{
  // --- Some forwards
  class Transform;
  template <unsigned int> class Vector;
  
  /**
   * \brief Transforms a multilinear index [0..3][0..3] to a linear index [0..15].
   */
  unsigned int at( unsigned int r, unsigned int c );
  
  /**
   * \brief Copy one matrix into another. This obviously assumes that the out matrix
   * was previously allocated.
   */
  void copy( const float* in, float* out );
  
  /**
   * \brief Resets the out matrix to identity. That is, filled with zeros except for 1s
   * in diagonals.
   */
  void identity( float* out );
  
  /**
   * \brief Completely blanks a matrix, filling it with 0s.
   */
  void zero( float* out );
  
  /**
   * \brief Multiply two matrices. Inplace multiplication is handled if one or both of the
   * source matrices are the same as the output.
   */
  void multiply( const float* lhs, const float* rhs, float* out );
  
  /**
   * \brief Adds a rotation defined by an angle and an axis to a (again, previously allocated)
   * matrix.
   */
  void rotate( float angle, const Vector<3>& vector, float* out );
  
  /**
   * \brief Adds a translation defined by a vector to a (as always, previously allocated) matrix.
   */
  void translate( const Vector<3>& vector, float* out );
  
  /**
   * \brief Adds a scale defined by a vector to a (guess what? previously allocated) matrix.
   */
  void scale( const Vector<3>& vector, float* out );
  
  /**
   * \brief Prints out a matrix on std::cout
   */
  void print_matrix( const float* m );
}

#endif // UTILS_MATRIX_H
