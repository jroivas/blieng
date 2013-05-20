#ifndef __GAME_LAYOUT_H
#define __GAME_LAYOUT_H

#include <QLayout>
#include <QLayoutItem>
#include <QSize>
#include <QWidget>
#include <vector>

namespace ui
{

class GameLayout : public QLayout
{
public:
	enum Position { Top, Right, Bottom, Left, Center };
	GameLayout(QWidget *parent, int margin = 0, int spacing = 0);
	GameLayout(int spacing = 0);
	~GameLayout();

	void addItem(QLayoutItem *item);
	void addWidget(QWidget *widget, Position pos);
	void setGeometry(const QRect &rect);

	QSize sizeHint() const;
	QLayoutItem *itemAt(int index) const;
	QLayoutItem *takeAt(int index);
	int count() const;

private:
	QSize getSizeHint(bool minimum) const;

	class Item {
	public:
		Item(QLayoutItem *i, Position p) : item(i), pos(p) { }
		bool isVisible() {
			QWidget *wid = item->widget();
			if (wid != NULL) return wid->isVisible();
			return false;
		}

		QLayoutItem *item;
		Position pos;
	};
	void add(QLayoutItem *item, Position pos);

	std::vector<Item* > items;
};

}

#endif
