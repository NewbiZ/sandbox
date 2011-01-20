#ifndef MAR_TGA_LOADER_H
#define MAR_TGA_LOADER_H

#include <string>

namespace mar
{
  class TGALoader
  {
  public:
    enum ImageType
    {
      NO_IMAGE_DATA,
      UNCOMPRESSED_COLOR_MAPPED,
      UNCOMPRESED_TRUE_COLOR,
      UNCOMPRESSED_BLACK_AND_WHITE,
      RLE_COLOER_MAPPED,
      RLE_TRUE_COLOR,
      RLE_BLACK_AND_WHITE
    };
  
    enum ImageOrigin
    {
      BOTTOM_LEFT,
      BOTTOM_RIGHT,
      TOP_LEFT,
      TOP_RIGHT
    };
  
  public:
    TGALoader();
    ~TGALoader();
  
    void load( const std::string& filename );
    void display();
  
    unsigned char        getDepth()  const;
    const unsigned char* getData()   const;
    const unsigned short getWidth()  const;
    const unsigned short getHeight() const;
  
  private:
    std::string filename_;
  
    // Header
    unsigned char  image_id_length;
    unsigned char  color_map_type;
    unsigned char  image_type;
    unsigned short first_entry_index;
    unsigned short color_map_length;
    unsigned char  color_map_entry_size;
    unsigned short x_origin;
    unsigned short y_origin;
    unsigned short width;
    unsigned short height;
    unsigned char  pixel_depth;
    unsigned char  alpha_channel_bits;
    ImageType      imageType;
    ImageOrigin    imageOrigin;
    
    // Footer
    unsigned long int extension_offset;
    unsigned long int developer_offset;
    char              signature[18];
  
    bool newFormat;
  
    unsigned char* data;
  };
}

#endif // MAR_TGA_LOADER_H
