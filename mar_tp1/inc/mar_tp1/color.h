/**
 * \file
 * \brief Declaration of the Color class
 */

#ifndef COLOR_H
#define COLOR_H

#include "mar_tp1/point.h"

namespace mar
{
  /**
   * \brief This class handles colors
   */
  class Color : public Point<4>
  {
  public: // --- Some default colors
    /**
     * \brief The color white (1,1,1).
     */
    static const Color White;
    
    /**
     * \brief The color black (0,0,0).
     */
    static const Color Black;
    
    /**
     * \brief The color red (1,0,0).
     */
    static const Color Red;
    
    /**
     * \brief The color green (0,1,0).
     */
    static const Color Green;
    
    /**
     * \brief The color blue (0,0,1).
     */
    static const Color Blue;
    
  public:
    /**
     * \brief Default constructor. Color undefined.
     */
    Color()
      : r(components[0]), g(components[1]), b(components[2]), a(components[3])
    {
    }
    
    /**
     * \brief Create a Color by providing its RGBA components. Default alpha is 1.
     */
    Color( float red, float green, float blue, float alpha=1 )
      : r(components[0]), g(components[1]), b(components[2]), a(components[3])
    {
      r = red;
      g = green;
      b = blue;
      a = alpha;
    }
    
    /**
     * \brief Create a Color from an array of 4 float components (RGBA).
     */
    Color( const float (&array)[Dimension] )
      : Point<4>(array), r(components[0]), g(components[1]), b(components[2]), a(components[3])
    {
    }
    
    /**
     * \brief Duplicate a Color.
     */
    Color( const Color& other )
      : Point<4>(other), r(components[0]), g(components[1]), b(components[2]), a(components[3])
    {
    }
    
    /**
     * \brief Assign a Color.
     */
    Color& operator=( const Color& other )
    {
      if ( this!=&other )
      {
        r = other.r;
        g = other.g;
        b = other.b;
        a = other.a;
      }
      return *this;
    }
    
    /**
     * \brief You can sublass Color. i.e. to create gradients.
     */
    virtual ~Color() {}
    
  public:
    /**
     * \brief Red component
     */
    float& r;
    
    /**
     * \brief Green component
     */
    float& g;
    
    /**
     * \brief Blue component
     */
    float& b;
    
    /**
     * \brief Alpha component
     */
    float& a;
  };
}

#endif // COLOR_H
