#include "mar_tp1/model.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <cmath>
#include <limits>

// Comment this line to use OBJ-exported normals
// Default behavior is to compute & average normals manually, discarding the exported ones,
// since most of the time they are no more than garbage.
#define USE_COMPUTED_NORMALS 1

namespace
{
  void chop( std::string& str )
  {
    std::string whitespaces (" \t\f\v\n\r");
    size_t found = str.find_last_not_of( whitespaces );
    if ( found!=std::string::npos )
      str.erase( found+1 );
    else
      str.clear();
  }
  
  void split( const std::string& str, char delim, std::vector<std::string>& tokens )
  {
    std::stringstream iss(str);
    std::string item;
    while ( std::getline(iss, item, delim) )
    {
      if ( item!=" " && !item.empty() )
      {
        chop(item);
        tokens.push_back(item);
      }
    }
  }
}

namespace mar
{
  Model::Model()
  {
    listId_ = 0;
  }
  
  Model::~Model()
  {
    if (listId_ != 0)
      glDeleteLists( listId_, 1 );
  }
  
  bool Model::load( const std::string& data_path, const std::string& mesh_filename, const std::string& material_filename )
  {
    dataPath_ = data_path;
    
    std::ifstream file_mesh;
    std::ifstream file_material;
    
    file_mesh.open( (data_path+mesh_filename).c_str() );
    if ( !material_filename.empty() )
      file_material.open( (data_path+material_filename).c_str() );
   
    if ( !file_mesh.good() || (!material_filename.empty() && !file_material.good()) )
      return false;
   
    minimumVertex_.x = std::numeric_limits<float>::max();
    minimumVertex_.y = std::numeric_limits<float>::max();
    minimumVertex_.z = std::numeric_limits<float>::max();
    
    maximumVertex_.x = std::numeric_limits<float>::min();
    maximumVertex_.y = std::numeric_limits<float>::min();
    maximumVertex_.z = std::numeric_limits<float>::min();
   
    std::string line;
    
    OBJMaterial& defaultMaterial = object_.materials[""];
    defaultMaterial.Ka[0] = 1;
    defaultMaterial.Ka[1] = 1;
    defaultMaterial.Ka[2] = 1;
    defaultMaterial.Ka[3] = 1;
    
    defaultMaterial.Kd[0] = 1;
    defaultMaterial.Kd[1] = 1;
    defaultMaterial.Kd[2] = 1;
    defaultMaterial.Kd[3] = 1;
    
    defaultMaterial.Ks[0] = 1;
    defaultMaterial.Ks[1] = 1;
    defaultMaterial.Ks[2] = 1;
    defaultMaterial.Ks[3] = 1;
    
    defaultMaterial.texture = 0;
    
    defaultMaterial.texScaleU = 1;
    defaultMaterial.texScaleV = 1;
    
    if ( !material_filename.empty() )
    {
      while ( !file_material.eof() )
      {
        std::getline( file_material, line );
        processMaterialLine( line );
      }
      file_material.close();
    }
    
    while ( !file_mesh.eof() )
    {
      std::getline( file_mesh, line );
      processMeshLine( line );
    }
    file_mesh.close();
    
    glEnable( GL_TEXTURE_2D );
    listId_ = glGenLists(1);
    glNewList( listId_, GL_COMPILE );
   
    glColor4f( 1.0f, 1.0f, 1.0f, 1.0f );
  
    build();
  
    glEndList();
    glDisable( GL_TEXTURE_2D );
    
    return true;
  }
  
