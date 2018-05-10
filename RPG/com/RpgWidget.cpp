#include "RpgWidget.h"
#include "RpgSound.h"

void RpgWidget::keyReleaseEvent(QKeyEvent *event){
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

void RpgWidget::closeEvent(QCloseEvent *event){
	Q_UNUSED(event)
}

RpgWidget::RpgWidget(QWidget *parent): QWidget(parent){

	// 新建一个Scene, 然后加入地图列表
	RpgScene *titleScene = new RpgScene(this);
	this->mapList.insert("titlescene", titleScene);
	// 设置显示偏移量
	titleScene->setScenePos(0.0f, 0.0f);
	// 设置当前地图
	this->stage->setScene(titleScene);
	this->stage->setFixedSize(ScreenWidth + 2, ScreenHeight + 2);
	this->stage->scale(1.0f, 1.0f);
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

	connect(titleScene->getRpgChoise(), &RpgChoice::enterDialogMode, this, [this](){
		this->modeStack.push(DialogMode);
		this->enterDialogMode();
	});

	connect(titleScene->getRpgChoise(), &RpgChoice::quitDialogMode, this, [this](){
		if(this->modeStack.top() == DialogMode){
			this->modeStack.pop();
		}else{
			qDebug() << "RpgChoice::[RpgDialog::quitDialogMode LambdaMoc]: Current Mode Stack top is not dialogMode!";
			return;
		}
		this->quitDialogMode();
	});

	connect(titleScene->getRpgBanner(), &RpgBanner::enterAutoMode, this, [this](){
		this->modeStack.push(AutoMode);
		this->enterAutoMode();
	});

	connect(titleScene->getRpgBanner(), &RpgBanner::quitAutoMode, this, [this](){
		if(this->modeStack.top() == AutoMode){
			this->modeStack.pop();
		}else{
			qDebug() << "RpgWidget::[RpgDialog::quitAutoMode LambdaMoc]: Current Mode Stack top is not autoMode!";
			return;
		}
		this->quitAutoMode();
	});

	connect(this, &RpgWidget::dialogModeKeyClick, titleScene->getRpgDialog(), &RpgDialog::receiveKey);
	connect(this, &RpgWidget::dialogModeKeyClick, titleScene->getRpgChoise(), &RpgChoice::receiveKey);

	this->modeStack.push(NormalMode);

	QTimer::singleShot(2000, this, &RpgWidget::ready);

	this->stage->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);
}

