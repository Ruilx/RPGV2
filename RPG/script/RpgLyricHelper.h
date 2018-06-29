#ifndef RPGLYRICHELPER_H
#define RPGLYRICHELPER_H

#include <RPG/Global.h>
#include <RPG/com/RpgLyric.h>

class RpgLyricHelper : public QObject
{
	Q_OBJECT

	RpgLyric *rpgLyric = nullptr;
public:
	explicit RpgLyricHelper(RpgLyric *rpgLyric, QObject *parent = nullptr) : QObject(parent){
		if(rpgLyric == nullptr){
			qDebug() << CodePath() << "RpgLyric nullptr.";
			return;
		}
		this->rpgLyric = rpgLyric;
	}

	Q_INVOKABLE void loadLyric(const QString &filename){
		if(this->rpgLyric == nullptr){
			qDebug() << CodePath() << "RpgLyric nullptr.";
			return;
		}
		this->rpgLyric->loadLyric(filename);
	}

	Q_INVOKABLE void exec(){
		if(this->rpgLyric == nullptr){
			qDebug() << CodePath() << "RpgLyric nullptr.";
			return;
		}
		this->rpgLyric->exec();
	}

	Q_INVOKABLE void setTextColor(const QString &color){
		if(this->rpgLyric == nullptr){
			qDebug() << CodePath() << "RpgLyric nullptr.";
			return;
		}
		this->rpgLyric->setTextColor(QColor(color));
	}

	Q_INVOKABLE void setFont(const QFont &font){
		if(this->rpgLyric == nullptr){
			qDebug() << CodePath() << "RpgLyric nullptr.";
			return;
		}
		this->rpgLyric->setFont(font);
	}

	Q_INVOKABLE void setPos(const QPoint &pos){
		if(this->rpgLyric == nullptr){
			qDebug() << CodePath() << "RpgLyric nullptr.";
			return;
		}
		this->rpgLyric->setPos(pos);
	}

	Q_INVOKABLE void _dumpCurrentLyric(){
		if(this->rpgLyric == nullptr){
			qDebug() << CodePath() << "RpgLyric nullptr.";
			return;
		}
		this->rpgLyric->_dumpCurrentLyric();
	}

signals:

public slots:
};

#endif // RPGLYRICHELPER_H