  void Model::processMaterialLine( const std::string& line )
  {
    if ( line.find("#")==0 ) // A comment
    {
      return;
    }
    else if ( line.find("newmtl ")==0 ) // A new material
    {
      std::vector<std::string> tokens;
      split( line, ' ', tokens );
      
      if ( tokens.size()!=2 )
        return;
      
      currentMaterial_ = tokens[1];
    }
    else if ( line.find("Ka ")==0 ) // Ambient color
    {
      std::vector<std::string> tokens;
      split( line, ' ', tokens );
      
      if ( tokens.size()!=4 )
        return;
      
      OBJMaterial& material = object_.materials[currentMaterial_];
      
      material.Ka[0] = std::strtod(tokens.at(1).c_str(), 0);
      material.Ka[1] = std::strtod(tokens.at(2).c_str(), 0);
      material.Ka[2] = std::strtod(tokens.at(3).c_str(), 0);
    }
    else if ( line.find("Kd ")==0 ) // Diffuse color
    {
      std::vector<std::string> tokens;
      split( line, ' ', tokens );
      
      if ( tokens.size()!=4 )
        return;
      
      OBJMaterial& material = object_.materials[currentMaterial_];
      
      material.Kd[0] = std::strtod(tokens.at(1).c_str(), 0);
      material.Kd[1] = std::strtod(tokens.at(2).c_str(), 0);
      material.Kd[2] = std::strtod(tokens.at(3).c_str(), 0);
    }
    else if ( line.find("Ks ")==0 ) // Specular color
    {
      std::vector<std::string> tokens;
      split( line, ' ', tokens );
      
      if ( tokens.size()!=4 )
        return;
      
      OBJMaterial& material = object_.materials[currentMaterial_];
      
      material.Ks[0] = std::strtod(tokens.at(1).c_str(), 0);
      material.Ks[1] = std::strtod(tokens.at(2).c_str(), 0);
      material.Ks[2] = std::strtod(tokens.at(3).c_str(), 0);
    }
    else if ( line.find("map_Kd ")==0 ) // Texture mapping
    {
      std::vector<std::string> tokens;
      split( line, ' ', tokens );
      
      OBJMaterial& material = object_.materials[currentMaterial_];
      
      if ( tokens.size()==2 )
      {
        resourceManager_.load_texture( dataPath_+tokens[1] );
        material.texture   = resourceManager_.get_texture( dataPath_+tokens[1] );
        material.texScaleU = 1;
        material.texScaleV = 1;
      }
      else if ( tokens.size()==6)
      {
        resourceManager_.load_texture( dataPath_+tokens[5] );
        material.texture = resourceManager_.get_texture( dataPath_+tokens[5] );
        material.texScaleU = std::strtod(tokens.at(2).c_str(), 0);
        material.texScaleV = std::strtod(tokens.at(3).c_str(), 0);
      }
      else
      {
        return;
      }
    }
    else // Whatever
    {
      return;
    }
  }
  
