#include "RpgBanner.h"

RpgBanner::RpgBanner(QGraphicsScene *parentScene, QObject *parent): RpgObject(parentScene, parent, nullptr){
	this->setBackgroundColor(QColor(Qt::transparent)); // 背景默认透明, 即不对后面的物件产生影响
	this->setZValue(BackgroundZValue); // 现在Layer初始化是最下面一层(BottomLayer)
	this->background->setZValue(0.0f);
	this->foreground->setZValue(0.1f);
	this->foreground->setGraphicsEffect(this->foregroundEffect);
	this->setStartOpacity(0.0f);
	this->setEndOpacity(1.0f);
	this->connect(this->foregroundAnimation, &QPropertyAnimation::finished, this, &RpgBanner::animationFinished);
	this->setFlag(QGraphicsItem::ItemIgnoresTransformations);
}

RpgBanner::RpgBanner(qreal startOpacity, qreal endOpacity, QGraphicsScene *parentScene, QObject *parent) : RpgBanner(parentScene, parent){
	this->setStartOpacity(startOpacity);
	this->setEndOpacity(endOpacity);
	this->setEasingCurveType(QEasingCurve::Linear);
}

void RpgBanner::setForegroundPixmap(const QPixmap &pixmap){
	QPixmap _t;
	if(this->getParentScene() == nullptr){
		qDebug() << CodePath() << "ParentScene is nullptr before calling setForegroundPixmap.";
		return;
	}
	if(pixmap.size() == QSize(ScreenWidth, ScreenHeight)){
		_t = pixmap;
	}else{
		_t = pixmap.scaled(this->getParentScene()->sceneRect().size().toSize(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
		QSize sceneSize = QSize(sceneSize.width(), sceneSize.height());
		if(_t.width() == this->getParentScene()->width()){
			_t = _t.copy(0, (_t.height() - sceneSize.height()) >> 1, _t.width(), sceneSize.height());
		}else if(_t.height() == this->getParentScene()->height()){
			_t = _t.copy((_t.width() - sceneSize.width()) >> 1, 0, sceneSize.width(), _t.height());
		}else{
			//			_t = _t.copy((_t.width() - sceneSize.width()) >> 1,
			//						 (_t.height() - sceneSize.height()) >> 1,
			//						 sceneSize.width(),
			//						 sceneSize.height());
			_t = pixmap;
		}
	}
	this->foreground->setPixmap(_t);
}

void RpgBanner::setBackgroundColor(const QColor &color){
	if(this->getParentScene() == nullptr){
		qDebug() << CodePath() << "ParentScene is nullptr before calling setBackgroundColor.";
		return;
	}
	QPixmap _t(this->getParentScene()->width(), this->getParentScene()->height());
	_t.fill(color);
	this->background->setPixmap(_t);
}

void RpgBanner::setBackgroundPixmap(const QPixmap &pixmap){
	QPixmap _t;
	if(this->getParentScene() == nullptr){
		qDebug() << CodePath() << "ParentScene is nullptr before calling setBackgroundPixmap.";
		return;
	}
	if(pixmap.size() == QSize(ScreenWidth, ScreenHeight)){
		_t = pixmap;
	}else{
		_t = pixmap.scaled(this->getParentScene()->sceneRect().size().toSize(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
		QSize sceneSize = QSize(sceneSize.width(), sceneSize.height());
		if(_t.width() == this->getParentScene()->width()){
			_t = _t.copy(0, (_t.height() - sceneSize.height()) >> 1, _t.width(), sceneSize.height());
		}else if(_t.height() == this->getParentScene()->height()){
			_t = _t.copy((_t.width() - sceneSize.width()) >> 1, 0, sceneSize.width(), _t.height());
		}else{
			//			_t = _t.copy((_t.width() - sceneSize.width()) >> 1,
			//						 (_t.height() - sceneSize.height()) >> 1,
			//						 sceneSize.width(),
			//						 sceneSize.height());
			_t = pixmap;
		}
	}
	this->background->setPixmap(_t);
}

void RpgBanner::exec(){
	if(this->getParentScene() == nullptr){
		qDebug() << CodePath() << "parentScene is not set. (Null)";
		return;
	}
	if(this->background->pixmap().isNull()){
		qDebug() << CodePath() << "backgroundPixmap is null.";
		return;
	}
	if(this->foreground->pixmap().isNull()){
		qDebug() << CodePath() << "foregroundPixmap is null.";
		return;
	}
	if(this->getProcessing() == true){
		qDebug() << CodePath() << "RpgBanner is Running, please don't call it repeatly!";
		return;
	}
	this->background->setPos(QPointF(0.0f, 0.0f));
	this->foreground->setPos(QPointF(0.0f, 0.0f));
	this->foregroundAnimation->setDuration(this->speed);

	switch(this->layer){
		case BottomLayer:
			//this->background->setZValue(BackgroundZValue);
			this->setZValue(BackgroundZValue);
			break;
		case TopLayer:
			//this->background->setZValue(TopZValue);
			this->setZValue(TopZValue);
			break;
	}

	this->showBanner();
}

void RpgBanner::execExit(){
	if(this->getParentScene() == nullptr){
		qDebug() << CodePath() << ": ParentScene is not set. (Null)";
		return;
	}
	if(this->background->pixmap().isNull()){
		qDebug() << CodePath() << ": backgroundPixmap is null.";
		return;
	}
	if(this->foreground->pixmap().isNull()){
		qDebug() << CodePath() << ": foregroundPixmap is null.";
		return;
	}
	if(this->getProcessing() == true){
		qDebug() << CodePath() << ": RpgBanner is running, please don't call it repeatly!";
		return;
	}
	this->background->setPos(QPointF(0.0f, 0.0f));
	this->foreground->setPos(QPointF(0.0f, 0.0f));
	this->foregroundAnimation->setDuration(this->speed);

	switch(this->layer){
		case BottomLayer:
			//this->background->setZValue(BackgroundZValue);
			this->setZValue(BackgroundZValue);
			break;
		case TopLayer:
			//this->background->setZValue(TopZValue);
			this->setZValue(TopZValue);
			break;
	}

	this->hideBanner();
}

void RpgBanner::showBanner(){
	if(this->background->isVisible()){
		qDebug() << CodePath() << "Item was shown, cannot show again.";
		return;
	}
	this->setProcessing(true);
	if(this->speed == SpeedInfinitly){
		this->foregroundEffect->setOpacity(this->foregroundAnimation->endValue().toDouble());
		if(this->foregroundAnimation->state() != QAbstractAnimation::Stopped){
			this->foregroundAnimation->setCurrentTime(this->foregroundAnimation->totalDuration());
			this->foregroundAnimation->stop();
			this->setProcessing(false);
		}
	}else{
		emit this->enterAutoMode();
		this->foregroundAnimation->start(QAbstractAnimation::KeepWhenStopped);
	}
	this->show();
}

void RpgBanner::hideBanner(){
	if(!this->background->isVisible()){
		qDebug() << CodePath() << "Item was not shown, cannot hide.";
		return;
	}
	this->setProcessing(true);
	if(this->speed == SpeedInfinitly){
		if(this->foregroundAnimation->state() != QAbstractAnimation::Stopped){
			this->foregroundAnimation->stop();
			this->foregroundEffect->setOpacity(0.0f);
			this->setProcessing(false);
		}
	}else{
		emit this->enterAutoMode();
		this->foregroundAnimation->start(QAbstractAnimation::KeepWhenStopped);
		Utils::msleep(this->foregroundAnimation->duration());
	}
	this->hide();
}

void RpgBanner::finished(){
	if(this->foregroundAnimation->state() != QAbstractAnimation::Stopped){
		this->foregroundAnimation->setCurrentTime(this->foregroundAnimation->totalDuration());
		this->foregroundAnimation->stop();
	}
	emit this->quitAutoMode();
	this->canBeInterrupted = false;
	this->setProcessing(false);
}

void RpgBanner::receiveKey(int key, Qt::KeyboardModifiers mod){
	if(!this->getProcessing()){
		return;
	}
	if(mod != Qt::NoModifier){
		return;
	}
	// 如果设定了允许用户按键打断的标识, 就可以被打断
	if(this->canBeInterrupted){
		//如果按键中断, 直接过渡结束
		if(key == Qt::Key_Return){
			this->finished();
		}
	}
}
