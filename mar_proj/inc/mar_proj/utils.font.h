/**
 * \file
 * \brief Utilities for handling fonts
 */

#ifndef UTILS_FONT_H
#define UTILS_FONT_H

#include <string>

namespace mar
{
  /**
   * \brief Prints a text on screen - bitmap billboard at (x;y)
   */
   void render_text( int x, int y, const std::string& text );
}

#endif // UTILS_FONT_H
