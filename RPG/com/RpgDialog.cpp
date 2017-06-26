#include "RpgDialog.h"

//RpgDialog *RpgDialog::_instance = nullptr;

RpgDialog::RpgDialog(QGraphicsScene *parentScene, QObject *parent): QObject(parent), RpgDialogBase(){
	//this->parentScene = parentScene;
	this->setGraphicsScene(parentScene);
	this->setTextColor(QColor(Qt::white));
	//		QFont font = QApplication::font();
	//		font.setPixelSize(22);
	//		this->setFont(font);
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

	connect(this->continueSymbolTimeLine, &QTimeLine::frameChanged, this, [this](int frame){
		if(frame < 4 && frame >= 0){
			this->continueSymbol->setPixmap(*this->continueSymbolPixmap[frame]);
		}
	});

	this->messageRect = QRect(messagePaddingH, messagePaddingV, this->getDialogRect().width() - messagePaddingH - messagePaddingH, this->getDialogHeight() - messagePaddingV - messagePaddingV);
	// 取消Cache... 看起来更卡了...
	//this->message->setCacheMode(QGraphicsItem::DeviceCoordinateCache);


	// 预置输出速度: 快
	this->slowprint = SpeedFast;

	this->messageShadowEffect->setColor(QColor(Qt::black));
	this->messageShadowEffect->setBlurRadius(5.0f);
	this->messageShadowEffect->setOffset(2.0f, 2.0f);
	this->message->setGraphicsEffect(this->messageShadowEffect);
}

void RpgDialog::exec(){
	if(this->parentScene == nullptr){
		qDebug() << "Rpgdialog::exec(): parentScene is not set.(Null)";
		return;
	}
	for(QPixmap *p: this->continueSymbolPixmap){
		if(p == nullptr){
			qDebug() << "Rpgdialog::exec(): continueSymbolPixmap[4] not set.";
			return;
		}
	}
	if(this->isRunning == true){
		qDebug() << "Dialog is still running, please don't call it repeatly!";
		return;
	}

	this->box->setPixmap(QPixmap::fromImage(this->getDialogImage()));
	this->box->setPos(this->getDialogPosition() + this->getViewportOffset());
	this->box->setZValue(DialogZValue + 0.0f);

	this->message->setTextWidth(this->messageRect.width());
	this->message->setPos(this->messageRect.topLeft());
	this->message->setZValue(DialogZValue + 1.0f);

	this->continueSymbol->setPixmap(*this->continueSymbolPixmap[0]);
	this->continueSymbol->setPos((this->getDialogRect().width() - this->continueSymbolPixmap[0]->width()) / 2, (this->getDialogHeight() - this->continueSymbolPixmap[0]->width()));
	this->continueSymbol->setZValue(DialogZValue + 1.0f);
	this->continueSymbol->setVisible(false);

	this->showDialog();
}

void RpgDialog::showDialog(){
	this->isRunning = true;
	emit this->enterDialogMode();
	this->parentScene->addItem(this->box);
	this->showText(this->messageIndex);
}

void RpgDialog::hideDialog(){
	if(this->continueSymbolTimeLine->state() != QTimeLine::NotRunning){
		this->continueSymbolTimeLine->stop();
	}
	this->box->hide();
	this->clearText();
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

void RpgDialog::receiveKey(int key, Qt::KeyboardModifiers mod){
	qDebug() << tr("RpgDialog::receiveKey(): receive key: %1::%2(%3).").arg(mod).arg(key).arg(QString(QChar(key)).toHtmlEscaped());
	if(key == Qt::Key_Return || key == Qt::Key_Space){
		if(this->showTextInProgressFlag  == true){
			// 正在显示字符, 则立即停止slowprint, 直接显示全部的字符
			this->quickShowFlag = true;
		}else{
			// 正在等待玩家操作, 则按下按键之后显示接下来的对话
			this->messageIndex ++;
			if(this->messageIndex >= this->messageList.length()){
				// 如果会话全部完成
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
