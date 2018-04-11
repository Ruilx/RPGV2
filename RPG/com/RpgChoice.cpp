#include "RpgChoice.h"



RpgChoice::RpgChoice(QGraphicsScene *parentScene, QObject *parent): QObject(parent), RpgDialogBase(){
	this->setGraphicsScene(parentScene);
	for(int i = 0; i < ChoiceBuff; i++){
		if(this->messages[i] == nullptr){
			// 字下阴影的设置
			this->messageShadowEffect[i] = new QGraphicsDropShadowEffect(this);
			this->messageShadowEffect[i]->setColor(QColor(Qt::black));
			this->messageShadowEffect[i]->setBlurRadius(5.0f);
			this->messageShadowEffect[i]->setOffset(2.0f, 2.0f);
			// 文字框设置
			this->messages[i] = new QGraphicsTextItem(this->box);
			this->messages[i]->document()->setDefaultStyleSheet(this->css);
			this->messages[i]->setGraphicsEffect(this->messageShadowEffect[i]);
		}
	}
	this->setTextColor(QColor(Qt::white));
	this->setFont(Global::dialogFont);

	this->upSymbolTimeLine->setFrameRange(0, 3);
	this->upSymbolTimeLine->setLoopCount(0);
	this->downSymbolTimeLine->setFrameRange(0, 3);
	this->downSymbolTimeLine->setLoopCount(0);

	//	foreach(QGraphicsTextItem *i, this->messages){
	//		i = new QGraphicsTextItem(this->box);
	//		i->document()->setDefaultStyleSheet(this->css);
	//	}

	QMatrix upDownMirror;
	upDownMirror.rotate(180);
	for(int i = 0; i < 4; i++){
		if(this->downSymbolPixmap[i] != nullptr){
			delete this->downSymbolPixmap[i];
			this->downSymbolPixmap[i] = nullptr;
		}
		QPixmap triangle = QPixmap::fromImage(*this->getContinueSymbol(i));
		this->downSymbolPixmap[i] = new QPixmap(triangle);
		if(this->upSymbolPixmap[i] != nullptr){
			delete this->upSymbolPixmap[i];
			this->downSymbolPixmap[i] = nullptr;
		}
		QPixmap upTriangle = triangle.transformed(upDownMirror, Qt::SmoothTransformation);
		this->upSymbolPixmap[i] = new QPixmap(upTriangle);
	}

	this->box->setZValue(DialogZValue);
	for(QGraphicsTextItem *i: this->messages){
		i->setZValue(0.1f);
	}
	this->upSymbol->setZValue(0.2f);
	this->downSymbol->setZValue(0.3f);
	this->choiceSymbol->setZValue(0.5f);

	connect(this->upSymbolTimeLine, &QTimeLine::frameChanged, this, [this](int frame){
		if(frame < 4 && frame >= 0){
			this->upSymbol->setPixmap(*this->upSymbolPixmap[frame]);
		}
	});
	connect(this->downSymbolTimeLine, &QTimeLine::frameChanged, this, [this](int frame){
		if(frame < 4 && frame >= 0){
			this->downSymbol->setPixmap(*this->downSymbolPixmap[frame]);
		}
	});

	this->choiceSymbol->setGraphicsEffect(this->choiceBarOpacityEffect);
	this->choiceBarAnimation->setEasingCurve(QEasingCurve::Linear);
	this->choiceBarAnimation->setStartValue(1.0f);
	this->choiceBarAnimation->setEndValue(1.0f);
	this->choiceBarAnimation->setKeyValueAt(0.5, 0.0f);
	this->choiceBarAnimation->setDuration(1500);
	this->choiceBarAnimation->setLoopCount(-1);

	this->messagesRect = QRect(messageMarginH, messageMarginV, this->getDialogRect().width() - messageMarginH - messageMarginH, this->getDialogHeight() - messageMarginV - messageMarginV);
	for(int i = 0; i < ChoiceBuff; i++){
		this->messages[i]->setTextWidth(this->messagesRect.width());
		//this->messages[i]->setPos(this->messagesRect.topLeft() + QPoint(0, this->messagesRect.height() + this->messageSpecingV * i));
		this->messages[i]->setPos(messageMarginH, messageMarginV + (messageSpecingV + this->getSelectBarImage().height()) * i);
	}

	// 预置输出速度: 快
	this->slowprint = SpeedFast;

	// 选择框的进场透明渐变
	this->boxOpacityEffect->setOpacity(1.0f);
	this->box->setGraphicsEffect(this->boxOpacityEffect);

	// 预载入位置, 不过不显示
	this->box->hide();
	this->parentScene->addItem(this->box);
}

