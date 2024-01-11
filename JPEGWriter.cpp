#include "JPEGWriter.h"
#include <stdexcept>
#include <iostream>

// Writes a JPEG image file from YCbCr data.
void JPEGWriter::writeJPEG(const std::string& filename, const std::vector<unsigned char>& ycbcrData, int width, int height, int colour) {
     // Initialize JPEG compression structures for storing compression information like width, height, colourspace
    struct jpeg_compress_struct cinfo;
    struct jpeg_error_mgr jerr;

    // Open the output file for writing in binary mode
    FILE* outfile = fopen(filename.c_str(), "wb");
    if (!outfile) {
        throw std::runtime_error("Cannot open output file");
    }

    cinfo.err = jpeg_std_error(&jerr); //Initiliaze jpeg error structure with default error handling routines
    jpeg_create_compress(&cinfo); //Initialize compression information structure
    jpeg_stdio_dest(&cinfo, outfile); //Set destination for compressed data

    //Setting image properties
    cinfo.image_width = width;  // Image width
    cinfo.image_height = height; // Image height
    cinfo.input_components = 3;  // Number of color components per pixel
    cinfo.in_color_space = JCS_YCbCr; // Colourspace of input image

    jpeg_set_defaults(&cinfo); //Set defualt compression parameters for compression info structure
    jpeg_set_colorspace(&cinfo, JCS_YCbCr); //Set colourspace to YCbCr
    
 // Set chroma subsampling factors based on the 'colour' parameter    
if(colour == 444){
     // 4:4:4 chroma subsampling (no subsampling), sampling factor 1 for all components
    cinfo.comp_info[0].h_samp_factor = 1; //Set 'Y' Luminance horizontal and vertical to 1
    cinfo.comp_info[0].v_samp_factor = 1;
    cinfo.comp_info[1].h_samp_factor = 1;//Set 'Cb' blue colour difference horizontal and vertical to 1
    cinfo.comp_info[1].v_samp_factor = 1;
    cinfo.comp_info[2].h_samp_factor = 1;//Set 'Cr' red colour difference and vertical to 1
    cinfo.comp_info[2].v_samp_factor = 1;
}else if(colour == 422){
    // 4:2:2 chroma subsampling
       cinfo.comp_info[0].h_samp_factor = 2; //Set 'Y' Luminance horizontal to 2 as horizontal resolution of chrominance is halved
    cinfo.comp_info[0].v_samp_factor = 1;
    cinfo.comp_info[1].h_samp_factor = 1;
    cinfo.comp_info[1].v_samp_factor = 1;
    cinfo.comp_info[2].h_samp_factor = 1;
    cinfo.comp_info[2].v_samp_factor = 1;
}// 4:2:0 chroma subsampling is the default


    jpeg_set_quality(&cinfo, 75, TRUE);  //Set the quality, 75/100 being good balance between image quality and file size

    jpeg_start_compress(&cinfo, TRUE); //Begins compression process, writes all header data first 
    
    JSAMPROW row_pointer;  // Pointer to a single row

//Writing the image data
while (cinfo.next_scanline < cinfo.image_height) {
    row_pointer = (JSAMPROW)&ycbcrData[cinfo.next_scanline * width * 3]; //Ensure every pixel has 3 components
    jpeg_write_scanlines(&cinfo, &row_pointer, 1);
}

jpeg_finish_compress(&cinfo); //FInish compression procession
fclose(outfile); //Close file 
jpeg_destroy_compress(&cinfo);//Release any memory assoicated with cinfo structure
}