  void Model::processMeshLine( const std::string& line )
  {
    if ( line.find("#")==0 ) // A comment
    {
      return;
    }
    else if ( line.find("v ")==0 ) // A vertex
    {
      std::vector<std::string> tokens;
      split( line, ' ', tokens );
     
      if ( tokens.size()!=4 ) return;
     
      OBJVertex vertex;
      vertex.coords[0] = strtod(tokens.at(1).c_str(), 0);
      vertex.coords[1] = strtod(tokens.at(2).c_str(), 0);
      vertex.coords[2] = strtod(tokens.at(3).c_str(), 0);
     
      object_.vertices.push_back(vertex);
      object_.computedNormals.push_back( OBJNormal() );
      
      // Remember the min/max components for the bounding box
      if ( vertex.coords[0]<minimumVertex_.x )
        minimumVertex_.x = vertex.coords[0];
      if ( vertex.coords[1]<minimumVertex_.y )
        minimumVertex_.y = vertex.coords[1];
      if ( vertex.coords[2]<minimumVertex_.z )
        minimumVertex_.z = vertex.coords[2];
      
      if ( vertex.coords[0]>maximumVertex_.x )
        maximumVertex_.x = vertex.coords[0];
      if ( vertex.coords[1]>maximumVertex_.y )
        maximumVertex_.y = vertex.coords[1];
      if ( vertex.coords[2]>maximumVertex_.z )
        maximumVertex_.z = vertex.coords[2];
    }
    else if ( line.find("usemtl ")==0 ) // A material usage
    {
      std::vector<std::string> tokens;
      split( line, ' ', tokens );
     
      if ( tokens.size()!=2 ) return;
     
      currentMaterial_ = tokens[1];
    }
    else if ( line.find("f ")==0 ) // A face
    {
      std::vector<std::string> tokens;
      split( line, ' ', tokens );
     
      OBJFace face;
     
      for ( int i=1; i<tokens.size(); ++i ) // Each item of the face
      {
        OBJFaceItem item;
       
        item.vertexIndex  = 0;
        item.normalIndex  = 0;
        item.textureIndex = 0;
       
        std::vector<std::string> items;
        split( tokens.at(i), '/', items );
        
        switch (items.size())
        {
          case 1:
          {
            if (!items.at(0).empty()) item.vertexIndex  = std::strtol(items.at(0).c_str(), 0, 10);
            break;
          }
          case 2:
          {
            if (!items.at(0).empty()) item.vertexIndex  = std::strtol(items.at(0).c_str(), 0, 10);
            if (!items.at(1).empty()) item.textureIndex = std::strtol(items.at(1).c_str(), 0, 10);
            break;
          }
          case 3:
          {
            if (!items.at(0).empty()) item.vertexIndex  = std::strtol(items.at(0).c_str(), 0, 10);
            if (!items.at(1).empty()) item.textureIndex = std::strtol(items.at(1).c_str(), 0, 10);
            if (!items.at(2).empty()) item.normalIndex  = std::strtol(items.at(2).c_str(), 0, 10);
            break;
          }
        }
        
        face.items.push_back( item );
      }
      
      face.material = currentMaterial_;
      
      object_.faces.push_back(face);

#ifdef USE_COMPUTED_NORMALS
      OBJVertex a = object_.vertices.at(face.items.at(0).vertexIndex);
      OBJVertex b = object_.vertices.at(face.items.at(1).vertexIndex);
      OBJVertex c = object_.vertices.at(face.items.at(2).vertexIndex);
      float x = (b.coords[1]-a.coords[1])*(c.coords[2]-a.coords[2])-(b.coords[2]-a.coords[2])*(c.coords[1]-a.coords[1]);
      float y = (b.coords[2]-a.coords[2])*(c.coords[0]-a.coords[0])-(b.coords[0]-a.coords[0])*(c.coords[2]-a.coords[2]);
      float z = (b.coords[0]-a.coords[0])*(c.coords[1]-a.coords[1])-(b.coords[1]-a.coords[1])*(c.coords[0]-a.coords[0]);
      
      float length = std::sqrt( x*x + y*y + z*z );
      
      x /= length;
      y /= length;
      z /= length;
     
      for (int i=0; i<face.items.size(); ++i)
      {
        if ( object_.computedNormals.at(face.items.at(i).vertexIndex).coords[0]==0 &&
             object_.computedNormals.at(face.items.at(i).vertexIndex).coords[1]==0 &&
             object_.computedNormals.at(face.items.at(i).vertexIndex).coords[2]==0  )
        {
          object_.computedNormals.at(face.items.at(i).vertexIndex).coords[0] =  x;
          object_.computedNormals.at(face.items.at(i).vertexIndex).coords[1] =  y;
          object_.computedNormals.at(face.items.at(i).vertexIndex).coords[2] =  z;
        }
        else
        {
          object_.computedNormals.at(face.items.at(i).vertexIndex).coords[0] =  object_.computedNormals.at(face.items.at(i).vertexIndex).coords[0]+x;
          object_.computedNormals.at(face.items.at(i).vertexIndex).coords[1] =  object_.computedNormals.at(face.items.at(i).vertexIndex).coords[1]+y;
          object_.computedNormals.at(face.items.at(i).vertexIndex).coords[2] =  object_.computedNormals.at(face.items.at(i).vertexIndex).coords[2]+z;
        }
      }
#endif
    }
    else if ( line.find("vn ")==0 ) // A normal
    {
#ifndef USE_COMPUTED_NORMALS
      std::vector<std::string> tokens;
      split( line, ' ', tokens );
     
      if ( tokens.size()!=4 ) return;
     
      OBJNormal normal;
      normal.coords[0] = std::strtod(tokens.at(1).c_str(), 0);
      normal.coords[1] = std::strtod(tokens.at(2).c_str(), 0);
      normal.coords[2] = std::strtod(tokens.at(3).c_str(), 0);
     
      object_.normals.push_back(normal);
#endif
    }
    else if ( line.find("vt ")==0 ) // A texture coordinate
    {
      std::vector<std::string> tokens;
      split( line, ' ', tokens );
     
      if ( tokens.size()!=4 ) return;
     
      OBJTexture texture;
      texture.coords[0] = std::strtod(tokens.at(1).c_str(), 0 );
      texture.coords[1] = std::strtod(tokens.at(2).c_str(), 0 );
      texture.coords[2] = std::strtod(tokens.at(3).c_str(), 0 );
     
      object_.textures.push_back(texture);
    }
    else // Whatever
    {
      return;
    }
  }
  
