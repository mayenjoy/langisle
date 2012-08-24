#include "stdafx.h"
#include "PageSOUser.h"
#include "GlobalVal.h"
#include "LIConfig.h"
#include "../comm/QNoFocusItemDelegate.h"
#include "BCMgr.h"
#include "PageSOAddUser.h"
#include "PageSOEditUser.h"
#include "PageSODelUser.h"
#include "UserMgr.h"
PageSOUser::PageSOUser(QWidget *parent)
	: QWidget(parent)
{
	QVBoxLayout *mainLayout = new QVBoxLayout;
	QHBoxLayout *dlLayout = new QHBoxLayout;
	QVBoxLayout *rightLayout = new QVBoxLayout;
	setLayout(mainLayout);

	//g_pbcmgr->getConfig()->setDefaultSet();
	//LISTMAPINFO lmi = g_pbcmgr->getConfig()->getDictInfo();

	QStringList userList = GlobalVal::s_um->getUserListOrderById();
	tableWidget = new QTableWidget(userList.size(), 2, this);
	//tableWidget->setSelectionMode(QAbstractItemView::NoSelection);
	tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
	tableWidget->setItemDelegate(new QNoFocusItemDelegate(this));
	tableWidget->setHorizontalHeaderLabels(QStringList() <<"ID"<<"�û�����");
	tableWidget->horizontalHeader()->setStretchLastSection(true);
	tableWidget->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);
	tableWidget->setAutoScroll(false);
	tableWidget->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
	tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

	tableWidget->verticalHeader()->hide();
	
	initUserList(userList);

	mainLayout->addWidget(new QLabel("�û��б�:(ѡ����Ӧ�û�������������Խ���ɾ��,ע�⣺��ǰ�û����ϲ���ɾ��������ɫ��)"));
	mainLayout->addLayout(dlLayout);

	dlLayout->addWidget(tableWidget);
	dlLayout->addLayout(rightLayout);

	//editmgrpswButton = new QPushButton("�޸Ĺ�������");
	addButton = new QPushButton("����û�");
	editButton = new QPushButton("�޸�����");
	delButton = new QPushButton("ɾ���û�");

	
	

	//rightLayout->addWidget(editmgrpswButton);
	rightLayout->addWidget(addButton);
	rightLayout->addWidget(editButton);
	rightLayout->addWidget(delButton);
	rightLayout->addStretch();

	//connect(editmgrpswButton, SIGNAL(clicked()), this, SLOT(editmgrpsw()));
	connect(addButton, SIGNAL(clicked()), this, SLOT(add()));

 	connect(editButton, SIGNAL(clicked()), this, SLOT(edit()));
	connect(delButton, SIGNAL(clicked()), this, SLOT(del()));

	connect(tableWidget, SIGNAL(itemSelectionChanged()), this, SLOT(itemSelectionChanged()));

}

PageSOUser::~PageSOUser()
{

}

bool PageSOUser::initUserList(QStringList userList)
{
	tableWidget->clearContents();

	QStringListIterator javaStyleIterator(userList);
	int i = 0;
	while (javaStyleIterator.hasNext())
	{
		QTableWidgetItem *newItem = new QTableWidgetItem();
        int j = i++;
        QString username=javaStyleIterator.next();
        newItem = new QTableWidgetItem(QString("%1").arg(j+1));

		
		if(username==g_pbcmgr->getCurUserName())
		{
			newItem->setBackgroundColor(QColor("green"));
		}
		
		tableWidget->setItem(j, 0, newItem);
		newItem = new QTableWidgetItem(username);

		/*
		if(username==g_pbcmgr->getCurUserName())
		{
			newItem->setBackgroundColor(QColor("green"));
		}
		*/
		tableWidget->setItem(j, 1, newItem);
	}
	return true;
}

/*
void PageSOUser::editmgrpsw()
{

}
*/

void PageSOUser::add()
{
	PageSOAddUser cu(this);
	cu.exec();
	QStringList userList = GlobalVal::s_um->getUserListOrderById();
	tableWidget->setRowCount(userList.size());
	initUserList(userList);

	

}


void PageSOUser::edit()
{

	DBG(qDebug() << tableWidget->currentRow());
	int  row = tableWidget->currentRow();
	if(row==-1)
	{
		QMessageBox::warning(this, tr("����"), tr("��ѡ���û�֮���ٽ��б༭."), QMessageBox::Ok);
	}else
	{
	    QString username = tableWidget->item(row, 1)->text();
		PageSOEditUser cu(this,username);
		if (QDialog::Accepted == cu.exec())
		{
			  QMessageBox::question(this, tr("�ɹ�"), tr("�����û����ݳɹ�."), QMessageBox::Ok);
		}else
		{
			  QMessageBox::warning(this, tr("��ʾ"), tr("δ�����û�����"), QMessageBox::Ok);
		}
	}

}


void PageSOUser::del()
{

	
	DBG(qDebug() << tableWidget->currentRow());
	int  row = tableWidget->currentRow();
	if(row==-1)
	{
        QMessageBox::warning(this, tr("����"), tr("��ѡ���û�֮���ٽ���ɾ��."), QMessageBox::Ok);
	}else
	{
	
				QString username = tableWidget->item(row, 1)->text();
				if(username!=g_pbcmgr->getCurUserName())
				{
					PageSODelUser cu(this,username);
					if (QDialog::Accepted == cu.exec())
					{
						if (-1 != row)
						{
							if (QMessageBox::Ok  == QMessageBox::question(this, "ȷ�ϲ���", QString("��ȷ��Ҫɾ����Ϊ\"%1\"���û��������������?").arg(username)
								/*"��ȷ��Ҫɾ�����û���?"*/, QMessageBox::Ok | QMessageBox::Cancel))
							{
								GlobalVal::s_um->removeUser(username);
								QString strPath = Global::makePathStr("/udata/" + username);
								Global::deleteDir(strPath);
								tableWidget->removeRow(tableWidget->currentRow());
								QStringList userList = GlobalVal::s_um->getUserListOrderById();
								tableWidget->setRowCount(userList.size());
								initUserList(userList);
							}
						}
					}
				}else
				{
				   QMessageBox::warning(this, tr("����"), tr("����ɾ���ѵ�¼�û�."), QMessageBox::Ok);
				
				}
	}
	
}

void PageSOUser::itemSelectionChanged()
{
}

/*
void PageSOUser::save()
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

void PageSOUser::add()
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

void PageSOUser::del()
{
	DBG(qDebug() << tableWidget->currentRow());
	int  row = tableWidget->currentRow();
	if (-1 != row)
	{
		if (QMessageBox::Ok  == QMessageBox::question(this, "ȷ�ϲ���", 
			"��ȷ��Ҫɾ���ôʵ���?", QMessageBox::Ok | QMessageBox::Cancel))
		{
			tableWidget->removeRow(tableWidget->currentRow());
		}
	}
	
}


void PageSOUser::up()
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

void PageSOUser::down()
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

void PageSOUser::itemSelectionChanged()
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
*/