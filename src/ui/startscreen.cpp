#include "startscreen.h"
#include "data.h"
#include <QApplication>

using ui::StartScreen;

StartScreen::StartScreen(QWidget *parent) : QWidget(parent)
{
    std::string startfile = blieng::Data::getInstance()->findFile("startscreen.png");
    if (startfile != "") {
        bg_img = QImage(startfile.c_str());
    }

    new_game = new QPushButton(tr("New Game"));
    settings = new QPushButton(tr("Settings"));
    quit = new QPushButton(tr("Quit"));

    /*
    new_game->setFlat(true);
    settings->setFlat(true);
    quit->setFlat(true);
    */
    new_game->setStyleSheet("* { color: black; background-color: rgba(0,0,225,100) }");
    settings->setStyleSheet("* { color: black; background-color: rgba(0,0,225,100) }");
    quit->setStyleSheet("* { color: black; background-color: rgba(0,0,225,100) }");

    layout.addWidget(new_game);
    layout.addWidget(settings);
    layout.addWidget(quit);

    setLayout(&layout);

    connect(new_game, SIGNAL(clicked()), this, SLOT(newGame()));
    connect(settings, SIGNAL(clicked()), this, SLOT(changeSettings()));
    connect(quit, SIGNAL(clicked()), QApplication::instance(), SLOT(quit()));
}

void StartScreen::newGame()
{
    new_game->setText(tr("Starting a new game..."));
    new_game->setStyleSheet("* { color: black; background-color: rgba(225,0,0,100) }");
    new_game->setMinimumHeight(200);
    settings->setVisible(false);
    //quit->setVisible(false);
}

void StartScreen::changeSettings()
{
    qDebug() << "Change settings";
}

void StartScreen::paintEvent(QPaintEvent *event)
{
    QPainter paint(this);
    paint.drawImage(0, 0, bg_img.scaled(size(), Qt::KeepAspectRatioByExpanding));
    paint.end();
}
