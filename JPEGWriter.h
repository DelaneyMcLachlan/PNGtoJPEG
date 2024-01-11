#ifndef JPEGWRITER_H
#define JPEGWRITER_H

#include <string>
#include <vector>
#include <jpeglib.h>

class JPEGWriter {
public:
    static void writeJPEG(const std::string& filename, const std::vector<unsigned char>& ycbcrData, int width, int height, int colour);
};

#endif // JPEGWRITER_H
