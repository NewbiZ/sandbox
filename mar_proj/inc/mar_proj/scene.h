#ifndef MAR_SCENE_H
#define MAR_SCENE_H

#include "mar_proj/model.h"
#include "mar_proj/shader.h"
#include "mar_proj/framebuffer.h"
#include <GLConsole/GLConsole.h>

namespace mar
{
  
  class Scene
  {
  public:
    explicit Scene();
    ~Scene();
    
  public:
    void setup( unsigned int width, unsigned int height );
    
  public:
    bool console_is_open() const;
    void toggle_console ();
    void send_keyboard  ( int key );
    void send_special   ( int key );
    
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
    Model             model_;
    float             rotation_;
    float             distance_;
    Shader            shaderPPP_;
    Shader            shaderSSAO_;
    Shader            shaderBlend_;
    FrameBuffer       framebuffer1_;
    FrameBuffer       framebuffer2_;
    FrameBuffer       framebuffer3_;
    mutable GLConsole console_;
    
    // --- SSAO control
    float& ssaoWidth_;
    float& ssaoHeight_;
    float& ssaoRadius_;
    float& ssaoPrecision_;
    float& ssaoNbRays_;
    
    // --- Blend control
    int& blendApply_;
    int& blurApply_;
  };
  
} // namespace mar

#endif // MAR_SCENE_H
