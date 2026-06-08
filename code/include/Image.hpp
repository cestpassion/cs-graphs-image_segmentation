#ifndef IMAGE_HPP
#define IMAGE_HPP

#include <string>
#include <vector>

class Image {
public:
    int width;
    int height;
    int channels;
    std::vector<unsigned char> pixels;

    Image();

    bool empty() const;

    bool load(const std::string& path);
    bool save(const std::string& path) const;

    Image toGrayscale() const;

    unsigned char getPixel(int x, int y, int c) const;
    void setPixel(int x, int y, int c, unsigned char value);

    static Image createColoredLabelImage(
        const std::vector<int>& labels,
        int width,
        int height
    );

private:
    int getIndex(int x, int y, int c) const;
};

#endif