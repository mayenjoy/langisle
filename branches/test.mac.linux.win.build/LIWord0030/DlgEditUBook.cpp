#include "stdafx.h"
#include "DlgEditUBook.h"
#include "WordViewBCDBModel.h"
#include "LITableView.h"
#include "BCMgr.h"
#include "../comm/QNoFocusItemDelegate.h"
#include "BookDB.h"
#include "DlgEditWord.h"
#include "DlgImportUBook.h"

DlgEditUBook::DlgEditUBook(QWidget *parent)
	: QDialog(parent)
{
	bdb = 0;
	QVBoxLayout *mainLayout = new QVBoxLayout;
	setLayout(mainLayout);

	QHBoxLayout *topLayout = new QHBoxLayout;

	wordTable = new LITableView;

	model = new WordViewBCDBModel;

	QList<BOOKINFO> m_biList;
	m_biList = g_pbcmgr->getSelectBook();
	if (m_biList.size() > 1 || m_biList.size() == 0)
	{
		DBG(qDebug() << "DlgEditUBook::DlgEditUBook m_biList 大于1,出问题了");
		return;
	}

	BOOKINFO bookInfo = m_biList.at(0);
	
	bookType bt = (bookType)bookInfo["btype"].toInt();
	bid = bookInfo["bid"].toInt();
	QString path = g_pbcmgr->getBookPath(bt, bid);
	bdb = new BookDB(path, path);

// udatadb中查看单词
// 	QString sql = QString("select %1.bwid as bwid, case when mdata.wtype = 2 then dict.word.word else uword.word end as word, %2.mean as mean from %3 inner join mdata on mdata.mid = %4.mid left join uword on uword.wid = mdata.wid left join dict.word on dict.word.wid = mdata.wid")
// 				.arg(tableName)
// 				.arg(tableName)
// 				.arg(tableName)
// 				.arg(tableName);

	QString sql = "select bwid, case when dwt.wid is null then swt.word else dwt.word end as word, swt.mean as mean from word as swt left join dict.word as dwt on dwt.wid = swt.wid";
	//QString sql = "select * from word";
	model->setSql(sql, *bdb->getdb());
	model->updateHeaderName();
	wordTable->setModel(model);
	//wordTable->hideColumn(0);
	wordTable->verticalHeader()->hide();
	//wordTable->horizontalHeader()->setResizeMode(QHeaderView::Interactive); 
	wordTable->setSelectionBehavior(QAbstractItemView::SelectRows);
	wordTable->setSelectionMode(QAbstractItemView::ExtendedSelection );
	wordTable->setShowGrid(false);
	wordTable->setItemDelegate(new QNoFocusItemDelegate(wordTable));
	wordTable->setMouseTracking(true);
	wordTable->resizeColumnsToContents();
	wordTable->verticalHeader()->setDefaultSectionSize(24);

	wordTable->setSortingEnabled(true);
	wordTable->sortByColumn(0, Qt::AscendingOrder);
	

	connect(wordTable, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(tableDoubleClicked(const QModelIndex &)));

	buttonImport	= new QPushButton("导入(&I)..."); 
	buttonExport	= new QPushButton("导出(&E)..."); 

	buttonAdd		= new QPushButton("添加(&A)..."); 
	buttonDel		= new QPushButton("删除(&D)..."); 
	buttonEdit		= new QPushButton("修改(&C)..."); 


	QVBoxLayout *rightLayout = new QVBoxLayout;
	
	rightLayout->addWidget(buttonAdd);
	rightLayout->addWidget(buttonDel);
	rightLayout->addWidget(buttonEdit);
	rightLayout->addStretch();
	rightLayout->addWidget(buttonImport);
	//rightLayout->addWidget(buttonExport);
	

	



	QObject::connect(buttonImport, SIGNAL(clicked()), this, SLOT(importData()));
	QObject::connect(buttonExport, SIGNAL(clicked()), this, SLOT(exportData()));

	QObject::connect(buttonAdd, SIGNAL(clicked()), this, SLOT(add()));
	QObject::connect(buttonDel, SIGNAL(clicked()), this, SLOT(del()));
	QObject::connect(buttonEdit, SIGNAL(clicked()), this, SLOT(edit()));

	buttonDel->setDisabled(true);
	buttonEdit->setDisabled(true);

	QDialogButtonBox *buttonBox = new QDialogButtonBox;
	buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

	QObject::connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
	QObject::connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

// 	connect(wordTable->selectionModel(), SIGNAL(currentRowChanged(const QModelIndex &, const QModelIndex &)),
// 		this, SLOT(currentRowChanged(const QModelIndex &, const QModelIndex &)));
	connect(wordTable->selectionModel(), SIGNAL(selectionChanged(const QItemSelection & , const QItemSelection & )),
		this, SLOT(selectionChanged(const QItemSelection & , const QItemSelection & )));
// 	connect(wordTable, SIGNAL(clicked ( const QModelIndex &  )),
// 		this, SLOT(clicked ( const QModelIndex &  )));

	topLayout->addWidget(wordTable);
	topLayout->addLayout(rightLayout);
	mainLayout->addLayout(topLayout);
	mainLayout->addWidget(buttonBox);

	resize(480, 320);

	setWindowTitle(QString("编辑词库[%1]").arg(bookInfo["name"].toString()));
}

