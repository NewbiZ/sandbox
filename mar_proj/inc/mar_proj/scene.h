#ifndef MAR_SCENE_H
#define MAR_SCENE_H

#include "mar_proj/model.h"
#include "mar_proj/shader.h"
#include "mar_proj/framebuffer.h"

namespace mar
{
  
  class Scene
  {
  public:
    explicit Scene();
    ~Scene();
    
  public:
    void setup();
    
  public:
    void turn_left ();
    void turn_right();
    
    void move_forward ();
    void move_backward();
    
  public:
    void render() const;
    
  protected:
    void setup_opengl();
    void setup_models();
    void setup_view  ();
    
  private:
    Model       model_;
    float       rotation_;
    float       distance_;
    Shader      shaderNormal_;
    Shader      shaderSSAO_;
    FrameBuffer framebuffer1_;
    FrameBuffer framebuffer2_;
  };
  
} // namespace mar

#endif // MAR_SCENE_H
