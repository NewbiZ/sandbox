#include "mar_proj/tga_loader.h"

#include <fstream>
#include <iostream>
#include <string>

namespace mar
{
  TGALoader::TGALoader()
  : newFormat( false ),
    data(0)
  {
  }
  
  TGALoader::~TGALoader()
  {
    delete [] data;
  }
  
  void TGALoader::load( const std::string& filename )
  {
    // --- Opening file ---
    filename_ = filename;
    std::ifstream in( filename.c_str(), std::ios::in | std::ios::binary );
    unsigned long  fileSize = 0;
    
    if ( in.bad() || !in.good() || !in.is_open() )
    {
      std::cerr << "Error opening file." << std::endl;
      std::cerr << "  bad()     = " << in.bad()     << std::endl;
      std::cerr << "  good()    = " << in.good()    << std::endl;
      std::cerr << "  is_open() = " << in.is_open() << std::endl;
      return;
    }
  
    // --- File size ---
    in.seekg( 0, std::ios::end );
    fileSize = in.tellg();
    in.seekg( 0, std::ios::beg );
  
    // --- Header loading ---
    unsigned char descriptor;
  
    in.read( (char*)&image_id_length,      1 );
    in.read( (char*)&color_map_type,       1 );
    in.read( (char*)&image_type,           1 );
    in.read( (char*)&first_entry_index,    2 );
    in.read( (char*)&color_map_length,     2 );
    in.read( (char*)&color_map_entry_size, 1 );
    in.read( (char*)&x_origin,             2 );
    in.read( (char*)&y_origin,             2 );
    in.read( (char*)&width,                2 );
    in.read( (char*)&height,               2 );
    in.read( (char*)&pixel_depth,          1 );
    in.read( (char*)&descriptor,           1 );
  
    alpha_channel_bits = descriptor & 0x0F;
  
    unsigned int origin = (descriptor & 0x30) >> 4;
    switch ( origin )
    {
      case 0: imageOrigin = BOTTOM_LEFT;  break;
      case 1: imageOrigin = BOTTOM_RIGHT; break;
      case 2: imageOrigin = TOP_LEFT;     break;
      case 3: imageOrigin = TOP_RIGHT;    break;
    }
  
    // Texture is not power of two
    if ( width & (width-1) || height & (height-1) )
    {
      return;
    }
  
    // --- Image ID loading ---
  
    // --- Color Map Data loading ---
  
    // --- Image Data loading ---
    unsigned char bpp = pixel_depth/8;
    unsigned int dataSize = width * height * bpp;
  
    // Allocate texels memory
    data = new unsigned char[dataSize];
  
    // Read all texels
    for ( unsigned int i=0; i<dataSize; ++i )
      in.read( (char*)(data+i), 1 );
    
    // Swap red and blue
    for ( unsigned int i=0; i<dataSize; i+=bpp )
    {
      unsigned char tmp = data[i];
      data[i] = data[i+2];
      data[i+2] = tmp;
    }
  
    // --- Footer loading ---
    in.seekg( -26, std::ios::end );
  
    in.read( (char*)&extension_offset,  4 );
    in.read( (char*)&developer_offset,  4 );
    in.read( (char*)&signature,        18 );
  
    // --- General informations ---
    std::string signature( (char*)&signature, (char*)&signature+16 );
    newFormat = signature.compare("TRUEVISION-XFILE") == 0;
  }
  
  void TGALoader::display()
  {
    std::cout << std::endl;
    std::cout << "INFORMATIONS" << std::endl;
    std::cout << "  filename:             " << filename_ << std::endl;
  
    std::cout << std::endl;
    std::cout << "HEADER" << std::endl;
    std::cout << "  image id length:      " << (unsigned int)image_id_length << std::endl;
    std::cout << "  color map type:       " << (unsigned int)color_map_type  << std::endl;
    std::cout << "  image type:           " << (unsigned int)image_type      << " ";
  
    switch ( image_type )
    {
      case  0: std::cout << "( No Image Data Included )"                    << std::endl; break;
      case  1: std::cout << "( Uncompressed, Color-mapped Image )"          << std::endl; break;
      case  2: std::cout << "( Uncompressed, True-color Image )"            << std::endl; break;
      case  3: std::cout << "( Uncompressed, Black-and-white Image )"       << std::endl; break;
      case  9: std::cout << "( Run-length encoded, Color-mapped Image )"    << std::endl; break;
      case 10: std::cout << "( Run-length encoded, True-color Image )"      << std::endl; break;
      case 11: std::cout << "( Run-length encoded, Black-and-white Image )" << std::endl; break;
    }
  
    std::cout << "  first entry index:    " << (unsigned int)first_entry_index     << std::endl;
    std::cout << "  color map length:     " << (unsigned int)color_map_length      << std::endl;
    std::cout << "  color map entry size: " << (unsigned int)color_map_entry_size  << std::endl;
    std::cout << "  x origin:             " << (unsigned int)x_origin              << std::endl;
    std::cout << "  y origin:             " << (unsigned int)y_origin              << std::endl;
    std::cout << "  width:                " << (unsigned int)width                 << std::endl;
    std::cout << "  height:               " << (unsigned int)height                << std::endl;
    std::cout << "  pixel depth:          " << (unsigned int)pixel_depth           << std::endl;
    std::cout << "  alpha channel bit:    " << (unsigned int)alpha_channel_bits    << std::endl;
    std::cout << "  image origin:         ";
  
    switch ( imageOrigin )
    {
      case BOTTOM_LEFT:  std::cout << "Bottom left"  << std::endl; break;
      case BOTTOM_RIGHT: std::cout << "Bottom right" << std::endl; break;
      case TOP_LEFT:     std::cout << "top left"     << std::endl; break;
      case TOP_RIGHT:    std::cout << "Top right"    << std::endl; break;
    }
  
    if ( newFormat )
    {
      std::cout << std::endl;
      std::cout << "FOOTER" << std::endl;
      std::cout << "  extension offset: " << extension_offset << std::endl;
      std::cout << "  developer offset: " << developer_offset << std::endl;
    }
    
    std::cout << std::endl;
    std::cout << "FORMAT" << std::endl;
    std::cout << "  new file format: " << (newFormat ? "true" : "false") << std::endl;
    std::cout << std::endl;
  }
  
  const unsigned char* TGALoader::getData() const
  {
    return data;
  }
  
  unsigned char TGALoader::getDepth() const
  {
    return pixel_depth;
  }
  
  const unsigned short TGALoader::getWidth() const
  {
    return width;
  }
  
  const unsigned short TGALoader::getHeight() const
  {
    return height;
  }
}
