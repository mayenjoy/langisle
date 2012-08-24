#include "StdAfx.h"
#include "LISearchButton.h"

LISearchButton::LISearchButton(QWidget *parent)
	: QAbstractButton(parent)
{
	qDebug() << "1";
	setObjectName(QLatin1String("SearchButton"));
#ifndef QT_NO_CURSOR
	setCursor(Qt::ArrowCursor);
#endif //QT_NO_CURSOR
	setFocusPolicy(Qt::NoFocus);
}

LISearchButton::~LISearchButton()
{

}

void LISearchButton::mousePressEvent(QMouseEvent *event)
{
	//if (m_menu && event->button() == Qt::LeftButton) {
	//	QWidget *p = parentWidget();
	//	if (p) {
	//		QPoint r = p->mapToGlobal(QPoint(0, p->height()));
	//		m_menu->exec(QPoint(r.x() + height() / 2, r.y()));
	//	}
	//	event->accept();
	//}
	QAbstractButton::mousePressEvent(event);
}

void LISearchButton::paintEvent(QPaintEvent *event)
{
	qDebug() << "1";
	Q_UNUSED(event);
	QPainterPath myPath;

	int radius = (height() / 5) * 2;
	QRect circle(height() / 3 - 1, height() / 4, radius, radius);
	myPath.addEllipse(circle);

	myPath.arcMoveTo(circle, 300);
	QPointF c = myPath.currentPosition();
	int diff = height() / 7;
	myPath.lineTo(qMin(width() - 2, (int)c.x() + diff), c.y() + diff);

	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing, true);
	painter.setPen(QPen(Qt::darkGray, 2));
	painter.drawPath(myPath);

	//if (m_menu) 
	{
		QPainterPath dropPath;
		dropPath.arcMoveTo(circle, 320);
		QPointF c = dropPath.currentPosition();
		c = QPointF(c.x() + 3.5, c.y() + 0.5);
		dropPath.moveTo(c);
		dropPath.lineTo(c.x() + 4, c.y());
		dropPath.lineTo(c.x() + 2, c.y() + 2);
		dropPath.closeSubpath();
		painter.setPen(Qt::darkGray);
		painter.setBrush(Qt::darkGray);
		painter.setRenderHint(QPainter::Antialiasing, false);
		painter.drawPath(dropPath);
	}
	painter.end();
}