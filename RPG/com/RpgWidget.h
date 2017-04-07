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
	void keyReleaseEvent(QKeyEvent *event){
		// 用户按下的按键
		int key = event->key();
		// 用户是否使用了Shift, Ctrl, Alt, Win(Meta)键对按键进行修饰(一块按下了)
		Qt::KeyboardModifiers mod = event->modifiers();
		if(key == Qt::Key_Escape){
			// 如果用户按下了ESC键, 任何情况下都可以进入系统菜单
			if(this->modeStack.top() == SystemMenuMode){
				// 如果已经在系统菜单模式, 则退出系统菜单模式
				//TODO: 系统菜单还没有写, 这里输出一条debug代替
				qDebug() << "RpgWidget: KeyReleaseEvent: User Release ESCAPE key in SystemMenuMode.";
				event->accept();
				return;
			}else{
				//TODO: 系统菜单还没写, 这里输出一条debug代替
				qDebug() << "RpgWidget: KeyReleaseEvent: User Released ESCAPE key.";
				event->accept();
				return;
			}
		}
		switch(this->modeStack.top()){
			case AutoMode:{
				// 自动模式下不接受任何按键.
				break;
			}
			case NormalMode:{
				// 普通模式, 接受W, A, S, D, Space, Return, ArrowU, ArrowD, ArrowL, ArrowR, Z(道具栏)
				//TODO: 没有写相关的运动函数, 这里输出一条Debug代替
				qDebug() << tr("RpgWidget: KeyReleaseEvent: User Release %1(%2) Key in NormalMode.").arg(key).arg(event->text().toHtmlEscaped());
				event->accept();
				break;
			}
			case SystemMenuMode:{
				// 系统菜单模式, 接受W, A, S, D, ArrowU, ArrowD, ArrowL, ArrowR, Space, Return, Esc(前面已经处理过了)
				//TODO: 没有写系统菜单栏目, 这里输出一条Debug代替
				qDebug() << tr("RpgWidget: KeyReleaseEvent: User Release %1(%2) Key in SystemMenuMode.").arg(key).arg(event->text().toHtmlEscaped());
				event->accept();
				break;
			}
			case DialogMode:{
				// 对话模式, 接受Return, Space
				emit this->dialogModeKeyClick(key, mod);
				event->accept();
				break;
			}
			case ItemMode:{
				// 道具栏(背包)模式, 接受W, A, S, D, ArrowU, ArrowD, ArrowL, ArrowR, Space, Return
				//TODO: 没有写系统背包栏目, 这里输出一条Debug代替
				qDebug() << tr("RpgWidget: KeyReleaseEvent: User Release %1(%2) Key in ItemMode.").arg(key).arg(event->text().toHtmlEscaped());
				event->accept();
				break;
			}
			default:{
				qDebug() << tr("RpgWidget: KeyReleaseEvent: Cannot recongized which mode it is? User Release %1(%2) Key.").arg(key).arg(event->text().toHtmlEscaped());
				event->ignore();
				break;
			}
		}
	}

