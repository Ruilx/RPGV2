#include "RpgBanner.h"

RpgBanner::RpgBanner(QGraphicsScene *parentScene, QObject *parent): QObject(parent){
	this->parentScene = parentScene;
	this->setBackgroundColor(QColor(Qt::transparent)); // 背景默认透明, 即不对后面的物件产生影响
	this->background->setZValue(BackgroundZValue); // 现在Layer初始化是最下面一层(BottomLayer)
	this->foreground->setZValue(0.1f);
	this->foreground->setGraphicsEffect(this->foregroundEffect);
	this->setStartOpacity(0.0f);
	this->setEndOpacity(1.0f);
	this->connect(this->foregroundAnimation, &QPropertyAnimation::finished, this, &RpgBanner::animationFinished);

	this->background->hide();
	this->parentScene->addItem(this->background);
}

RpgBanner::RpgBanner(qreal startOpacity, qreal endOpacity, QGraphicsScene *parentScene, QObject *parent){
	RpgBanner(parentScene, parent);
	this->setStartOpacity(startOpacity);
	this->setEndOpacity(endOpacity);
	this->setEasingCurveType(QEasingCurve::Linear);
}

void RpgBanner::setForegroundPixmap(const QPixmap &pixmap){
	QPixmap _t;
	if(this->parentScene == nullptr){
		qDebug() << CodePath() << "ParentScene is nullptr before calling setForegroundPixmap.";
		return;
	}
	if(pixmap.size() == QSize(ScreenWidth, ScreenHeight)){
		_t = pixmap;
	}else{
		_t = pixmap.scaled(this->parentScene->sceneRect().size().toSize(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
		QSize sceneSize = QSize(sceneSize.width(), sceneSize.height());
		if(_t.width() == this->parentScene->width()){
			_t = _t.copy(0, (_t.height() - sceneSize.height()) >> 1, _t.width(), sceneSize.height());
		}else if(_t.height() == this->parentScene->height()){
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
	if(this->parentScene == nullptr){
		qDebug() << CodePath() << "ParentScene is nullptr before calling setBackgroundColor.";
		return;
	}
	QPixmap _t(this->parentScene->width(), this->parentScene->height());
	_t.fill(color);
	this->background->setPixmap(_t);
}

void RpgBanner::exec(){
	if(this->parentScene == nullptr){
		qDebug() << CodePath() << ": parentScene is not set. (Null)";
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
	if(this->isRunning == true){
		qDebug() << CodePath() << ": RpgBanner is Running, please don't call it repeatly!";
		return;
	}
	this->background->setPos(this->parentScene->sceneRect().topLeft());
	this->foreground->setPos(this->parentScene->sceneRect().topLeft());
	this->foregroundAnimation->setDuration(this->speed);

	this->showBanner();
}

void RpgBanner::execExit(){
	if(this->parentScene == nullptr){
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
	if(this->isRunning == true){
		qDebug() << CodePath() << ": RpgBanner is running, please don't call it repeatly!";
		return;
	}
	this->background->setPos(this->parentScene->sceneRect().topLeft());
	this->foreground->setPos(this->parentScene->sceneRect().topLeft());
	this->foregroundAnimation->setDuration(this->speed);

	this->hideBanner();
}

void RpgBanner::showBanner(){
	if(this->background->isVisible()){
		qDebug() << CodePath() << "Item was shown, cannot show again.";
		return;
	}
	this->isRunning = true;
	if(this->speed == SpeedInfinitly){
		this->foregroundEffect->setOpacity(this->foregroundAnimation->endValue().toDouble());
		if(this->foregroundAnimation->state() != QAbstractAnimation::Stopped){
			this->foregroundAnimation->setCurrentTime(this->foregroundAnimation->totalDuration());
			this->foregroundAnimation->stop();
			this->isRunning = false;
		}
	}else{
		emit this->enterAutoMode();
		this->foregroundAnimation->start(QAbstractAnimation::KeepWhenStopped);
	}
	this->background->show();
}

void RpgBanner::hideBanner(){
	if(!this->background->isVisible()){
		qDebug() << CodePath() << "Item was not shown, cannot hide.";
		return;
	}
	this->isRunning = true;
	if(this->speed == SpeedInfinitly){
		if(this->foregroundAnimation->state() != QAbstractAnimation::Stopped){
			this->foregroundAnimation->stop();
			this->foregroundEffect->setOpacity(0.0f);
			this->isRunning = false;
		}
	}else{
		emit this->enterAutoMode();
		this->foregroundAnimation->start(QAbstractAnimation::KeepWhenStopped);
		Utils::msleep(this->foregroundAnimation->duration());
	}
	this->background->hide();
}

void RpgBanner::finished(){
	if(this->foregroundAnimation->state() != QAbstractAnimation::Stopped){
		this->foregroundAnimation->setCurrentTime(this->foregroundAnimation->totalDuration());
		this->foregroundAnimation->stop();
	}
	emit this->quitAutoMode();
	this->canBeInterrupted = false;
	this->isRunning = false;
}

void RpgBanner::receiveKey(int key, Qt::KeyboardModifiers mod){
	if(!this->isRunning){
		return;
	}
	qDebug() << tr("RpgBanner::receiveKey(): receive key: %1::%2(%3).").arg(mod).arg(key).arg(QString(QChar(key)).toHtmlEscaped());
	// 如果设定了允许用户按键打断的标识, 就可以被打断
	if(this->canBeInterrupted){
		//如果按键中断, 直接过渡结束
		if(key == Qt::Key_Return){
			this->finished();
		}
	}
}
