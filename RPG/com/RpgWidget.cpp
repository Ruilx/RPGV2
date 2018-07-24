#include "RpgWidget.h"
#include "RpgSound.h"

void RpgWidget::keyReleaseEvent(QKeyEvent *event){
	// 用户按下的按键
	int key = event->key();
	// 用户是否使用了Shift, Ctrl, Alt, Win(Meta)键对按键进行修饰(一块按下了)
	Qt::KeyboardModifiers mod = event->modifiers();

	RpgState *state = RpgState::instance();
	if(key == Qt::Key_Escape && state->getTop() != RpgState::AutoMode){
		// 用户按下Esc键, 任何情况下都可以进入系统菜单(额... 如果是AutoMode的话就不能点了)
		if(state->getTop() == RpgState::SystemMenuMode){
			// 如果已经在系统模式, 则退出系统模式
			qDebug() << "RpgWidget: KeyReleaseEvent: User Release ESCAPE key in SystemMenuMode.";
			event->accept();
			return;
		}else{
			state->pushMode(RpgState::SystemMenuMode);
			qDebug() << CodePath() << "Enter SystemMenuMode.";
			event->accept();
			return;
		}
	}
	//交给state的key函数, 让它去进行调用分配
	state->receiveKey(key, mod, this->stage->scene());
	event->accept();

//	switch(state->getTop()){
//		case RpgState::AutoMode:
//			// 自动模式下不接受任何按键.
//			break;
//		case RpgState::NormalMode:
//			// 普通模式, 接受W, A, S, D, Space, Return, ArrowU, ArrowD, ArrowL, ArrowR, Z(道具栏)
//			//TODO: 没有写相关的运动函数, 这里输出一条Debug代替
//			qDebug() << tr("RpgWidget: KeyReleaseEvent: User Release %1(%2) Key in NormalMode.").arg(key).arg(event->text().toHtmlEscaped());
//			event->accept();
//			break;
//		case RpgState::SystemMenuMode:
//			// 系统菜单模式, 接受W, A, S, D, ArrowU, ArrowD, ArrowL, ArrowR, Space, Return, Esc(前面已经处理过了)
//			//TODO: 没有写系统菜单栏目, 这里输出一条Debug代替
//			qDebug() << tr("RpgWidget: KeyReleaseEvent: User Release %1(%2) Key in SystemMenuMode.").arg(key).arg(event->text().toHtmlEscaped());
//			event->accept();
//			break;
//		case RpgState::DialogMode:
//			// 对话模式, 接受Return(Enter), Space
//			emit this->dialogModeKeyClick(key, mod);
//			event->accept();
//			break;
//		case RpgState::ItemMode:
//			// 道具栏(背包)模式, 接受W, A, S, D, ArrowU, ArrowD, ArrowL, ArrowR, Space, Return
//			//TODO: 没有写系统背包栏目, 这里输出一条Debug代替
//			qDebug() << tr("RpgWidget: KeyReleaseEvent: User Release %1(%2) Key in ItemMode.").arg(key).arg(event->text().toHtmlEscaped());
//			event->accept();
//			break;
//		default:
//			qDebug() << tr("RpgWidget: KeyReleaseEvent: Cannot recongized which mode it is? User Release %1(%2) Key.").arg(key).arg(event->text().toHtmlEscaped());
//			event->ignore();
//			break;
//	}
}

void RpgWidget::closeEvent(QCloseEvent *event){
	Q_UNUSED(event)
}

