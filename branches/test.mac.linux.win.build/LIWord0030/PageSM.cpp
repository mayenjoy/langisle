#include "stdafx.h"
#include "SpeakMgr.h"
#include "GlobalVal.h"
#include "PageSM.h"
#include "StudyMgrBCDBModel.h"
#include "LITableView.h"
#include "BCMgr.h"
#include "../comm/QNoFocusItemDelegate.h"
#include "PageSMStatus.h"


PageSM::PageSM(QWidget *parent,const QString &type)
: QDialog(parent),type(type)
{
 
	QVBoxLayout *mainLayout = new QVBoxLayout;
	QHBoxLayout *dlLayout = new QHBoxLayout;
	QVBoxLayout *rightLayout = new QVBoxLayout;
	setLayout(mainLayout);

	QList<BOOKINFO> m_biList;
	m_biList = g_pbcmgr->getSelectBook();
	if ( m_biList.size() == 0)
	{
		DBG(qDebug() << "PageSM::PageSM m_biList 等于0,出问题了");
		return;
	}
    
	QString sql;


	QString sFp = g_pbcmgr->getUDataDB()->getConfigInfo("defaultFP");
	QStringList ql = sFp.split(",");

	if (m_biList.size()>0)
	{
		BOOKINFO bookInfo = m_biList.at(0);
		bookType bt = (bookType)bookInfo["btype"].toInt();
		QString tableName = QString("%1%2").arg(btUser == bt ? "u" : "o").arg(bookInfo["bid"].toString());
		int maxfp = g_pbcmgr->getUDataDB()->getMaxFP();
		sql =QString("select m.mid as mmid,m.wid as mwid, case when m.sc>%1 then case when m.st>=%2 then '掌握'else '学习中'end else '未学' end as status,case when m.wtype=2 then dwt.word else uword.word end as word ,case when m.wtype=2 then dwt.pron  else null end as pron,o.mean as mean").arg(0).arg(maxfp);
			if (ql.size()>0)
			{	
				QString num=QString("");
				float i=1.0;
				for (QStringList::iterator it = ql.begin();
					it != ql.end(); ++it) 
				{ 
					QString current = *it;
					if (it==ql.end()-1)
					{
						num+=QString(" else %1.0 end ").arg(i++);
					} else if (it==ql.begin())
					{
						num+=QString(" case when m.st=%1 then %2.0 ").arg(current).arg(i++);
					}
					else
					{
						num+=QString(" when m.st=%1 then %2.0 ").arg(current).arg(i++);
					}

				}
				sql+=QString(" ,case when ((%1)>m.sc) then 0 when (m.sc is null) then null else round((1-(%2)/m.sc)*100) end as error").arg(num).arg(num);
				sql+=QString(" ,case when ((%1)>m.sc) then 100 when (m.sc is null) then null else round(((%2)/%3)*100) end as knowndegree").arg(num).arg(num).arg(ql.size()); 
				sql+=QString(" ,case when ((%1)>m.sc) then m.sc-1 when (m.sc is null) then null else %2-1 end as phase").arg(num).arg(num); 
			}
           sql+=QString(",m.sc, m.st, m.lst from %1 as o left join mdata as m on o.mid = m.mid left join dict.word as dwt on m.wid = dwt.wid left join uword  on m.wid = uword.wid").arg(tableName);
		if (type=="all")
		{	
		} 
		else if(type=="learned")
		{
			sql+=QString(" where m.sc>0");
		}
		else if (type=="known")
		{	
			sql +=QString(" where m.st>=%1").arg(maxfp);
		}else if (type=="learning")
		{
			sql +=QString(" where m.sc>0 and m.st<%2").arg(maxfp);
		}else if (type=="notlearn")
		{
            sql +=QString(" where m.sc is null");

		}
		for (int i=1;i<m_biList.size();i++)
		{
			BOOKINFO bookInfo = m_biList.at(i);
			bookType bt = (bookType)bookInfo["btype"].toInt();
			QString tableName = QString("%1%2").arg(btUser == bt ? "u" : "o").arg(bookInfo["bid"].toString());
			sql +=QString(" union select m.mid as mmid,m.wid as mwid, case when m.sc>%1 then case when m.st>=%2 then '掌握'else '学习中'end else '未学' end as status,case when m.wtype=2 then dwt.word  else uword.word end as word ,case when m.wtype=2 then dwt.pron  else null end as pron,o.mean as mean").arg(0).arg(maxfp);
			if (ql.size()>0)
			{	
				QString num=QString("");
				float i=1.0;
				for (QStringList::iterator it = ql.begin();
					it != ql.end(); ++it) 
				{ 
					QString current = *it;
					if (it==ql.end()-1)
					{
						num+=QString(" else %1.0 end ").arg(i++);
					} else if (it==ql.begin())
					{
						num+=QString(" case when m.st=%1 then %2.0 ").arg(current).arg(i++);
					}
					else
					{
						num+=QString(" when m.st=%1 then %2.0 ").arg(current).arg(i++);
					}

				}
				sql+=QString(" ,case when ((%1)>m.sc) then 0 when (m.sc is null) then null else round((1-(%2)/m.sc)*100) end as error").arg(num).arg(num);
				sql+=QString(" ,case when ((%1)>m.sc) then 100 when (m.sc is null) then null else round(((%2)/%3)*100) end as knowndegree").arg(num).arg(num).arg(ql.size()); 
				sql+=QString(" ,case when ((%1)>m.sc) then m.sc-1 when (m.sc is null) then null else %2-1 end as phase").arg(num).arg(num); 
			}
			sql+=QString(",m.sc,m.lst from %1 as o left join mdata as m on o.mid = m.mid left join dict.word as dwt on m.wid = dwt.wid left join uword  on m.wid = uword.wid").arg(tableName);
			if (type=="all")
			{	
			} 
			else if(type=="learned")
			{
				sql+=QString(" where m.sc>0");
			}
			else if (type=="known")
			{	
				sql +=QString(" where m.st>=%1").arg(maxfp);
			}else if (type=="learning")
			{
				sql +=QString(" where m.sc>0 and m.st<%2").arg(maxfp);
			}else if (type=="notlearn")
			{
				sql +=QString(" where m.sc is null");

			}

		}
	}

	DBG(qDebug() << "PageSM::PageSM SQL:"<<sql);

    wordTable = new LITableView;
  
	

	   DBG(QDateTime   start   =   QDateTime::currentDateTime());
	model = new StudyMgrBCDBModel;
	//model->setSFP(sFp);
	model->setSql(sql,*g_pbcmgr->getUDataDB()->getdb());
	model->updateHeaderName();
	wordTable->setModel(model);
	wordTable->verticalHeader()->hide();
	wordTable->setSelectionBehavior(QAbstractItemView::SelectRows);
	wordTable->setSelectionMode(QAbstractItemView::SingleSelection );
	wordTable->setShowGrid(false);
	wordTable->setItemDelegate(new QNoFocusItemDelegate(wordTable));
	wordTable->setMouseTracking(true);
	wordTable->resizeColumnsToContents();
	wordTable->verticalHeader()->setDefaultSectionSize(24);
	wordTable->setSortingEnabled(true);
	DBG(qDebug() << "PageSM::PageSM 耗时" << start.msecsTo(QDateTime::currentDateTime()));
	wordTable->hideColumn(0);
	wordTable->hideColumn(10);
	//wordTable->sortByColumn(3, Qt::AscendingOrder);
	lableWordtotal = new QLabel(QString("单词列表(列表单词总数：%1)").arg(model->getRowCount()));
	mainLayout->addWidget(lableWordtotal);
	mainLayout->addLayout(dlLayout);
	buttonModifyStatus = new QPushButton("修改状态(&M)"); 

	rightLayout->addWidget(buttonModifyStatus);
	rightLayout->addStretch();

	dlLayout->addWidget(wordTable);
	dlLayout->addLayout(rightLayout);

	connect(wordTable, SIGNAL(clicked ( const QModelIndex &  )),this, SLOT(clicked ( const QModelIndex &  )));
	connect(buttonModifyStatus, SIGNAL(clicked()), this, SLOT(modifyStatus()));


}

