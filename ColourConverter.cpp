#include "ColourConverter.h"


void ColourConverter::RGBtoYCbCr(const std::vector<unsigned char>& rgbData, std::vector<unsigned char>& ycbcrData, int width, int height) {
    ycbcrData.resize(width * height * 3); // Prepare the output vector, resize to be three components per pixel as default is 4:2:0

    for (int y = 0; y < height; ++y) { //Iterate over each row
        for (int x = 0; x < width; ++x) { //Iterate over each column in a row
            int idx = (y * width + x) * 3; //Calculate index of current pixel

            //Extract RGB data
            unsigned char r = rgbData[idx];
            unsigned char g = rgbData[idx + 1];
            unsigned char b = rgbData[idx + 2];

            // Apply the conversion formula, change RGB to it's respective YCbCr values according to ITU-R BT.601
            unsigned char y = static_cast<unsigned char>(0.299 * r + 0.587 * g + 0.114 * b);
            unsigned char cb = static_cast<unsigned char>(-0.168736 * r - 0.331264 * g + 0.5 * b + 128);
            unsigned char cr = static_cast<unsigned char>(0.5 * r - 0.418688 * g - 0.081312 * b + 128);

            // Store the results
            ycbcrData[idx] = y;
            ycbcrData[idx + 1] = cb;
            ycbcrData[idx + 2] = cr;

        }
    }
}