bool RpgWidget::biosSet(const QByteArray &initializationConfigureJson){
	if(initializationConfigureJson.isEmpty()){
		qDebug() << CodePath() << "Initialization configure text is empty.";
		return false;
	}
	QJsonParseError error;
	QJsonDocument initDoc = QJsonDocument::fromJson(initializationConfigureJson, &error);
	if(error.error != QJsonParseError::NoError){
		qDebug() << CodePath() << "Read initialize configure json parse error." << error.errorString();
		return false;
	}
	if(!initDoc.isObject()){
		QString type;
		if(initDoc.isNull()){
			type = "Null";
		}else if(initDoc.isArray()){
			type = "Array";
		}else if(initDoc.isEmpty()){
			type = "Empty";
		}else{
			type = "Unknown";
		}
		qDebug() << CodePath() << "Root object must be an object." << type << "found.";
		return false;
	}
	QJsonObject root = initDoc.object();
	QStringList keys = root.keys();
	for(const QString &key: keys){
		QJsonValue value = root.value(key);
		if(key == "init"){
			// Execute Script Scene
			if(!value.isString()){
				qDebug() << CodePath() << "Init value is not a string." << Utils::detectedJsonValue(value) << "found.";
				continue;
			}
			this->bootScriptSceneName = value.toString();
		}else if(key == "map"){
			// Map List
			if(!value.isObject()){
				qDebug() << CodePath() << "Map object is not an object." << Utils::detectedJsonValue(value) << "found.";
				continue;
			}
			QJsonObject mapObj = value.toObject();
			QStringList keys = mapObj.keys();
			for(const QString &key: keys){
				QJsonValue value = mapObj.value(key);
				if(!value.isObject()){
					qDebug() << CodePath() << "Map Scene is not an object." << Utils::detectedJsonValue(value) << "found.";
					continue;
				}
				QString mapName;
				QString scriptName;
				QJsonObject sceneNameObj = value.toObject();
				QStringList keys = sceneNameObj.keys();
				for(const QString &key: keys){
					QJsonValue value = sceneNameObj.value(key);
					if(key == "map"){
						mapName = value.toString();
					}else if(key == "script"){
						scriptName = value.toString();
					}else{
						continue;
					}
				}
				if(mapName.isEmpty() || scriptName.isEmpty()){
					qDebug() << CodePath() << "Map file name or script name is/are empty, the scene:" << key << "invaild.";
					continue;
				}else{

					RpgFileManager::instance()->addFile(RpgFileManager::MapFile, key, QUrl(mapName));
					RpgFileManager::instance()->addFile(RpgFileManager::ScriptFile, key, QUrl(scriptName));

					RpgScene *scene = new RpgScene(this);
					scene->setScript(scriptName);
					scene->setMap(mapName);

					this->mapList.insert(key, scene);
				}
			}
		}else if(key == "music"){
			// Music List init
			if(!value.isObject()){
				qDebug() << CodePath() << "Music object is not an object." << Utils::detectedJsonValue(value) << "found.";
				continue;
			}
			QJsonObject musicObj = value.toObject();
			QStringList keys = musicObj.keys();
			for(const QString &key: keys){
				QJsonValue value = musicObj.value(key);
				QString musicFile = value.toString();
				if(musicFile.isEmpty()){
					qDebug() << CodePath() << "Music file name is empty";
					continue;
				}else{
					RpgFileManager::instance()->addFile(RpgFileManager::MusicFile, key, musicFile);
					//RpgMusic::instance()->addMusic(key, musicFile);
				}
			}
		}else if(key == "se"){
			// Sound effect init
			if(!value.isObject()){
				qDebug() << CodePath() << "Sound effect is not an object." << Utils::detectedJsonValue(value) << "found.";
				continue;
			}
			QJsonObject soundObj = value.toObject();
			QStringList keys = soundObj.keys();
			for(const QString &key: keys){
				QJsonValue value = soundObj.value(key);
				QString soundFile = value.toString();
				if(soundFile.isEmpty()){
					qDebug() << CodePath() << "Sound file name is empty";
					continue;
				}else{
					RpgFileManager::instance()->addFile(RpgFileManager::SeFile, key, soundFile);
					//RpgSound::instance()->addSound(key, soundFile);
				}
			}
		}else if(key == "image"){
			// Image init
			if(!value.isObject()){
				qDebug() << CodePath() << "Image effect is not an object." << Utils::detectedJsonValue(value) << "found.";
				continue;
			}
			QJsonObject imageObj = value.toObject();
			QStringList keys = imageObj.keys();
			for(const QString &key: keys){
				QJsonValue value = soundObj.value(key);
				QString imageFile = value.toString();
				if(imageFile.isEmpty()){
					qDebug() << CodePath() << "Image file name is empty.";
					continue;
				}else{
					RpgFileManager::instance()->addFile(RpgFileManager::ImageFile, key, imageFile);

				}
			}
		}else if(key == "lyric"){
			// Lyric init
			if(!value.isObject()){
				qDebug() << CodePath() << "Lyric is not an object." << Utils::detectedJsonValue(value) << "found.";
				continue;
			}
			QJsonObject lyricObj = value.toObject();
			QStringList keys = lyricObj.keys();
			for(const QString &key: keys){
				QJsonValue value = lyricObj.value(key);
				QString lyricFile = value.toString();
				if(lyricFile.isEmpty()){
					qDebug() << CodePath() << "Lyric file name is empty.";
					continue;
				}else{
					RpgFileManager::instance()->addFile(RpgFileManager::LyricFile, key, lyricFile);
				}
			}
		}else if(key == "font"){
			// Font init
			if(!value.isObject()){
				qDebug() << CodePath() << "Font is not an object." << Utils::detectedJsonValue(value) << "found.";
				continue;
			}
			QJsonObject fontObj = value.toObject();
			QStringList keys = fontObj.keys();
			for(const QString &key: keys){
				QJsonValue value = fontObj.value(key);
				QString fontFile = value.toString();
				if(fontFile.isEmpty()){
					qDebug() << CodePath() << "Font file name is empty.";
					continue;
				}else{
					RpgFileManager::instance()->addFile(RpgFileManager::FontFile, key, fontFile);
				}
			}
		}else if(key == "avatar"){
			// Avatar init
			if(!value.isObject()){
				qDebug() << CodePath() << "Avatar is not an object." << Utils::detectedJsonValue(value) << "found.";
				continue;
			}
			QJsonObject avatarObj = value.toObject();
			QStringList keys = avatarObj.keys();
			for(const QString &key: keys){
				QJsonValue value = avatarObj.value(key);
				QString avatarFile = value.toString();
				if(avatarFile.isEmpty()){
					qDebug() << CodePath() << "Font file name is empty.";
					continue;
				}else{
					RpgFileManager::instance()->addFile(RpgFileManager::AvatarFile, key, avatarFile);
				}
			}
		}else if(key == "file"){
			// Normal file
			if(!value.isObject()){
				qDebug() << CodePath() << "file is not an object." << Utils::detectedJsonValue(value) << "found.";
				continue;
			}
			QJsonObject fileObj = value.toObject();
			QStringList keys = fileObj.keys();
			for(const QString &key: keys){
				QJsonValue value = fileObj.value(key);
				QString file = value.toString();
				if(file.isEmpty()){
					qDebug() << CodePath() << "Font file name is empty.";
					continue;
				}else{
					RpgFileManager::instance()->addFile(RpgFileManager::NormalFile, key, file);
				}
			}
		}else{
			// Extra
			continue;
		}
	}
	return true;
}

