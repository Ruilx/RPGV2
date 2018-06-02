#ifndef RPGUTILSHELPER_H
#define RPGUTILSHELPER_H

#include <QObject>
#include <RPG/utils/Utils.h>

class RpgUtilsHelper : public QObject
{
	Q_OBJECT
public:
	explicit RpgUtilsHelper(QObject *parent = nullptr) : QObject(parent){

	}

	Q_INVOKABLE void msleep(int ms){
		Utils::msleep(ms);
	}

signals:

public slots:
};

#endif // RPGUTILSHELPER_H
