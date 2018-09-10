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

#include <RPG/script/RpgBannerHelper.h>
#include <RPG/script/RpgChoiceHelper.h>
#include <RPG/script/RpgDialogHelper.h>
#include <RPG/script/RpgMusicHelper.h>
#include <RPG/script/RpgSoundHelper.h>
#include <RPG/script/RpgUtilsHelper.h>
#include <RPG/script/RpgLyricHelper.h>

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
	RpgBannerHelper *bannerHelper = nullptr;
	RpgChoiceHelper *choiceHelper = nullptr;
	RpgDialogHelper *dialogHelper = nullptr;
	RpgMusicHelper  *musicHelper  = nullptr;
	RpgSoundHelper  *soundHelper  = nullptr;
	RpgUtilsHelper  *utilsHelper  = nullptr;
	RpgLyricHelper  *lyricHelper  = nullptr;

	QString mapFile;

	QPropertyAnimation *posAnimation = new QPropertyAnimation(this, "sceneRect");
public:
	RpgScene(QObject *parent = nullptr) : QGraphicsScene(parent){
		this->setScenePos(0.0f, 0.0f);

		this->banner = new RpgBanner(this, this);
		this->choice = new RpgChoice(this, this);
		this->dialog = new RpgDialog(this, this);
		this->lyric  = new RpgLyric(this, this);

		RpgState::instance()->registerRpgObject(this->banner, RpgState::AutoMode);
		RpgState::instance()->registerRpgObject(this->dialog, RpgState::DialogMode);
		RpgState::instance()->registerRpgObject(this->choice, RpgState::DialogMode);

		this->script = new RpgScript(this);
		this->bannerHelper = new RpgBannerHelper(this->banner, this);
		this->choiceHelper = new RpgChoiceHelper(this->choice, this);
		this->dialogHelper = new RpgDialogHelper(this->dialog, this);
		this->musicHelper  = new RpgMusicHelper(RpgMusic::instance(), this);
		this->soundHelper  = new RpgSoundHelper(RpgSound::instance(), this);
		this->lyricHelper  = new RpgLyricHelper(this->lyric, this);

		this->utilsHelper  = new RpgUtilsHelper(this);

		this->script->addJsValue("RpgBanner", this->bannerHelper);
		this->script->addJsValue("RpgChoice", this->choiceHelper);
		this->script->addJsValue("RpgDialog", this->dialogHelper);
		this->script->addJsValue("RpgMusic",  this->musicHelper);
		this->script->addJsValue("RpgSound",  this->soundHelper);
		this->script->addJsValue("RpgLyric",  this->lyricHelper);

		this->script->addJsValue("RpgUtils", this->utilsHelper);

		this->lyric->setRpgMusic(RpgMusic::instance());

		this->connect(this->banner, &RpgBanner::enterAutoMode, [](){
			RpgState::instance()->pushMode(RpgState::AutoMode);
		});

		this->connect(this->banner, &RpgBanner::quitAutoMode, [](){
			RpgState::instance()->popMode();
		});

		this->connect(this->dialog, &RpgDialog::enterDialogMode, [](){
			RpgState::instance()->pushMode(RpgState::DialogMode);
		});

		this->connect(this->dialog, &RpgDialog::quitDialogMode, [](){
			RpgState::instance()->popMode();
		});

		this->connect(this->choice, &RpgChoice::enterDialogMode, [](){
			RpgState::instance()->pushMode(RpgState::DialogMode);
		});

		this->connect(this->choice, &RpgChoice::quitDialogMode, [](){
			RpgState::instance()->popMode();
		});

		this->posAnimation->setDuration(500);
		this->posAnimation->setEasingCurve(QEasingCurve::InOutQuad);
	}

	void setSceneRect(const QRectF &rect){ QGraphicsScene::setSceneRect(rect);}
	inline void setSceneRect(qreal x, qreal y, qreal w, qreal h) { this->setSceneRect(QRectF(x, y, w, h)); }
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
			duration = int(qSqrt(qAbs(x2 - x1) * qAbs(x2 - x1) + qAbs(y2 - y1) * qAbs(y2 - y1))) * 3;
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
