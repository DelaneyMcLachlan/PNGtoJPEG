#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string>
#include "PNGReader.h"
#include "ColourConverter.h"
#include "JPEGWriter.h"
#include <iostream>

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " <input_png_file> <output_jpeg_file> <color_type>\n";
        return 1;
    }

    std::string inputFileName = argv[1];
    std::string outputFileName = argv[2];
    int colourType = std::stoi(argv[3]);

    // Validate color type
    if (colourType != 444 && colourType != 422 && colourType != 420) {
        std::cerr << "Invalid color type. Please choose from 444, 422, or 420.\n";
        return 1;
    }

    try {
        std::vector<unsigned char> rgbData;
        std::vector<unsigned char> ycbcrData;
        int width, height, colorType;

        // Read PNG
        PNGReader::readPNG(inputFileName, rgbData, width, height, colorType);

        // Convert RGB to YCbCr
        ColourConverter::RGBtoYCbCr(rgbData, ycbcrData, width, height);

        // Write JPEG
        JPEGWriter::writeJPEG(outputFileName, ycbcrData, width, height, colourType);

        std::cout << "Conversion completed successfully.\n";
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}