#ifndef RPGSOUND_H
#define RPGSOUND_H

#include <QtCore>
#include <QSound>
#include <RPG/Global.h>

class RpgSound : public QObject
{
	Q_OBJECT
	static RpgSound *_instance;

	QHash<QString, QSound*> soundMap;
public:
	static RpgSound *instance(){
		if(_instance == nullptr){
			_instance = new RpgSound(nullptr);
		}
		return _instance;
	}

	explicit RpgSound(QObject *parent = nullptr) : QObject(parent){
		this->soundMap.clear();
	}

	void addSound(const QString &soundName, const QString &fileName);
	void removeSound(const QString &soundName);

	void clearSound();
signals:
	void started(const QString &soundName);
	void stopped(const QString &soundName);
public slots:
	void play(const QString &soundName, /*qreal volume = 1.0f,*/ int times = 1);
	void stop(const QString &soundName);
};

#endif // RPGSOUND_H
