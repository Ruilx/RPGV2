#ifndef UTILS_H
#define UTILS_H

#include <QtCore>

class Utils
{
public:
	static void msleep(int msec){
		QEventLoop eventLoop;
		QTimer::singleShot(msec, &eventLoop, SLOT(quit()));
		eventLoop.exec();
	}
};

#endif // UTILS_H