void PageSM::clicked ( const QModelIndex & index )
{
	
	QString word = model->getRecordByRow(index.row(), "word").toString();
	GlobalVal::s_pSpeakMgr->speak(word);

}

void PageSM::modifyStatus()
{

  
	QModelIndexList mil = wordTable->selectionModel()->selectedRows();

	if (mil.size()>1)
	{
		QMessageBox::warning(this, tr("错误"), tr("修改状态只能选择一个."), QMessageBox::Ok);
		return;
	}

	if (mil.size()==0)
	{
		QMessageBox::warning(this, tr("错误"), tr("必须选择一个单词修改."), QMessageBox::Ok);
		return;
	}

	QString status;
	int maxfp = g_pbcmgr->getUDataDB()->getMaxFP();
	QModelIndex mi = mil.at(0);
	int sc = model->getRecordByRow(mi.row(),"sc").toInt();
	if (sc>0)
	{
		DBG(qDebug() << model->getRecordByRow(mi.row(),"st"));
		if (model->getRecordByRow(mi.row(),"st").toInt()>=maxfp)
		{
			status = QString("掌握");

		}else
		{
			status = QString("学习中");
		}

	}else
	{
             status = QString("未学");
	}
	PageSMStatus pageSmStatus(this,model->getRecordByRow(mil.at(0).row(),"mmid").toInt(),model->getRecordByRow(mil.at(0).row(),"word").toString(),status);
	pageSmStatus.exec();
	update();
	
}

PageSM::~PageSM()
{
	if(model)
	delete model;
}

void PageSM::update()
{
	DBG(QDateTime   start   =   QDateTime::currentDateTime());
	model->update();
	//wordTable->resizeColumnsToContents();
	//g_pbcmgr->getUDataDB()->updateStudyProgress();
    DBG(qDebug() << " PageSM::update::PageSM::update 耗时" << start.msecsTo(QDateTime::currentDateTime()));
	lableWordtotal->setText(QString("单词列表(列表单词总数：%1)").arg(model->getRowCount()));
}
