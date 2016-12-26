#ifndef MAINW_H
#define MAINW_H

#include <QMainWindow>
#include <RPG/com/RpgWidget.h>

class Mainw : public QMainWindow
{
	Q_OBJECT
	RpgWidget *w = new RpgWidget(this);
public:
	Mainw(QWidget *parent = 0);
	~Mainw();
};

#endif // MAINW_H
