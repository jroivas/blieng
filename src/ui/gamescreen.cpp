#include "gamescreen.h"
#include "blieng/data.h"

using ui::GameScreen;

/* FIXME: Should this be implemented with QGraphicsItem, QGraphicsScene, etc.
 */

GameScreen::GameScreen(QWidget *parent) : QWidget(parent)
{
	maps = new blieng::Maps("world1");
	loadImage();
}

void GameScreen::loadImage()
{
	bgimage = QImage(maps->getSolvedMapImageFile().c_str());
	image_pos = QPoint(0, 0);
}

/* Validate that map fits on the screen
*/
void GameScreen::validateImage()
{
	if (image_pos.x() > 0) image_pos.setX(0);
	if (image_pos.y() > 0) image_pos.setY(0);
	if (image_pos.x() < -(bgimage.width() - width())) image_pos.setX(-(bgimage.width() - width()));
	if (image_pos.y() < -(bgimage.height() - height())) image_pos.setY(-(bgimage.height() - height()));
}

void GameScreen::paintEvent(QPaintEvent *event)
{
	QPainter paint(this);
	validateImage();
	paint.drawImage(image_pos, bgimage);
}

void GameScreen::mousePressEvent(QMouseEvent *event)
{
	if (event->buttons() & Qt::LeftButton) {
		canmove = true;
		last_pos = event->pos();
	}
}

void GameScreen::mouseReleaseEvent(QMouseEvent *event)
{
	if (canmove) {
		QPoint now_pos = event->pos();
		QPoint pos_diff = now_pos - last_pos;
		if (pos_diff.manhattanLength() > 10) {
			//qDebug() << "Diffed " << now_pos << pos_diff << pos_diff.manhattanLength();
			image_pos += pos_diff;
			update();
		}
	}
	canmove = false;
}

void GameScreen::mouseMoveEvent(QMouseEvent *event)
{
}
