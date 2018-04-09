#ifndef RPGCHOICE_H
#define RPGCHOICE_H

#include <QtCore>
#include <QtWidgets>
#include <QPropertyAnimation>
#include <RPG/About.h>
#include <RPG/Global.h>
#include <RPG/utils/Utils.h>
#include <RPG/core/RpgDialogBase.h>

/**
 * @brief The RpgChoice class
 * RPGChoice类
 * 是在GraphicsScene上形成一个类似对话框的文字选择器
 * 是一个单独的线程控制文字输出
 * 当设定好显示的设置文字之后, 之后就交给RpgChoice线程进行显示, 同时调用者会收到进入DialogMode信号
 * 然后改变输入定向, 并等待RPGChoice结束, 发送退出DialogMode的信号
 *
 *
 */

class RpgChoice : public QObject, public RpgDialogBase
{
	Q_OBJECT
	// 标志: 是否在运行
	bool isRunning = false;
	// 指定显示在哪个Scene上
	QGraphicsScene *parentScene = nullptr;

	const int choiseBuff = 3;		// 默认一页显示几个选项
	// 构成
	QGraphicsPixmapItem *box = new QGraphicsPixmapItem(nullptr);
	QGraphicsTextItem *messages[this->choiseBuff] = {nullptr};
	QGraphicsPixmapItem *upSymbol = new QGraphicsPixmapItem(this->box);
	QGraphicsPixmapItem *downSymbol = new QGraphicsPixmapItem(this->box);

	QGraphicsPixmapItem *choiceSymbol = new QGraphicsPixmapItem(this->box);

	QGraphicsDropShadowEffect *messageShadowEffect = new QGraphicsDropShadowEffect(this);
	QGraphicsOpacityEffect *choiceBarOpacityEffect = new QGraphicsOpacityEffect(this);
	QPropertyAnimation *choiceBarAnimation = new QPropertyAnimation(this->choiceBarOpacityEffect, "opacity", this);

	// 选项列表
	QStringList messageList;		// 选项列表
	QStringList messageReadyList;	// 选项缓存
	int messageIndex = 0;			// 现在处于第一显示的选项序号[0, len-choiceBuff]
	int messageCurrentIndex = 0;	// 现在选项的序号[0, choiceBuff]

	// 消息
	int messageMarginH = 10;
	int messageMarginV = 6;
	int messageSpecingV = 0;
	QRect messagesRect = QRect(messageMarginH, messageMarginV, ScreenWidth - messageMarginH - messageMarginH, ScreenHeight - messageMarginV - messageMarginV);

	// Viewport Offset?
	QPointF viewportOffset = QPoint(0, 0);

public:
	/**
	 * @brief RpgChoice
	 * 单字输出模式, 预先定义快中慢三个节奏
	 */
	enum Speed{
		SpeedSlow = 55,
		SpeedNormal = 30,
		SpeedFast = 17,
		SpeedInfinitly = 0
	};
private:
	// Num: 单字输出
	int slowprint = 0;
	// Flag: 输出的一个状态
	bool showTextInProgressFlag = false;
	// Flag: 被用户中断, 下一次将显示全部信息
	bool quickShowFlag = false;

	// String: 文字CSS
	const QString css = "r{ color: #FF3333; }g{ color: #33FF33; }b{ color: #9999FF; }y{ color: #EEEE33 }";

	// 上下显示的三角形
	QTimeLine *upSymbolTimeLine = new QTimeLine(1000, this);
	QTimeLine *downSymbolTimeLine = new QTimeLine(1000, this);

