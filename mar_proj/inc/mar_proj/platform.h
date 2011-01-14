/**
 * \file
 * \brief Voodoo platform-specific fixes
 */

#ifndef PLATFORM_H
#define PLATFORM_H

/**
 * M_PI is not defined on strictly standard-compliant platforms
 */
#ifndef M_PI
#   define M_PI 3.14159265358979323846
#endif

/**
 * Default OpenGL libs from XP to Seven are 1.1
 * Easier to define this than expect the user to update his
 * OpenGL headers.
 */
#ifndef GL_CLAMP_TO_EDGE
#   define GL_CLAMP_TO_EDGE 0x812F
#endif

#endif // PLATFORM_H
