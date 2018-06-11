#ifndef RPGVARHELPER_H
#define RPGVARHELPER_H

#include <QObject>
#include <RPG/com/RpgVar.h>

class RpgVarHelper : public QObject
{
	Q_OBJECT

	RpgVar *rpgVar = nullptr;
public:
	explicit RpgVarHelper(RpgVar *rpgVar, QObject *parent = nullptr) : QObject(parent){
		if(rpgVar == nullptr){
			qDebug() << CodePath() << "RpgVar is nullptr.";
			return;
		}
		this->rpgVar = rpgVar;
	}

	Q_INVOKABLE void setValue(const QString &key, const QString &value){
		if(this->rpgVar == nullptr){
			qDebug() << CodePath() << "RpgVar is nullptr.";
			return;
		}
		this->rpgVar->setValue(key, value);
	}

	Q_INVOKABLE QString getValue(const QString &key){
		if(this->rpgVar == nullptr){
			qDebug() << CodePath() << "RpgVar is nullptr.";
			return QString();
		}
		return this->rpgVar->getValue(key).toString();
	}

	Q_INVOKABLE int getVarCount(){
		if(this->rpgVar == nullptr){
			qDebug() << CodePath() << "RpgVar is nullptr.";
			return 0;
		}
		return this->rpgVar->getVarCount();
	}

	Q_INVOKABLE void removeValue(const QString &key){
		if(this->rpgVar == nullptr){
			qDebug() << CodePath() << "RpgVar is nullptr.";
			return;
		}
		this->rpgVar->removeValue(key);
	}

	Q_INVOKABLE bool loadData(int slot){
		if(this->rpgVar == nullptr){
			qDebug() << CodePath() << "RpgVar is nullptr.";
			return false;
		}
		QString file = QString("data/save/%1.sav").arg(slot);
		return this->rpgVar->loadData(file);
	}

	Q_INVOKABLE bool saveData(int slot){
		if(this->rpgVar == nullptr){
			qDebug() << CodePath() << "RpgVar is nullptr.";
			return false;
		}
		QString file = QString("data/save/%1.sav").arg(slot);
		return this->rpgVar->saveData(file);
	}

signals:

public slots:
};

#endif // RPGVARHELPER_H
