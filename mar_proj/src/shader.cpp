#include "mar_proj/shader.h"

#include <iostream>
#include <fstream>

namespace mar
{
  Shader::Shader()
    : vertex_(0), fragment_(0), program_(0)
  {
  }
  
  Shader::Shader( const std::string& vertexFilename, const std::string& fragmentFilename )
    : vertex_(0), fragment_(0), program_(0)
  {
    load( vertexFilename, fragmentFilename );
  }
  
  Shader::~Shader()
  {
    disable();
    cleanup();
  }
  
  GLuint Shader::uniformLocation( const std::string& uniform ) const
  {
    return glGetUniformLocation( program_, uniform.c_str() );
  }
  
  void Shader::cleanup()
  {
    if ( !program_ )
      return; // nothing to do
    
    if ( vertex_ )
    {
      glDetachShader( program_, vertex_ );
      glDeleteShader( vertex_ );
    }
    
    if ( vertex_ )
    {
      glDetachShader( program_, fragment_ );
      glDeleteShader( fragment_ );
    }
    
    if ( program_ )
      glDeleteProgram( program_ );
  }
  
  static bool stringFromFile( const std::string& filename, std::string& str )
  {
    std::ifstream file( filename.c_str() );
    
    if ( !file )
    {
      std::cerr << "Error: cannot open file \"" << filename << "\"." << std::endl;
      return false;
    }
    
    std::string line;
    while ( std::getline(file,line) )
      str += line;
    
    return true;
  }
  
  bool Shader::load( const std::string& vertexFilename, const std::string& fragmentFilename )
  {
    vertex_   = glCreateShader( GL_VERTEX_SHADER   );
    fragment_ = glCreateShader( GL_FRAGMENT_SHADER );
    program_  = glCreateProgram();
    
    std::string vertexSource;
    std::string fragmentSource;
    
    bool result = true;
    
    result &= stringFromFile( vertexFilename,   vertexSource   );
    result &= stringFromFile( fragmentFilename, fragmentSource );
    
    // Sanity check
    if ( !result )
    {
      std::cerr << "Error: cannot load (one or both) shader source files \""
                << vertexFilename << "\", \"" << fragmentFilename << "\"" << std::endl;
      return false;
    }
    
    const char* vertexSourceArray[]   = { vertexSource.c_str() };
    const char* fragmentSourceArray[] = { fragmentSource.c_str() };
    
    glShaderSource( vertex_,   1, vertexSourceArray,   0 );
    glShaderSource( fragment_, 1, fragmentSourceArray, 0 );

    glCompileShader( vertex_   );
    glCompileShader( fragment_ );
    
    glAttachShader( program_, vertex_   );
    glAttachShader( program_, fragment_ );

    glLinkProgram( program_ );
    
    // Sanity check
    int vertexStatus   = GL_TRUE;
    int fragmentStatus = GL_TRUE;
    int linkStatus     = GL_TRUE;

    glGetShaderiv ( vertex_,   GL_COMPILE_STATUS, &vertexStatus   );
    glGetShaderiv ( fragment_, GL_COMPILE_STATUS, &fragmentStatus );
    glGetProgramiv( program_,  GL_LINK_STATUS,    &linkStatus     );

    int   vertexInfoLength    = 0;
    int   fragmentInfoLength  = 0;
    int   programInfoLength   = 0;

    int   vertexInfoWritten   = 0;
    int   fragmentInfoWritten = 0;
    int   programInfoWritten  = 0;

    char* vertexInfoLog   = "<empty>";
    char* fragmentInfoLog = "<empty>";
    char* programInfoLog  = "<empty>";

    glGetShaderiv( vertex_, GL_INFO_LOG_LENGTH, &vertexInfoLength );
    if ( vertexInfoLength>0 )
    {
      vertexInfoLog = new char[vertexInfoLength];
      glGetShaderInfoLog( vertex_, vertexInfoLength, &vertexInfoWritten, vertexInfoLog );
    }

    glGetShaderiv( fragment_, GL_INFO_LOG_LENGTH, &fragmentInfoLength );
    if ( fragmentInfoLength>0 )
    {
      fragmentInfoLog = new char[fragmentInfoLength];
      glGetShaderInfoLog( fragment_, fragmentInfoLength, &fragmentInfoWritten, fragmentInfoLog );
    }
    
    glGetProgramiv( program_, GL_INFO_LOG_LENGTH, &programInfoLength );
    if ( programInfoLength>0 )
    {
      programInfoLog = new char[programInfoLength];
      glGetProgramInfoLog( program_, programInfoLength, &programInfoWritten, programInfoLog );
    }
    
    bool resultStatus = true;
    
    if ( !vertexStatus )
    {
      std::cerr << "Error while building vertex shader." << std::endl;
      std::cerr << "Driver shader compiler output: " << std::endl;
      std::cerr << vertexInfoLog << std::endl;
      cleanup(); // detach & delete shaders
      resultStatus = false;
    }
    
    if ( !fragmentStatus )
    {
      std::cerr << "Error while building fragment shader." << std::endl;
      std::cerr << "Driver shader compiler output: " << std::endl;
      std::cerr << fragmentInfoLog << std::endl;
      cleanup(); // detach & delete shaders
      resultStatus = false;
    }
    
    if ( !linkStatus )
    {
      std::cerr << "Error while linking shader program." << std::endl;
      std::cerr << "Driver shader program linker output: " << std::endl;
      std::cerr << programInfoLog << std::endl;
      cleanup(); // detach & delete shaders
      resultStatus = false;
    }
    
    if ( vertexInfoLength>0 )
      delete [] vertexInfoLog;
    if ( fragmentInfoLength>0 )
      delete [] fragmentInfoLog;
    if ( programInfoLength>0)
      delete [] programInfoLog;
    
    return resultStatus;
  }
  
  void Shader::enable() const
  {
    glUseProgram( program_ );
  }
  
  void Shader::disable() const
  {
    glUseProgram( 0 );
  }
}