	// 对话框图片
	QPixmap dialogPixmap;
	QPixmap *upSymbolPixmap[4] = {nullptr};
	QPixmap *downSymbolPixmap[4] = {nullptr};

public:
	/**
	 * @brief RpgChoice RPG对话选择窗口构造
	 * @param parentScene 设置显示到的Scene
	 * @param parent QObject要用的
	 */
	explicit RpgChoice(QGraphicsScene *parentScene = nullptr, QObject *parent = 0): QObject(parent), RpgDialogBase(){
		this->setGraphicsScene(parentScene);
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
		for(int i = 0; i < this->choiseBuff; i++){
			if(this->messages[i] == nullptr){
				this->messages[i] = new QGraphicsTextItem(this->box);
				this->messages[i]->document()->setDefaultStyleSheet(this->css);
			}
		}
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
		this->choiceBarAnimation->setStartValue(0);
		this->choiceBarAnimation->setEndValue(0);
		this->choiceBarAnimation->setKeyValueAt(0.5, 1);
		this->choiceBarAnimation->setDuration(2000);

		//	for(int i = 0; i < this->choiseBuff; i++){
		//		this->messagesRect
		//	}
		this->messagesRect = QRect(messageMarginH, messageMarginV, this->getDialogRect().width() - messageMarginH - messageMarginH, this->getDialogHeight() - messageMarginV - messageMarginV);

		this->slowprint = SpeedFast;
		this->messageShadowEffect->setColor(QColor(Qt::black));
		this->messageShadowEffect->setBlurRadius(5.0f);
		this->messageShadowEffect->setOffset(2.0f, 2.0f);
		for(int i = 0; i < this->choiseBuff; i++){
			this->messages[i]->setGraphicsEffect(this->messageShadowEffect);
		}
	}
public:
	/**
	 * @brief addChoiceText
	 * @param text 添加的选择
	 */
	void addChoiceText(const QString &text){ this->messageReadyList.append(text); }
	/**
	 * @brief addChoiceText
	 * @param textList 添加一串选择
	 */
	void addChoiceText(const QStringList &textList){ this->messageReadyList.append(textList); }
	/**
	 * @brief setSlowprint
	 * @param speed速度
	 */
	void setSlowprint(int speed){ this->slowprint = speed; }
	/**
	 * @brief setFont 设置字体
	 * @param font
	 */
	void setFont(const QFont &font){
//		foreach(QGraphicsTextItem *a, this->messages){
//			a->setFont(font);
//		}
		for(int i = 0; i < ChoiseBuff; i++){
			this->messages[i]->setFont(font);
		}
	}
	/**
	 * @brief setTextColor 设置字体颜色
	 * @param color
	 */
	void setTextColor(const QColor &color){
//		foreach(QGraphicsTextItem *a, this->messages){
//			a->setDefaultTextColor(color);
//		}
		for(int i = 0; i < ChoiseBuff; i++){
			this->messages[i]->setDefaultTextColor(color);
		}
	}
	/**
	 * @brief getViewportOffset
	 * @return
	 * 获得Viewport的偏移坐标
	 */
	QPointF getViewportOffset() const { return this->viewportOffset; }
	/**
	 * @brief setViewportOffset
	 * @param offset
	 * 设置Viewport的偏移坐标
	 */
	void setViewportOffset(const QPointF &offset){ this->viewportOffset = offset; }
	/**
	 * @brief clearChoiceText
	 * 清除文字选项
	 */
	void clearChoiceText(){ this->messageList.clear(); this->messageReadyList.clear(); }
	/**
	 * @brief setGraphicsScene
	 * @param scene
	 */
	void setGraphicsScene(QGraphicsScene *scene){
		this->parentScene = scene;
	}
	/**
	 * @brief exec
	 * 开始执行并进入Dialog模式
	 */
	void exec(){
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
		if(this->isRunning == true){
			qDebug() << CodePath() << ": Choise is still running, please don't call it repeadly!";
			return;
		}
		if(this->messageReadyList.isEmpty()){
			qDebug() << CodePath() << ": messageReadyList is Empty, show what?";
			return;
		}else{
			this->messageList.clear();
			this->messageList.append(this->messageReadyList);
		}

		this->box->setPixmap(QPixmap::fromImage(this->getDialogImage()));
		this->box->setPos(this->getDialogPosition() + this->parentScene->sceneRect().topLeft());

		for(int i = 0; i < this->choiseBuff; i++){
			this->messages[i]->setTextWidth(this->messagesRect.width());
			this->messages[i]->setPos(this->messagesRect.topLeft() + QPoint(0, this->messagesRect.height() + this->messageSpecingV) * i);
		}

		this->upSymbol->setPixmap(*this->upSymbolPixmap[0]);
		this->upSymbol->setPos((this->getDialogRect().width() - this->upSymbolPixmap[0]->width()) / 2, this->upSymbolPixmap[0]->height());
		this->upSymbol->setVisible(false);

		this->downSymbol->setPixmap(*this->downSymbolPixmap[0]);
		this->downSymbol->setPos((this->getDialogRect().width() - this->downSymbolPixmap[0]->width()) / 2, this->getDialogRect().height() - this->downSymbolPixmap[0]->height());
		this->downSymbol->setVisible(false);

		this->showChoice();
	}

protected:
	/**
	 * @brief showChoice
	 * 显示Choice
	 */
	void showChoice(){
		this->isRunning = true;
		emit this->enterDialogMode();
		this->parentScene->addItem(this->box);
		this->showText(this->messageIndex, false);
		this->setSlowprint(SpeedInfinitly);
	}