  void Model::build()
  {
    glEnable( GL_NORMALIZE );
    glEnable( GL_TEXTURE_2D );
    
    static float ambient [4];
    static float diffuse [4];
    static float specular[4];

    for ( int i=0; i<object_.faces.size(); ++i ) // Each face
    {
      OBJFace& face = object_.faces.at(i);
      OBJMaterial& material = object_.materials[face.material];
      
      //glMaterialfv( GL_FRONT, GL_AMBIENT,  material.Ka );
      //glMaterialfv( GL_FRONT, GL_DIFFUSE,  material.Kd );
      //glMaterialfv( GL_FRONT, GL_SPECULAR, material.Ks );
      
      glBindTexture( GL_TEXTURE_2D, material.texture );
      
      glBegin( GL_POLYGON );
      for ( int j=0; j<object_.faces.at(i).items.size(); ++j ) // Each vertex
      {
        OBJFaceItem& item = object_.faces.at(i).items.at(j);
       
#ifdef USE_COMPUTED_NORMALS
        glNormal3fv( object_.computedNormals.at(item.vertexIndex).coords );
#else
        glNormal3fv( object_.normals.at(item.vertexIndex).coords );
#endif
        
        if ( item.textureIndex > 0 ) // Texture coordinates are provided
          glTexCoord2f( object_.textures.at(item.textureIndex).coords[0]*material.texScaleU,
                        object_.textures.at(item.textureIndex).coords[1]*material.texScaleV );
       
        glVertex3fv( object_.vertices.at(item.vertexIndex).coords );
      }
     
      glEnd();
    }
  
    // UNCOMMENT TO DISPLAY MESH NORMALS
    /*
    
    for ( int i=1; i<object_.vertices.size(); ++i ) // Each face
    {
      glBegin( GL_LINES );
        glColor3f( 0.0, 1.0, 0.0 );
        
        glVertex3f( object_.vertices.at(i).coords[0],
                    object_.vertices.at(i).coords[1],
                    object_.vertices.at(i).coords[2] );
#ifdef USE_COMPUTED_NORMALS
        glVertex3f( object_.vertices.at(i).coords[0]+object_.computedNormals.at(i).coords[0]*5.0,
                    object_.vertices.at(i).coords[1]+object_.computedNormals.at(i).coords[1]*5.0,
                    object_.vertices.at(i).coords[2]+object_.computedNormals.at(i).coords[2]*5.0 );
#else
          glVertex3f( object_.vertices.at(i).coords[0]+object_.normals.at(i).coords[0]*5.0,
                      object_.vertices.at(i).coords[1]+object_.normals.at(i).coords[1]*5.0,
                      object_.vertices.at(i).coords[2]+object_.normals.at(i).coords[2]*5.0 );
#endif
        glColor3f( 1.0, 1.0, 1.0 );
    
      glEnd();
    
      glPointSize(5);
      glBegin( GL_POINTS );
#ifdef USE_COMPUTED_NORMALS
        glVertex3f( object_.vertices.at(i).coords[0]+object_.computedNormals.at(i).coords[0]*5.0,
                    object_.vertices.at(i).coords[1]+object_.computedNormals.at(i).coords[1]*5.0,
                    object_.vertices.at(i).coords[2]+object_.computedNormals.at(i).coords[2]*5.0 );
#else
        glVertex3f( object_.vertices.at(i).coords[0]+object_.normals.at(i).coords[0]*5.0,
                    object_.vertices.at(i).coords[1]+object_.normals.at(i).coords[1]*5.0,
                    object_.vertices.at(i).coords[2]+object_.normals.at(i).coords[2]*5.0 );
#endif
      glEnd();
    }
    */
    glDisable( GL_NORMALIZE );
  }
  