void RpgChoice::exec(){
	if(this->parentScene == nullptr){
		qDebug() << CodePath() << ": parentScene is not set.(Null)";
		return;
	}
	for(QPixmap *p: this->upSymbolPixmap){
		if(p == nullptr){
			qDebug() << CodePath() << ": upSymbolPixmap[4] not set.";
			return;
		}
	}
	for(QPixmap *p: this->downSymbolPixmap){
		if(p == nullptr){
			qDebug() << CodePath() << ": downSymbolPixmap[4] not set.";
			return;
		}
	}
	if(this->isRunning == true){
		qDebug() << CodePath() << ": Choice is still running, please don't call it repeadly!";
		return;
	}
	if(this->messageReadyList.isEmpty()){
		qDebug() << CodePath() << ": messageReadyList is Empty, show what?";
		return;
	}else{
		this->messageList.clear();
		this->messageList = this->messageReadyList;
	}

	// 设置对话框背景, 支持每次对话框形状不同
	this->box->setPixmap(QPixmap::fromImage(this->getDialogImage()));
	this->box->setPos(this->getDialogPosition() + this->parentScene->sceneRect().topLeft());

	this->choiceSymbol->setPixmap(QPixmap::fromImage(this->getSelectBarImage()));

	//		for(int i = 0; i < ChoiceBuff; i++){
	//			this->messages[i]->setTextWidth(this->messagesRect.width());
	//			this->messages[i]->setPos(this->messagesRect.topLeft() + QPoint(0, this->messagesRect.height() + this->messageSpecingV) * i);
	//		}

	this->upSymbol->setPixmap(*this->upSymbolPixmap[0]);
	this->upSymbol->setPos((this->getDialogRect().width() - this->upSymbolPixmap[0]->width()) / 2, /*this->upSymbolPixmap[0]->height() / 2*/ 0);
	this->upSymbol->setVisible(false);

	this->downSymbol->setPixmap(*this->downSymbolPixmap[0]);
	this->downSymbol->setPos((this->getDialogRect().width() - this->downSymbolPixmap[0]->width()) / 2, this->getDialogRect().height() - this->downSymbolPixmap[0]->height());
	this->downSymbol->setVisible(false);

	this->enterOrExitAnimationSetting(true);
	this->showChoice();
}

int RpgChoice::waitingForChooseComplete(int second){
	if(!this->isRunning){
		qDebug() << CodePath() << ": Not running yet, cannot wait.";
		return -1;
	}
	QEventLoop eventLoop(this);
	this->connect(this, &RpgChoice::quitDialogMode, &eventLoop, &QEventLoop::quit);
	if(second > 0){
		QTimer::singleShot(second * 1000, &eventLoop, &QEventLoop::quit);
	}
	eventLoop.exec();
	return this->messageChose;
}

void RpgChoice::showChoice(){
	this->isRunning = true;
	emit this->enterDialogMode();
	this->showChoiceBox(this->messageCurrentIndex);
	this->box->show();
	if(this->choiceBarAnimation->state() == QAbstractAnimation::Stopped){
		this->choiceBarAnimation->start();
	}
	this->enterOrExitAnimationStart();
	this->showText(this->messageIndex, false);
	this->slowprintBackup = this->slowprint;
	this->setSlowprint(SpeedInfinitly);
}

