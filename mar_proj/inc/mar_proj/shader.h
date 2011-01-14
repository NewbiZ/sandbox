#ifndef MAR_SHADER_H
#define MAR_SHADER_H

#include <GL/glew.h>
#include <string>

namespace mar
{
  class Shader
  {
  public:
    explicit Shader();
    explicit Shader( const std::string& vertexFilename, const std::string& fragmentFilename );
    ~Shader();
    
  public:
    bool load( const std::string& vertexFilename, const std::string& fragmentFilename );
    
  public:
    GLuint uniformLocation( const std::string& uniform ) const;
    
  public:
    void enable () const;
    void disable() const;
    
  protected:
    void cleanup();
    
  public:
    GLuint vertex_;
    GLuint fragment_;
    GLuint program_;
  };
} // namespace mar

#endif // MAR_SHADER_H
