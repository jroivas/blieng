#include "imageloader.h"
#include "blieng/data.h"

using ui::ImageLoader;


ImageLoader::ImageLoader()
{
}

QImage ImageLoader::load(std::string name)
{
    QImage res = QImage();

    char *data = NULL;
    unsigned int cnt = blieng::Data::getInstance()->readData(name, &data);
    if (cnt > 0) {
        res.loadFromData((const unsigned char*)data, cnt);
    }

    return res;
}
