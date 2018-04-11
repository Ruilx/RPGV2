#include "RpgDialog.h"

//RpgDialog *RpgDialog::_instance = nullptr;

RpgDialog::RpgDialog(QGraphicsScene *parentScene, QObject *parent): QObject(parent), RpgDialogBase(){
	this->setGraphicsScene(parentScene);
	this->setTextColor(QColor(Qt::white));
	this->message->setFont(Global::dialogFont);

	this->continueSymbolTimeLine->setFrameRange(0, 3);
	this->continueSymbolTimeLine->setLoopCount(0);

	this->message->document()->setDefaultStyleSheet(this->css);
	//		QTextOption textOption;
	//		textOption.setWrapMode(QTextOption::WrapAnywhere);
	//		this->message->document()->setDefaultTextOption(textOption);
	//		QTextBlockFormat textBlockFormat = this->message->textCursor().blockFormat();
	//		textBlockFormat.setLineHeight(60, QTextBlockFormat::FixedHeight);
	//		QTextCursor textCursor = this->message->textCursor();{
	//			textCursor.setBlockFormat(textBlockFormat);
	//		}
	//		this->message->setTextCursor(textCursor);

	for(int i = 0 ; i < 4; i++){
		if(this->continueSymbolPixmap[i] != nullptr){
			delete this->continueSymbolPixmap[i];
			this->continueSymbolPixmap[i] = nullptr;
		}
		this->continueSymbolPixmap[i] = new QPixmap(QPixmap::fromImage(*this->getContinueSymbol(i)));
	}

	this->box->setZValue(DialogZValue);
	this->message->setZValue(0.1f);
	this->continueSymbol->setZValue(0.2f);
	if(!this->characterBoxPixmap.isNull()){
		this->characterBox->setZValue(-0.1f);
	}

	connect(this->continueSymbolTimeLine, &QTimeLine::frameChanged, this, [this](int frame){
		if(frame < 4 && frame >= 0){
			this->continueSymbol->setPixmap(*this->continueSymbolPixmap[frame]);
		}
	});

	this->messageRect = QRect(messageMarginH, messageMarginV, this->getDialogRect().width() - messageMarginH - messageMarginH, this->getDialogHeight() - messageMarginV - messageMarginV);
	this->message->setTextWidth(this->messageRect.width());
	this->message->setPos(this->messageRect.topLeft());

	// 取消Cache... 看起来更卡了...
	//this->message->setCacheMode(QGraphicsItem::DeviceCoordinateCache);

	// 预置输出速度: 快
	this->slowprint = SpeedFast;

	this->messageShadowEffect->setColor(QColor(Qt::black));
	this->messageShadowEffect->setBlurRadius(5.0f);
	this->messageShadowEffect->setOffset(2.0f, 2.0f);
	this->message->setGraphicsEffect(this->messageShadowEffect);

	//this->boxOpacityEffect->setOpacity(0.0f);
	this->boxOpacityEffect->setOpacity(1.0f);
	this->box->setGraphicsEffect(this->boxOpacityEffect);

	// 头像位置动画设置
	characterAnimation->setItem(this->characterBox);
	characterAnimation->setTimeLine(this->characterTimeLine);

	// 预载入位置, 不过不显示
	this->box->hide();
	this->parentScene->addItem(this->box);
}

void RpgDialog::exec(){
	if(this->parentScene == nullptr){
		qDebug() << "RpgDialog::exec(): parentScene is not set.(Null)";
		return;
	}
	for(QPixmap *p: this->continueSymbolPixmap){
		if(p == nullptr){
			qDebug() << "RpgDialog::exec(): conti2nueSymbolPixmap[4] not set.";
			return;
		}
	}
	if(this->isRunning == true){
		qDebug() << "RpgDialog::exec(): Dialog is still running, please don't call it repeatly!";
		return;
	}
	if(this->messageReadyList.isEmpty()){
		qDebug() << "RpgDialog::exec(): messageReadyList is Empty, show what?";
		return;
	}else{
		this->messageList.clear();
		this->messageList = this->messageReadyList;
	}

	// 设置对话框背景, 支持每次对话框形状不同
	this->box->setPixmap(QPixmap::fromImage(this->getDialogImage()));
	//this->box->setPos(this->getDialogPosition() + this->getViewportOffset());
	this->box->setPos(this->getDialogPosition() + this->parentScene->sceneRect().topLeft());
	//this->box->setZValue(DialogZValue);

	//this->message->setZValue(0.3f);

	this->continueSymbol->setPixmap(*this->continueSymbolPixmap[0]);
	this->continueSymbol->setPos((this->getDialogRect().width() - this->continueSymbolPixmap[0]->width()) / 2, (this->getDialogHeight() - this->continueSymbolPixmap[0]->width()));
	//this->continueSymbol->setZValue(0.2f);
	this->continueSymbol->setVisible(false);

	if(!this->characterBoxPixmap.isNull()){
		this->characterBox->setPixmap(this->characterBoxPixmap);
		this->characterBox->setPos(ScreenWidth - this->characterBox->pixmap().width(), -(ScreenHeight * 0.8 - this->getDialogHeight() - this->getMarginV()) + (ScreenHeight * 0.8 - this->characterBox->pixmap().height()));
		this->characterBox->setFlag(QGraphicsItem::ItemStacksBehindParent, true);
		//this->characterBox->setZValue( - 1.0f);
		//this->characterBox->stackBefore(this->box);
	}

	this->enterOrExitAnimationSetting(true);
	this->showDialog();
}

