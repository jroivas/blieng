#include "gamescreen.h"
#include <boost/foreach.hpp>

using ui::GameScreen;
using ui::GameLayout;

GameScreen::GameScreen(QWidget *parent) : QWidget(parent)
{
	chrgen = new GenerateCharacter(this);
	mapscreen = new MapScreen("world1", this);
	character_view = new CharacterView(this);

	current_location = NULL;
	target_location = NULL;
	target_path = NULL;
	walker = new QTimer();
	walker->setInterval(100);

#if 0
	layout.addWidget(chrgen, 0, 0, 0, Qt::AlignLeft);
	layout.addWidget(mapscreen, 0, 1, 1, Qt::AlignHCenter);
	layout.addWidget(character_view, 1, 1, 0, Qt::AlignHCenter | Qt::AlignBottom);
	layout.addWidget(chrgen, 0, 0, Qt::AlignLeft);
	layout.addWidget(mapscreen, 0, 1, Qt::AlignCenter);
	layout.addWidget(character_view, 3, 1, Qt::AlignHCenter | Qt::AlignBottom);
#else
	layout.addWidget(chrgen, GameLayout::Left);
	layout.addWidget(mapscreen, GameLayout::Center);
	layout.addWidget(character_view, GameLayout::Bottom);
#endif

	//mapscreen->setMaximumWidth(1000);
	mapscreen->setMinimumWidth(400);
	mapscreen->setMinimumHeight(300);
	//mapscreen->setMaximumHeight(700);
	character_view->setMinimumHeight(64);
	character_view->setMinimumWidth(400);
	//character_view->setMaximumHeight(64);

	character_count = 3;

	connect(chrgen, SIGNAL(generated(zomb::PlayerCharacter *)), this, SLOT(newCharacter(zomb::PlayerCharacter *)));
	connect(mapscreen, SIGNAL(townSelected(blieng::Town *)), this, SLOT(targetTown(blieng::Town *)));
	connect(walker, SIGNAL(timeout()), this, SLOT(doWalk()));
	connect(this, SIGNAL(fellowship(QPointF)), mapscreen, SLOT(fellowship(QPointF)));
	connect(this, SIGNAL(changeFellowship(std::vector<ui::CharacterData *>)), mapscreen, SLOT(changedFellowship(std::vector<ui::CharacterData *>)));

	//std::vector<zomb::PlayerCharacter *> getCharacters();

	setLayout(&layout);

	moveToHomeTown();
}

void GameScreen::moveToHomeTown()
{
	BOOST_FOREACH(blieng::Town *town, mapscreen->getMaps()->getTowns()) {
		if (town->isValue("start") && town->getBoolValue("start")) {
			current_location = town;
			blieng::Point *pos = town->getPosition();
			emit fellowship(QPointF(pos->x, pos->y));
			return;
		}
	}
}

void GameScreen::newCharacter(zomb::PlayerCharacter *chr)
{
	qDebug() << "New character" << chr;
	characters.push_back(chr);
	if (characters.size() >= character_count) {
		chrgen->hide();
	}
	character_view->setCharacters(characters);
	emit changeFellowship(character_view->getCharacters());
}

