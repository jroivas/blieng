#include "gamescreen.h"
#include <boost/foreach.hpp>

using ui::GameScreen;
using ui::GameLayout;

GameScreen::GameScreen(QWidget *parent) : QWidget(parent)
{
	chrgen = new GenerateCharacter(this);
	mapscreen = new MapScreen("world1", this);
	character_view = new CharacterView(this);

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

	mapscreen->setMaximumWidth(1000);
	mapscreen->setMinimumWidth(600);
	mapscreen->setMinimumHeight(300);
	mapscreen->setMaximumHeight(700);
	character_view->setMinimumHeight(64);

	character_count = 3;

	connect(chrgen, SIGNAL(generated(zomb::PlayerCharacter *)), this, SLOT(newCharacter(zomb::PlayerCharacter *)));

	setLayout(&layout);
}

void GameScreen::newCharacter(zomb::PlayerCharacter *chr)
{
	qDebug() << "New character" << chr;
	characters.push_back(chr);
	if (characters.size() >= character_count) {
		character_view->setCharacters(characters);
		chrgen->hide();
	}
}

GameLayout::GameLayout(QWidget *parent, int margin, int spacing) : QLayout(parent)
{
	setMargin(margin);
	setSpacing(spacing);
}

GameLayout::GameLayout(int spacing)
{
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

			item->item->setGeometry( QRect(rect.x() + rect.width() + right_size + spacing(), top_size, item->item->geometry().width(), item->item->sizeHint().height()) );
		}
	}

	if (center_item != NULL) {
		center_item->item->setGeometry( QRect(left_size, top_size, rect.width() - right_size - left_size, center_size) );
	}
}
