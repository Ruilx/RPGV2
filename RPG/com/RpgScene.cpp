#include "RpgScene.h"

#include <RPG/script/RpgSceneHelper.h>
#include <RPG/script/RpgBannerHelper.h>
#include <RPG/script/RpgChoiceHelper.h>
#include <RPG/script/RpgDialogHelper.h>
#include <RPG/script/RpgMusicHelper.h>
#include <RPG/script/RpgSoundHelper.h>
#include <RPG/script/RpgUtilsHelper.h>
#include <RPG/script/RpgLyricHelper.h>

RpgScene::RpgScene(QObject *parent) : QGraphicsScene(parent){
	this->setScenePos(0.0f, 0.0f);

	this->banner = new RpgBanner(this, this);
	this->choice = new RpgChoice(this, this);
	this->dialog = new RpgDialog(this, this);
	this->lyric  = new RpgLyric(this, this);

	RpgState::instance()->registerRpgObject(this->banner, RpgState::AutoMode);
	RpgState::instance()->registerRpgObject(this->dialog, RpgState::DialogMode);
	RpgState::instance()->registerRpgObject(this->choice, RpgState::DialogMode);

	this->script = new RpgScript(this);
//	this->sceneHelper  = new RpgSceneHelper(this, this);
//	this->bannerHelper = new RpgBannerHelper(this->banner, this);
//	this->choiceHelper = new RpgChoiceHelper(this->choice, this);
//	this->dialogHelper = new RpgDialogHelper(this->dialog, this);
//	this->musicHelper  = new RpgMusicHelper(RpgMusic::instance(), this);
//	this->soundHelper  = new RpgSoundHelper(RpgSound::instance(), this);
//	this->lyricHelper  = new RpgLyricHelper(this->lyric, this);

//	this->utilsHelper  = new RpgUtilsHelper(this);

	this->script->addJsValue("RpgScene",  new RpgSceneHelper(this, this));
	this->script->addJsValue("RpgBanner", new RpgBannerHelper(this->banner, this));
	this->script->addJsValue("RpgChoice", new RpgChoiceHelper(this->choice, this));
	this->script->addJsValue("RpgDialog", new RpgDialogHelper(this->dialog, this));
	this->script->addJsValue("RpgMusic",  new RpgMusicHelper(RpgMusic::instance(), this));
	this->script->addJsValue("RpgSound",  new RpgSoundHelper(RpgSound::instance(), this));
	this->script->addJsValue("RpgLyric",  new RpgLyricHelper(this->lyric, this));

	this->script->addJsValue("RpgUtils", new RpgUtilsHelper(this));

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
