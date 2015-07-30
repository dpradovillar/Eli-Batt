#ifndef UTILS_H
#define UTILS_H

#include <QtCore>

class UiHandler {
public:
    virtual void onProgressUpdate(qreal percent) = 0;
};

#endif // UTILS_H
