/**
 * \file
 * \brief Declaration of the Model class
 */

#ifndef MODEL_H
#define MODEL_H

#include <string>
#include <vector>
#include <map>
#include <GL/glfw.h>
#include "mar_tp1/resource_manager.h"
#include "mar_tp1/vector.h"

namespace mar
{
  namespace
  {
    // --- Some forwards
    struct OBJObject;
    struct OBJFace;
    struct OBJVertex;
    struct OBJNormal;
    struct OBJTexture;
    struct OBJFaceItem;
    struct OBJMaterial;
    
    struct OBJFace
    {
      std::vector<OBJFaceItem> items;
      std::string              material;
    };
    
    struct OBJFaceItem
    {
      OBJFaceItem()
      {
        vertexIndex  = 0;
        normalIndex  = 0;
        textureIndex = 0;
      }
     
      unsigned int vertexIndex;
      unsigned int normalIndex;
      unsigned int textureIndex;
    };
    
    struct OBJVertex
    {
      OBJVertex()
      {
        coords[0] = 0;
        coords[1] = 0;
        coords[2] = 0;
      }
     
      float coords[3];
    };
    
    struct OBJNormal
    {
      OBJNormal()
      {
        coords[0] = 0;
        coords[1] = 0;
        coords[2] = 0;
      }
     
      float coords[3];
    };
    
    struct OBJTexture
    {
      OBJTexture()
      {
        coords[0] = 0;
        coords[1] = 0;
        coords[2] = 0;
      }
     
      float coords[3];
    };
    
    struct OBJMaterial
    {
      float  Ka[4];
      float  Kd[4];
      float  Ks[4];
      
      GLuint texture;
      
      float texScaleU;
      float texScaleV;
    };
    
    struct OBJObject
    {
      OBJObject()
      {
        vertices.push_back( OBJVertex()  );
        normals.push_back ( OBJNormal()  );
        textures.push_back( OBJTexture() );
        computedNormals.push_back( OBJNormal() );
      }
     
      std::vector<OBJVertex>            vertices;
      std::vector<OBJNormal>            computedNormals;
      std::vector<OBJNormal>            normals;
      std::vector<OBJTexture>           textures;
      std::vector<OBJFace>              faces;
      std::map<std::string,OBJMaterial> materials;
    };
  }
  
  /**
   * \brief This class handles a 3D model. Only the OBJ file format is supported.
   */
  class Model
  {
  public:
    /**
     * \brief Create a new, empty model.
     */
    Model();
    
    /**
     * \brief Destructs a model.
     */
    ~Model();
  
  public:
    /**
     * \brief Load an OBJ model.
     * \param data_path         Path from where to look for textures (when processing MTL)
     * \param mesh_filename     Path to the OBJ file
     * \param material_filename Path to the MTL file
     * \return true if the model was loaded successfully, false otherwise
     */
    bool load( const std::string& data_path, const std::string& mesh_filename, const std::string& material_filename="" );
    
    /**
     * \brief Render the model
     * \param withBoundingBox true if the bounding box should be rendered as well
     */
    void render( bool withBoundingBox=false ) const;
    
  public:
    /**
     * \brief Retrieve minimum vertex of the bounding box.
     */
    const Vector<3>& minimumVertex() const;
    
    /**
     * \brief Retrieve maximum vertex of the bounding box.
     */
    const Vector<3>& maximumVertex() const;
    
  protected:
    /**
     * \brief Process a single line of an OBJ file.
     */
    void processMeshLine( const std::string& line );
    
    /**
     * \brief Process a single line of a MTL file.
     */
    void processMaterialLine( const std::string& line );
    
    /**
     * \brief Build the display list for the model.
     */
    void build();
    
    /**
     * \brief Render the bounding box of the model
     * \see render
     */
    void renderBoundingBox() const;
   
  private:
    /**
     * \brief OBJ structure containing the model.
     */
    OBJObject object_;
    
    /**
     * \brief ID of the model display list.
     */
    GLuint listId_;
    
    /**
     * \brief Name of the current material during parsing.
     */
    std::string currentMaterial_;
    
    /**
     * \brief Resource manager containing material textures.
     */
    ResourceManager resourceManager_;
    
    /**
     * \brief Path to the texture files.
     */
    std::string dataPath_;
    
    /**
     * \brief Minimum vertex of the bounding box.
     */
    Vector<3> minimumVertex_;
    
    /**
     * \brief Maximum vertex of the bounding box.
     */
    Vector<3> maximumVertex_;
  };
}

#endif // MODEL_H
