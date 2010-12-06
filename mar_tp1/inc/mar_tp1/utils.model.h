/**
 * \file
 * \brief Utilities for handling models
 */

#ifndef UTILS_MODEL_H
#define UTILS_MODEL_H

#include "mar_tp1/model.h"

namespace mar
{
  /**
   * \brief Test if two models do collide, by checking their bounding boxes.
   * \todo How to handle bounding boxes from different object spaces ?
   *       We should provide both object->world transformations of the input
   *       models, and compare world space coordinates of bounding boxes...
   *       Pffff....
   */
  bool collide( const Model& m1, const Model& m2 );
}

#endif // UTILS_MODEL_H
