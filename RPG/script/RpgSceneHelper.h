#ifndef RPGSCENEHELPER_H
#define RPGSCENEHELPER_H

#include <QObject>
#include <RPG/com/RpgScene.h>
#include <RPG/com/RpgItem.h>

class RpgSceneHelper : public QObject
{
	Q_OBJECT

	RpgScene *rpgScene = nullptr;
public:
	RpgSceneHelper(RpgScene *rpgScene, QObject *parent = nullptr) : QObject(parent){
		if(rpgScene == nullptr){
			qDebug() << CodePath() << "RpgScene nullptr.";
			return;
		}
		this->rpgScene = rpgScene;
	}

	Q_INVOKABLE bool addRpgItem(RpgItem *item){
		if(this->rpgScene == nullptr){
			qDebug() << CodePath() << "RpgScene nullptr.";
			return false;
		}
		return this->rpgScene->addRpgItem(item);
	}

	Q_INVOKABLE bool removeRpgItem(RpgItem *item){
		if(this->rpgScene == nullptr){
			qDebug() << CodePath() << "RpgScene nullptr.";
			return false;
		}
		return this->rpgScene->removeRpgItem(item);
	}

	Q_INVOKABLE void setScenePos(qreal x, qreal y){
		if(this->rpgScene == nullptr){
			qDebug() << CodePath() << "RpgScene nullptr.";
			return;
		}
		return this->rpgScene->setScenePos(x, y);
	}

	Q_INVOKABLE int moveSceneToLocation(qreal x, qreal y, int duration = -1){
		if(this->rpgScene == nullptr){
			qDebug() << CodePath() << "RpgScene nullptr.";
			return -1;
		}
		return this->rpgScene->moveSceneToLocation(x, y, duration);
	}

	Q_INVOKABLE void waitingForMoveSceneComplete(){
		if(this->rpgScene == nullptr){
			qDebug() << CodePath() << "RpgScene nullptr.";
			return;
		}
		this->rpgScene->waitingForMoveSceneComplete();
	}

signals:

public slots:
};

#endif // RPGSCENEHELPER_H