void RpgChoice::hideChoice(){
	if(this->upSymbolTimeLine->state() != QTimeLine::NotRunning){
		this->upSymbolTimeLine->stop();
	}
	if(this->downSymbolTimeLine->state() != QTimeLine::NotRunning){
		this->downSymbolTimeLine->stop();
	}
	if(this->choiceBarAnimation->state() != QAbstractAnimation::Stopped){
		this->choiceBarAnimation->stop();
	}
	this->enterOrExitAnimationStart();
	Utils::msleep(300);
	this->box->hide();
	this->clearChoiceText();
	this->setSlowprint(this->slowprintBackup);
	this->slowprintBackup = 0;
	for(QGraphicsTextItem *i: this->messages){
		i->document()->clear();
	}
	emit this->quitDialogMode();
	this->isRunning = false;
}

void RpgChoice::showText(int index, bool noSpeedRender){
	if(index > (this->messageList.length() <= ChoiceBuff ? 0 : this->messageList.length() - ChoiceBuff)){
		/* 如果列表1项, 显示内存3项, 则index只能在[0]中选择
			 * 如果列表2项, 显示内存3项, 则index只能在[0]中选择
			 * 如果列表3项, 显示内存3项, 则index只能在[0]中选择
			 * 如果列表4项, 显示内存3项, 则index能在[0, 1]中选择
			 * 如果列表5项, 显示内存3项, 则index能在[0, 1, 2]中选择
			 */
		qDebug() << CodePath() << ": index is out of range.";
		return;
	}
	QStringList msgs;
	for(int i = 0; i < qMin(ChoiceBuff, this->messageList.length()); i++){
		msgs.append(this->messageList.at(index + i));
	}

	if(noSpeedRender){
		for(int i = 0; i < qMin(ChoiceBuff, this->messageList.length()); i++){
			this->messages[i]->setHtml(msgs.at(i));
		}
	}else{
		this->showTextInProgressFlag = true;
		if(this->upSymbolTimeLine->state() != QTimeLine::NotRunning){
			this->upSymbolTimeLine->stop();
			this->downSymbolTimeLine->stop();
		}
		if(this->downSymbolTimeLine->state() != QTimeLine::NotRunning){
			this->upSymbol->setVisible(false);
			this->downSymbol->setVisible(false);
		}
		if(this->slowprint > 0){
			this->quickShowFlag = false;
			for(int i = 0; i < qMin(ChoiceBuff, this->messageList.length()); i++){
				for(int j = 0; j < msgs.at(i).length(); j++){
					this->messages[i]->setHtml(msgs.at(i).left(j + 1));
					Utils::msleep(slowprint);
					if(this->quickShowFlag == true){
						for(; i < qMin(ChoiceBuff, this->messageList.length()); i++){
							this->messages[i]->setHtml(msgs.at(i));
						}
						this->quickShowFlag = false;
						break;
					}
				}
			}
		}else{
			for(int i = 0; i < ChoiceBuff; i++){
				this->messages[i]->setHtml(msgs.at(i));
			}
		}
		this->showTextInProgressFlag = false;
	}

	if(index > 0){
		if(this->upSymbolTimeLine->state() == QTimeLine::NotRunning){
			this->upSymbol->setVisible(true);
			this->upSymbolTimeLine->start();
		}
	}else{
		if(this->upSymbolTimeLine->state() != QTimeLine::NotRunning){
			this->upSymbol->setVisible(false);
			this->upSymbolTimeLine->stop();
		}
	}
	if(index < (this->messageList.length() <= ChoiceBuff ? 0 : this->messageList.length() - ChoiceBuff)){
		if(this->downSymbolTimeLine->state() == QTimeLine::NotRunning){
			this->downSymbol->setVisible(true);
			this->downSymbolTimeLine->start();
		}
	}else{
		if(this->downSymbolTimeLine->state() != QTimeLine::NotRunning){
			this->downSymbol->setVisible(false);
			this->downSymbolTimeLine->stop();
		}
	}
}

void RpgChoice::showChoiceBox(int index){
	if(index < 0 || index >= qMin(ChoiceBuff, this->messageList.length())){
		qDebug() << CodePath() << ": index is out of range: " << index << "</>[0," << ChoiceBuff << "]";
	}
	this->choiceSymbol->setPos(messageMarginH, this->messagesRect.top() + messageMarginV + (messageSpecingV + this->getSelectBarImage().height()) * index);
	//		if(!this->choiceSymbol->isVisible()){
	//			this->choiceSymbol->setVisible(true);
	//		}
	//		if(this->choiceBarAnimation->state() != QAbstractAnimation::Running){
	//			this->choiceBarAnimation->start();
	//		}
}