  void Model::render( bool withBoundingBox ) const
  {
    if ( listId_ != 0 )
      glCallList( listId_ );
    
    if ( withBoundingBox )
      renderBoundingBox();
  }
  
  void Model::renderBoundingBox() const
  {
    glDisable( GL_TEXTURE_2D );
    
    glColor4f( 1, 0, 0, 1 );
    static const float red[]   = {1,0,0,1};
    static const float black[] = {0,0,0,1};
    glMaterialfv( GL_FRONT, GL_EMISSION, red );
    
    glLineWidth( 1 );
    glBegin( GL_LINES );
      // Back lines
      glVertex3f( minimumVertex_.x, minimumVertex_.y, minimumVertex_.z );
      glVertex3f( minimumVertex_.x, maximumVertex_.y, minimumVertex_.z );
      
      glVertex3f( minimumVertex_.x, maximumVertex_.y, minimumVertex_.z );
      glVertex3f( maximumVertex_.x, maximumVertex_.y, minimumVertex_.z );
      
      glVertex3f( maximumVertex_.x, maximumVertex_.y, minimumVertex_.z );
      glVertex3f( maximumVertex_.x, minimumVertex_.y, minimumVertex_.z );
      
      glVertex3f( maximumVertex_.x, minimumVertex_.y, minimumVertex_.z );
      glVertex3f( minimumVertex_.x, minimumVertex_.y, minimumVertex_.z );
    
      // Front lines
      glVertex3f( minimumVertex_.x, minimumVertex_.y, maximumVertex_.z );
      glVertex3f( maximumVertex_.x, minimumVertex_.y, maximumVertex_.z );
      
      glVertex3f( maximumVertex_.x, minimumVertex_.y, maximumVertex_.z );
      glVertex3f( maximumVertex_.x, maximumVertex_.y, maximumVertex_.z );
      
      glVertex3f( maximumVertex_.x, maximumVertex_.y, maximumVertex_.z );
      glVertex3f( minimumVertex_.x, maximumVertex_.y, maximumVertex_.z );
      
      glVertex3f( minimumVertex_.x, maximumVertex_.y, maximumVertex_.z );
      glVertex3f( minimumVertex_.x, minimumVertex_.y, maximumVertex_.z );
      
      // Top lines
      glVertex3f( minimumVertex_.x, maximumVertex_.y, maximumVertex_.z );
      glVertex3f( minimumVertex_.x, maximumVertex_.y, minimumVertex_.z );
      
      glVertex3f( maximumVertex_.x, maximumVertex_.y, maximumVertex_.z );
      glVertex3f( maximumVertex_.x, maximumVertex_.y, minimumVertex_.z );
      
      // Bottom lines
      glVertex3f( minimumVertex_.x, minimumVertex_.y, maximumVertex_.z );
      glVertex3f( minimumVertex_.x, minimumVertex_.y, minimumVertex_.z );
      
      glVertex3f( maximumVertex_.x, minimumVertex_.y, maximumVertex_.z );
      glVertex3f( maximumVertex_.x, minimumVertex_.y, minimumVertex_.z );
    glEnd();
    
    glColor4f( 1, 1, 1, 1 );
    
    glMaterialfv( GL_FRONT, GL_EMISSION, black );
    
    glEnable( GL_TEXTURE_2D );
  }
  
  const Vector<3>& Model::minimumVertex() const
  {
    return minimumVertex_;
  }
  
  const Vector<3>& Model::maximumVertex() const
  {
    return maximumVertex_;
  }
}
