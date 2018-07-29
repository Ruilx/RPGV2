#ifndef RPGVAR_H
#define RPGVAR_H

#include <QtCore>
#include <RPG/Global.h>

class RpgVar : public QObject
{
	Q_OBJECT
	static RpgVar *_instance;

	QHash<QString, QVariant> var;
public:
	static RpgVar *instance(){
		if(_instance == nullptr){
			_instance = new RpgVar(nullptr);
		}
		return _instance;
	}

	explicit RpgVar(QObject *parent = 0) : QObject(parent){
		this->var.clear();
	}

	void setValue(const QString &key, QVariant value){
		if(this->var.contains(key)){
			this->var.remove(key);
		}
		this->var.insert(key, value);
	}

	QVariant getValue(const QString &key) const{
		return this->var.value(key);
	}

	int getVarCount(){
		return this->var.count();
	}

	void removeValue(const QString &key){
		if(this->var.contains(key)){
			this->var.remove(key);
		}else{
			qDebug() << CodePath() << "try to remove an unset varable.";
		}
	}

	bool loadData(const QString &file){
		if(!QFile::exists(file)){
			qDebug() << CodePath() << "Specific file" << file << "not exists.";
			return false;
		}
		QSettings slot(file, QSettings::IniFormat);
		slot.beginGroup("Vars");
		QStringList keys = slot.childKeys();
		for(const QString &key: keys){
			this->setValue(key, slot.value(key));
		}
		slot.endGroup();
		qDebug() << CodePath() << "Load var values successful. Loaded:" << keys.length() << "items.";
		return true;
	}

	bool saveData(const QString &file){
		if(QFile::exists(file)){
			qDebug() << CodePath() << "Specific file exists, will clear the data before save. file:" << file;
		}
		QSettings slot(file, QSettings::IniFormat);
		slot.clear();
		slot.beginGroup("Vars");
		QStringList keys = this->var.keys();
		for(const QString &key: keys){
			slot.setValue(key, this->getValue(key));
		}
		slot.endGroup();
		qDebug() << CodePath() << "Save var values successful. Saved:" << keys.length() << "items.";
		return true;
	}

signals:

public slots:
};

#endif // RPGVAR_H
