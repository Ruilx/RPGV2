#include "Mainw.h"

Mainw::Mainw(QWidget *parent)
	: QMainWindow(parent)
{
	this->setCentralWidget(this->w);
	this->setFixedSize(ScreenWidth + 2, ScreenHeight + 2); // Margin: 1 (Left, Right, Top, Bottom)

	this->connect(this->w, &RpgWidget::readyToClose, this, &Mainw::close);
}

Mainw::~Mainw()
{

}
