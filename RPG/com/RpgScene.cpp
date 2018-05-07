#include "RpgScene.h"

bool RpgScene::addRpgItem(RpgItem *item){
	if(item == nullptr || item->isNull()){
		qDebug() << CodePath() << "Giving RpgItem is not valid.";
		return false;
	}
	switch(item->getItemPosition()){
		case RpgItem::Pos_Absolute:
			break;
		case RpgItem::Pos_Relative:
			item->setPos(this->sceneRect().topLeft() + item->getPos());
			break;
	}
	this->addItem(item->getItem());
	return true;
}

bool RpgScene::removeRpgItem(RpgItem *item){
	if(item == nullptr){
		qDebug() << CodePath() << "Giving RpgItem is not valid.";
		return false;
	}
	this->removeItem(item->getItem());
	return true;
}
