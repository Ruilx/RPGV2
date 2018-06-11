#ifndef RPGSTATE_H
#define RPGSTATE_H

#include <QtCore>
#include <QKeyEvent>
#include <RPG/utils/Utils.h>
#include <RPG/core/RpgObject.h>

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
		UnknownMode = -1,
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

	QHash<Mode, QVector<RpgObject*> > modeObjects;

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
		if(this->modeStack.isEmpty()){
			qDebug() << CodePath() << "Mode stack is empty, cannot pop.";
			return UnknownMode;
		}
		return this->modeStack.pop();
	}

	void registerRpgObject(RpgObject *obj, Mode mode){
		if(obj == nullptr){
			qDebug() << CodePath() << "RpgObject is null.";
			return;
		}
		if(this->modeObjects.contains(mode)){
			QVector<RpgObject*> objs = this->modeObjects.value(mode);
			objs.append(obj);
			this->modeObjects.insert(mode, objs);
		}else{
			QVector<RpgObject*> objs;
			objs.append(obj);
			this->modeObjects.insert(mode, objs);
		}
	}

	void unregisterRpgObject(RpgObject *obj, Mode mode){
		if(obj == nullptr){
			qDebug() << CodePath() << "RpgObject is null.";
			return;
		}
		if(this->modeObjects.contains(mode)){
			QVector<RpgObject*> objs = this->modeObjects.value(mode);
			int index = -1;
			if((index = objs.indexOf(obj)) == -1){
				qDebug() << CodePath() << "The mode" << mode << "does not registered this RpgObject" << obj;
				return;
			}
			objs.removeAt(index);
			this->modeObjects.insert(mode, objs);
		}else{
			qDebug() << CodePath() << "The mode" << mode << "does not have any objects called" << obj;
			return;
		}
	}

signals:

public slots:
	void receiveKey(int key, Qt::KeyboardModifiers mod, const QGraphicsScene *scene){
		qDebug() << CodePath() << "Receive key:" << Utils::_keyModifierToString(mod) << "+" << Utils::_keyToString((Qt::Key)key);
		if(this->modeStack.isEmpty()){
			qDebug() << CodePath() << "Mode stack is empty, cannot getting Mode.";
			return;
		}
		if(scene == nullptr){
			qDebug() << CodePath() << "Must specified a vaild scene to handle the key.";
			return;
		}
		Mode topMode = this->modeStack.top();
		QVector<RpgObject*> objects = this->modeObjects.value(topMode);
		for(RpgObject *obj: objects){
			if(obj == nullptr){
				qDebug() << "RpgObject is nullptr, cannot touch it.";
				continue;
			}
			if(obj->getParentScene() == scene){
				// 请在被调用的Object确定当前是否在运行(isRunning=True)并如果没有运行请直接返回
				// 这里可以依次向每个Object调用其receiveKey函数
				obj->receiveKey(key, mod);
			}
		}
	}
};

#endif // RPGSTATE_H
