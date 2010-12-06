/**
 * \file
 * \brief Declaration of the Transform class
 */

#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <iostream>
#include <iomanip>
#include "mar_tp1/vector.h"

namespace mar
{
  /**
   * \brief This class handles a transform in heterogeneous space. Translation, rotation, scaling.
   */
  class Transform
  {
  public:
    /**
     * \brief Create an identity transform.
     */
    Transform();
    
    /**
     * \brief Destruct the transform.
     */
    ~Transform();
    
  public:
    /**
     * \brief Add a rotation to the transform.
     * \param angle  Angle of rotation
     * \param vector Vector around which we will rotate
     */
    void rotate( float angle, Vector<3> vector );
    
    /**
     * \brief Add a translation to the transform
     * \param vector Translation vector.
     */
    void translate( Vector<3> vector );
    
    /**
     * \brief Add a scaling to the transform.
     * \param vector Components by which the scaling will operate
     */
    void scale( Vector<3> vector );
    
    /**
     * \brief Combine with an other transform matrix.
     * \param matrix 4x4 matrix representing a transform.
     */
    void combine( float* matrix );
    
  public:
    /**
     * \brief Retrieve 4x4 transform matrix.
     */
    const float* matrix() const;
  
  public:
    /**
     * \brief Combine the current transform to the OpenGL model-view matrix.
     */
    void apply() const;
    
  private:
    /**
     * \brief The underlying transform matrix
     */
    float matrix_[4*4];
  };
}

#endif // TRANSFORM_H
