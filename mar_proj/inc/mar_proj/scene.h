#ifndef MAR_SCENE_H
#define MAR_SCENE_H

#include "mar_proj/model.h"
#include "mar_proj/shader.h"
#include "mar_proj/framebuffer.h"
#include "mar_proj/resource_manager.h"

namespace mar
{
  class Scene
  {
  public:
    explicit Scene( float& radius, float& precision, int& nbrays, int& power, int& blend, int& blur );
    ~Scene();
    
  public:
    void setup( unsigned int width, unsigned int height );
    
  public:
    void load_model( const std::string& filename );
    
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
    Shader      shaderPPP_;
    Shader      shaderSSAO_;
    Shader      shaderBlend_;
    FrameBuffer framebuffer1_;
    FrameBuffer framebuffer2_;
    FrameBuffer framebuffer3_;
    
    float width_;
    float height_;
    
    float& pass2Radius_;
    float& pass2Precision_;
    int&   pass2Nbrays_;
    int&   pass2Power_;
    int&   pass3Blend_;
    int&   pass3Blur_;
  };
} // namespace mar

#endif // MAR_SCENE_H
