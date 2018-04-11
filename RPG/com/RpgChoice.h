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
	QList<bool> messageEnabledList;	// 选项可用性(可不可以选择)
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
	explicit RpgChoice(QGraphicsScene *parentScene = nullptr, QObject *parent = 0);
public:
	/**
	 * @brief addChoiceText
	 * @param text 添加的选择
	 */
	void addChoiceText(const QString &text){ this->messageReadyList.append(text); this->messageEnabledList.append(true);}
	/**
	 * @brief addChoiceText
	 * @param textList 添加一串选择
	 */
	void addChoiceText(const QStringList &textList){
		this->messageReadyList.append(textList);
		for(int i = 0; i < textList.length(); i++){
			this->messageEnabledList.append(true);
		}
	}
	/**
	 * @brief setChoiceEnabled
	 * @param choiceIndex
	 * @param enabled
	 * 设置某项选项的可用状态
	 */
	void setChoiceEnabled(int choiceIndex, bool enabled){
		if(choiceIndex >= this->messageEnabledList.length()){
			qDebug() << "choice index is out of range.";
		}
		this->messageEnabledList[choiceIndex] = enabled;
	}
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
	void exec();
	/**
	 * @brief waitingForChooseComplete
	 * @param second 选择的时候等待用户选择的时间(秒)
	 * @return -1失败(因为还未启动) >0成功, 返回选择的项目
	 * 等待用户选择
	 */
	int waitingForChooseComplete(int second = -1);

	/**
	 * @brief getChose
	 * @return
	 * 获得用户的上次选择的结果
	 */
	int getChose() const{ return this->messageChose; }
protected:
	/**
	 * @brief showChoice
	 * 显示Choice
	 */
	void showChoice();

	/**
	 * @brief hideChoice
	 * 隐藏Choice
	 */
	void hideChoice();

	/**
	 * @brief showText
	 * @param index
	 * 显示选项index, index+1, index+2
	 */
	void showText(int index, bool noSpeedRender = true);

	/**
	 * @brief showChoiceBox
	 * @param index
	 * 显示那个闪烁的显示光标在ChooseBuff中的第几个
	 */
	void showChoiceBox(int index);
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
	void receiveKey(int key, Qt::KeyboardModifiers mod);

};

#endif // RPGCHOICE_H

/** 废弃代码
 if(this->messageIndex > 0 && this->messageIndex < this->messageList.length() - (ChoiceBuff -1))
 */
