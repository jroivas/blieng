#include "loadingscreen.h"
#include <math.h>
#include "data.h"

static double pi = 3.14159;

using ui::LoadingScreen;

LoadingScreen::LoadingScreen(QWidget *parent) : QWidget(parent)
{
	std::string startfile = blieng::Data::getInstance()->findFile("startscreen.png");
	if (startfile != "") {
		bg_img = QImage(startfile.c_str());
	}

	angle = 0;
	radius = 42;
	anim = new QTimer();
	anim->setInterval(10);
	anim->start();
	connect(anim, SIGNAL(timeout()), this, SLOT(update()));
}

void LoadingScreen::drawCircle(QPainter *painter, double _angle, double _radius)
{
	double rads = _angle*pi/180;
	double xdiff = radius * cos(rads);
	double ydiff = radius * sin(rads);

	int xpos = size().width() / 2;
	int ypos = size().height() / 2;
	xpos += xdiff;
	ypos += ydiff;

	painter->setBrush(QColor(Qt::black));
	painter->setPen(QColor(Qt::gray));
	painter->drawEllipse(xpos, ypos, _radius, _radius);
}

void LoadingScreen::paintEvent(QPaintEvent *event)
{
	QPainter paint(this);
	paint.drawImage(0, 0, bg_img.scaled(size(), Qt::KeepAspectRatioByExpanding));

	int i=0;
	for (i=0; i<10; i++) {
		drawCircle(&paint, angle - 20 * i, 10 - i);
		//drawCircle(&paint, angle - 20 * i, i);
	}
	int xpos = size().width() / 2;
	int ypos = size().height() / 2;

	paint.setPen(QColor(Qt::black));
 	QFont ff;
	ff.setPointSize(34);
	paint.setFont(ff);
	QRect bb = paint.boundingRect(QRect(0, 0, width(), 50), Qt::AlignLeft, "Loading...");
	paint.drawText(xpos - bb.width()/2 + 20, ypos + 150, "Loading...");

	angle += 1;
	//angle -= 1;
	while (angle > 360) angle -= 360;
	while (angle < -360) angle += 360;
	paint.end();
}
