#include "Mainw.h"

Mainw::Mainw(QWidget *parent)
	: QMainWindow(parent)
{
	this->setCentralWidget(this->w);
	this->setFixedSize(642, 482);
}

Mainw::~Mainw()
{

}
