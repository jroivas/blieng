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
    _buttons = ComboBox_None;
    updateGeometries();
}

void ZombComboBox::updateGeometries()
{
    unsigned int norm_margin_left = 0;
    if (_style == ComboBox_Horizontal) {
        norm_margin_left = 20;
    }
    static const unsigned int norm_margin = 2;

    QRect geom = geometry();
    _topLeft = QPoint(norm_margin, norm_margin);
    _bottomRight = QPoint(geom.size().width() - norm_margin, geom.size().height() - norm_margin);

    sub_geom = QRect(QPoint(norm_margin, norm_margin), geom.size());

    if (_style == ComboBox_Horizontal) {
        _leftBoxPoint = QPoint(norm_margin + norm_margin_left, geom.size().height() - norm_margin);
        _rightBoxPoint = QPoint(geom.size().width() - norm_margin - norm_margin_left, norm_margin);
        _topRight = QPoint(geom.size().width() - norm_margin, geom.size().height() - norm_margin);

        _leftBox = QRect(_topLeft, _leftBoxPoint);
        _rightBox = QRect(_rightBoxPoint, _topRight);
        _box = QRect(norm_margin + norm_margin_left, norm_margin, geom.size().width() - norm_margin*2 - norm_margin_left*2, geom.size().height() - norm_margin*2);
        _smaller_box = QRect(norm_margin*2 + norm_margin_left + 2, norm_margin*2, geom.size().width() - norm_margin*4 - norm_margin_left*2 - 2, geom.size().height() - norm_margin*4);
        _full_box = QRect(_topLeft, _bottomRight);

        _polyLeft = QPolygon();
        _polyLeft.append(QPoint(norm_margin*2, (geom.size().height() - norm_margin) / 2 + norm_margin));
        _polyLeft.append(QPoint(norm_margin_left, norm_margin*2 + 1));
        _polyLeft.append(QPoint(norm_margin_left, geom.size().height() - norm_margin*2 + 1));

        _polyRight = QPolygon();
        _polyRight.append(QPoint(geom.size().width() - norm_margin*2, (geom.size().height() - norm_margin) / 2 + norm_margin));
        _polyRight.append(QPoint(geom.size().width() - norm_margin_left + norm_margin, norm_margin*2 + 1));
        _polyRight.append(QPoint(geom.size().width() - norm_margin_left + norm_margin, geom.size().height() - norm_margin*2 + 1));

    }
}

bool ZombComboBox::event(QEvent *event)
{
    if (event->type() == QEvent::Resize ||
        event->type() == QEvent::Show) {
        updateGeometries();
    }
    return QWidget::event(event);
}

void ZombComboBox::paintEvent(QPaintEvent *event)
{
    QPainter paint(this);
    paint.setPen(_bg);
    paint.setBrush(_bg);
    paint.drawRect(sub_geom);

    paint.setBrush(_none);

    unsigned int norm_margin_left = 0;
    if (_style == ComboBox_Horizontal) {
        norm_margin_left = 20;
    }
    QRect namebox = paint.boundingRect(0, 0, geometry().width() - norm_margin_left*2, geometry().height(), Qt::AlignLeft, currentText());
    QPoint textpos = QPoint(geometry().width()/2 - namebox.width()/2 + 1, geometry().height()/2 + namebox.height()/2 - 1);
    paint.setPen(_text_color);
    paint.drawText(textpos, currentText());

    paint.setPen(_border);

    paint.drawRect(_full_box);

    if (_buttons == ComboBox_Item) {
        paint.setPen(_border_down);
        paint.setBrush(_none);
        paint.drawRect(_smaller_box);
    }

    if (_style == ComboBox_Horizontal) {
        paint.setBrush(_none);

        paint.setPen(_buttons == ComboBox_Left?_border_down:_border);
        paint.drawRect(_leftBox);

        paint.setPen(_buttons == ComboBox_Right?_border_down:_border);
        paint.drawRect(_rightBox);

        paint.setPen(_buttons == ComboBox_Left?_border_down:_border);
        paint.setBrush(_buttons == ComboBox_Left?_border_down:_border);
        paint.drawPolygon(_polyLeft);

        paint.setPen(_buttons == ComboBox_Right?_border_down:_border);
        paint.setBrush(_buttons == ComboBox_Right?_border_down:_border);
        paint.drawPolygon(_polyRight);
    }
}

void ZombComboBox::mouseMoveEvent(QMouseEvent *)
{
}

void ZombComboBox::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
        if (_leftBox.contains(e->pos())) {
            _buttons = ComboBox_Left;
        } else if (_rightBox.contains(e->pos())) {
            _buttons = ComboBox_Right;
        } else if (_box.contains(e->pos())) {
            _buttons = ComboBox_Item;
        }
    }
    update();
}

void ZombComboBox::prevItem()
{
    int index = currentIndex() + 1;
    if (index >= count()) index = 0;
    setCurrentIndex(index);
}

void ZombComboBox::nextItem()
{
    int index = currentIndex() - 1;
    if (index < 0) index = count()-1;
    setCurrentIndex(index);
}

void ZombComboBox::mouseReleaseEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
        if (_leftBox.contains(e->pos())) {
            nextItem();
        } else if (_rightBox.contains(e->pos())) {
            prevItem();
        } else if (_box.contains(e->pos())) {
            emit clicked();
        }
    }
    _buttons = ComboBox_None;
    update();
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
    if (indexChanged) {
        _currentIndex = QPersistentModelIndex(normalized);
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
    int itemCount = count();
    index = qBound(0, index, itemCount);

    QStandardItem *item = new QStandardItem(text);
    if (userData.isValid()) item->setData(userData, Qt::UserRole);
    model->insertRow(index, item);

    if (!_currentIndex.isValid()) {
        setCurrentIndex(0);
    }
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
