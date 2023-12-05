// colormanager.h

#ifndef COLORMANAGER_H
#define COLORMANAGER_H

#include <QString>
#define BASE_COLOR_BLUE rgba(10,30,80,255)
class ColorManager {
public:
    static QString getDarkBlueColor();
    static QString getLightBlueColor();
};

#endif // COLORMANAGER_H
