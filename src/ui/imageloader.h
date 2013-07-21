#ifndef __UI_IMAGE_LOADER_H
#define __UI_IMAGE_LOADER_H

#include <QImage>

namespace ui
{

class ImageLoader
{
public:
    ImageLoader();
    static QImage load(std::string name);
};

}

#endif
