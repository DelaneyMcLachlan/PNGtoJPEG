#ifndef PNGREADER_H
#define PNGREADER_H

#include <string>
#include <vector>

class PNGReader {
public:
    static void readPNG(const std::string& filePath, std::vector<unsigned char>& rgbData, int &width, int &height, int &colorType);
};

#endif // PNGREADER_H
