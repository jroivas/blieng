#include "mapscreen.h"
#include "blieng/data.h"
#include <boost/foreach.hpp>

using ui::MapScreen;

static const double fella_size = 40;


/* FIXME: Should this be implemented with QGraphicsItem, QGraphicsScene, etc.
 */

MapScreen::MapScreen(QString mapname, QWidget *parent) : QWidget(parent)
{
	maps = new blieng::Maps(mapname.toStdString());
	create_world = new zomb::CreateWorld(maps);
	zoomlevel = 100;
	loadImage();
}

void MapScreen::loadImage()
{
	bgimage = QImage(maps->getSolvedMapImageFile().c_str());
	image_pos = QPoint(0, 0);
}

/* Validate that map fits on the screen
*/
void MapScreen::validateImage()
{
	if (image_pos.x() > 0) image_pos.setX(0);
	if (image_pos.y() > 0) image_pos.setY(0);
	if (image_pos.x() < -(bgimage.width() - width())) image_pos.setX(-(bgimage.width() - width()));
	if (image_pos.y() < -(bgimage.height() - height())) image_pos.setY(-(bgimage.height() - height()));
}

void MapScreen::fellowship(QPointF pos)
{
	fellowship_pos = pos;
	update();
}

void MapScreen::changedFellowship(std::vector<ui::CharacterData *> fellows)
{
	the_fellowship = fellows;
	update();
}

void MapScreen::paintEvent(QPaintEvent *event)
{
	QPainter paint(this);
	validateImage();
	double zoomfactor = zoomlevel / 100.0;
	paint.drawImage(image_pos / zoomfactor, bgimage);

	QPen blackpen(QColor(0,0,0,255));
	blackpen.setWidth(3);
	paint.setPen(blackpen);
	BOOST_FOREACH(blieng::Path path, maps->getPaths()) {
		blieng::Point prev(false);
		BOOST_FOREACH(blieng::Point point, path.getPoints()) {
			if (prev.isValid()) {
				QPoint a = QPoint(prev.x, prev.y) + image_pos;
				QPoint b = QPoint(point.x, point.y) + image_pos;
				a /= zoomfactor;
				b /= zoomfactor;
				paint.drawLine(a, b);
			}
			prev = point;
		}
	}

	paint.setBrush(QColor(0,0,0,255));
	BOOST_FOREACH(blieng::Town *town, maps->getTowns()) {
		paint.setPen(QColor(0,0,0,255));
		QPoint town_pos(town->getPositionX(), town->getPositionY());
		paint.drawEllipse((image_pos + town_pos) / zoomfactor, town->getSize(), town->getSize());

		QRect namebox = paint.boundingRect(0, 0, 300, 100, Qt::AlignLeft, town->getName().c_str());
		QPoint textpos = image_pos + town_pos;
		paint.setPen(QColor(255,0,0,255));
		textpos.setX(textpos.x() - namebox.width()/2);

		paint.drawText(textpos / zoomfactor, town->getName().c_str());
	}

	unsigned int fella = 0;
	BOOST_FOREACH(ui::CharacterData *fellow, the_fellowship) {
		QPointF fpos;
		switch(fella) {
			case 0:
				fpos = QPointF(-1*fella_size/2, -1*fella_size/2);
				break;
			case 1:
				fpos = QPointF(fella_size/2, -1*fella_size/2);
				break;
			case 2:
				fpos = QPointF(-1*fella_size/2, fella_size/2);
				break;
			case 3:
				fpos = QPointF(fella_size/2, fella_size/2);
				break;
			default:
				fpos = QPointF(0, 0);
				break;
		}
		fpos -= QPointF(fella_size/2, fella_size/2);
		paint.drawImage((image_pos + fellowship_pos + fpos) / zoomfactor, fellow->image.scaled(fella_size, fella_size));
		++fella;
	}
}

void MapScreen::mousePressEvent(QMouseEvent *event)
{
	if (event->buttons() & Qt::LeftButton) {
		canmove = true;
		last_pos = event->pos();
	}
}

void MapScreen::wheelEvent(QWheelEvent *event)
{
	int numDegrees = event->delta() / 8;
	int numSteps = numDegrees / 15;

	zoomlevel += numSteps;

	bgimage = QImage(maps->getSolvedMapImageFile().c_str());
	bgimage = bgimage.scaled(bgimage.size() / (zoomlevel / 100.0));
	update();
}

void MapScreen::mouseReleaseEvent(QMouseEvent *event)
{
	if (canmove) {
		QPoint now_pos = event->pos();
		QPoint pos_diff = now_pos - last_pos;
		if (pos_diff.manhattanLength() > 10) {
			//qDebug() << "Diffed " << now_pos << pos_diff << pos_diff.manhattanLength();
			image_pos += pos_diff;
			update();
		} else {
			BOOST_FOREACH(blieng::Town *town, maps->getTowns()) {
				QPoint town_pos(town->getPositionX(), town->getPositionY());
				town_pos += image_pos;
				QPoint town_diff = town_pos - now_pos;
				if (town_diff.manhattanLength() <= town->getSize()) {
					qDebug() << "Clicked town " << town->getName().c_str();
					emit townSelected(town);
				}
			}
		}
	}
	canmove = false;
}

void MapScreen::mouseMoveEvent(QMouseEvent *event)
{
}
