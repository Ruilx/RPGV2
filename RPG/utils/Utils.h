#ifndef UTILS_H
#define UTILS_H

#include <QtCore>
#include <RPG/Global.h>

class Utils
{
public:
	Q_INVOKABLE static void msleep(int msec){
		QEventLoop eventLoop;
		QTimer::singleShot(msec, &eventLoop, SLOT(quit()));
		eventLoop.exec();
	}

	static QByteArray readFile(const QString &fileName){
		QFile f(fileName);
		if(!f.open(QIODevice::ReadOnly)){
			qDebug() << CodePath() << "Cannot open" << fileName << "to read information.";
			return QByteArray();
		}
		QByteArray data = f.readAll();
		f.close();
		if(data.isEmpty()){
			qDebug() << CodePath() << "File" << fileName << "content empty";
			return data;
		}
		return data;
	}

	static QString detectedJsonValue(const QJsonValue &value){
		QString type;
		if(value.isNull()){
			type = "Null";
		}else if(value.isArray()){
			type = "Array";
		}else if(value.isObject()){
			type = "Object";
		}else if(value.isBool()){
			type = "Bool";
		}else if(value.isDouble()){
			type = "Double";
		}else if(value.isString()){
			type = "String";
		}else if(value.isUndefined()){
			type = "Undefined";
		}else{
			type = "Unknown";
		}
		return type;
	}
};

#endif // UTILS_H
