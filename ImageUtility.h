#ifndef IMAGEUTILITY_H
#define IMAGEUTILITY_H

#include <vector>

class ImageUtility {
public:
    static void scale16bitTo8bit(std::vector<unsigned char>& dest, const std::vector<unsigned short>& src);
};

#endif // IMAGEUTILITY_H
