#include "mar_proj/framebuffer.h"

#include <iostream>

namespace mar
{
  FrameBuffer::FrameBuffer()
    : fbo_(0), depthBuffer_(0), colorBuffer_(0)
  {
  }
  
  FrameBuffer::~FrameBuffer()
  {
    disable();
    
    if ( fbo_ )
      glDeleteFramebuffersEXT( 1, &fbo_ );
    
    if ( colorBuffer_ )
      glDeleteRenderbuffersEXT( 1, &colorBuffer_ );
    
    if ( depthBuffer_ )
      glDeleteRenderbuffersEXT( 1, &depthBuffer_ );
  }
  
  void FrameBuffer::setup( unsigned int width, unsigned int height )
  {
    glGenFramebuffersEXT ( 1, &fbo_         );
    glGenTextures        ( 1, &depthBuffer_ );
    glGenTextures        ( 1, &colorBuffer_ );
    
    glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, fbo_ );
    
    //------- Color buffer
    glBindTexture( GL_TEXTURE_2D, colorBuffer_ );
    
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
    
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0 );
    
    glFramebufferTexture2DEXT( GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, colorBuffer_, 0 );
    
    //------- Depth buffer
    glBindTexture( GL_TEXTURE_2D, depthBuffer_ );
    
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
    
    glTexImage2D( GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0 );
    
    glFramebufferTexture2DEXT( GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_TEXTURE_2D, depthBuffer_, 0 );
    
    //------- Sanity check
    GLenum status = glCheckFramebufferStatusEXT( GL_FRAMEBUFFER_EXT );
    
    if ( status!=GL_FRAMEBUFFER_COMPLETE_EXT )
    {
      std::cerr << "Error: Framebuffer status is incomplete!" << std::endl;
      return;
    }
    
    disable();
  }
  
  void FrameBuffer::enable() const
  {
    glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, fbo_ );
    glBindTexture( GL_TEXTURE_2D, 0 );
  }
  
  void FrameBuffer::disable() const
  {
    glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, 0 );
    glBindRenderbufferEXT( GL_RENDERBUFFER_EXT, 0 );
    glBindTexture( GL_TEXTURE_2D, 0 );
  }
  
  GLuint FrameBuffer::depth_buffer() const
  {
    return depthBuffer_;
  }
  
  GLuint FrameBuffer::color_buffer() const
  {
    return colorBuffer_;
  }
}