#include "stdafx.h"
#include "PageSODict.h"
#include "GlobalVal.h"
#include "LIConfig.h"
#include "../comm/QNoFocusItemDelegate.h"
#include "BCMgr.h"
PageSODict::PageSODict(QWidget *parent)
	: QWidget(parent)
{
	QVBoxLayout *mainLayout = new QVBoxLayout;
	QHBoxLayout *dlLayout = new QHBoxLayout;
	QVBoxLayout *rightLayout = new QVBoxLayout;
	setLayout(mainLayout);

	//g_pbcmgr->getConfig()->setDefaultSet();
	LISTMAPINFO lmi = g_pbcmgr->getConfig()->getDictInfo();

	tableWidget = new QTableWidget(lmi.size(), 3, this);
	//tableWidget->setSelectionMode(QAbstractItemView::NoSelection);
	tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
	tableWidget->setItemDelegate(new QNoFocusItemDelegate(this));
	tableWidget->setHorizontalHeaderLabels(QStringList() << "" << "名称" << "地址");
	tableWidget->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);
	tableWidget->setAutoScroll(false);
	tableWidget->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);

	tableWidget->verticalHeader()->hide();

	for (int i = 0; i < lmi.size(); i++)
	{

		QTableWidgetItem *newItem = new QTableWidgetItem();
		newItem->setCheckState(lmi.at(i)["used"].toBool() ? Qt::Checked : Qt::Unchecked);
		newItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);

		tableWidget->setItem(i, 0, newItem);

		newItem = new QTableWidgetItem(lmi.at(i)["name"].toString());
		tableWidget->setItem(i, 1, newItem);


		newItem = new QTableWidgetItem(lmi.at(i)["addr"].toString());
		tableWidget->setItem(i, 2, newItem);

	}
	//DBG(qDebug() << lmi);



	mainLayout->addWidget(new QLabel("词典列表:(双击修改;\"[LIWord]\"将被替换为查询的单词)"));
	mainLayout->addLayout(dlLayout);

	dlLayout->addWidget(tableWidget);
	dlLayout->addLayout(rightLayout);

	QPushButton *addButton = new QPushButton("添加");
	QPushButton *delButton = new QPushButton("移除");

	upButton = new QPushButton("上移");
	downButton = new QPushButton("下移");

	rightLayout->addWidget(upButton);
	rightLayout->addWidget(downButton);
	rightLayout->addWidget(addButton);
	rightLayout->addWidget(delButton);
	rightLayout->addStretch();

	connect(addButton, SIGNAL(clicked()), this, SLOT(add()));
	connect(delButton, SIGNAL(clicked()), this, SLOT(del()));

	connect(upButton, SIGNAL(clicked()), this, SLOT(up()));
	connect(downButton, SIGNAL(clicked()), this, SLOT(down()));

	connect(tableWidget, SIGNAL(itemSelectionChanged()), this, SLOT(itemSelectionChanged()));

}

PageSODict::~PageSODict()
{

}

void PageSODict::save()
{
	LISTMAPINFO lmi;
	for (int i = 0; i < tableWidget->rowCount(); i++)
	{
		QMap<QString, QVariant> dictInfo;
		dictInfo["used"] = tableWidget->item(i, 0)->checkState();

		dictInfo["name"] = tableWidget->item(i, 1)->text();
		dictInfo["addr"] = tableWidget->item(i, 2)->text();
		lmi << dictInfo;
	}
	g_pbcmgr->getConfig()->setDictInfo(lmi);
}

void PageSODict::add()
{
	tableWidget->insertRow(tableWidget->rowCount());
	QTableWidgetItem *newItem = new QTableWidgetItem();
	newItem->setCheckState(Qt::Checked);
	newItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);
	tableWidget->setItem(tableWidget->rowCount() - 1, 0, newItem);

	newItem = new QTableWidgetItem(); 
	tableWidget->setItem(tableWidget->rowCount() - 1, 1, newItem);
	tableWidget->setCurrentCell(tableWidget->rowCount() - 1, 1);
	tableWidget->editItem(tableWidget->item(tableWidget->rowCount() - 1, 1));
}