	/**
	 * @brief hideChoice
	 * 隐藏Choice
	 */
	void hideChoice(){
		if(this->upSymbolTimeLine->state() != QTimeLine::NotRunning){
			this->upSymbolTimeLine->stop();
		}
		if(this->downSymbolTimeLine->state() != QTimeLine::NotRunning){
			this->downSymbolTimeLine->stop();
		}

		this->box->hide();
		this->parentScene->removeItem(this->box);
		this->clearChoiceText();
		emit this->quitDialogMode();
		this->isRunning = false;
	}

	/**
	 * @brief showText
	 * @param index
	 * 显示选项index, index+1, index+2
	 */
	void showText(int index, bool noSpeedRender = true){

		if(index > this->messageList.length() - this->choiseBuff){
			qDebug() << CodePath << ": index is out of range.";
			return;
		}
		QStringList msgs;
		for(int i = 0; i < this->choiseBuff; i++){
			msgs.append(this->messageList.at(index + i));
		}

		if(noSpeedRender){
			for(int i = 0; i < this->choiseBuff; i++){
				this->messages[i]->setHtml(msgs.at(i));
			}
		}else{
			this->showTextInProgressFlag = true;
			this->upSymbolTimeLine->stop();
			this->downSymbolTimeLine->stop();
			this->upSymbol->setVisible(false);
			this->downSymbol->setVisible(false);
			if(this->slowprint > 0){
				this->quickShowFlag = false;
				for(int i = 0; i < this->choiseBuff; i++){
					for(int j = 0; j < msgs.at(i).length(); j++){
						this->messages[i]->setHtml(msgs.at(i).left(j + 1));
						Utils::msleep(slowprint);
						if(this->quickShowFlag == true){
							for(; i < this->choiseBuff; i++){
								this->messages[i]->setHtml(msgs.at(i));
							}
							this->quickShowFlag = false;
							break;
						}
					}
				}
			}else{
				for(int i = 0; i < this->choiseBuff; i++){
					this->messages[i]->setHtml(msgs.at(i));
				}
			}
			this->showTextInProgressFlag = false;
		}

		if(index > 0){
			this->upSymbol->setVisible(true);
			this->upSymbolTimeLine->start();
		}else{
			this->upSymbol->setVisible(false);
			this->upSymbolTimeLine->stop();
		}
		if(index <= this->messageList.length() - this->choiseBuff){
			this->downSymbol->setVisible(true);
			this->downSymbolTimeLine->start();
		}else{
			this->downSymbol->setVisible(false);
			this->downSymbolTimeLine->stop();
		}
	}

	void showChoiceBox(int index){
		if(index < 0 || index >= this->choiseBuff){
			qDebug() << CodePath() << ": index is out of range: " << index << "</>[0," << this->choiseBuff;
		}
		this->choiceSymbol->setPos(messageMarginH, messageMarginV + (messageSpecingV + this->getSelectBarImage().height()) * index);
		if(!this->choiceSymbol->isVisible()){
			this->choiceSymbol->setVisible(true);
		}
		if(!this->choiceBarAnimation->state() != QAbstractAnimation::Running){
			this->choiceBarAnimation->start();
		}
	}

signals:
	/**
	 * @brief enterDialogMode
	 * 信号: 进入Dialog模式
	 */
	void enterDialogMode();
	/**
	 * @brief quitDialogMode
	 * 信号: 退出Dialog模式
	 */
	void quitDialogMode();

public slots:
	/**
	 * @brief receiveKey
	 * @param key
	 * @param mod
	 * 槽: 接收主函数传来的按键中断请求
	 *
	 */
	void receiveKey(int key, Qt::KeyboardModifiers mod){
		qDebug() << CodePath() << tr(": receive key: %1::%2(%3).").arg(mod).arg(key).arg(QString(QChar(key)).toHtmlEscaped());
		if(key == Qt::Key_Return || key == Qt::Key_Space){
			if(this->showTextInProgressFlag == true){
				// 正在显示字符, 则立即停止slowprint, 直接显示全部的字符
				this->quickShowFlag = true;
			}
		}else if(key == Qt::Key_Up){
			// 显示上一个选项
			if(this->messageIndex == 0){
				// 正在显示0~choiceBuff-1项
				if(this->messageCurrentIndex == 0){
					// 光标在第一个(不能往上了)
					qDebug() << CodePath() << ": First choice first, cannot up.";
					return;
				}else if(this->messageCurrentIndex <= this->choiseBuff -1){
					// 光标在choiceBuff-1项(上移一格)
					this->messageCurrentIndex -= 1;
					this->showChoiceBox(this->messageCurrentIndex);
				}
			}else if(this->messageIndex > 0 && this->messageIndex < this->messageList.length() - 2){
				// 正在显示1~总长-2
			}
		}
	}

};

#endif // RPGCHOICE_H
