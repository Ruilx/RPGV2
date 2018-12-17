#ifndef RPGEXCEPTION_H
#define RPGEXCEPTION_H

#include <QException>
#include <RPG/Global.h>
#include <QtDebug>

class RpgException : public QException
{
	QString info;
public:
	RpgException(const QString &info) Q_DECL_NOTHROW{
		this->setInfo(info);
	}

	void setInfo(const QString &info) Q_DECL_NOTHROW{
		this->info = info;
	}

	QString getInfo() const Q_DECL_NOTHROW{
		return info;
	}

	void printInfo() const Q_DECL_NOTHROW{
		qCritical() << CodePath() << "Exception:" << this->info;
	}

	void raise() const Q_DECL_NOTHROW{
		throw *this;
	}
};

#endif // RPGEXCEPTION_H