void RpgWidget::ready(){
	qDebug() << "[Debug][System] Ready...";
	RpgScene *titleScene = this->mapList.value("titlescene", nullptr);
	if(titleScene == nullptr){
		qDebug() << "RpgWidget::ready(): titleScene(\"titlescene\") not exist.";
		return;
	}

#ifdef QT_DEBUG
//	int blockCol = titleScene->width() / MapBlockWidth;
//	int blockRow = titleScene->height() / MapBlockHeight;
//	RpgTileSetBase rpgTileSetBase("data/images/tilesets/016-ForestTown02.png");
//	for(int i = 0; i < blockRow; i++){
//		for(int j = 0; j < blockCol; j++){
//			RpgMapBlock *block = new RpgMapBlock(j, i, rpgTileSetBase.getRpgTilePixmap(2, 0) , true, titleScene, nullptr, this);
//			//qDebug() << QPixmap::fromImage(*rpgTileSetBase->getRpgTile(0, 0));
//			//block->addPixmap();
//			block->show();
//		}
//	}

//	RpgMusic::instance()->addMusic("title", "data/sounds/月光（オルゴール）.mp3");
//	RpgMusic::instance()->addMusic("op1", "data/sounds/雨の日に.mp3");

//	RpgSound::instance()->addSound("select", "data/se/select.wav");

//	QPixmap *bg = new QPixmap("data/images/background/title2.png");
//	titleScene->getRpgBanner()->setForegroundPixmap(*bg);
//	titleScene->getRpgBanner()->setStartOpacity(0.0f);
//	titleScene->getRpgBanner()->setEndOpacity(1.0f);
//	titleScene->getRpgBanner()->setSpeed(RpgBanner::SpeedNormal);
//	titleScene->getRpgBanner()->exec();
//	titleScene->getRpgBanner()->waitingForBannerComplete();

//	Utils::msleep(3000);

//	titleScene->getRpgBanner()->setStartOpacity(1.0f);
//	titleScene->getRpgBanner()->setEndOpacity(0.0f);
//	titleScene->getRpgBanner()->execExit();
//	titleScene->getRpgBanner()->waitingForBannerComplete();

	//Utils::msleep(1000);


//	RpgMusic::instance()->playMusic("title");

//	QPixmap *title = new QPixmap("data/images/background/タイトル画面_背景.jpg");
//	QPixmap mix_title = QPixmap("data/images/background/mix/タイトルロゴ.png");
//	QPixmap mix_character = QPixmap("data/images/background/mix/タイトル_キャラ01.png");

//	RpgItem *item_title = new RpgItem(nullptr);
//	item_title->setPixmap(mix_title);
//	item_title->setPos(QPointF((title->width() - mix_title.width()) / 2, -5));
//	item_title->setZValue(BackgroundZValue + 0.2f);

//	RpgItem *item_character = new RpgItem(nullptr);
//	item_character->setPixmap(mix_character);
//	item_character->setPos(QPointF(7, (title->height() - mix_character.height())));
//	item_character->setZValue(BackgroundZValue + 0.1f);
//	QPainter p(title);{
//		p.setCompositionMode(QPainter::CompositionMode_SourceOver);
//		p.drawPixmap((7), (title->height() - mix_character.height()), mix_character);
//		p.drawPixmap((title->width() - mix_title.width()) / 2, -5, mix_title);
//		p.end();
//	}
//	titleScene->addRpgItem(item_title);
//	titleScene->addRpgItem(item_character);

//	titleScene->getRpgBanner()->setForegroundPixmap(*title);
//	titleScene->getRpgBanner()->setStartOpacity(0.0f);
//	titleScene->getRpgBanner()->setEndOpacity(1.0f);
//	titleScene->getRpgBanner()->exec();
//	titleScene->getRpgBanner()->waitingForBannerComplete();

	titleScene->getRpgChoise()->addChoiceText("始める");
	titleScene->getRpgChoise()->addChoiceText("続ける");
	titleScene->getRpgChoise()->addChoiceText("開発資料室");
	titleScene->getRpgChoise()->addChoiceText("終わる");
	titleScene->getRpgChoise()->setChoiceEnabled(1, false);
	titleScene->getRpgChoise()->exec();
	qDebug() << "Selected:" << titleScene->getRpgChoise()->waitingForChooseComplete();

	if(titleScene->getRpgChoise()->getChose() == 3){
		this->setCanBeClose(true);
		RpgMusic::instance()->stopMusic();
		this->doReadyToClose();
	}

//	QPixmap *aaa = new QPixmap("data/images/drawing/03.png");
//	titleScene->getRpgDialog()->setCharacterPixmap(*aaa);
//	titleScene->getRpgDialog()->addText("**测试一段可显示的文字**<r>测试一段红色的文字</r>**<g>测试一段绿色的文字</g>**<b>测试一段蓝色的文字</b>**<y>测试一段黄色的文字<y>**");
//	titleScene->getRpgDialog()->exec();

//	QEventLoop eventloop(this);
//	this->connect(titleScene->getRpgDialog(), &RpgDialog::quitDialogMode, &eventloop, &QEventLoop::quit);
//	eventloop.exec();

	//		titleScene->getRpgDialog()->setCharacterPixmap(QPixmap());
	//		titleScene->getRpgDialog()->addText("123456");
	//		titleScene->getRpgDialog()->exec();

//	RpgMusic::instance()->playMusic("op1");

//	RpgAutoTileBase aTileImage("data/images/autotiles/test.png", "test");
//	//aTileImage._dumpImage(0);

//	titleScene->getRpgDialog()->setDialogWidth(250);
//	titleScene->getRpgDialog()->addText("Autotile已经导出.");
//	titleScene->getRpgDialog()->exec();
//	titleScene->getRpgDialog()->waitingForDialogComplete();

//	if(title){
//		delete title;
//	}
//	if(bg){
//		delete bg;
//	}
#endif
}
