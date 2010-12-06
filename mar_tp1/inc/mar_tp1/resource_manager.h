/**
 * \file
 * \brief Declaration of the ResourceManager class
 */

#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <string>
#include <map>
#include <GL/glfw.h>

namespace mar
{
  /**
   * \brief This class handles resources (only texture here).
   */
  class ResourceManager
  {
  public:
    /**
     * \brief Create a ResourceManager.
     */
    ResourceManager();
    
    /**
     * \brief Destruct a ResourceManager.
     */
    ~ResourceManager();
    
  public:
    /**
     * \brief Load a (TGA) texture from a file.
     * \param filename Path of the TGA file
     * \param clamp    If true, texture will be clamped. It will be wrapped otherwise.
     */
    bool load_texture( const std::string& filename, bool clamp=false );
    
    /**
     * \brief Release a previously loaded texture.
     * \param filename Path of the loaded texture.
     */
    bool release_texture( const std::string& filename );
    
    /**
     * \brief Retrieve a texture ID from a previsouly loaded filename.
     */
    GLuint get_texture( const std::string& filename );
    
  private:
    /**
     * \brief Type of associative container for filename->ID mapping.
     */
    typedef std::map<std::string,GLuint> TextureList;
    
    /**
     * \brief Map filenames to texture ID.
     */
    TextureList textures_;
  };
}

#endif // RESOURCE_MANAGER_H
