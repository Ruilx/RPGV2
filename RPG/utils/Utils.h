#ifndef UTILS_H
#define UTILS_H

#include <QtCore>
#include <RPG/Global.h>

#define EnumKeyToString(key) #key

class Utils
{
public:
	Q_INVOKABLE static void msleep(int msec);

	static QByteArray readFile(const QString &fileName);

	static QString detectedJsonValue(const QJsonValue &value);

	static QString _keyModifierToString(Qt::KeyboardModifiers mod);

	static QString _keyToString(Qt::Key key);
};

#endif // UTILS_H
