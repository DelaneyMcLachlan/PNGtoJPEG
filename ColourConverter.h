#ifndef COLORCONVERTER_H
#define COLORCONVERTER_H

#include <vector>

class ColourConverter {
public:
    static void RGBtoYCbCr(const std::vector<unsigned char>& rgbData, std::vector<unsigned char>& ycbcrData, int width, int height);
};

#endif // COLORCONVERTER_H
