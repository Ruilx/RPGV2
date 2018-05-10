#include "RpgScene.h"

bool RpgScene::addRpgItem(RpgItem *item){
	if(item == nullptr || item->isNull()){
		qDebug() << CodePath() << "Giving RpgItem is not valid.";
		return false;
	}
	switch(item->getItemPosition()){
		case RpgItem::Pos_Absolute:
			break;
			item->setRealPos(item->pos());
		case RpgItem::Pos_Relative:
			item->setRealPos(this->sceneRect().topLeft() + item->getPos());
			break;
	}
	qDebug() << "RealPos:" << item->getRealPos();
	qDebug() << "ZValue:" << item->zValue();
	this->addItem(item);
	return true;
}

bool RpgScene::removeRpgItem(RpgItem *item){
	if(item == nullptr){
		qDebug() << CodePath() << "Giving RpgItem is not valid.";
		return false;
	}
	this->removeItem(item);
	return true;
}
