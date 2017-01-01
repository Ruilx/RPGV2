#include "Mainw.h"

Mainw::Mainw(QWidget *parent)
	: QMainWindow(parent)
{
	this->setCentralWidget(this->w);
	this->setFixedSize(642, 482);

	QImage *a = new QImage(100, 100, QImage::Format_RGBA8888_Premultiplied);
	a->fill(Qt::transparent);
	QPainter p(a);
	p.setCompositionMode(QPainter::CompositionMode_Source);
	p.setPen(QColor(Qt::black));
	p.drawLine(0, 0, 50, 50);
	a->save("1.png");
	p.drawLine(0, 0, 75, 75);
	a->save("2.png");
	p.drawLine(0, 0, 100, 100);
	a->save("3.png");
	p.end();
	a->save("4.png");
}

Mainw::~Mainw()
{

}
