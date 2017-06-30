#ifndef RPGBANNER_H
#define RPGBANNER_H

#include <QObject>

class RpgBanner : public QObject
{
	Q_OBJECT
public:
	explicit RpgBanner(QObject *parent = nullptr);

signals:

public slots:
};

#endif // RPGBANNER_H