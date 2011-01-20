#include "mar_proj/resource_manager.h"

#include <iostream>
#include "mar_proj/platform.h"
#include "mar_proj/tga_loader.h"

namespace mar
{
  ResourceManager::ResourceManager()
  {
  }
  
  ResourceManager::~ResourceManager()
  {
    //TODO: should release remaining resources here!
  }

  bool ResourceManager::load_texture( const std::string& filename )
  {
    // ---State preservation---
    GLuint previous_texture;
    glGetIntegerv( GL_TEXTURE_BINDING_2D, (GLint*)&previous_texture );
    
    // Generate a new texture id
    GLuint texture_id;
    glGenTextures( 1, &texture_id );
    
    // Load the texture using GLFW
    glBindTexture( GL_TEXTURE_2D, texture_id );
    
    TGALoader loader;
    loader.load( filename );
    
    // Apply some standard parameters to the texture (linear filtering)
    //TODO: should let the user choose his own texture parameters
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
    
    if ( loader.getDepth() == 24 )
      glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB8, loader.getWidth(), loader.getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, loader.getData() );
    else if ( loader.getDepth() == 32 )
      glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA8, loader.getWidth(), loader.getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, loader.getData() );
    else
      glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB8, loader.getWidth(), loader.getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, loader.getData() );
    
    // Add the texture to the list of loaded one
    textures_.insert( std::make_pair(filename, texture_id) );
    
    // ---State preservation---
    glBindTexture( GL_TEXTURE_2D, previous_texture );
  }
  
  bool ResourceManager::release_texture( const std::string& filename )
  {
    // Retrieve texture id
    GLuint texture_id = get_texture(filename);
    
    if ( texture_id!=0 )
    {
      glDeleteTextures( 1, &texture_id ); // Texture found
      textures_.erase( filename );
      return true;
    }
    else
    {
      std::cerr << "Warning: cannot release non loaded texture \"" << filename << "\"" << std::endl;
      return false;
    }
  }
  
  GLuint ResourceManager::get_texture( const std::string& filename ) const
  {
    TextureList::const_iterator it = textures_.find( filename );
    
    if ( it==textures_.end() )
    {
      std::cerr << "Warning: cannot retrieve non loaded texture \"" << filename << "\"" << std::endl;
      return 0;   // Texture not found
    }
    else
      return it->second; // Texture found
  }
}
