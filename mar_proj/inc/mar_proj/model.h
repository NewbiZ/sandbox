#ifndef MAR_MODEL_H
#define MAR_MODEL_H

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
    //float* triangles_;
    //float* quads_;
    //float* vertexBuffer_;
    //float* normals_;
    //
    //int totalTriangles_;
    //int totalQuads_;
    //int totalPoints_;
    //int totalFaces_;
      float* Faces_Triangles;
      float* Faces_Quads;
      float* Vertex_Buffer;
      float* Normals;
      int TotalConnectedTriangles;	
      int TotalConnectedQuads;	
      int TotalConnectedPoints;
      int TotalFaces;
  };

} // namespace mar

#endif // MAR_MODEL_H
