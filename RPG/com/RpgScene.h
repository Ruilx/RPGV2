#ifndef RPGSCENE_H
#define RPGSCENE_H

#include <QGraphicsScene>
#include <QtWidgets>
#include <RPG/com/RpgDialog.h>
#include <RPG/com/RpgBanner.h>
#include <RPG/com/RpgChoice.h>
#include <RPG/com/RpgMusic.h>
#include <RPG/com/RpgSound.h>
#include <RPG/com/RpgItem.h>
#include <RPG/com/RpgScript.h>
#include <RPG/com/RpgState.h>
#include <RPG/com/RpgLyric.h>

#include <RPG/core/RpgFileManager.h>

#include <QPropertyAnimation>

/**
 * @brief The RpgScene class
 * RPGScene类是RPG游戏中的场景类, 其本质是一个QGraphicsScene, 在scene上增加需要的内容
 * 每一个Scene配备一个RPGDialog, 一个RPGSelectDialog, 一个RPGItemDialog, 一个RPGSystemDialog
 * 每一个组件在一个Scene只能出现一次, Scene中存储着地图的地图块, 精灵等
 *
 * --> RpgDialog类不再是单例模式, 而是以Scene返回方式获取.
 * --> RpgChoice类不再是单例模式, 而是以Scene返回方式获取.
 */
class RpgScene : public QGraphicsScene
{
	Q_OBJECT
	RpgDialog *dialog = nullptr;
	RpgBanner *banner = nullptr;
	RpgChoice *choice = nullptr;
	RpgLyric  *lyric  = nullptr;

	RpgScript *script = nullptr;

	QString mapFile;

	QPropertyAnimation *posAnimation = new QPropertyAnimation(this, "sceneRect");

	QPointF prevPos = QPointF();

	void mousePressEvent(QGraphicsSceneMouseEvent *event){
		qDebug() << "MOUSE PRESS IN SCENE POS:" << event->pos();
	}

	void mouseMoveEvent(QGraphicsSceneMouseEvent *event){
	}
public:
	RpgScene(QObject *parent = nullptr);

//	void setSceneRect(const QRectF &rect){ QGraphicsScene::setSceneRect(rect);}
//	inline void setSceneRect(qreal x, qreal y, qreal w, qreal h) { this->setSceneRect(QRectF(x, y, w, h)); }
	inline void setScenePos(const QPointF &point){ this->setSceneRect(point.x(), point.y(), ScreenWidthF, ScreenHeightF); }
	inline void setScenePos(qreal left, qreal top){ this->setScenePos(QPointF(left, top)); }

	QRectF getSceneRect() const{ return this->sceneRect(); }
	QPointF getScenePos() const{ return this->sceneRect().topLeft(); } // 应该写成ViewPos好一点, 但是看上面都写成ScenePos了...闲了改过来
	QSizeF getSceneSize() const{ return this->sceneRect().size(); }
	QSizeF getMapSize() const{ return this->itemsBoundingRect().size(); }
	QSizeF getMapBlockSize() const{ return this->itemsBoundingRect().size() / 32.0f; }

	RpgDialog *getRpgDialog() const{ Q_ASSERT_X(this->dialog, "RpgDialog Nulled", "RpgDialog not found!"); return this->dialog; }
	RpgBanner *getRpgBanner() const{ Q_ASSERT_X(this->banner, "RpgBanner Nulled", "RpgBanner not found!"); return this->banner; }
	RpgChoice *getRpgChoise() const{ Q_ASSERT_X(this->choice, "RpgChoice Nulled", "RpgChoice not found!"); return this->choice; }
	RpgLyric  *getRpgLyric () const{ Q_ASSERT_X(this->lyric , "RpgLyric  Nulled", "RpgLyric  not found!"); return this->lyric ; }

	bool addRpgItem(RpgItem *item);
	bool removeRpgItem(RpgItem *item);

	void setScriptFile(const QString &filename){ this->script->setScriptName(filename); }
	void setScript(const QString &name){
		this->script->setScriptName(name);
	}

	void setMapFile(const QString &filename){ this->mapFile = filename; }
	void setMap(const QString &name){
		QString filename = RpgFileManager::instance()->getFileString(RpgFileManager::MapFile, name);
		if(filename.isEmpty()){
			qDebug() << CodePath() << "Map name:" << name << "not found in the file manager";
			return;
		}
		this->mapFile = filename;
	}

	QString execScript(){
		QString res = this->script->exec();
		qDebug() << CodePath() << res;
		return res;
	}

	int moveSceneToLocation(const QPointF &leftTop, int duration = -1){
		if(this->posAnimation == nullptr){
			qDebug() << CodePath() << "animation is nullptr.";
			return 0;
		}
		if(this->posAnimation->state() != QPropertyAnimation::Stopped){
			this->posAnimation->setCurrentTime(this->posAnimation->duration());
			this->posAnimation->stop();
		}
		this->posAnimation->setStartValue(this->getSceneRect());
		this->posAnimation->setEndValue(QRectF(leftTop, this->getSceneSize()));
		if(duration == -1){
			qreal x1 = this->posAnimation->startValue().toRectF().topLeft().x();
			qreal y1 = this->posAnimation->startValue().toRectF().topLeft().y();
			qreal x2 = this->posAnimation->endValue().toRectF().topLeft().x();
			qreal y2 = this->posAnimation->endValue().toRectF().topLeft().y();
			duration = int(qSqrt(qAbs(x2 - x1) * qAbs(x2 - x1) + qAbs(y2 - y1) * qAbs(y2 - y1))) * 2;
		}
		if(duration > 0){
			this->posAnimation->setDuration(duration);
			this->posAnimation->start();
			return duration;
		}else{
			this->setSceneRect(this->posAnimation->endValue().toRectF());
			return 0;
		}
	}

	int moveSceneToLocation(qreal x, qreal y, int duration = -1){
		return this->moveSceneToLocation(QPointF(x, y), duration);
	}

	int moveSceneToLocation(int x, int y, int duration = -1){
		return this->moveSceneToLocation(QPointF(qreal(x), qreal(y)), duration);
	}

	void waitingForMoveSceneComplete(){
		if(this->posAnimation->state() == QPropertyAnimation::Stopped){
			qDebug() << CodePath() << "Not running yet, cannot wait.";
			return;
		}
		QEventLoop eventLoop(this);
		this->connect(this->posAnimation, &QPropertyAnimation::finished, &eventLoop, &QEventLoop::quit);
		if(this->posAnimation->state() != QPropertyAnimation::Stopped){
			eventLoop.exec();
		}
		return;
	}

signals:
};

#endif // RPGSCENE_H
