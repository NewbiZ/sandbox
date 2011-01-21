#ifndef MAR_FRAMEBUFFER_H
#define MAR_FRAMEBUFFER_H

#include <GL/glew.h>
#include <string>

namespace mar
{
  class FrameBuffer
  {
  public:
    explicit FrameBuffer( size_t colorbuffers=1 );
    ~FrameBuffer();
  
  public:
    void setup( unsigned int width, unsigned int height );
    
  public:
    void enable () const;
    void disable() const;
    
  public:
    GLuint depth_buffer() const;
    GLuint color_buffer(size_t index=0) const;
    
  //private:
    GLuint  fbo_;
    GLuint  depthBuffer_;
    GLuint* colorBuffer_;
    size_t  nbColorBuffers_;
    
    static GLenum colorbuffers_[GL_MAX_COLOR_ATTACHMENTS_EXT];
  };
} // namespace mar

#endif // MAR_FRAMEBUFFER_H
