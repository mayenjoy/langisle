#ifndef QNOFOCUSITEMDELEGATE_H
#define QNOFOCUSITEMDELEGATE_H

#include <QItemDelegate>

class QNoFocusItemDelegate : public QItemDelegate
{
	Q_OBJECT

public:
	QNoFocusItemDelegate(QObject *parent);
	~QNoFocusItemDelegate();

protected:
	virtual void drawFocus ( QPainter * painter, const QStyleOptionViewItem & option, const QRect & rect ) const;
private:

};

#endif // QNOFOCUSITEMDELEGATE_H
