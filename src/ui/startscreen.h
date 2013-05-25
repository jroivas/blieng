#ifndef __UI_STARTSCREEN_H
#define __UI_STARTSCREEN_H

#include <QtGui>

namespace ui
{

class StartScreen : public QWidget
{
	Q_OBJECT
public:
	StartScreen(QWidget *parent=0);
	void paintEvent(QPaintEvent *event);

public slots:
	void newGame();
	void changeSettings();

private:
	QImage bg_img;
	QPushButton *new_game;
	QPushButton *settings;
	QPushButton *quit;
	QVBoxLayout layout;
};

}
#endif
