/**
 * \file
 * \brief Declaration of the Point<Dimension> classes
 */

#ifndef POINT_H
#define POINT_H

#include <iostream>
#include <iterator>
#include <cassert>

namespace mar
{
  // --- Some typedefs
  template <unsigned int> class Point;
  
  /**
   * \brief A 2D Point.
   */
  typedef Point<2> Point2;
  
  /**
   * \brief A 3D Point.
   */
  typedef Point<3> Point3;
  
  /**
   * \brief Dummy point class. Vectors and Colors are derived from this.
   */
  template <unsigned int DimensionT>
  class Point
  {
  public:
    /**
     * \brief Stores the dimension of the Point.
     */
    static const unsigned int Dimension = DimensionT;
    
  public:
    /**
     * \brief Current type of Point. Just for code readability.
     */
    typedef Point<DimensionT> type;
    
  public:
    /**
     * \brief Default point constructor. All components are 0-initialized.
     */
    Point()
    {
      std::fill( components, components+Dimension, 0 );
    }
    
    /**
     * \brief Create a Point from an other one.
     */
    Point( const Point<DimensionT>& other )
    {
      std::copy( other.components, other.components+Dimension, components );
    }
    
    /**
     * \brief Create a Point from an array of components (at least Dimension-sized).
     */
    Point( const float* array )
    {
      std::copy( array, array+Dimension, components );
    }
    
    /**
     * \brief Assign from an other Point.
     */
    Point<DimensionT>& operator=( const Point<DimensionT>& other )
    {
      if ( this!=&other )
        std::copy( other.components, other.components+Dimension, components );
      
      return *this;
    }
    
    /**
     * \brief You can subclass Point.
     */
    virtual ~Point() {}
    
  public:
    /**
     * \brief Retrieve the desired component. Range from 0 to Dimension-1.
     */
    float& operator[]( unsigned int c )
    {
      assert( c<Dimension );
      return components[c];
    }
    
    /**
     * \brief Retrieve the desired component. Range from 0 to Dimension-1.
     */
    const float& operator[]( unsigned int c ) const
    {
      assert( c<Dimension );
      return components[c];
    }
    
  public:
    /**
     * \brief The components of the point.
     */
    float components[DimensionT];
  };
}


#endif // POINT_H
