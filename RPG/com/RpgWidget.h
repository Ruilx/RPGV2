#ifndef RPGWIDGET_H
#define RPGWIDGET_H

#include <QtWidgets>
#include <QGraphicsScene>
#include <RPG/Global.h>
#include <RPG/About.h>
#include <RPG/utils/Utils.h>
#include <RPG/com/RpgDialog.h>
#include <RPG/core/RpgMapBlock.h>
#include <RPG/com/RpgScene.h>
#include <RPG/core/RpgTileSetBase.h>
#include <RPG/com/RpgMusic.h>
#include <RPG/core/RpgAutoTileBase.h>
#include <RPG/com/RpgLyric.h>
#include <Rpg/core/RpgFileManager.h>

/**
 * @brief The RpgWidget class
 * QWidget, 构建的RPG演出台幕
 * 处理用户输入, 场景调度, 信息发送, 模式栈
 */
class RpgWidget : public QWidget
{
	Q_OBJECT
	/**
	 * @brief mapList
	 * 地图的列表, 由名字对应一个地图的scene, 游戏会载入各种地图到相应的场景中
	 */
	QHash<QString, RpgScene*> mapList;
	/**
	 * @brief stage
	 * 舞台(窗口)用来看见地图的窗口, 控制舞台显示机制, 对话框等显示.
	 */
	QGraphicsView *stage = new QGraphicsView(this);
	/**
	 * @brief timerId
	 * 用于处理this->startTimer(int)的返回值, 在timerEvent中this->killTimer(timerId)
	 * 向其中传值, 关闭timerEvent.
	 */
	int timerId = -1;
	/**
	 * @brief dialog
	 * RPG对话框, 用来显示文字, 并接受Return, Space按键以显示一些字符.
	 */
	//RpgDialog *dialog;
	/**
	 * @brief The Mode enum
	 * 定义游戏会有哪些模式, 各个模式中按键接受方式, 显示方式等都不相同, 用这个Enum来规范现在
	 * 游戏运行在什么模式下.
	 * AutoMode: 自动模式, 在此模式下, 系统会自动运行, 用户无法干涉游戏, 用于剧情演示.
	 * NormalMode: 普通模式, 在此模式下, 用户可以操纵主人公在地图上行走, 触发剧情等.
	 * SystemMenuMode: 系统菜单模式, 在此模式中, 时间会暂停, 用户操纵系统菜单, 载入退出游戏.
	 * DialogMode: 对话模式, 在此模式中, 用户会看到对话或其他提示语.
	 * ItemMode: 背包模式, 在此模式中, 时间暂停, 用户可以查看背包状态, 使用道具等操作.
	 */
	enum Mode{
		AutoMode = 0,
		NormalMode,
		SystemMenuMode,
		DialogMode,
		ItemMode
	};
	/**
	 * @brief modeStack
	 * 游戏模式的栈, 新增游戏模式之后会将旧模式压栈, 然后新模式结束之后自动继续执行旧模式.
	 */
	QStack<Mode> modeStack;

	/**
	 * @brief keyReleaseEvent
	 * @param event
	 * 判断按键离开的事件
	 */
	void keyReleaseEvent(QKeyEvent *event);

	/**
	 * @brief canBeExit
	 * 是否在不经询问的情况下关闭窗口(退出游戏), 设置为true的时候, 关闭窗口不再询问
	 */
	bool canBeClose = false;
	/**
	 * @brief closeEvent
	 * @param event
	 * 关闭窗口事件(未使用)
	 */
	void closeEvent(QCloseEvent *event);
	/**
	 * @brief bootScriptScene
	 * 用于启动的脚本名称
	 */
	QString bootScriptSceneName;

