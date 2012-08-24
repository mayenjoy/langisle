#include "stdafx.h"
#include "LITableView.h"
#include "QNoFocusItemDelegate.h"
#include "BCDBModel.h"


LITableView::LITableView(QWidget *parent)
	: QTableView(parent)
{
	horizontalHeader()->setMouseTracking (true);
	horizontalHeader ()->installEventFilter (this);
	//installEventFilter (this);
	setAlternatingRowColors(true);
	horizontalHeader()->setHighlightSections(false); // 表头,项目被全部选中时高亮
}

LITableView::~LITableView()
{

}


void LITableView::leaveEvent ( QEvent * event )
{
	BCDBModel *m = (BCDBModel *)model();
	m->setHoverRow(-1);
	int columnCount = model()->columnCount();
	for (int i = columnCount - 1; i >= 0; i--)
	{
		update(model()->index(n_orow, i));

	}
	n_orow = -1;
}

void LITableView::mouseMoveEvent ( QMouseEvent * event)
{

	QTableView::mouseMoveEvent (event);
	int nrow = indexAt(event->pos()).row();
	updateRow(nrow);
	
}

void LITableView::updateRow(int row)
{
	if (row == n_orow)
		return;
	BCDBModel *m = (BCDBModel *)model();
	m->setHoverRow(row);
	int columnCount = model()->columnCount();
	for (int i = columnCount - 1; i >= 0; i--)
	{

		update(model()->index(n_orow, i));
		update(model()->index(row, i));

	}
	n_orow = row;
}

void LITableView::wheelEvent ( QWheelEvent * event )
{
	QTableView::wheelEvent (event);
	int nrow = indexAt(event->pos()).row();
	updateRow(nrow);
	

	
}

bool LITableView::eventFilter (QObject* object,QEvent* event) 
{
   if ( object == horizontalHeader () ) 
   {
	   if ( event->type() == QEvent::Enter ) 
	   {
			leaveEvent(event);
	   }	
   }
//    else if (event->type() == QEvent::ToolTip)
//    {
// 	   DBG(qDebug() << "LITableView::eventFilter ToolTip");
//    }
//    else if (event->type() == QEvent::ToolTipChange)
//    {
// 	   DBG(qDebug() << "LITableView::eventFilter ToolTipChange");
//    }
   return QTableView::eventFilter (object, event);
} 


void LITableView::selectAllRow()
{
	BCDBModel *m = (BCDBModel *)model();
	for (int i = 0; i < m->getRowCount(); i++)
	{
		selectionModel()->select(m->index(i, 0), QItemSelectionModel::Select | QItemSelectionModel::Rows);

	}

}