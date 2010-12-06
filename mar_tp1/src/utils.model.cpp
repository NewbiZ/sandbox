#include "mar_tp1/utils.model.h"

namespace mar
{
  // TODO:
  // Will have to rewrite the model class to provide complete 8 points boundings boxes. 2 points
  // (min/max) is only coherent if all bounding boxes fall in the same object space :(
  bool collide( const Model& m1, const Model& m2 )
  {
    const Vector<3>& min1 = m1.minimumVertex();
    const Vector<3>& max1 = m1.maximumVertex();
    
    const Vector<3>& min2 = m2.minimumVertex();
    const Vector<3>& max2 = m2.maximumVertex();
    
    // The idea is that for a bounding box to be outside of another, it basically has to be
    // outside on at least 1 axis
    // CHECKME: Is that really smart?
    //          Eeer not that sure that it works in the end :P
    //          Doh.
    return !( (min1.x<min2.x && max1.x<min2.x  || min1.x>max2.x && max1.x>max2.x) || // outside in x
              (min1.y<min2.y && max1.y<min2.y  || min1.y>max2.y && max1.y>max2.y) || // outside in y
              (min1.z<min2.z && max1.z<min2.z  || min1.z>max2.z && max1.z>max2.z) ); // outside in z
  }
}