void RpgChoice::enterOrExitAnimationSetting(bool enter){
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
}

void RpgChoice::enterOrExitAnimationStart(){
	if(this->box->isVisible() == false){
		qWarning() << "Box current visible is false, cannot open animation.";
		return;
	}
	this->entryAniGroup->start(QAbstractAnimation::KeepWhenStopped);
}

void RpgChoice::receiveKey(int key, Qt::KeyboardModifiers mod){
	if(!this->isRunning){
		return;
	}
	qDebug() << CodePath() << tr(": receive key: %1::%2(%3).").arg(mod).arg(key).arg(QString(QChar(key)).toHtmlEscaped());
	if(key == Qt::Key_Return || key == Qt::Key_Space){
		if(this->showTextInProgressFlag == true){
			// 正在显示字符, 则立即停止slowprint, 直接显示全部的字符
			this->quickShowFlag = true;
		}else{
			// 如果不是, 则意味着用户选中了这个选项, 可以关闭选项框了
			this->enterOrExitAnimationSetting(false);
			this->hideChoice();
			// 重置会话信息位置
			this->messageChose = this->messageIndex + this->messageCurrentIndex;
			this->messageCurrentIndex = 0;
			this->messageIndex = 0;
		}
	}else if(key == Qt::Key_Up || key == Qt::Key_W){
		// 显示上一个选项 [光标希望向上走!]
		if(this->messageList.length() == 1){
			// 如果列表只有一个选项, 啥都不做
			return;
		}else{
			// 大于一个选项, 往上没有需要注意的问题
			if(this->messageIndex == 0){
				// 正在显示 0 - 2 项
				if(this->messageCurrentIndex == 0){
					// 光标在第一个(光标和列表都不能往上了)
					qDebug() << CodePath() << ": First choice first, cannot up.";
					return;
				}else{
					// 光标不在第一个(就可以正常往上了)
					this->messageCurrentIndex--;
					this->showChoiceBox(this->messageCurrentIndex);
				}
			}else{
				// 正在显示 1 项以上(不在第一个)
				if(this->messageCurrentIndex == 0){
					// 光标在第一个(光标不能往上了)
					// 向上滚动一行显示的内容, 光标位置不变
					this->messageIndex--;
					this->showText(this->messageIndex);
				}else{
					// 光标不在第一个选项上(就可以正常往上了)
					this->messageCurrentIndex--;
					this->showChoiceBox(this->messageCurrentIndex);
				}
			}
		}
	}else if(key == Qt::Key_Down || key == Qt::Key_S){
		// 显示下一个选项 [光标希望向下走!]
		if(this->messageList.length() == 1){
			// 如果列表只有一个选项, 啥都不做
			return;
		}else{
			// 大于一个选项需要注意光标向下的条件是min(ChoiceBuff 和 this->messageList->length())最小值作为长度
			if(this->messageIndex >= (this->messageList.length() <= ChoiceBuff ? 0 : this->messageList.length() - ChoiceBuff)){
				// 正在显示后三项
				if(this->messageCurrentIndex == qMin(ChoiceBuff, this->messageList.length()) -1){
					// 光标在最后一个了(光标和列表都不能往下了)
					qDebug() << CodePath() << ": Last choice last, cannot down.";
					return;
				}else{
					// 光标不在最后一个(就可以正常往下了)
					this->messageCurrentIndex++;
					this->showChoiceBox(this->messageCurrentIndex);
				}
			}else{
				// 正在显示非后三项(不在最后几个)
				if(this->messageCurrentIndex == qMin(ChoiceBuff, this->messageList.length()) -1){
					// 光标不能往下了
					// 向下滚动一行显示的内容, 光标位置不变
					this->messageIndex++;
					this->showText(this->messageIndex);
				}else{
					// 光标不在最后一个选项上(就可以正常往下了)
					this->messageCurrentIndex++;
					this->showChoiceBox(this->messageCurrentIndex);
				}
			}
		}
	}
}