void GameScreen::solveTargetPath()
{
	if (target_location == NULL) return;

	//target_path = new blieng::Path();
	blieng::Point *from = current_location->getPosition();
	blieng::Point *to = target_location->getPosition();
#if 0 
	double X1 = current_location->getPositionX();
	double Y1 = current_location->getPositionY();
	double X2 = target_location->getPositionX();
	double Y2 = target_location->getPositionY();
#endif

	std::vector<blieng::Path *> test_paths;

	BOOST_FOREACH(blieng::Path *path, mapscreen->getMaps()->getPaths()) {
		blieng::Point *start = path->getStart();
		blieng::Point *end = path->getEnd();
		//if (start->x == X1 && start->y == Y1) {
		if (*start == *from) {
			blieng::Path *tmp = new blieng::Path();
			tmp->append(path);
			test_paths.push_back(tmp);
		}

		BOOST_FOREACH(blieng::Path *continue_path, test_paths) {
			if (*(continue_path->getEnd()) == *start) {
				test_paths.push_back(continue_path->combine(path));
			}
		}
	}
	qDebug() << "PATHS";
	BOOST_FOREACH(blieng::Path *candi, test_paths) {
		if (*candi->getEnd() == *to) {
			qDebug() << "from: " << candi->getStart()->toString().c_str() << " to: " << candi->getEnd()->toString().c_str();
			target_path = candi;
			waypoint = NULL;
		} else {
			delete candi;
		}
	}

	test_paths.clear();
	
	walk_progress = 0;
	walk_speed = 5; //TODO Solve from fellowship by slowest character
	walker->start();
}

void GameScreen::doWalk()
{
	if (target_location == NULL) return;
	if (target_path == NULL) return;

	if (waypoint == NULL) {
		waypoint = target_path->takeFirst();
	}
	if (waypoint == NULL) {
		walker->stop();
		return;
	}
	blieng::Point *to_point = target_path->getStart();
	if (to_point == NULL) return;

	if (*to_point == *waypoint) {
		waypoint = target_path->takeFirst();
	}
	walk_length = waypoint->length(to_point);
	
	walk_progress += walk_speed;
	blieng::Point *pos = waypoint->traverse(to_point, walk_progress, walk_length);
	if (pos == NULL) {
		//Finished
		walker->stop();
	} else if (pos == to_point) {
		//waypoint = target_path->takeFirst();
		waypoint = to_point;
		walk_progress = 0;
	} else {
		//qDebug() << "Walking to " << pos->x << ", " << pos->y;
		emit fellowship(QPointF(pos->x, pos->y));
	}

}

void GameScreen::targetTown(blieng::Town *town)
{
	target_location = town;
	solveTargetPath();
}

GameLayout::GameLayout(QWidget *parent, int margin, int spacing) : QLayout(parent)
{
	setMargin(margin);
	setSpacing(spacing);
}

GameLayout::GameLayout(int spacing)
{
	setMargin(0);
	setSpacing(spacing);
}

GameLayout::~GameLayout()
{
}

QSize GameLayout::sizeHint() const
{
	return getSizeHint(false);
}

QSize GameLayout::getSizeHint(bool minimum) const
{
	QSize totalSize;
	BOOST_FOREACH(Item *item, items) {
		QSize itemSize;

		if (minimum) itemSize = item->item->minimumSize();
		else itemSize = item->item->sizeHint();

		if (item->pos == Top || item->pos == Bottom || item->pos == Center) {
			totalSize.rheight() += itemSize.height();
		}

		if (item->pos == Left || item->pos == Right || item->pos == Center) {
			totalSize.rwidth() += itemSize.width();
		}
	}
	return totalSize;
}

QLayoutItem *GameLayout::itemAt(int index) const
{
	int pos = 0;
	BOOST_FOREACH(Item *item, items) {
		if (index == pos) {
			return item->item;
		}
		pos++;
	}
	return NULL;
}

QLayoutItem *GameLayout::takeAt(int index)
{
	std::vector<Item* >::iterator it = items.begin();
	int pos = 0;
	while (it != items.end()) {
		if (index == pos) {
			Item *tmp = *it;
			items.erase(it);
			return tmp->item;
		}
		pos++;
		it++;
	}
	return NULL;
}

int GameLayout::count() const
{
	return items.size();
}

void GameLayout::add(QLayoutItem *item, Position pos)
{
	items.push_back(new Item(item, pos));
}

void GameLayout::addItem(QLayoutItem *item)
{
	add(item, Left);
}

void GameLayout::addWidget(QWidget *widget, Position pos)
{
	add(new QWidgetItem(widget), pos);
}

