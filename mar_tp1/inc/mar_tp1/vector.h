/**
 * \file
 * \brief Declaration of the Vector<Dimension> classes
 */

#ifndef VECTOR_H
#define VECTOR_H

#include "mar_tp1/point.h"

namespace mar
{
  // --- Some typedefs
  template <unsigned int> class Vector;
  
  /**
   * \brief A 2D Vector.
   */
  typedef Vector<2> Vec2;
  
  /**
   * \brief A 3D Vector.
   */
  typedef Vector<3> Vec3;
  
  /**
   * \brief A 4D Vector.
   */
  typedef Vector<4> Vec4;
  
  /**
   * \brief Placeholder for vectors of weird dimensions. Do not allow instantiation.
   */
  template <unsigned int DimensionT>
  class Vector
  {
  private:
    /**
     * \brief Do no define.
     */
    Vector();
    
    /**
     * \brief Do no define.
     */
    ~Vector();
  };
  
  /**
   * \brief 2D vectors
   */
  template <>
  class Vector<2> : public Point<2>
  {
  public:
    /**
     * \brief Create a 2D Vector. 0 initialied.
     */
    Vector()
      : x(components[0]), y(components[1])
    {
    }
    
    /**
     * \brief Create a 2D Vector from an other one.
     */
    Vector( const Vector<2>& other )
      : Point<2>(other), x(components[0]), y(components[1])
    {
    }
    
    /**
     * \brief Create a 2D Vector from an array (2-sized)
     */
    Vector( const float (&array)[Dimension] )
      : Point<2>(array), x(components[0]), y(components[1])
    {
    }
    
    /**
     * \brief Create a 2D Vector from x and y components.
     */
    Vector( float x, float y )
      : x(components[0]), y(components[1])
    {
      this->x = x;
      this->y = y;
    }
    
    /**
     * \brief Assign a 2D Vector.
     */
    Vector<2>& operator=( const Vector<2>& other )
    {
      if ( this!=&other )
      {
        x = other.x;
        y = other.y;
      }
      return *this;
    }
    
    /**
     * \brief Destruct a Vector.
     */
    ~Vector() {}
    
  public:
    /**
     * \brief X component.
     */
    float& x;
    
    /**
     * \brief Y component.
     */
    float& y;
  };
  
  /**
   * \brief 3D vectors
   */
  template <>
  class Vector<3> : public Point<3>
  {
  public:
    /**
     * \brief Construct a 3D Vector. 0-initialized.
     */
    Vector()
      : x(components[0]), y(components[1]), z(components[2])
    {
    }
    
    /**
     * \brief Construct a 3D Vector from an other one.
     */
    Vector( const Vector<3>& other )
      : Point<3>(other), x(components[0]), y(components[1]), z(components[2])
    {
    }
    
    /**
     * \brief Construct a 3D Vector from an array (3-sized).
     */
    Vector( const float (&array)[Dimension] )
      : Point<3>(array), x(components[0]), y(components[1]), z(components[2])
    {
    }
    
    /**
     * \brief Construct a 3D Vector from x, y and z components.
     */
    Vector( float x, float y, float z )
      : x(components[0]), y(components[1]), z(components[2])
    {
      this->x = x;
      this->y = y;
      this->z = z;
    }
    
    /**
     * \brief Assign a 3D Vector.
     */
    Vector<3>& operator=( const Vector<3>& other )
    {
      if ( this!=&other )
      {
        x = other.x;
        y = other.y;
        z = other.z;
      }
      return *this;
    }
    
    /**
     * \brief Destruct a Vector.
     */
    ~Vector() {}
    
  public:
    /**
     * \brief X component.
     */
    float& x;
    
    /**
     * \brief Y component.
     */
    float& y;
    
    /**
     * \brief Z component.
     */
    float& z;
  };
  
  /**
   * \brief 4D vectors
   */
  template <>
  class Vector<4> : public Point<4>
  {
  public:
    /**
     * \brief Construct a 4D Vector. 0-initialized.
     */
    Vector()
      : x(components[0]), y(components[1]), z(components[2]), w(components[3])
    {
    }
    
    /**
     * \brief Construct a 4D Vector from an other one.
     */
    Vector( const Vector<4>& other )
      : Point<4>(other), x(components[0]), y(components[1]), z(components[2]), w(components[3])
    {
    }
    
    /**
     * \brief Construct a 4D Vector from an array (4-sized).
     */
    Vector( const float (&array)[Dimension] )
      : Point<4>(array), x(components[0]), y(components[1]), z(components[2]), w(components[3])
    {
    }
    
    /**
     * \brief Construct a 4D Vector from an x, y, z and w components.
     */
    Vector( float x, float y, float z, float w )
      : x(components[0]), y(components[1]), z(components[2]), w(components[3])
    {
      this->x = x;
      this->y = y;
      this->z = z;
      this->w = w;
    }
    
    /**
     * \brief Assign a 4D Vector.
     */
    Vector<4>& operator=( const Vector<4>& other )
    {
      if ( this!=&other )
      {
        x = other.x;
        y = other.y;
        z = other.z;
        w = other.w;
      }
      return *this;
    }
    
    /**
     * \brief Destruct a Vector.
     */
    ~Vector() {}
    
  public:
    /**
     * \brief X component.
     */
    float& x;
    
    /**
     * \brief Y component.
     */
    float& y;
    
    /**
     * \brief Z component.
     */
    float& z;
    
    /**
     * \brief W component.
     */
    float& w;
  };
}

#endif // VECTOR_H
