#ifndef RPGSTATE_H
#define RPGSTATE_H

#include <QtCore>
#include <QKeyEvent>

/**
 * @brief The RpgState class
 * RPG State 保存当前RPG的运行状态, 游戏状态栈, 以及用户输入和注册的映射方式
 */
class RpgState : public QObject
{
	Q_OBJECT
	static RpgState *_instance;
public:
	/**
	 * @brief instance 获取单例对象
	 * @return
	 */
	static RpgState *instance(){
		if(_instance == nullptr){
			_instance = new RpgState(nullptr);
		}
		return _instance;
	}

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
private:
	/**
	 * @brief modeStack
	 * 模式栈, 新增模式会将旧模式压栈, 然后新模式结束之后继续执行旧模式
	 */
	QStack<Mode> modeStack;

public:
	explicit RpgState(QObject *parent = nullptr) : QObject(parent){
		if(this->modeStack.isEmpty()){
			this->modeStack.push(AutoMode);
		}
	}

	inline Mode getTop() const{
		return this->modeStack.top();
	}

	void pushMode(const Mode &mode){
		this->modeStack.push(mode);
	}

	Mode popMode(){
		return this->modeStack.pop();
	}

signals:

public slots:
	void receiveKey(int key, Qt::KeyboardModifiers mod){

	}
};

#endif // RPGSTATE_H
