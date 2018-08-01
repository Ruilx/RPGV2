#ifndef RPGFILENOTFOUNDEXCEPTION_H
#define RPGFILENOTFOUNDEXCEPTION_H

#include <RPG/exception/RpgException.h>
#include <QFile>

class RpgFileNotFoundException : public RpgException
{
public:
	RpgFileNotFoundException(const QFile &file): RpgException(file.fileName()){

	}
};

#endif // RPGFILENOTFOUNDEXCEPTION_H
