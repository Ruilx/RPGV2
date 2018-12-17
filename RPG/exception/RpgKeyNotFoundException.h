#ifndef RPGKEYNOTFOUNDEXCEPTION_H
#define RPGKEYNOTFOUNDEXCEPTION_H

#include "RpgException.h"

class RpgKeyNotFoundException : public RpgException
{
public:
	RpgKeyNotFoundException(const QString &key, const QString &list): RpgException(key)
	{
		this->setInfo(QObject::tr("The key: %1 not found in %2").arg(key, list));
	}
};

#endif // RPGKEYNOTFOUNDEXCEPTION_H
