#ifndef RPGCHOICE_H
#define RPGCHOICE_H

#include <QtCore>
#include <QtWidgets>
#include <QPropertyAnimation>
#include <QEasingCurve>
#include <RPG/About.h>
#include <RPG/Global.h>
#include <RPG/utils/Utils.h>
#include <RPG/core/RpgDialogBase.h>

#define ChoiceBuff 3 // 默认一页显示几个选项
/**
 * @brief The RpgChoice class
 * RPGChoice类
 * 是在GraphicsScene上形成一个类似对话框的文字选择器
 * 是一个单独的线程控制文字输出
 * 当设定好显示的设置文字之后, 之后就交给RpgChoice线程进行显示, 同时调用者会收到进入DialogMode信号
 * 然后改变输入定向, 并等待RPGChoice结束, 发送退出DialogMode的信号
 * dlgasobj 分支: 类并非单例模式, 但用户层不能将其生成对象, 只能使用Scene->getRpgDialog()得到handle
 *
 */

class RpgChoice : public QObject, public RpgDialogBase
{
	Q_OBJECT
	// 标志: 是否在运行
	bool isRunning = false;
	// 指定显示在哪个Scene上
	QGraphicsScene *parentScene = nullptr;

	// 构成
	QGraphicsPixmapItem *box = new QGraphicsPixmapItem(nullptr);			// 那蓝色的小盒子
	QGraphicsTextItem *messages[ChoiceBuff] = {nullptr};					// 可见的那三行字
	QGraphicsPixmapItem *upSymbol = new QGraphicsPixmapItem(this->box);		// 上面的小三角
	QGraphicsPixmapItem *downSymbol = new QGraphicsPixmapItem(this->box);	// 下面的小三角
	QGraphicsPixmapItem *choiceSymbol = new QGraphicsPixmapItem(this->box);	// 选择框蒙版

	QGraphicsDropShadowEffect *messageShadowEffect[ChoiceBuff]; // 字下面的阴影

	QParallelAnimationGroup *entryAniGroup = new QParallelAnimationGroup(this); // 加载动画组
	QGraphicsOpacityEffect *boxOpacityEffect = new QGraphicsOpacityEffect(this); // 文本框透明度(作为动画表示)

	QGraphicsOpacityEffect *choiceBarOpacityEffect = new QGraphicsOpacityEffect(this);	// 选择框透明度(闪烁效果)
	QPropertyAnimation *choiceBarAnimation = new QPropertyAnimation(this->choiceBarOpacityEffect, "opacity", this); // 选择框变透明的动画

	// 选项列表
	QStringList messageList;		// 选项列表
	QStringList messageReadyList;	// 选项缓存
	int messageIndex = 0;			// 现在处于第一显示的选项序号[0, len-choiceBuff]
	int messageCurrentIndex = 0;	// 现在选项的序号[0, choiceBuff]
	int messageChose = 0;			// 结果选中的序号

	// 消息
	int messageMarginH = 10;
	int messageMarginV = 6;
	int messageSpecingV = 2;
	QRect messagesRect = QRect(messageMarginH, messageMarginV, ScreenWidth - messageMarginH - messageMarginH, ScreenHeight - messageMarginV - messageMarginV);

	// Viewport Offset?
//	QPointF viewportOffset = QPoint(0, 0);

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
	int slowprintBackup = 0;
	// Flag: 输出的一个状态
	bool showTextInProgressFlag = false;
	// Flag: 被用户中断, 下一次将显示全部信息
	bool quickShowFlag = false;

	// String: 文字CSS
	const QString css = "r{ color: #FF3333; }g{ color: #33FF33; }b{ color: #9999FF; }y{ color: #EEEE33 }";

	// 上下显示的三角形
	QTimeLine *upSymbolTimeLine = new QTimeLine(1000, this);
	QTimeLine *downSymbolTimeLine = new QTimeLine(1000, this);

	// 对话框图片, 需要预先转换成Pixmap的Image
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
			qDebug() << "Message[" << i << "].pos = " << this->messages[i]->pos();
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
		for(int i = 0; i < ChoiceBuff; i++){
			this->messages[i]->setFont(font);
		}
	}
	/**
	 * @brief setTextColor 设置字体颜色
	 * @param color
	 */
	void setTextColor(const QColor &color){
		for(int i = 0; i < ChoiceBuff; i++){
			this->messages[i]->setDefaultTextColor(color);
		}
	}
//	/**
//	 * @brief getViewportOffset
//	 * @return
//	 * 获得Viewport的偏移坐标
//	 */
//	QPointF getViewportOffset() const { return this->viewportOffset; }
//	/**
//	 * @brief setViewportOffset
//	 * @param offset
//	 * 设置Viewport的偏移坐标
//	 */
//	void setViewportOffset(const QPointF &offset){ this->viewportOffset = offset; }
	/**
	 * @brief clearChoiceText
	 * 清除文字选项
	 */
	void clearChoiceText(){ this->messageList.clear(); this->messageReadyList.clear(); }
	/**
	 * @brief setGraphicsScene
	 * @param scene
	 * 设置对话框显示到的Scene
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
	/**
	 * @brief waitingForChooseComplete
	 * @param second 选择的时候等待用户选择的时间(秒)
	 * @return -1失败(因为还未启动) >0成功, 返回选择的项目
	 * 等待用户选择
	 */
	int waitingForChooseComplete(int second = -1){
		if(!this->isRunning){
			qDebug() << CodePath() << ": Not running yet, cannot waiting.";
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
protected:
	/**
	 * @brief showChoice
	 * 显示Choice
	 */
	void showChoice(){
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

	/**
	 * @brief showText
	 * @param index
	 * 显示选项index, index+1, index+2
	 */
	void showText(int index, bool noSpeedRender = true){

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
			this->upSymbolTimeLine->stop();
			this->downSymbolTimeLine->stop();
			this->upSymbol->setVisible(false);
			this->downSymbol->setVisible(false);
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
			this->upSymbol->setVisible(true);
			this->upSymbolTimeLine->start();
		}else{
			this->upSymbol->setVisible(false);
			this->upSymbolTimeLine->stop();
		}
		if(index < (this->messageList.length() <= ChoiceBuff ? 0 : this->messageList.length() - ChoiceBuff)){
			this->downSymbol->setVisible(true);
			this->downSymbolTimeLine->start();
		}else{
			this->downSymbol->setVisible(false);
			this->downSymbolTimeLine->stop();
		}
	}

	void showChoiceBox(int index){
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
private:
	/**
	 * @brief enterOrExitAnimationSetting
	 * @param enter
	 * 进入动画或退出动画设置
	 */
	void enterOrExitAnimationSetting(bool enter);
	/**
	 * @brief enterOrExitAnimationStart
	 * 开启动画
	 */
	void enterOrExitAnimationStart();
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

};

#endif // RPGCHOICE_H

/** 废弃代码
 if(this->messageIndex > 0 && this->messageIndex < this->messageList.length() - (ChoiceBuff -1))
 */