RpgWidget::RpgWidget(QWidget *parent): QWidget(parent){

	// 新建一个Scene, 然后加入地图列表
	//RpgScene *titleScene = new RpgScene(this);
	//this->mapList.insert("titlescene", titleScene);
	// 设置显示偏移量
	//titleScene->setScenePos(0.0f, 0.0f);
	// 设置当前地图
	//this->stage->setScene(titleScene);
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

	//	connect(titleScene->getRpgDialog(), &RpgDialog::enterDialogMode, this, [this](){
	//		this->modeStack.push(DialogMode);
	//		this->enterDialogMode();
	//	});

	//	connect(titleScene->getRpgDialog(), &RpgDialog::quitDialogMode, this, [this](){
	//		if(this->modeStack.top() == DialogMode){
	//			this->modeStack.pop();
	//		}else{
	//			qDebug() << "RpgWidget::[RpgDialog::quitDialogMode LambdaMoc]: Current Mode Stack top is not dialogMode!";
	//			return;
	//		}
	//		this->quitDialogMode();
	//	});

	//	connect(titleScene->getRpgChoise(), &RpgChoice::enterDialogMode, this, [this](){
	//		this->modeStack.push(DialogMode);
	//		this->enterDialogMode();
	//	});

	//	connect(titleScene->getRpgChoise(), &RpgChoice::quitDialogMode, this, [this](){
	//		if(this->modeStack.top() == DialogMode){
	//			this->modeStack.pop();
	//		}else{
	//			qDebug() << "RpgChoice::[RpgDialog::quitDialogMode LambdaMoc]: Current Mode Stack top is not dialogMode!";
	//			return;
	//		}
	//		this->quitDialogMode();
	//	});

	//	connect(titleScene->getRpgBanner(), &RpgBanner::enterAutoMode, this, [this](){
	//		this->modeStack.push(AutoMode);
	//		this->enterAutoMode();
	//	});

	//	connect(titleScene->getRpgBanner(), &RpgBanner::quitAutoMode, this, [this](){
	//		if(this->modeStack.top() == AutoMode){
	//			this->modeStack.pop();
	//		}else{
	//			qDebug() << "RpgWidget::[RpgDialog::quitAutoMode LambdaMoc]: Current Mode Stack top is not autoMode!";
	//			return;
	//		}
	//		this->quitAutoMode();
	//	});

	//	connect(this, &RpgWidget::dialogModeKeyClick, titleScene->getRpgDialog(), &RpgDialog::receiveKey);
	//	connect(this, &RpgWidget::dialogModeKeyClick, titleScene->getRpgChoise(), &RpgChoice::receiveKey);

	this->modeStack.push(NormalMode);

	QTimer::singleShot(2000, this, &RpgWidget::ready);

	this->stage->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);
}