int RpgDialog::waitingForDialogComplete(){
	if(!this->isRunning){
		qDebug() << CodePath() << ": Not running yet, cannot wait.";
		return -1;
	}
	QEventLoop eventLoop(this);
	this->connect(this, &RpgDialog::quitDialogMode, &eventLoop, &QEventLoop::quit);
	eventLoop.exec();
	return 0;
}

void RpgDialog::showDialog(){
	this->isRunning = true;
	emit this->enterDialogMode();
	this->box->show();
	this->enterOrExitAnimationStart();
	this->showText(this->messageIndex);
}

void RpgDialog::hideDialog(){
	if(this->continueSymbolTimeLine->state() != QTimeLine::NotRunning){
		this->continueSymbolTimeLine->stop();
	}
	this->enterOrExitAnimationStart();
	Utils::msleep(300);
	this->box->hide();
	//this->parentScene->removeItem(this->box);
	this->clearText();
	this->characterBoxPixmap = QPixmap();
	this->characterBox->setPixmap(this->characterBoxPixmap);
	emit this->quitDialogMode();
	this->isRunning = false;
}

void RpgDialog::showText(int index){
	if(index >= this->messageList.length()){
		qDebug() << "RpgDialog::showText(): index is out of range.";
		return;
	}
	QString msg = this->messageList.at(index);
	this->showTextInProgressFlag = true;
	this->continueSymbolTimeLine->stop();
	this->continueSymbol->setVisible(false);
	if(this->slowprint > 0){
		this->quickShowFlag = false;
		for(int i = 0; i < msg.length(); i++){
			this->message->setHtml(msg.left(i + 1));
			Utils::msleep(slowprint);
			if(this->quickShowFlag == true){
				this->message->setHtml(msg);
				this->quickShowFlag = false;
				break;
			}
		}
	}else{
		this->message->setHtml(msg);
	}
	this->showTextInProgressFlag = false;
	this->continueSymbol->setVisible(true);
	this->continueSymbolTimeLine->start();
}

void RpgDialog::enterOrExitAnimationSetting(bool enter){
	if(this->entryAniGroup->state() != QParallelAnimationGroup::Stopped){
		this->entryAniGroup->stop();
	}
	this->entryAniGroup->clear();
	QPropertyAnimation *boxAnimation = new QPropertyAnimation(this->boxOpacityEffect, "opacity"); // 窗口的动画过渡效果
	if(enter){
		// Enter
		boxAnimation->setDuration(300);
		boxAnimation->setEasingCurve(QEasingCurve::OutQuad);
		boxAnimation->setStartValue(0.0f);
		boxAnimation->setEndValue(1.0f);
	}else{
		// Exit
		boxAnimation->setDuration(300);
		boxAnimation->setEasingCurve(QEasingCurve::OutQuad);
		boxAnimation->setStartValue(1.0f);
		boxAnimation->setEndValue(0.0f);
	}
	this->entryAniGroup->addAnimation(boxAnimation);

	if(!this->characterBoxPixmap.isNull()){
		if(this->characterTimeLine->state() != QTimeLine::NotRunning){
			this->characterTimeLine->setCurrentTime(this->characterTimeLine->duration());
			this->characterTimeLine->stop();
			this->characterTimeLine->setCurrentTime(0);
		}
		this->characterTimeLine->setDuration(300);
		this->characterTimeLine->setFrameRange(0, 18);
		QEasingCurve characterEasingCurve(QEasingCurve::OutQuad);

		this->characterAnimation->clear();
		if(enter){
			// Enter
			for(int i = 0; i < 18; i++){
				qreal progress = characterEasingCurve.valueForProgress(i / 18.0f) * 100.0f;
				characterAnimation->setPosAt(i / 18.0f, this->characterBox->pos() + QPoint(100.0f - progress, 0));
			}
		}else{
			// Exit
			for(int i = 0; i < 18; i++){
				qreal progress = characterEasingCurve.valueForProgress(i / 18.0f) * 100.0f;
				characterAnimation->setPosAt(i / 18.0f, this->characterBox->pos() + QPoint(progress, 0));
			}
		}
	}
}

void RpgDialog::enterOrExitAnimationStart(){
	if(this->box->isVisible() == false){
		qWarning() << "Box current visible is false, cannot open animation.";
		return;
	}
	this->entryAniGroup->start(QAbstractAnimation::KeepWhenStopped);
	if(!this->characterBoxPixmap.isNull()){
		if(this->characterTimeLine->state() != QTimeLine::NotRunning){
			this->characterTimeLine->stop();
		}
		this->characterTimeLine->start();
	}
}

void RpgDialog::receiveKey(int key, Qt::KeyboardModifiers mod){
	if(!this->isRunning){
		return;
	}
	qDebug() << tr("RpgDialog::receiveKey(): receive key: %1::%2(%3).").arg(mod).arg(key).arg(QString(QChar(key)).toHtmlEscaped());
	if(key == Qt::Key_Return || key == Qt::Key_Space){
		if(this->showTextInProgressFlag == true){
			// 正在显示字符, 则立即停止slowprint, 直接显示全部的字符
			this->quickShowFlag = true;
		}else{
			// 正在等待玩家操作, 则按下按键之后显示接下来的对话
			this->messageIndex ++;
			if(this->messageIndex >= this->messageList.length()){
				// 如果会话全部完成
				this->enterOrExitAnimationSetting(false);
				this->hideDialog();
				// 重置会话信息位置
				this->messageIndex = 0;
				return;
			}else{
				// 如果还没有, 则显示接下来的对话
				this->showText(this->messageIndex);
			}
		}
	}
}