	/**
	 * @brief biosSet 加载基本配置Json
	 * @param initializationConfigureJson 基本配置Json
	 * @return 是否成功加载
	 */
	bool biosSet(const QByteArray &initializationConfigureJson);

public:
	/**
	 * @brief RpgWidget
	 * @param parent
	 * RpgWidget构造函数
	 */
	explicit RpgWidget(QWidget *parent = nullptr);
	/**
	 * @brief getCanBeClose
	 * @return
	 * 获得是否不经过用户同意就关闭窗口
	 */
	inline bool getCanBeClose() const{return this->canBeClose;}
	/**
	 * @brief setCanBeClose
	 * @param e
	 * 设置是否不经过用户同意就关闭窗口
	 */
	void setCanBeClose(bool e){ this->canBeClose = e; }

signals:
	/**
	 * @brief normalModeKeyClick
	 * @param key
	 * @param mod
	 * 普通模式下点击了按键, 信号
	 */
	void normalModeKeyClick(int key, Qt::KeyboardModifiers mod);
	/**
	 * @brief systemMenuModeKeyClick
	 * @param key
	 * @param mod
	 * 系统菜单模式下点击了按键, 信号
	 */
	void systemMenuModeKeyClick(int key, Qt::KeyboardModifiers mod);
	/**
	 * @brief dialogModeKeyClick
	 * @param key
	 * @param mod
	 * 对话模式下点击了按键, 信号
	 */
	void dialogModeKeyClick(int key, Qt::KeyboardModifiers mod);
	/**
	 * @brief itemModeKeyClick
	 * @param key
	 * @param mod
	 * 背包模式下点击了按键, 信号
	 */
	void itemModeKeyClick(int key, Qt::KeyboardModifiers mod);
	/**
	 * @brief readyToClose
	 * 游戏想要关闭窗口(和MainW关联, 让MainW关闭窗口)
	 */
	void readyToClose();

public slots:
	/**
	 * @brief enterAutoMode, exitAutoMode
	 * 进入或退出自动模式(接受自动引导模块传来的信号), 槽
	 */
	void enterAutoMode(){
		qDebug() << "[DEBUG][System] Enter [Auto] Mode.";
	}
	void quitAutoMode(){
		qDebug() << "[DEBUG][System] Quit [Auto] Mode.";
	}

	/**
	 * @brief enterDialogMode, exitDialogMode
	 * 进入/退出对话模式(接受对话模块传来的信号), 槽
	 */
	void enterDialogMode(){
		qDebug() << "[DEBUG][System] Enter [Dialog] Mode.";
	}
	void quitDialogMode(){
		qDebug() << "[DEBUG][System] Quit [Dialog] Mode.";
	}
	/**
	 * @brief enterNormalMode, exitNormalMode
	 * 进入/退出普通模式(接受普通模式模块传来的信号), 槽
	 */
	void enterNormalMode(){
		qDebug() << "[DEBUG][System] Enter Normal Mode.";
	}
	void quitNormalMode(){
		qDebug() << "[DEBUG][System] Quit Normal Mode.";
	}
	/**
	 * @brief enterSystemMenuMode, exitSystemMenuMode
	 * 进入/退出系统菜单模式(接受系统菜单模块传来的信号), 槽
	 */
	void enterSystemMenuMode(){
		qDebug() << "[DEBUG][System] Enter SystemMenu Mode.";
	}
	void quitSystemMenuMode(){
		qDebug() << "[DEBUG][System] Exit SystemMenu Mode.";
	}
	/**
	 * @brief enterItemMenuMode, exitItemMenuMode
	 * 进入/退出背包模式(接受背包界面模块传来的信号), 槽
	 */
	void enterItemMenuMode(){
		qDebug() << "[DEBUG][System] Enter ItemMenu Mode.";
	}
	void quitItemMenuMode(){
		qDebug() << "[DEBUG][System] Quit ItemMenu Mode.";
	}

	/**
	 * @brief ready
	 * 构建成功, 窗口显示后自动进行的函数
	 */
	void ready();
	/**
	 * @brief doReadyToClose
	 * 调用此函数将会发送一个关闭信号给MainW, MainW会协助关闭窗口.
	 */
	void doReadyToClose(){ emit this->readyToClose(); }
};

#endif // RPGWIDGET_H
