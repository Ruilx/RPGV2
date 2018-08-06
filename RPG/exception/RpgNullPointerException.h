#ifndef RPGNULLPOINTEREXCEPTION_H
#define RPGNULLPOINTEREXCEPTION_H

#include <RPG/exception/RpgException.h>

class RpgNullPointerException : public RpgException
{
public:
	RpgNullPointerException(const QString &name): RpgException(name){
	}
};

#endif // RPGNULLPOINTEREXCEPTION_H
