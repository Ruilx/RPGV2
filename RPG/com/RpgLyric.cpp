#include "RpgLyric.h"



void RpgLyric::lyricAnimationShow(){
	if(this->isVisible() == false){
		qDebug() << CodePath() << "This object is not visible, cannot show the box.";
		return;
	}
	if(this->lyricOpacityAnimation->state() != QPropertyAnimation::Stopped){
		//qDebug() << CodePath() << "Lyric animation is working, cannot running when animation is running.";
		this->lyricOpacityAnimation->stop();
		qDebug() << CodePath() << "SHOW========STOPPED========";
		//this->lyricOpacityAnimation->setCurrentTime(this->lyricOpacityAnimation->totalDuration());
		//return;
	}
	this->lyricOpacityAnimation->setDirection(QPropertyAnimation::Forward);
	this->lyricOpacityAnimation->setCurrentTime(0);
	this->lyricOpacityAnimation->start();
}

void RpgLyric::lyricAnimationHide(){
	if(this->isVisible() == false){
		qDebug() << CodePath() << "This object is not visible, cannot hide the box.";
		return;
	}
	if(this->lyricOpacityAnimation->state() != QPropertyAnimation::Stopped){
		//qDebug() << CodePath() << "Lyric animation is working, cannot running when animation is running.";
		this->lyricOpacityAnimation->stop();
		qDebug() << CodePath() << "HIDE========STOPPED========";
		//this->lyricOpacityAnimation->setCurrentTime(0);
		//return;
	}
	this->lyricOpacityAnimation->setDirection(QPropertyAnimation::Backward);
	this->lyricOpacityAnimation->setCurrentTime(this->lyricOpacityAnimation->totalDuration());
	this->lyricOpacityAnimation->start();
	Utils::msleep(150);
}

RpgLyric::RpgLyric(QGraphicsScene *parentScene, QObject *parent): RpgObject(parentScene, parent, nullptr){
	this->lyric->setDefaultTextColor(QColor(Qt::white));
	this->lyric->document()->setDefaultStyleSheet(this->css);

	//		QFont font = Global::applicationFont;{
	//			font.setFamily("aquafont");
	//			font.setPixelSize(22);
	//			font.setBold(false);
	//		}

	QFont font = Global::applicationFont;{
		font.setPixelSize(18);
		font.setBold(false);
	}

	this->lyric->setFont(font);

	this->setZValue(LyricZValue);
	this->lyric->setZValue(0.0f);

	this->lyricShadowEffect->setColor(QColor("#202020"));
	this->lyricShadowEffect->setBlurRadius(5.0f);
	this->lyricShadowEffect->setOffset(1.0f, 1.0f);
	this->lyric->setGraphicsEffect(this->lyricShadowEffect);

	this->lyricOpacityAnimation->setEasingCurve(QEasingCurve::OutCubic);
	this->lyricOpacityAnimation->setStartValue(0.0f);
	this->lyricOpacityAnimation->setEndValue(1.0f);
	this->lyricOpacityAnimation->setDuration(300);
	this->lyricOpacityAnimation->setLoopCount(1);

}

void RpgLyric::setRpgMusic(RpgMusic *music){
	if(this->musicObj != nullptr){
		this->musicObj->disconnect();
	}
	this->musicObj = music;
	this->connect(this->musicObj, &RpgMusic::seeked, this, [this](int ms){
		if(this->lyricI == this->lyricMap.constEnd() || !this->getProcessing()){
			return;
		}
		while(this->lyricI != this->lyricMap.constEnd() && int(this->lyricI.key()) < ms){
			this->lyricI++;
		}
		if(int(this->lyricI.key()) <= ms + 300){
			//			qDebug() << "Key:" << this->lyricI.key() << "Ms:" << ms << "Ms+300:" << ms + 300;
			this->lyricAnimationHide();
			Utils::msleep(int(this->lyricI.key()) - ms);
			this->lyric->setHtml(this->lyricI.value());
			this->lyricAnimationShow();
			this->lyricI++;
		}
	});

	this->connect(this->musicObj, &RpgMusic::stopped, this, [this](){
		if(!this->getProcessing()){
			return;
		}
		this->lyricAnimationHide();
		Utils::msleep(300);
		this->hideLyric();
		this->lyricI = this->lyricMap.constBegin();
		this->setProcessing(false);
	});
}