#if 0
void GameLayout::setGeometry(const QRect &rect)
{
	int top_size = 0;
	int bottom_size = 0;
	int left_size = 0;
	int right_size = 0;
	QLayout::setGeometry(rect);
	Item *center_item = NULL;

	BOOST_FOREACH(Item* item, items) {
		if (item->pos == Top) {
			item->item->setGeometry( QRect(rect.x(), top_size, rect.width(), item->item->sizeHint().height()) );

			top_size += item->item->geometry().height() + spacing();
		}
		else if (item->pos == Bottom) {
			item->item->setGeometry( QRect(item->item->geometry().x(), item->item->geometry().y(), rect.width(), item->item->sizeHint().height()) );

			bottom_size += item->item->geometry().height() + spacing();

			item->item->setGeometry( QRect(rect.x(), rect.y() + rect.height() - bottom_size + spacing(), item->item->geometry().width(), item->item->sizeHint().height()) );
		}
		else if (item->pos == Center) {
			center_item = item;
		}
	}

	int center_size = rect.height() - top_size - bottom_size;

	BOOST_FOREACH(Item* item, items) {
		if (item->pos == Left) {
			item->item->setGeometry( QRect(rect.x() + left_size, top_size, item->item->sizeHint().width(), center_size) );

			left_size += item->item->geometry().width() + spacing();
		}
		else if (item->pos == Right) {
			item->item->setGeometry( QRect(item->item->geometry().x(), item->item->geometry().y(), item->item->sizeHint().width(), center_size) );

			right_size += item->item->geometry().width() + spacing();

			item->item->setGeometry( QRect(rect.x() + rect.width() - right_size + spacing(), top_size, item->item->geometry().width(), item->item->sizeHint().height()) );
		}
	}

	if (center_item != NULL) {
		center_item->item->setGeometry( QRect(left_size, top_size, rect.width() - right_size - left_size, center_size) );
	}
}
#endif

void GameLayout::setGeometry(const QRect &rect)
{
	int top_size = 0;
	int bottom_size = 0;
	int left_size = 0;
	int right_size = 0;
	QLayout::setGeometry(rect);
	Item *center_item = NULL;

	int center_size = 0;

	BOOST_FOREACH(Item* item, items) {
		if (!item->isVisible()) continue;
		if (item->pos == Left) {
			item->item->setGeometry( QRect(rect.x() + left_size, top_size, item->item->sizeHint().width(), item->item->sizeHint().height()) );

			left_size += item->item->geometry().width() + spacing();
		}
		else if (item->pos == Right) {
			item->item->setGeometry( QRect(item->item->geometry().x(), item->item->geometry().y(), item->item->sizeHint().width(), item->item->sizeHint().height()) );

			right_size += item->item->geometry().width() + spacing();

			item->item->setGeometry( QRect(rect.x() + rect.width() - right_size + spacing(), top_size, item->item->geometry().width(), item->item->sizeHint().height()) );
		}
	}

	BOOST_FOREACH(Item* item, items) {
		if (!item->isVisible()) continue;
		if (item->pos == Top) {
			item->item->setGeometry( QRect(rect.x() + left_size, top_size, rect.width() - left_size - right_size, item->item->sizeHint().height()) );

			top_size += item->item->geometry().height() + spacing();
		}
		else if (item->pos == Bottom) {
			item->item->setGeometry( QRect(item->item->geometry().x() + left_size, item->item->geometry().y(), rect.width() - left_size - right_size, item->item->sizeHint().height()) );

			bottom_size += item->item->geometry().height() + spacing();

			item->item->setGeometry( QRect(rect.x() + left_size, rect.y() + rect.height() - bottom_size + spacing(), item->item->geometry().width() - left_size - right_size, item->item->sizeHint().height()) );
		}
		else if (item->pos == Center) {
			center_item = item;
		}
	}

	center_size = rect.height() - top_size - bottom_size;


	if (center_item != NULL) {
		center_item->item->setGeometry( QRect(left_size, top_size, rect.width() - right_size - left_size, center_size) );
	}
}