void PageSODict::del()
{
	DBG(qDebug() << tableWidget->currentRow());
	int  row = tableWidget->currentRow();
	if (-1 != row)
	{
		if (QMessageBox::Ok  == QMessageBox::question(this, "确认操作", /*QString("您确定要删除名为\"%1\"的词典吗?").arg(tableWidget->item(row, 1)->text())*/
			"您确定要删除该词典吗?", QMessageBox::Ok | QMessageBox::Cancel))
		{
			tableWidget->removeRow(tableWidget->currentRow());
		}
	}
	
}


void PageSODict::up()
{
	QList<QTableWidgetItem *> selection = tableWidget->selectedItems();

	int prev = -1;
	for( int i = selection.length() - 1; i >= 0; i -= 1 ) {
		int current = selection[i]->row();
		if( current != prev ) {
			QTableWidgetItem *item0 = tableWidget->takeItem(current,0);
			QTableWidgetItem *item1 = tableWidget->takeItem(current,1);
			QTableWidgetItem *item2 = tableWidget->takeItem(current,2);

			QTableWidgetItem *itemBelow0 = tableWidget->takeItem(current-1,0);
			QTableWidgetItem *itemBelow1 = tableWidget->takeItem(current-1,1);
			QTableWidgetItem *itemBelow2 = tableWidget->takeItem(current-1,2);

			tableWidget->setItem(current,0,itemBelow0);
			tableWidget->setItem(current,1,itemBelow1);
			tableWidget->setItem(current,2,itemBelow2);

			tableWidget->setItem(current-1,0,item0);
			tableWidget->setItem(current-1,1,item1);
			tableWidget->setItem(current-1,2,item2);

			tableWidget->setCurrentCell(current-1, selection[0]->column(), QItemSelectionModel::SelectCurrent);
			

			prev = current;
		}
	}

	itemSelectionChanged();
	tableWidget->setFocus();
	
}

void PageSODict::down()
{
	QList<QTableWidgetItem *> selection = tableWidget->selectedItems();

	int prev = -1;
	for( int i = selection.length() - 1; i >= 0; i -= 1 ) {
		int current = selection[i]->row();
		if( current != prev ) {
			QTableWidgetItem *item0 = tableWidget->takeItem(current,0);
			QTableWidgetItem *item1 = tableWidget->takeItem(current,1);
			QTableWidgetItem *item2 = tableWidget->takeItem(current,2);

			QTableWidgetItem *itemBelow0 = tableWidget->takeItem(current+1,0);
			QTableWidgetItem *itemBelow1 = tableWidget->takeItem(current+1,1);
			QTableWidgetItem *itemBelow2 = tableWidget->takeItem(current+1,2);

			tableWidget->setItem(current,0,itemBelow0);
			tableWidget->setItem(current,1,itemBelow1);
			tableWidget->setItem(current,2,itemBelow2);

			tableWidget->setItem(current+1,0,item0);
			tableWidget->setItem(current+1,1,item1);
			tableWidget->setItem(current+1,2,item2);

			tableWidget->setCurrentCell(current+1, selection[0]->column(), QItemSelectionModel::SelectCurrent);



			prev = current;
		}
	}
	
	itemSelectionChanged();
	tableWidget->setFocus();
}

void PageSODict::itemSelectionChanged()
{
	int cr = tableWidget->currentRow();
	int rc = tableWidget->rowCount();
	if (rc > 0)
	{
		if (cr == 0)
		{

			upButton->setDisabled(true);
			downButton->setDisabled(false);

		}
		else if (cr == rc - 1){
			upButton->setDisabled(false);
			downButton->setDisabled(true);
		}
		else 
		{
			upButton->setDisabled(false);
			downButton->setDisabled(false);
		}
	}
	else
	{
		upButton->setDisabled(true);
		downButton->setDisabled(true);
	}

}