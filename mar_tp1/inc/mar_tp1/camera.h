/**
 * \file
 * \brief Declaration of the Camera class
 */

#ifndef CAMERA_H
#define CAMERA_H

#include "mar_tp1/vector.h"
#include "mar_tp1/resource_manager.h"

namespace mar
{
  /**
   * \brief This class handles a camera and its associated skybox.
   */
  class Camera
  {
  public:
    /**
     * \brief Initialize a new camera with a position and an angle.
     */
    Camera( Vector<3> position, float angle );
    
  public: // Movement & viewing
    /**
     * \brief Move the camera forward.
     */
    void forward( float distance );
    
    /**
     * \brief Move the camera backward.
     */
    void backward( float distance );
    
    /**
     * \brief Rotate the camera around the Y-axis.
     */
    void rotate( float angle );
    
  public:
    /**
     * \brief Retrieve the current position of the camera.
     */
    const Vector<3>& position() const;
    
    /**
     * \brief Retrieve the current angle (around Y-axis) of the camera.
     */
    float angle() const;
    
  public: // Skybox
    /**
     * \brief Load skybox textures.
     * \param top   the top texture
     * \param front the front texture
     * \param back  the back texture
     * \param left  the left texture
     * \param right the right texture
     */
    void loadSkybox( const std::string& top,
                     const std::string& front,
                     const std::string& back,
                     const std::string& left,
                     const std::string& right );
    
  public:
    /**
     * \brief Transform and render the camera.
     */
    void transform_and_render() const;
    
    /**
     * \brief Render the field of view of the camera.
     */
    void render_fov() const;
    
  private:
    /**
     * \brief Render the skybox of the camera.
     */
    void renderSkybox() const;
    
  private:
    /**
     * \brief Current position of the camera.
     */
    Vector<3> position_;
    
    /**
     * \brief Current angle of the camera (around the Y-axis).
     */
    float angle_;
    
    /**
     * \brief Resource manager containing the textures.
     */
    ResourceManager resourceManager_;
    
    /**
     * \brief Texture of the skybox top plane.
     */
    GLuint sky_top;
    
    /**
     * \brief Texture of the skybox top plane.
     */
    GLuint sky_front;
    
    /**
     * \brief Texture of the skybox back plane.
     */
    GLuint sky_back;
    
    /**
     * \brief Texture of the skybox left plane.
     */
    GLuint sky_left;
    
    /**
     * \brief Texture of the skybox right plane.
     */
    GLuint sky_right;
    
    /**
     * \brief Display list ID of the skybox.
     */
    
    GLuint sky_geometry;
  };
}

#endif // CAMERA_H
