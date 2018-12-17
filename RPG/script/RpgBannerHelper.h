#ifndef RPGBANNERHELPER_H
#define RPGBANNERHELPER_H

#include <RPG/Global.h>
#include <RPG/com/RpgBanner.h>

#include <RPG/exception/RpgNullPointerException.h>

class RpgBannerHelper: public QObject
{
	Q_OBJECT

	RpgBanner *rpgBanner = nullptr;
public:
	RpgBannerHelper(RpgBanner *rpgBanner, QObject *parent = nullptr) : QObject(parent){
		if(rpgBanner == nullptr){
			throw RpgNullPointerException("rpgBanner");
			qDebug() << CodePath() << "RpgBanner nullptr.";
			return;
		}
		this->rpgBanner = rpgBanner;
	}

	Q_INVOKABLE void exec(){
		if(this->rpgBanner == nullptr){
			throw RpgNullPointerException("this->rpgBanner");
			qDebug() << CodePath() << "RpgBanner nullptr";
			return;
		}
		this->rpgBanner->exec();
	}

	Q_INVOKABLE void execExit(){
		if(this->rpgBanner == nullptr){
			throw RpgNullPointerException("this->rpgBanner");
			qDebug() << CodePath() << "RpgBanner nullptr";
			return;
		}
		this->rpgBanner->execExit();
	}

	Q_INVOKABLE int waitingForBannerComplete(){
		if(this->rpgBanner == nullptr){
			throw RpgNullPointerException("this->rpgBanner");
			qDebug() << CodePath() << "RpgBanner nullptr";
			return -1;
		}
		return this->rpgBanner->waitingForBannerComplete();
	}

//	Q_INVOKABLE void setForegroundPixmap(const QPixmap &pixmap){
//		if(this->rpgBanner == nullptr){
//			qDebug() << CodePath() << "RpgBanner nullptr";
//			return;
//		}
//		this->rpgBanner->setForegroundPixmap(pixmap);
//	}

	Q_INVOKABLE void setForegroundPixmap(const QString &name){
		if(this->rpgBanner == nullptr){
			throw RpgNullPointerException("this->rpgBanner");
			qDebug() << CodePath() << "RpgBanner nullptr";
			return;
		}
		this->rpgBanner->setForegroundPixmap(name);
	}

//	Q_INVOKABLE void setBackgroundColor(const QColor &color){
//		if(this->rpgBanner == nullptr){
//			qDebug() << CodePath() << "RpgBanner nullptr";
//			return;
//		}
//		this->rpgBanner->setBackgroundColor(color);
//	}

	Q_INVOKABLE void setBackgroundColor(const QString &color){
		if(this->rpgBanner == nullptr){
			throw RpgNullPointerException("this->rpgBanner");
			qDebug() << CodePath() << "RpgBanner nullptr";
			return;
		}
		this->rpgBanner->setBackgroundColor(QColor(color));
	}

	Q_INVOKABLE void setBackgroundPixmap(const QPixmap &pixmap){
		if(this->rpgBanner == nullptr){
			throw RpgNullPointerException("this->rpgBanner");
			qDebug() << CodePath() << "RpgBanner nullptr";
			return;
		}
		this->rpgBanner->setBackgroundPixmap(pixmap);
	}

	Q_INVOKABLE void setBackgroundWithForeground(){
		if(this->rpgBanner == nullptr){
			throw RpgNullPointerException("this->rpgBanner");
			qDebug() << CodePath() << "RpgBanner nullptr";
			return;
		}
		this->rpgBanner->setBackgroundWithForeground();
	}

	Q_INVOKABLE void setSpeed(int ms){
		if(this->rpgBanner == nullptr){
			throw RpgNullPointerException("this->rpgBanner");
			qDebug() << CodePath() << "RpgBanner nullptr";
			return;
		}
		this->rpgBanner->setSpeed(ms);
	}

	Q_INVOKABLE void setStartOpacity(qreal opacity){
		if(this->rpgBanner == nullptr){
			throw RpgNullPointerException("this->rpgBanner");
			qDebug() << CodePath() << "RpgBanner nullptr";
			return;
		}
		this->rpgBanner->setStartOpacity(opacity);
	}

	Q_INVOKABLE void setEndOpacity(qreal opacity){
		if(this->rpgBanner == nullptr){
			throw RpgNullPointerException("this->rpgBanner");
			qDebug() << CodePath() << "RpgBanner nullptr";
			return;
		}
		this->rpgBanner->setEndOpacity(opacity);
	}

	Q_INVOKABLE void setEasingCurveType(QEasingCurve::Type type){
		if(this->rpgBanner == nullptr){
			throw RpgNullPointerException("this->rpgBanner");
			qDebug() << CodePath() << "RpgBanner nullptr";
			return;
		}
		this->rpgBanner->setEasingCurveType(type);
	}

	Q_INVOKABLE void setCanBeInterrupt(bool able){
		if(this->rpgBanner == nullptr){
			throw RpgNullPointerException("this->rpgBanner");
			qDebug() << CodePath() << "RpgBanner nullptr";
			return;
		}
		this->rpgBanner->setCanBeInterrupt(able);
	}
};

#endif // RPGBANNERHELPER_H
