#include "ImageUtility.h"

void ImageUtility::scale16bitTo8bit(std::vector<unsigned char>& dest, const std::vector<unsigned short>& src) {
    dest.resize(src.size() / 2);  // Allocating space for 8-bit data
    for (size_t i = 0; i < src.size(); ++i) {
        dest[i] = static_cast<unsigned char>(src[i] / 256);  // Scaling down
    }
}
