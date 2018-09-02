#ifndef RPGITEMHELPER_H
#define RPGITEMHELPER_H

#include <RPG/Global.h>
#include <RPG/com/RpgItem.h>

class RpgItemHelper : public QObject
{
	Q_OBJECT

	RpgItem *rpgItem = nullptr;
public:
	explicit RpgItemHelper(RpgItem *rpgItem, QObject *parent = nullptr) : QObject(parent){
		if(rpgItem == nullptr){
			qDebug() << CodePath() << "RpgItem nullptr";
			return;
		}
		this->rpgItem = rpgItem;
	}

	Q_INVOKABLE void setPixmap(const QString &name){
		if(this->rpgItem == nullptr){
			qDebug() << CodePath() << "RpgItem nullptr";
			return;
		}
		this->rpgItem->setPixmap(name);
	}

	Q_INVOKABLE void setPos(qreal x, qreal y){
		if(this->rpgItem == nullptr){
			qDebug() << CodePath() << "RpgItem nullptr";
			return;
		}
		this->rpgItem->setPos(QPointF(x, y));
	}

	Q_INVOKABLE qreal getPosX() const{
		if(this->rpgItem == nullptr){
			qDebug() << CodePath() << "RpgItem nullptr";
			return 0.f;
		}
		return this->rpgItem->getPos().x();
	}

	Q_INVOKABLE qreal getPosY() const{
		if(this->rpgItem == nullptr){
			qDebug() << CodePath() << "RpgItem nullptr";
			return 0.f;
		}
		return this->rpgItem->getPos().y();
	}

	Q_INVOKABLE void setRealPos(qreal x, qreal y){
		if(this->rpgItem == nullptr){
			qDebug() << CodePath() << "RpgItem nullptr";
			return;
		}
		this->rpgItem->setRealPos(QPointF(x, y));
	}

	Q_INVOKABLE qreal getRealPosX(){
		if(this->rpgItem == nullptr){
			qDebug() << CodePath() << "RpgItem nullptr";
			return 0.f;
		}
		return this->rpgItem->getRealPos().x();
	}

	Q_INVOKABLE qreal getRealPosY(){
		if(this->rpgItem == nullptr){
			qDebug() << CodePath() << "RpgItem nullptr";
			return 0.f;
		}
		return this->rpgItem->getRealPos().y();
	}

	Q_INVOKABLE bool isNull(){
		if(this->rpgItem == nullptr){
			qDebug() << CodePath() << "RpgItem nullptr";
			return true;
		}
		return this->rpgItem->isNull();
	}



signals:

public slots:
};

#endif // RPGITEMHELPER_H
