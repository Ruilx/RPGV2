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

signals:

public slots:
};

#endif // RPGSCENEHELPER_H
