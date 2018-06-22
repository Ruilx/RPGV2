#include "RpgLyric.h"



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

void RpgLyric::loadLyric(const QString &filename){
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
