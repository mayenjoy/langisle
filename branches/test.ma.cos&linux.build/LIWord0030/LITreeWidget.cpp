#include "StdAfx.h"
#include "LITreeWidget.h"

LITreeWidget::LITreeWidget(QWidget *parent)
	: QTreeWidget(parent)
{

}

LITreeWidget::~LITreeWidget()
{

}

void LITreeWidget::resizeEvent(QResizeEvent *event)
{
	QTreeWidget::resizeEvent(event);
	//qDebug() << header()->sectionSize(0) << event->size().width();
	resizeColumnToContents(0);
	if (header()->sectionSize(0) < event->size().width())
	{
		header()->resizeSection(0, event->size().width());
	}

}