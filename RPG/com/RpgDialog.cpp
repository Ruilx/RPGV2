#include "RpgDialog.h"

//RpgDialog *RpgDialog::_instance = nullptr;

RpgDialog::RpgDialog(QGraphicsScene *parentScene, QObject *parent): RpgObject(parentScene, parent, nullptr){
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

	// ZValue设置
	this->setZValue(DialogZValue);
	this->box->setZValue(0.0f);
	this->message->setZValue(0.1f);
	this->continueSymbol->setZValue(0.2f);
	if(!this->characterBoxPixmap.isNull()){
		this->characterBox->setZValue(-0.1f);
	}

	// 三角形动画时间轴
	connect(this->continueSymbolTimeLine, &QTimeLine::frameChanged, this, [this](int frame){
		if(frame < 4 && frame >= 0){
			this->continueSymbol->setPixmap(this->skin.getContinueSymbolImage(frame));
		}
	});

	//this->setDialogRect(QRect(this->getDialogPosition().toPoint(), QSize(400, 125)));
	//this->setDialogSize(QSize(ScreenWidth - MarginH - Margin, Height));

	// 对话框坐标
	this->dialogPos = QPoint(this->marginH, ScreenHeight - this->marginV - this->skin.getDialogSize().height());

	// 消息框大小
	this->messageRect = QRect(messageMarginH, messageMarginV, this->skin.getDialogSize().width() - (messageMarginH << 1), this->skin.getDialogSize().height() - (messageMarginV << 1));

	this->message->setTextWidth(this->messageRect.width());
	this->message->setPos(this->messageRect.topLeft());

	// 取消Cache... 看起来更卡了...
	//this->message->setCacheMode(QGraphicsItem::DeviceCoordinateCache);

	// 预置输出速度: 快
	this->slowprint = SpeedFast;

	// 字下阴影
	this->messageShadowEffect->setColor(QColor(Qt::black));
	this->messageShadowEffect->setBlurRadius(5.0f);
	this->messageShadowEffect->setOffset(2.0f, 2.0f);
	this->message->setGraphicsEffect(this->messageShadowEffect);

	// 窗口过渡句柄
	//this->boxOpacityEffect->setOpacity(0.0f);
	this->boxOpacityEffect->setOpacity(1.0f);
	this->box->setGraphicsEffect(this->boxOpacityEffect);

	// 头像位置动画设置
	characterAnimation->setItem(this->characterBox);
	characterAnimation->setTimeLine(this->characterTimeLine);

	this->setFlag(QGraphicsItem::ItemIgnoresTransformations);
}

void RpgDialog::exec(){
	RpgObject::exec();
	if(this->getParentScene() == nullptr){
		qDebug() << CodePath() << "RpgDialog::exec(): parentScene is not set.(Null)";
		return;
	}
	for(int i = 0; i < 4; i++){
		if(this->skin.getContinueSymbolImage(i).isNull()){
			qDebug() << CodePath() << "RpgDialog::exec(): conti2nueSymbolPixmap[4] not set.";
			return;
		}
	}
	if(this->getProcessing() == true){
		qDebug() << CodePath() << "RpgDialog::exec(): Dialog is still running, please don't call it repeatly!";
		return;
	}
	if(this->messageReadyList.isEmpty()){
		qDebug() << CodePath() << "RpgDialog::exec(): messageReadyList is Empty, show what?";
		return;
	}else{
		this->messageList.clear();
		this->messageList = this->messageReadyList;
	}

	// 设置对话框背景, 支持每次对话框形状不同
	this->box->setPixmap(this->skin.getDialogImage());
	this->box->setPos(this->dialogPos);

	this->continueSymbol->setPixmap(this->skin.getContinueSymbolImage(0));
	this->continueSymbol->setPos((this->skin.getDialogSize().width() - this->skin.getContinueSymbolSize().width()) / 2, (this->skin.getDialogSize().height() - this->skin.getContinueSymbolSize().width()));
	//this->continueSymbol->setZValue(0.2f);
	this->continueSymbol->setVisible(false);

	if(this->characterBoxPixmap.isNull() && this->skin.getDialogSize().width() >= (ScreenWidth - (marginH << 1))){
		if(this->skin.getDialogSize().width() < (ScreenWidth - (marginH << 1))){
			qDebug() << CodePath() << "The dialog position is not fit to show the character avatar, it must more than screenWidth - most twice of marginH.";
		}
	}else{
		this->characterBox->setPixmap(this->characterBoxPixmap);
		if(this->avatarAround == Avatar_BehindDialog){
			this->characterBox->setPos(ScreenWidth - this->characterBox->pixmap().width(), -(ScreenHeight * 0.8 - this->skin.getDialogSize().height() - this->marginV) + (ScreenHeight * 0.8 - this->characterBox->pixmap().height()));
			this->characterBox->setFlag(QGraphicsItem::ItemStacksBehindParent, true);
			//this->characterBox->setZValue( - 1.0f);
			//this->characterBox->stackBefore(this->box);
		}else if(this->avatarAround == Avatar_FrontDialog){
			this->characterBox->setPos(ScreenWidth - this->characterBox->pixmap().width(), -(ScreenHeight * 0.8 - this->skin.getDialogSize().height() - this->marginV) + (ScreenHeight * 0.8 - this->characterBox->pixmap().height()));
			this->characterBox->setFlag(QGraphicsItem::ItemStacksBehindParent, false);
		}
	}

	this->enterOrExitAnimationSetting(true);
	this->showDialog();
}

int RpgDialog::waitingForDialogComplete(){
	if(!this->getProcessing()){
		qDebug() << CodePath() << ": Not running yet, cannot wait.";
		return -1;
	}
	QEventLoop eventLoop(this);
	this->connect(this, &RpgDialog::quitDialogMode, &eventLoop, &QEventLoop::quit);
	eventLoop.exec();
	return 0;
}

void RpgDialog::showDialog(){
	this->setProcessing(true);
	emit this->enterDialogMode();
	//this->box->show();
	this->show();
	this->enterOrExitAnimationStart();
	this->showText(this->messageIndex);
}

void RpgDialog::hideDialog(){
	this->setProcessing(false);
	if(this->continueSymbolTimeLine->state() != QTimeLine::NotRunning){
		this->continueSymbolTimeLine->stop();
	}
	this->enterOrExitAnimationStart();
	Utils::msleep(300);
	//this->box->hide();
	this->hide();
	//this->parentScene->removeItem(this->box);
	this->clearText();
	this->characterBoxPixmap = QPixmap();
	this->characterBox->setPixmap(this->characterBoxPixmap);
	emit this->quitDialogMode();

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
	//if(this->box->isVisible() == false){
	if(this->isVisible() == false){
		qWarning() << CodePath() << "Box current visible is false, cannot open animation.";
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
	if(!this->getProcessing()){
		return;
	}
	if(mod != Qt::NoModifier){
		return;
	}
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
