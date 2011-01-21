#include "mar_proj/framebuffer.h"

#include <iostream>

namespace mar
{
  GLenum FrameBuffer::colorbuffers_[GL_MAX_COLOR_ATTACHMENTS_EXT] = {};
  
  FrameBuffer::FrameBuffer( size_t colorbuffers )
    : fbo_(0), depthBuffer_(0), nbColorBuffers_(colorbuffers)
  {
    colorBuffer_ = new GLuint[nbColorBuffers_];
  }
  
  FrameBuffer::~FrameBuffer()
  {
    disable();
    
    if ( fbo_ )
      glDeleteFramebuffersEXT( 1, &fbo_ );
    
    //TODO: should delete textures not renderbuffers?
    for ( int i=0; i<nbColorBuffers_; ++i )
      glDeleteRenderbuffersEXT( 1, &colorBuffer_[i] );
    
    if ( depthBuffer_ )
      glDeleteRenderbuffersEXT( 1, &depthBuffer_ );
  }
  
  void FrameBuffer::setup( unsigned int width, unsigned int height )
  {
    static bool initialized = false;
    if ( !initialized )
    {
      for ( int i=0; i<GL_MAX_COLOR_ATTACHMENTS_EXT; ++i )
        colorbuffers_[i] = GL_COLOR_ATTACHMENT0_EXT+i;
      initialized = true;
    }
    
    glGenFramebuffersEXT (               1, &fbo_         );
    glGenTextures        (               1, &depthBuffer_ );
    glGenTextures        ( nbColorBuffers_, colorBuffer_  );
    
    glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, fbo_ );
    
    //------- Color buffers
    for ( int i=0; i<nbColorBuffers_; ++i )
    {
      glBindTexture( GL_TEXTURE_2D, colorBuffer_[i] );
      
      glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
      glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
      glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
      glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
      
      glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0 );
      
      glFramebufferTexture2DEXT( GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT+i, GL_TEXTURE_2D, colorBuffer_[i], 0 );
    }
    
    //------- Depth buffer
    glBindTexture( GL_TEXTURE_2D, depthBuffer_ );
    
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
    
    glTexImage2D( GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0 );
    
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
    glDrawBuffers( nbColorBuffers_, colorbuffers_ );
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
  
  GLuint FrameBuffer::color_buffer( size_t index ) const
  {
    return colorBuffer_[index];
  }
}