#ifndef RPGNULLPOINTEREXCEPTION_H
#define RPGNULLPOINTEREXCEPTION_H

#include <RPG/exception/RpgException.h>

class RpgNullPointerException : public RpgException
{
public:
	RpgNullPointerException(const QString &name): RpgException(name){
		this->setInfo(QObject::tr("Pointer %1 is null.").arg(name));
	}
};

#endif // RPGNULLPOINTEREXCEPTION_H