public:
	/**
	 * @brief RpgWidget
	 * @param parent
	 * RpgWidget构造函数
	 */
	explicit RpgWidget(QWidget *parent = nullptr): QWidget(parent){

		// 新建一个Scene, 然后加入地图列表
		RpgScene *titleScene = new RpgScene(this);
		this->mapList.insert("titlescene", titleScene);
		// 设置显示偏移量
		titleScene->setScenePos(50.0f, 50.0f);
		// 设置当前地图
		this->stage->setScene(titleScene);
		this->stage->setFixedSize(ScreenWidth + 2, ScreenHeight + 2);
		this->stage->scale(2.0f, 1.0f);
		//this->stage->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
		//this->stage->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
		QPalette pal;{
			pal.setColor(QPalette::Base, Qt::black);
		}
		this->stage->setPalette(pal);

		QVBoxLayout *mainLay = new QVBoxLayout;
		this->setLayout(mainLay);
		mainLay->addWidget(this->stage);
		mainLay->setMargin(0);

		connect(titleScene->getRpgDialog(), &RpgDialog::enterDialogMode, this, [this](){
			this->modeStack.push(DialogMode);
			this->enterDialogMode();
		});

		connect(titleScene->getRpgDialog(), &RpgDialog::quitDialogMode, this, [this](){
			if(this->modeStack.top() == DialogMode){
				this->modeStack.pop();
			}else{
				qDebug() << "RpgWidget::[RpgDialog::quitDialogMode LambdaMoc]: Current Mode Stack top is not dialogMode!";
				return;
			}
			this->quitDialogMode();
		});

		connect(this, &RpgWidget::dialogModeKeyClick, titleScene->getRpgDialog(), &RpgDialog::receiveKey);

		this->modeStack.push(NormalMode);

		QTimer::singleShot(1000, this, &RpgWidget::ready);

		this->stage->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);
	}

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

public slots:
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
	void ready(){
		qDebug() << "Ready...";
		RpgScene *titleScene = this->mapList.value("titlescene", nullptr);
		if(titleScene == nullptr){
			qDebug() << "RpgWidget::ready(): titleScene(\"titlescene\") not exist.";
			return;
		}

		titleScene->getRpgDialog()->addText("我做梦都没想到，会有一天，竟然用<r>这种尴尬</r>的方式，和那个女孩子相遇了。");
		titleScene->getRpgDialog()->addText("私は一日持っていた夢の中でそれを考えなかった、私は非常に恥ずかしい状況でその女の子に会った");
		titleScene->getRpgDialog()->addText("但是当我遇见她的时候，她的心情异常的平静，好像并没有对此感到吃惊。");
		titleScene->getRpgDialog()->addText("“难道她认识我？”");
		titleScene->getRpgDialog()->addText("她说她看到我就有一种似曾相识的感觉。");
		titleScene->getRpgDialog()->addText("<b>似曾相识的感觉？</b>我倒是记得一位和她很像的女生，跟我关系非常好，但是相比之下，这么让人相敬如宾的女孩子我还是头一回见。");
		titleScene->getRpgDialog()->addText("不对。一定不是她，她在那场灾难中没有活下来，即便活下来了，烧伤的痕迹应该也能看出来，<g>即便是她</g>，那么这三年她是怎么生活过来的。");
		titleScene->getRpgDialog()->addText("一大堆问题忽然全部堆到我的脑海里，我飞速的计算着，飞速的猜测着，但是并没有任何结果。");
		titleScene->getRpgDialog()->addText("当时只是尴尬了说了一句：“对不起”便匆匆离去。但之后真的回味无穷。");
		titleScene->getRpgDialog()->addText("真的是她吗，我不相信。三年前，我亲眼看到她被大火淹没，从此再无音信。");
		titleScene->getRpgDialog()->addText("……");
		titleScene->getRpgDialog()->addText("我当时真的傻眼了，我想冲回去救她，但是被其他人拦住强制拖出了火灾区。");
		titleScene->getRpgDialog()->addText("眼看着我离她越来越远，我极力挣扎着。我只记得我被营救出去的那一刻，后面出现了<y>爆炸</y>。");
		titleScene->getRpgDialog()->addText("一声刻骨铭心的爆炸。");
		titleScene->getRpgDialog()->addText("她，从此生死不明。");
		titleScene->getRpgDialog()->exec();


#ifdef _DEBUG
		int blockCol = titleScene->width() / MapBlockWidth;
		int blockRow = titleScene->height() / MapBlockHeight;
		for(int i = 0; i < blockRow; i++){
			for(int j = 0; j < blockCol; j++){
				RpgMapBlock *block = new RpgMapBlock(j, i, true, nullptr);
			}
		}
#endif
	}
};

#endif // RPGWIDGET_H
