#ifndef RPGVAR_H
#define RPGVAR_H

#include <QtCore>
#include <RPG/Global.h>

class RpgVar : public QObject
{
	Q_OBJECT
	QHash<QString, QVariant> var;
public:
	explicit RpgVar(QObject *parent = 0);

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

signals:

public slots:
};

#endif // RPGVAR_H
