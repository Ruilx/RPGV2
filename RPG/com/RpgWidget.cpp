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
					scene->setScript(key);
					scene->setMap(key);

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
				QJsonValue value = imageObj.value(key);
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


	RpgScene *initializeScene = this->mapList.value(this->bootScriptSceneName);
	this->stage->setScene(initializeScene);


	RpgTileSetBase RpgTileSetBase("data/images/tilesets/016-ForestTown02.png");
	for(int i = 0; i < 50; i++){
		for(int j = 0; j < 50; j++){
			RpgMapBlock *block = nullptr;
			if(i < 25 && j < 25){
				block = new RpgMapBlock(j, i, RpgTileSetBase.getRpgTilePixmap(2, 0), true, initializeScene, nullptr, this);
			}else if(i >= 25 && j < 25){
				block = new RpgMapBlock(j, i, RpgTileSetBase.getRpgTilePixmap(3, 0), true, initializeScene, nullptr, this);
			}else if(i < 25 && j >= 25){
				block = new RpgMapBlock(j, i, RpgTileSetBase.getRpgTilePixmap(4, 0), true, initializeScene, nullptr, this);
			}else if(i >= 25 && j >= 25){
				block = new RpgMapBlock(j, i, RpgTileSetBase.getRpgTilePixmap(1, 0), true, initializeScene, nullptr, this);
			}
			block->show();
		}
	}

	QString nextScene = initializeScene->execScript();

	while(nextScene != "exit"){

	}

	this->setCanBeClose(true);
	RpgMusic::instance()->stopMusic();
	this->doReadyToClose();

}
