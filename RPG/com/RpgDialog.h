#ifndef RPGDIALOG_H
#define RPGDIALOG_H

#include <QtCore>
#include <QtWidgets>
#include <RPG/About.h>
#include <RPG/Global.h>
#include <RPG/utils/Utils.h>
#include <RPG/core/RpgDialogBase.h>

#define DialogZValue 100.0f

class RpgDialog : public QObject, public RpgDialogBase
{
	Q_OBJECT
	// 标志: 是否在运行
	bool isRunning = false;
	// 指定显示在哪个Scene上
	QGraphicsScene *parentScene = nullptr;
	// 构成
	QGraphicsPixmapItem *box = new QGraphicsPixmapItem(nullptr);
	QGraphicsTextItem *message = new QGraphicsTextItem(this->box);
	QGraphicsPixmapItem *continueSymbol = new QGraphicsPixmapItem(this->box);

	// 消息列表
	QStringList messageList;	//消息存储列表
	int messageIndex = 0;		//现在正在显示的消息序号

	// 消息
	int messagePaddingH = 10;
	int messagePaddingV = 6;
	QRect messageRect = QRect(messagePaddingH, messagePaddingV, ScreenWidth - messagePaddingH - messagePaddingH, ScreenHeight - messagePaddingV - messagePaddingV);

	// 单字输出/速度
	// 速度列表
public:
	enum Speed{
		SpeedSlow = 55,
		SpeedNormal = 30,
		SpeedFast = 17,
		SpeedInfinitly = 0
	};
private:
	int slowprint = 0;			// 是否使用单字输出
	bool showTextInProgressFlag = false; // (流程Flag)正在单字输出显示文字状态
	bool quickShowFlag = false; // (流程Flag)被用户中断, 下一次将剩下的字符全部显示状态

	// 文字CSS
	const QString css = "red{ color: #FF3333; }green{ color: #33FF33; }blue{ color: #9999FF; }";

	// 继续显示的三角形
	QTimeLine *continueSymbolTimeLine = new QTimeLine(1000, this);

	// 需要预先转换成Pixmap的Image
	QPixmap dialogPixmap;
	QPixmap *continueSymbolPixmap[4] = {nullptr};

public:
	// 返回单例模式
	static RpgDialog *getInstance(){
		if(_instance == nullptr){
			_instance = new RpgDialog();
		}
		return _instance;
	}
protected:
	// 构造
	explicit RpgDialog(QGraphicsScene *parentScene = nullptr, QObject *parent = 0): QObject(parent), RpgDialogBase(){
		this->parentScene = parentScene;
		this->setTextColor(QColor(Qt::white));
		QFont font = QApplication::font();
		font.setPixelSize(22);
		this->setFont(font);

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

	}
public:
	// 在文字列表中添加文字
	void addText(const QString &text){
		this->messageList.append(text);
	}

	// 设置缓慢输出
	void setSlowprint(int speed){
		this->slowprint = speed;
	}

	// 设置字体
	void setFont(const QFont &font){
		this->message->setFont(font);
	}

	// 设置文字颜色
	void setTextColor(const QColor &color){
		this->message->setDefaultTextColor(color);
	}

	// 清除文字列表
	void clearText(){
		this->messageList.clear();
	}

	// 设置映射到屏幕Scene
	void setGraphicsScene(QGraphicsScene *scene){
		this->parentScene = scene;
	}

	// 执行且进入Dialog模式
	void exec(){
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
		this->box->setPos(this->getDialogPosition());
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

	// 显示Dialog函数
	void showDialog(){
		this->isRunning = true;
		emit this->enterDialogMode();
		this->parentScene->addItem(this->box);
		this->showText(this->messageIndex);
	}

	// 隐藏Dialog函数
	void hideDialog(){
		if(this->continueSymbolTimeLine->state() != QTimeLine::NotRunning){
			this->continueSymbolTimeLine->stop();
		}
		this->box->hide();
		this->clearText();
		emit this->quitDialogMode();
		this->isRunning = false;
	}
protected:
	// 显示文字函数
	void showText(int index){
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


signals:
	// 信号: 进入Dialog模式
	void enterDialogMode();
	// 信号: 退出Dialog模式
	void quitDialogMode();
public slots:
	void receiveKey(int key, Qt::KeyboardModifiers mod){
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

private:
	// 单例模式
	static RpgDialog *_instance;
};

#endif // RPGDIALOG_H
