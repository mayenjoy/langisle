#include "stdafx.h"
#include "QNoFocusItemDelegate.h"
#include "LITableView.h"
#include "LISqlQueryModel.h"
QNoFocusItemDelegate::QNoFocusItemDelegate(QObject *parent)
	: QItemDelegate(parent)
{

}

QNoFocusItemDelegate::~QNoFocusItemDelegate()
{
	
}
	
void QNoFocusItemDelegate::drawFocus ( QPainter * painter, const QStyleOptionViewItem & option, const QRect & rect ) const
{
	
}


