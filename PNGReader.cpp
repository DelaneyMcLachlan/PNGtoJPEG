#include "PNGReader.h"
#include <png.h>
#include <stdexcept>
#include <iostream>

// Reads a PNG file and extracts its RGB data, dimensions, and color type.
void PNGReader::readPNG(const std::string& filePath, std::vector<unsigned char>& rgbData, int &width, int &height, int &colorType) {
    // Open the file for reading in binary mode
    FILE *fp = fopen(filePath.c_str(), "rb");
    if (!fp) {
        throw std::runtime_error("File could not be opened for reading");
    }

    // Read the PNG file header to validate the file format
    png_byte header[8];
    fread(header, 1, 8, fp);
    //png_sig_cmp function provided by libpng to check if first 8 bytes of file are the standard png magic number
    if (png_sig_cmp(header, 0, 8)) {
        fclose(fp);
        throw std::runtime_error("File is not recognized as a PNG file");
    }

// Initialize libPNG structures for reading the PNG file
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        fclose(fp);
        throw std::runtime_error("png_create_read_struct failed");
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        fclose(fp);
        throw std::runtime_error("png_create_info_struct failed");
    }

      // Set up error handling, setjmp saves current environment which libPNG can jump back to in case of error
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        fclose(fp);
        throw std::runtime_error("Error during init_io");
    }

    png_init_io(png_ptr, fp); //Initiliaze input/output for PNG reading process
    png_set_sig_bytes(png_ptr, 8); //Read from next byte after PNG signature
    png_read_info(png_ptr, info_ptr);//Read file stream from PNG into info_ptr structure

    //Utilizes libPNG functions for getting metadata like width, height, colour type, bit depth
    width = png_get_image_width(png_ptr, info_ptr);
    height = png_get_image_height(png_ptr, info_ptr);
    colorType = png_get_color_type(png_ptr, info_ptr);
    png_byte bit_depth = png_get_bit_depth(png_ptr, info_ptr);

    
    // Print the header data
    std::cout << "Width: " << width << std::endl;
    std::cout << "Height: " << height << std::endl;
    std::cout << "Bit Depth: " << bit_depth << std::endl; 
    std::cout << "Color Type: " << colorType << std::endl; 

    // Set up transformation to ensure 8-bit RGB format
    if (colorType == PNG_COLOR_TYPE_PALETTE) //Convert palette (indexed colour) to RGB format
        png_set_palette_to_rgb(png_ptr);
    if (colorType == PNG_COLOR_TYPE_GRAY && bit_depth < 8) //Converts Y-only grayscale images to 8 bits 
        png_set_expand_gray_1_2_4_to_8(png_ptr);
    if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS)) //Check if image contains tRNS chunk, if yes convert to alpha channel
        png_set_tRNS_to_alpha(png_ptr);
    if (bit_depth == 16) //If 16bpc, scale down to 8bpc
        png_set_strip_16(png_ptr);
    if (colorType == PNG_COLOR_TYPE_GRAY || colorType == PNG_COLOR_TYPE_GRAY_ALPHA)//Convert grayscale with or without alpha channel to RGB
        png_set_gray_to_rgb(png_ptr);

    // Update the PNG structure with the transformation if applicable
    png_read_update_info(png_ptr, info_ptr);

    // Allocate memory to read the image
    png_bytep* row_pointers = new png_bytep[height]; //Create array of row pointers with each pointer corresponding to one row of the image
    for (int y = 0; y < height; y++) {
        row_pointers[y] = new png_byte[png_get_rowbytes(png_ptr, info_ptr)];
    }

    // Read the PNG image data
    png_read_image(png_ptr, row_pointers);

    // Convert to a flat array
    int rowBytes = png_get_rowbytes(png_ptr, info_ptr);
    rgbData.resize(height * rowBytes);
    for (int y = 0; y < height; y++) {
        memcpy(&rgbData[y * rowBytes], row_pointers[y], rowBytes);
        delete[] row_pointers[y];
    }
    delete[] row_pointers;

    for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
        int idx = (y * width + x) * 3; // 3 bytes per pixel (RGB)
        unsigned char r = rgbData[idx];
        unsigned char g = rgbData[idx + 1];
        unsigned char b = rgbData[idx + 2];
    }
  
}
    // Cleanup
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    fclose(fp);
}