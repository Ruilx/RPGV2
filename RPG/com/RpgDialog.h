#ifndef RPGDIALOG_H
#define RPGDIALOG_H

#include <QtCore>
#include <QtWidgets>
#include <RPG/About.h>
#include <RPG/Global.h>
#include <RPG/utils/Utils.h>
#include <RPG/core/RpgDialogBase.h>

/**
 * @brief The RpgDialog class
 * RPGDialog类
 * 是在GraphicsScene上形成一个类似对话框的文字输出器
 * 是一个单独的线程控制文字输出
 * 当设定好显示的内容之后, 之后就交给RpgDialog线程进行显示, 同时调用者会收到进入DialogMode信号
 * 然后改变输入定向, 并等待RPGDialog结束, 发送退出DialogMode信号.
 * @abandoned //这个类属于单例模式, 不能将其生成对象, 调用方法: RpgDialog::getInstance()即可得到实例.
 * dlgasobj 分支: 这个类不再是单例模式, 但用户层不能将其生成对象, 只能使用Scene->getRpgDialog()得到Handle
 */
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

	// Viewport Offset( 一般Scene要比View大, 所以如果要浮于View中间, 则还需要View相对于Scene的视窗大小 )
	QPointF viewportOffset = QPoint(0, 0);

public:
	/**
	 * @brief The Speed enum
	 * 单字输出模式, 预先定义了快中慢三个节奏.
	 */
	enum Speed{
		SpeedSlow = 55,
		SpeedNormal = 30,
		SpeedFast = 17,
		SpeedInfinitly = 0
	};
private:
	// Num: 单字输出, 0表示不使用单字输出, 数字越小表示输出速度越快
	int slowprint = 0;
	// Flag: 正在输出的一个状态
	bool showTextInProgressFlag = false;
	// Flag: 被用户中断, 下一次将会显示全部信息
	bool quickShowFlag = false;

	// String: 文字CSS
	const QString css = "r{ color: #FF3333; }g{ color: #33FF33; }b{ color: #9999FF; }y{ color: #EEEE33 }";

	// 继续显示的三角形
	QTimeLine *continueSymbolTimeLine = new QTimeLine(1000, this);

	// 需要预先转换成Pixmap的Image
	QPixmap dialogPixmap;
	QPixmap *continueSymbolPixmap[4] = {nullptr};

public:
	/**
	 * @brief RpgDialog RPG对话窗口构造
	 * @param parentScene 设置显示到的Scene
	 * @param parent QObject要用的, 虽然各种不知道这是什么... 需要发送Signal的东西
	 */
	explicit RpgDialog(QGraphicsScene *parentScene = nullptr, QObject *parent = 0);
public:
	/**
	 * @brief addText
	 * @param text 添加的文字
	 * 在文本列表中添加一条文字
	 */
	void addText(const QString &text){ this->messageList.append(text); }

	/**
	 * @brief addText
	 * @param textList 添加的文本列表
	 * 在文本列表中添加一堆文字
	 */
	void addText(const QStringList &textList){ this->messageList.append(textList); }

	/**
	 * @brief setSlowprint
	 * @param speed 速度
	 * 设置缓慢输出速度
	 */
	void setSlowprint(int speed){ this->slowprint = speed; }

	/**
	 * @brief setFont
	 * @param font 字体
	 * 设置字体
	 */
	void setFont(const QFont &font){ this->message->setFont(font); }

	/**
	 * @brief setTextColor
	 * @param color
	 * 设置字体颜色
	 */
	void setTextColor(const QColor &color){ this->message->setDefaultTextColor(color); }

	/**
	 * @brief getViewportOffset
	 * @return QPoint
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
	 * @brief clearText
	 * 清除文字列表(清空列表)
	 */
	void clearText(){ this->messageList.clear(); }

	/**
	 * @brief setGraphicsScene
	 * @param scene
	 * 设置对话框显示到的Scene
	 */
	void setGraphicsScene(QGraphicsScene *scene){
		this->parentScene = scene;
//		this->setViewportOffset(QPointF(this->parentScene->sceneRect().left(), this->parentScene->sceneRect().top()));
	}

	/**
	 * @brief exec
	 * 开始执行并进入Dialog模式
	 */
	void exec();
protected:
	/**
	 * @brief showDialog
	 * 显示Dialog函数
	 */
	void showDialog();

	/**
	 * @brief hideDialog
	 * 隐藏Dialog函数
	 */
	void hideDialog();
protected:
	/**
	 * @brief showText
	 * @param index
	 * 显示文字列表第Index个文本
	 */
	void showText(int index);


signals:
	/**
	 * @brief enterDialogMode
	 * @type SIGNAL
	 * 信号: 进入Dialog模式
	 */
	void enterDialogMode();
	/**
	 * @brief quitDialogMode
	 * @type SIGNAL
	 * 信号: 退出Dialog模式
	 */
	void quitDialogMode();
public slots:
	/**
	 * @brief receiveKey
	 * @param key QKey
	 * @param mod Qt::KeyboardModifiers
	 * @type SLOT
	 * 槽: 接收主函数传来的按键中断请求, 用于改变文字或退出Dialog模式
	 */
	void receiveKey(int key, Qt::KeyboardModifiers mod);
};

#endif // RPGDIALOG_H