void RpgWidget::ready(){
	qDebug() << CodePath() << "[System] Ready...";
	//	RpgScene *titleScene = this->mapList.value("titlescene", nullptr);
	//	if(titleScene == nullptr){
	//		qDebug() << "RpgWidget::ready(): titleScene(\"titlescene\") not exist.";
	//		return;
	//	}

	QString scriptInitPath = "data/scripts/initialize.json";
	QByteArray initializeJson = Utils::readFile(scriptInitPath);
	if(initializeJson.isEmpty()){
		qDebug() << CodePath() << "Read initializaion file failed.";
		this->setCanBeClose(true);
		this->doReadyToClose();
		return;
	}
	if(!this->biosSet(initializeJson)){
		qDebug() << CodePath() << "BIOS initializaion file failed.";
		this->setCanBeClose(true);
		this->doReadyToClose();
		return;
	}
	if(this->bootScriptSceneName.isEmpty()){
		qDebug() << CodePath() << "BIOS had not specified what script will execute first.";
		this->setCanBeClose(true);
		this->doReadyToClose();
		return;
	}
	RpgScene *initializaScene = this->mapList.value(this->bootScriptSceneName);
	this->stage->setScene(initializaScene);
	QString nextScene = initializaScene->execScript();

//	RpgLyric *lyric = new RpgLyric(initializaScene, initializaScene);
//	RpgLyric *lyric = initializaScene->getRpgLyric();
//	lyric->loadLyric("data/lyrics/カンタレラ~grace edition~.lrc");
//	lyric->_dumpCurrentLyric();
//	lyric->exec();

	while(nextScene != "exit"){

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

	//RpgMusic::instance()->addMusic("title", "data/sounds/月光（オルゴール）.mp3");
//	RpgMusic::instance()->addMusic("title", "H:/kuruiduki_WolfData/BGM/沈降.ogg");
//	RpgMusic::instance()->addMusic("op1", "data/sounds/雨の日に.mp3");

//	RpgSound::instance()->addSound("select", "data/se/select.wav");
//	RpgSound::instance()->addSound("banned", "data/se/banned.wav");

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

//	Utils::msleep(1000);


//	RpgMusic::instance()->playMusic("title");

//	QPixmap *title = new QPixmap("data/images/background/タイトル画面_背景.jpg");
//	QPixmap mix_title = QPixmap("data/images/background/mix/タイトルロゴ.png");
//	QPixmap mix_character = QPixmap("data/images/background/mix/タイトル_キャラ01.png");

//	RpgItem *item_title = new RpgItem(titleScene->getRpgBanner());
//	item_title->setPixmap(mix_title);
//	item_title->setRealPos(titleScene->getRpgBanner()->pos() + QPointF((title->width() - mix_title.width()) / 2, -5));
//	item_title->setZValue(0.3);

//	RpgItem *item_character = new RpgItem(titleScene->getRpgBanner());
//	item_character->setPixmap(mix_character);
//	item_character->setRealPos(titleScene->getRpgBanner()->pos() + QPointF(7, (title->height() - mix_character.height())));
//	item_character->setZValue(0.2);
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
//	qDebug() << titleScene->getRpgBanner();
//	qDebug() << titleScene->getRpgBanner()->childItems();
//	qDebug() << item_title << item_character;
//	titleScene->getRpgBanner()->exec();
//	titleScene->getRpgBanner()->waitingForBannerComplete();

//	titleScene->getRpgChoise()->addChoiceText("始める");
//	titleScene->getRpgChoise()->addChoiceText("続ける");
//	titleScene->getRpgChoise()->addChoiceText("?????");
//	titleScene->getRpgChoise()->addChoiceText("終わる");
//	titleScene->getRpgChoise()->setDialogWidth(250);
//	titleScene->getRpgChoise()->setChoiceEnabled(1, false);
//	titleScene->getRpgChoise()->setChoiceEnabled(2, false);
//	titleScene->getRpgChoise()->exec();
//	qDebug() << "Selected:" << titleScene->getRpgChoise()->waitingForChooseComplete();

//	if(titleScene->getRpgChoise()->getChose() == 3){
//		this->setCanBeClose(true);
//		RpgMusic::instance()->stopMusic();
//		this->doReadyToClose();
//	}

//	RpgMusic::instance()->stopMusic();

//	QPixmap *aaa = new QPixmap("data/images/drawing/03.png");
//	titleScene->getRpgDialog()->setCharacterPixmap(*aaa);
//	titleScene->getRpgDialog()->addText("**测试一段可显示的文字**<r>测试一段红色的文字</r>**<g>测试一段绿色的文字</g>**<b>测试一段蓝色的文字</b>**<y>测试一段黄色的文字<y>**");
//	titleScene->getRpgDialog()->exec();
//	titleScene->getRpgDialog()->waitingForDialogComplete();

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