DlgEditUBook::~DlgEditUBook()
{
	delete model;
	if (bdb)
		delete bdb;

}

void DlgEditUBook::tableDoubleClicked( const QModelIndex & index )
{
    Q_UNUSED(index);
	DBG(qDebug() << "DlgEditUBook::tableDoubleClicked" << index);
}

void DlgEditUBook::importData()
{
	DBG(qDebug() << "DlgEditUBook::importData");
	DlgImportUBook diub(this, bdb);
	if (QDialog::Accepted == diub.exec())
	{
		g_pbcmgr->getUbcdb()->updateWordCount(bid, bdb->wordCount());
		model->update();
		wordTable->resizeColumnsToContents();
	}
}

void DlgEditUBook::exportData()
{
	DBG(qDebug() << "DlgEditUBook::exportData");
}

void DlgEditUBook::add()
{
	DBG(qDebug() << "DlgEditUBook::add");
	DlgEditWord dlg(this, bdb, DEAdd);
	
	if (QDialog::Accepted == dlg.exec())
	{
		g_pbcmgr->getUbcdb()->updateWordCount(bid, bdb->wordCount());
		model->update();
		wordTable->resizeColumnsToContents();
	}
}

void DlgEditUBook::del()
{
	
	QModelIndexList mil = wordTable->selectionModel()->selectedRows();
	foreach (QModelIndex mi, mil)
	{
		DBG(qDebug() << "DlgEditUBook::del" << mi);
		int bwid = model->getRecordByRow(mi.row(), "bwid").toInt();
		bdb->delWord(bwid);
	}

	model->update();
	wordTable->resizeColumnsToContents();
	g_pbcmgr->getUbcdb()->updateWordCount(bid, bdb->wordCount());
}

void DlgEditUBook::edit()
{
	DBG(qDebug() << "DlgEditUBook::edit");
	QModelIndexList mil = wordTable->selectionModel()->selectedRows();
	QModelIndex mi = mil.at(0);
	DlgEditWord dlg(this, bdb, DEEdit);
	dlg.setWord(model->getRecordByRow(mi.row(), "word").toString());
	dlg.setMean(model->getRecordByRow(mi.row(), "mean").toString());
	if (QDialog::Accepted == dlg.exec())
	{
		model->update();
		wordTable->resizeColumnsToContents();
	}
	
}


// void  DlgEditUBook::currentRowChanged (const QModelIndex & current, const QModelIndex & previous)
// {
// 	DBG(qDebug() << "DlgEditUBook::currentRowChanged");
// }
// 
// void  DlgEditUBook::clicked ( const QModelIndex & index )
// {
// 	DBG(qDebug() << "DlgEditUBook::clicked");
// }

void	DlgEditUBook::selectionChanged ( const QItemSelection & selected, const QItemSelection & deselected )
{
     Q_UNUSED(selected);
    Q_UNUSED(deselected);
	//DBG(qDebug() << "DlgEditUBook::selectionChanged" << selected.size() << deselected.size());
	
	int scount = wordTable->selectionModel()->selectedRows().size();
	DBG(qDebug() << "DlgEditUBook::selectionChanged" << scount);
	if (scount > 0)
	{
		
		buttonDel->setDisabled(false);

	}
	else
	{
		buttonDel->setDisabled(true);
	}

	if (scount == 1) 
		buttonEdit->setDisabled(false);
	else
		buttonEdit->setDisabled(true);
	
}
