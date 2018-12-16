#ifndef COMMON_H
#define COMMON_H

#include <QApplication>

#define DEBUG_LOG_PATH  QApplication::applicationDirPath() + "/debug.log"

const QString BUILD_TIME_STR = __DATE__;

#endif // COMMON_H