void RpgLyric::loadLyricFromFile(const QString &filename){
	if(this->getProcessing()){
		qDebug() << CodePath() << "Cannot load the lyric while it is running!";
		return;
	}
	if(filename.isEmpty() || !QFile::exists(filename)){
		qDebug() << CodePath() << QString("filename \"%1\" is not exist.").arg(filename);
		return;
	}
	QFile f(filename);
	if(!f.open(QIODevice::ReadOnly)){
		qDebug() << CodePath() << QString("filename \"%1\" cannot open: %2").arg(filename).arg(f.errorString());
		return;
	}
	this->lyricMap.clear();
	QTextStream ts(&f);
	ts.setCodec("utf-8");
	while(!ts.atEnd()){
		QString line = ts.readLine();
		if(!line.startsWith("[")){
			qDebug() << CodePath() << QString("Invalid line data: not starts with '[': \"%1\". Ignored.").arg(line);
			continue;
		}
		int rightBraPos = line.indexOf("]");
		if(rightBraPos == -1){
			qDebug() << CodePath() << QString("Invalid line data: cannot find timestamp tip: \"%1\". Ignored.").arg(line);
			continue;
		}
		QString timestamp = line.mid(1, rightBraPos -1);
		QString lyricText;
		if(line.length() >= (rightBraPos +1)){
			lyricText = line.mid(rightBraPos + 1);
		}

		bool ok = false;
		int intTimestamp = timestamp.toInt(&ok);
		if(!ok){
			double doubleTimestamp = timestamp.toDouble(&ok);
			if(!ok){
				QTime timeTimestamp = QTime::fromString(timestamp, "hh:mm:ss.zzz");
				if(timeTimestamp.isNull() || !timeTimestamp.isValid()){
					timeTimestamp = QTime::fromString(timestamp, "mm:ss.zzz");
					if(timeTimestamp.isNull() || !timeTimestamp.isValid()){
						qDebug() << CodePath() << QString("Invalid line data: timestamp cannot resolve to [int], [double] [hh:mm:ss.zzz] or [mm:ss:zzz]: \"%1\". Ignored.").arg(line);
						continue;
					}else{
						this->lyricMap.insert(timeTimestamp.msecsSinceStartOfDay(), lyricText);
						continue;
					}
				}else{
					this->lyricMap.insert(timeTimestamp.msecsSinceStartOfDay(), lyricText);
					continue;
				}
			}else{
				this->lyricMap.insert(int(doubleTimestamp * 1000), lyricText);
				continue;
			}
		}else{
			this->lyricMap.insert(intTimestamp, lyricText);
			continue;
		}
	}
	this->lyricI = this->lyricMap.constBegin();
	if(f.isOpen()){
		f.close();
	}
}

void RpgLyric::loadLyric(const QString &name){
	QString filename = RpgFileManager::instance()->getFileString(RpgFileManager::LyricFile, name);
	if(filename.isEmpty()){
		qDebug() << CodePath() << "Name:" << name << "no file name being found.";
		return;
	}
	this->loadLyricFromFile(filename);
}

void RpgLyric::exec(){
	RpgObject::exec();
	if(this->getParentScene() == nullptr){
		qDebug() << CodePath() << "Parent scene is not set. (Null)";
		return;
	}
	if(this->lyricMap.isEmpty()){
		qDebug() << CodePath() << "Lyric map is empty, please load one lyric first, cannot execute.";
		return;
	}
	if(this->getProcessing() == true){
		qDebug() << CodePath() << "RpgLyric is Running, please don't call it repeatly!";
		return;
	}
	if(this->musicObj != nullptr && this->musicObj->getLoop() != 1){
		qDebug() << CodePath() << "The loop BGM cannot display the lyric...";
		return;
	}
	this->lyric->setPos(this->pos);
	this->showLyric();
}

void RpgLyric::showLyric(){
	if(this->lyric->isVisible()){
		qDebug() << CodePath() << "Item was shown, cannot show again.";
		return;
	}
	this->setProcessing(true);
	this->lyric->setOpacity(1.0f);
	this->show();
}

void RpgLyric::hideLyric(){
	if(!this->lyric->isVisible()){
		qDebug() << CodePath() << "Item was not shown, cannot hide.";
	}
	this->lyric->setOpacity(0.0f);
	this->hide();
}
