#ifndef RPGFILECANNOTREADEXCEPTION_H
#define RPGFILECANNOTREADEXCEPTION_H

#include <RPG/exception/RpgException.h>
#include <QFile>

class RpgFileCannotReadException : public RpgException
{
public:
	RpgFileCannotReadException(const QFile &file): RpgException(file.fileName() + " : " + file.errorString()){

	}
};

#endif // RPGFILECANNOTREADEXCEPTION_H
