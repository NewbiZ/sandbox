#include "mar_proj/model.h"

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include "mar_proj/vector.h"
#include "mar_proj/utils.vector.h"

namespace mar
{
  Model::Model()
    : totalFaces_(0), totalVertices_(0)
  {
  }
  
  Model::Model( const std::string& filename )
    : totalFaces_(0), totalVertices_(0)
  {
    load( filename );
  }
  
  Model::~Model()
  {
    glDeleteBuffers( 1, &vboVertices_ );
    glDeleteBuffers( 1, &vboFaces_    );
  }
  
  void Model::render() const
  {
    glBindBuffer( GL_ARRAY_BUFFER, vboVertices_ );
    glVertexPointer( 3, GL_FLOAT, 6*sizeof(float), (((float*)0)+0) );
    glNormalPointer(    GL_FLOAT, 6*sizeof(float), (((float*)0)+3) );
    
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, vboFaces_ );
    
    glEnableClientState( GL_VERTEX_ARRAY );
    glEnableClientState( GL_NORMAL_ARRAY );

    glDrawElements( GL_TRIANGLES, totalFaces_*3, GL_UNSIGNED_INT, 0 );

    glDisableClientState( GL_VERTEX_ARRAY );
    glDisableClientState( GL_NORMAL_ARRAY );
  }

  void split( const std::string& str, char delim, std::vector<std::string>& tokens )
  {
    std::stringstream iss(str);
    std::string item;
    while ( std::getline(iss, item, delim) )
    {
      if ( !item.empty() )
        tokens.push_back(item);
    }
  }
  
  bool Model::load( const std::string& filename )
  {
    float*        vertices = 0;
    unsigned int* faces    = 0;
    
    std::ifstream file( filename.c_str(), std::ios::in );
    
    if ( !file )
    {
      std::cerr << "Error: Cannot open model file " << filename << "." << std::endl;
      return false;
    }
    
    std::string line;
    
    // Check for the PLY magic number
    std::getline( file, line );
    if ( line.compare("ply")!=0 )
    {
      std::cerr << "Error: Model file " << filename << " does not seem to be of PLY format." << std::endl;
      return false;
    }
    
    // Check for ASCII PLY format
    std::getline( file, line );
    if ( line.find("ascii")==std::string::npos )
    {
      std::cerr << "Error: Model file " << filename << " is in an unsupported PLY format (not ASCII)." << std::endl;
      return false;
    }
    
    // Parse file header
    std::getline( file, line );
    while ( line.compare("end_header")!=0 )
    {
      std::vector<std::string> tokens;
      split( line, ' ', tokens );
      
      if ( tokens[0].compare("element")==0 ) // --- Elements
      {
        if ( tokens[1].compare("vertex")==0 ) // - vertices
        {
          totalVertices_ = strtoul(tokens[2].c_str(),0,0);
        }
        else if ( tokens[1].compare("face")==0 ) // - faces
        {
          totalFaces_ = strtoul(tokens[2].c_str(),0,0);
        }
        else
        {
          // should not fall here
        }
      }
      else if ( tokens[0].compare("property")==0 ) // --- Properties
      {
        // we do not handle properties. We assume that vertices are composed
        // of 3 floats (x,y,z) and faces 3 indices.
      }
      else // --- Comments & unknowns
      {
        // nothing to do, skip them
      }
      
      std::getline( file, line );
    }
    
    vertices = new float[6*totalVertices_];
    faces    = new unsigned int[3*totalFaces_];
    
    // Parse vertices
    for ( int i=0; i<totalVertices_; ++i )
    {
      std::getline( file, line );
      std::vector<std::string> tokens;
      split( line, ' ', tokens );
      
      vertices[6*i+0] = strtod( tokens[0].c_str(), 0 );
      vertices[6*i+1] = strtod( tokens[1].c_str(), 0 );
      vertices[6*i+2] = strtod( tokens[2].c_str(), 0 );
    }
    
    // Parse faces
    for ( int i=0; i<totalFaces_; ++i )
    {
      std::getline( file, line );
      std::vector<std::string> tokens;
      split( line, ' ', tokens );
      
      faces[3*i+0] = strtoul( tokens[1].c_str(), 0, 0 );
      faces[3*i+1] = strtoul( tokens[2].c_str(), 0, 0 );
      faces[3*i+2] = strtoul( tokens[3].c_str(), 0, 0 );
      
      Vec3 a( vertices[faces[3*i+0]*6+0], vertices[faces[3*i+0]*6+1], vertices[faces[3*i+0]*6+2] );
      Vec3 b( vertices[faces[3*i+1]*6+0], vertices[faces[3*i+1]*6+1], vertices[faces[3*i+1]*6+2] );
      Vec3 c( vertices[faces[3*i+2]*6+0], vertices[faces[3*i+2]*6+1], vertices[faces[3*i+2]*6+2] );
      Vec3 normal( cross(b-a, c-a) );
      
      vertices[faces[3*i+0]*6+3] = normal.x;
      vertices[faces[3*i+0]*6+4] = normal.y;
      vertices[faces[3*i+0]*6+5] = normal.z;
      
      vertices[faces[3*i+1]*6+3] = normal.x;
      vertices[faces[3*i+1]*6+4] = normal.y;
      vertices[faces[3*i+1]*6+5] = normal.z;
      
      vertices[faces[3*i+2]*6+3] = normal.x;
      vertices[faces[3*i+2]*6+4] = normal.y;
      vertices[faces[3*i+2]*6+5] = normal.z;
    }
    
    glGenBuffers( 1, &vboVertices_ );
    glGenBuffers( 1, &vboFaces_  );
    
    glBindBuffer( GL_ARRAY_BUFFER, vboVertices_ );
    glBufferData( GL_ARRAY_BUFFER, sizeof(float)*6*totalVertices_, vertices, GL_STATIC_DRAW );
    
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, vboFaces_ );
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*3*totalFaces_, faces, GL_STATIC_DRAW );
    
    delete [] vertices;
    delete [] faces;
  }
}