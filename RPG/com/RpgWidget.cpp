#include "RpgWidget.h"

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

#ifdef DEBUG
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

	QPixmap *bg = new QPixmap("data/images/background/title.png");
	titleScene->getRpgBanner()->setForegroundPixmap(*bg);
	titleScene->getRpgBanner()->setStartOpacity(0.0f);
	titleScene->getRpgBanner()->setEndOpacity(1.0f);
	titleScene->getRpgBanner()->setSpeed(RpgBanner::SpeedNormal);
	titleScene->getRpgBanner()->exec();
	titleScene->getRpgBanner()->waitingForBannerComplete();

	Utils::msleep(3000);

	titleScene->getRpgBanner()->setStartOpacity(1.0f);
	titleScene->getRpgBanner()->setEndOpacity(0.0f);
	titleScene->getRpgBanner()->execExit();
	titleScene->getRpgBanner()->waitingForBannerComplete();

	//		titleScene->getRpgChoise()->addChoiceText("The First Choice");
	//		titleScene->getRpgChoise()->addChoiceText("The Second Choice");
	//		titleScene->getRpgChoise()->addChoiceText("The Third Choice");
	//		titleScene->getRpgChoise()->addChoiceText("The Fourth Choice");
	//		titleScene->getRpgChoise()->addChoiceText("The Fifth Choice");
	//		titleScene->getRpgDialog()->exec();

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

	QPixmap *aaa = new QPixmap("data/images/drawing/03.png");
	titleScene->getRpgDialog()->setCharacterPixmap(*aaa);
//	titleScene->getRpgDialog()->addText("主人，睡醒了吗");
//	titleScene->getRpgDialog()->addText("主人再不起床的话，太阳都要照到主人的屁屁上了");
//	titleScene->getRpgDialog()->addText("……");
//	titleScene->getRpgDialog()->addText("主人睁开眼睛看看我嘛，诶~ 要不要玩亲亲？");
//	titleScene->getRpgDialog()->addText("……<br>……<br>……");
//	titleScene->getRpgDialog()->addText("嘿嘿，主人的脸好可爱呀，忍不住就要亲上去啦");
//	titleScene->getRpgDialog()->addText("好想跟主人一直一直在一起呢");
//	titleScene->getRpgDialog()->addText("……<br>……<br>……");
//	titleScene->getRpgDialog()->addText("……啊，主人，您醒了呢，呼呼~");
//	titleScene->getRpgDialog()->addText("主人主人，小琳已经为主人做好早饭啦，主人要赶快洗漱然后用餐呢");
//	titleScene->getRpgDialog()->addText("今天天气不错呢，主人今天适合穿这件衣服");
//	titleScene->getRpgDialog()->addText("而且，而且，跟小琳的样子一样呢");
//	titleScene->getRpgDialog()->addText("……嗯？<br>主人穿衣服啦，难道要小琳为主人穿衣服吗，嘿嘿，荣幸之至呢");
//	titleScene->getRpgDialog()->addText("抱抱主人~");
//	titleScene->getRpgDialog()->addText("……<br>……<br>……");
	titleScene->getRpgDialog()->addText("跟主人一直一直在一起呢");
	titleScene->getRpgDialog()->exec();
	titleScene->getRpgDialog()->waitingForDialogComplete();

	titleScene->getRpgChoise()->addChoiceText("继续游戏");
	titleScene->getRpgChoise()->addChoiceText("关闭系统");
	titleScene->getRpgChoise()->exec();
	qDebug() << "Selected:" << titleScene->getRpgChoise()->waitingForChooseComplete();

	if(titleScene->getRpgChoise()->getChose() == 1){
		this->setCanBeClose(true);
		this->doReadyToClose();
	}

//	titleScene->getRpgDialog()->addText("サクラの花が咲く度に");
//	titleScene->getRpgDialog()->addText("こんな気持になるのはいつごろからだっけ");
//	titleScene->getRpgDialog()->addText("昼過ぎの 町外れの校舎");
//	titleScene->getRpgDialog()->addText("散らかった部室 机の上にばら撒かれた楽譜");
//	titleScene->getRpgDialog()->addText("ずっと何かを思い出さないまま");
//	titleScene->getRpgDialog()->addText("誰かの声がする 誰かをずっと呼んている");
//	titleScene->getRpgDialog()->addText("日の光が少しずつ空気を緩めて");
//	titleScene->getRpgDialog()->addText("もうすぐ春がやってくる");
//	titleScene->getRpgDialog()->addText("微睡んだあくびをする 僕の名前を呼ぶ");
//	titleScene->getRpgDialog()->addText("誰かをずっと探している");
//	titleScene->getRpgDialog()->addText("そんな風に目が覚める");
//	titleScene->getRpgDialog()->addText("ここはあの街から随分と離れた都会の片隅");
//	titleScene->getRpgDialog()->addText("遠い昔の思い出は 春の匂いと一緒に");
//	titleScene->getRpgDialog()->addText("今年もまた 僕の元へ");
//	titleScene->getRpgDialog()->exec();

	//		titleScene->getRpgDialog()->setCharacterPixmap(*aaa);
	//		titleScene->getRpgDialog()->addText("……");
	//		titleScene->getRpgDialog()->addText("她离开这个世界已经三年了。");
	//		titleScene->getRpgDialog()->addText("三年里，我感觉都是在痛苦中度过的。");
	//		titleScene->getRpgDialog()->addText("接受了现实，但又很舍不得。");
	//		titleScene->getRpgDialog()->addText("她说过要一辈子在一起的，就因为那场大火，让我们阴阳两隔。");
	//		titleScene->getRpgDialog()->addText("大火袭来的一瞬间，她把我推了出去，随后，身后就出现了爆炸，她也被淹没在了火海里没了踪迹。");
	//		titleScene->getRpgDialog()->addText("火灾扑灭之后的搜救，未见她的踪迹。<br>大家都认为她已经死了，尸骨都没有找到。");
	//		titleScene->getRpgDialog()->addText("……");
	//		titleScene->getRpgDialog()->addText("临海公园。");
	//		titleScene->getRpgDialog()->addText("我已经在这里呆坐了整整三天了。每天疲惫地看着大海，无时不刻回想着那场灾难。");
	//		titleScene->getRpgDialog()->addText("约定好脆弱，好脆弱……");
	//		titleScene->getRpgDialog()->addText("");
	//		titleScene->getRpgDialog()->addText(" ");
	//		titleScene->getRpgDialog()->addText(" ");
	//		titleScene->getRpgDialog()->addText(" ");
	//		titleScene->getRpgDialog()->exec();
#endif
}
