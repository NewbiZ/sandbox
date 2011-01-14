#ifndef MAR_FRAMEBUFFER_H
#define MAR_FRAMEBUFFER_H

#include <GL/glew.h>
#include <string>

namespace mar
{
  class FrameBuffer
  {
  public:
    explicit FrameBuffer();
    ~FrameBuffer();
  
  public:
    void setup();
    
  public:
    void enable () const;
    void disable() const;
    
  public:
    GLuint depth_buffer() const;
    GLuint color_buffer() const;
    
  //private:
    GLuint fbo_;
    GLuint depthBuffer_;
    GLuint colorBuffer_;
  };
} // namespace mar

#endif // MAR_FRAMEBUFFER_H
