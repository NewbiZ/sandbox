#ifndef MAR_MODEL_H
#define MAR_MODEL_H

#include <GL/glew.h>
#include <string>

namespace mar
{
  class Model
  {
  public:
    explicit Model();
    explicit Model( const std::string& filename );
    ~Model();
    
  public:
    bool load( const std::string& filename );
    
  public:
    void render() const;
    
  private:
    unsigned int totalVertices_;
    unsigned int totalFaces_;
    
    GLuint vboVertices_;
    GLuint vboFaces_;
  };

} // namespace mar

#endif // MAR_MODEL_H
