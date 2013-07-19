#include "zombcombobox.h"
#include "blieng/configure.h"
#include <QPainter>
#include <QApplication>
#include <QDebug>
#include <QStyleOptionComboBox>

using ui::ZombComboBox;

ZombComboBox::ZombComboBox(QWidget *parent) : QWidget(parent), ui::ZombStyles(), modelColumn(0)
{
    model = new QStandardItemModel();
    _style = ComboBox_Horizontal;
}

void ZombComboBox::paintEvent(QPaintEvent *event)
{
    QPainter paint(this);

    QRect geom = geometry();
    unsigned int norm_margin_left = 0;
    if (_style == ComboBox_Horizontal) {
        norm_margin_left = 20;
    }
    static const unsigned int norm_margin = 2;

    QPoint topLeft;
    topLeft = QPoint(norm_margin, norm_margin);
    QPoint bottomRight;
    bottomRight = QPoint(geom.size().width() - norm_margin, geom.size().height() - norm_margin);

    QRect geom2 = QRect(QPoint(2, 2), geom.size());

    paint.setPen(_bg);
    paint.setBrush(_bg);
    paint.drawRect(geom2);

    paint.setPen(_text_color);
    paint.setBrush(_none);

    QRect namebox = paint.boundingRect(0, 0, geom.width() - norm_margin_left*2, geom.height(), Qt::AlignLeft, currentText());
    QPoint textpos = QPoint(geom.width()/2 - namebox.width()/2 + 1, geom.height()/2 + namebox.height()/2 - 1);
    paint.drawText(textpos, currentText());

    paint.setPen(_border);

    paint.drawRect(QRect(topLeft, bottomRight));

    if (_style == ComboBox_Horizontal) {
        QPoint leftBox;
        leftBox = QPoint(norm_margin + norm_margin_left, geom.size().height() - norm_margin);

        QPoint rightBox;
        rightBox = QPoint(geom.size().width() - norm_margin - norm_margin_left, norm_margin);
        QPoint topRight;
        topRight = QPoint(geom.size().width() - norm_margin, geom.size().height() - norm_margin);

        paint.drawRect(QRect(topLeft, leftBox));
        paint.drawRect(QRect(rightBox, topRight));
    }
}

QString ZombComboBox::currentText() const
{
    if (_currentIndex.isValid()) {
        return itemText(_currentIndex);
    }
    return QString();
}

QSize ZombComboBox::sizeHint() const
{
    ensurePolished();

    int w = 0, h = 0;

    QStyleOptionComboBox option;
    option.initFrom(this);
    option.editable = false;

    QString s(currentText());
    bool empty = s.isEmpty();
    if (empty) {
        s = QString::fromLatin1("AAAA");
    }
    QFontMetrics fm = fontMetrics();
    QSize sz = fm.size(Qt::TextShowMnemonic, s);

    if (!empty || !w) w += sz.width();
    if (!empty || !h) h = qMax(h, sz.height());

    option.rect.setSize(QSize(w, h));
    QSize sizeHint = (style()->sizeFromContents(QStyle::CT_ComboBox, &option, QSize(w, h), this).expandedTo(QApplication::globalStrut()));
    return sizeHint;
}

void ZombComboBox::setCurrentIndex(const QModelIndex &mi)
{
    QModelIndex normalized;
    if (mi.column() != modelColumn)
        normalized = model->index(mi.row(), modelColumn, mi.parent());
    if (!normalized.isValid())
        normalized = mi;    // Fallback to passed index.

    bool indexChanged = (normalized != _currentIndex);
    if (indexChanged)
        _currentIndex = QPersistentModelIndex(normalized);
    if (indexChanged) {
        update();
        //_q_emitCurrentIndexChanged(currentIndex);
    }
}   


void ZombComboBox::setCurrentIndex(int index)
{
    QModelIndex mi = model->index(index, modelColumn, _root);
    setCurrentIndex(mi);
}

void ZombComboBox::insertItem(int index, const QString &text, const QVariant &userData)
{
    index = qBound(0, index, count());
    if (!_currentIndex.isValid()) {
        setCurrentIndex(0);
    }

    QStandardItem *item = new QStandardItem(text);
    if (userData.isValid()) item->setData(userData, Qt::UserRole);
    model->insertRow(index, item);

    /*if (index < 0) {
        _items.push_back(
    }

    std::vector<QVariant>::iterator ii = _items.begin();

    int num = 0;
    while (ii != _items.end()) {
        ii++;
        num++;
    }
    */
}

int ZombComboBox::count() const
{
    return model->rowCount(_root);
}

int ZombComboBox::currentIndex() const
{
    return _currentIndex.row();
}

QString ZombComboBox::itemText(const QModelIndex &index) const
{
    return index.isValid() ? model->data(index, itemRole()).toString() : QString();
}

int ZombComboBox::itemRole() const
{
    return Qt::DisplayRole;
}

QString ZombComboBox::itemText(int index) const
{
    QModelIndex mi = model->index(index, modelColumn, _root);
    return itemText(mi);
}
