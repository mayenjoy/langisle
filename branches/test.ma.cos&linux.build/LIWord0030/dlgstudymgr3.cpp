#include "stdafx.h"
#include "dlgstudymgr3.h"
#include "BCMgr.h"
#include "studymgrmodel.h"
#include "studymgrstudystatusdelegate.h"
#include "QNoFocusItemDelegate.h"
#include "dlgexportword.h"
#include "ExportWordQA.h"
DlgStudyMgr3::DlgStudyMgr3(QWidget *parent)
	: QDialog(parent)
{
	QVBoxLayout *mainLayout = new QVBoxLayout;
	QHBoxLayout *bottomLayout = new QHBoxLayout;
	QHBoxLayout *okcancelLayout = new QHBoxLayout;

	bookSelectInfo = new QLabel("test");
	
	wordTable = new LITableView(this);
	wordTable->setShowGrid(false);
	wordTable->setSortingEnabled(true);
	wordTable->setItemDelegate(new QNoFocusItemDelegate(wordTable));
	wordTable->setSelectionBehavior(QAbstractItemView::SelectItems);
	wordTable->setSelectionMode(QAbstractItemView::SingleSelection);
	wordTable->verticalHeader()->hide();
	wordTable->horizontalHeader()->setMovable(true);
	wordTable->setMouseTracking(true);

	wordTable->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
	wordTable->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);

	wordTable->horizontalHeader()->setResizeMode(QHeaderView::Interactive); 
	wordTable->setTabKeyNavigation(false);
	wordTable->horizontalHeader()->setStretchLastSection( true ); 

	mainLayout->addWidget(bookSelectInfo);
	mainLayout->addWidget(wordTable);

	setLayout(mainLayout);

	pudb = g_pbcmgr->getUDataDB();
	pudb->init();

	model = new StudyMgrModel(this, wordTable);
	QString sql = QString("select word,mid,case when lst is null then 0 when (julianday(\'now\', \'localtime\') - julianday(lst)) * 86400.0 > st and st < %1 then 1 else 2 end as status,st,sc,datetime(lst) as lst,datetime((julianday(lst) * 86400.0 + st)/86400.0) as nst from \
						  (select mid,wtype,mdata.wid,sc,st,word,lst from studyData.mdata as mdata inner join dict.word on mdata.wtype = 2 and mdata.wid = dict.word.wid \
						  union \
						  select mid,wtype,mdata.wid,sc,st,word,lst from studyData.mdata as mdata INNER join uword on mdata.wtype = 1 and mdata.wid = uword.wid)").arg(pudb->getMaxFP());

	model->setSql(sql, *pudb->getdb());
	wordTable->setModel(model);
	wordTable->hideColumn(1);
	model->updateHeaderName();
	wordTable->horizontalHeader()->setSortIndicator(-1, Qt::AscendingOrder);
	connect(model, SIGNAL(updateCheck()), this, SLOT(updateBookSelectInfo()));

	for (int i = 0; i < model->record().count(); i++)
	{
		QString name = model->record().field(i).name();
		int width = model->getSizeSection(name);
		if (width)
		{
			wordTable->horizontalHeader()->resizeSection(i, width);
		}
	}

	

	

	wordTable->verticalHeader()->hide();

	wordTable->setItemDelegateForColumn(2, new StudyMgrStudyStatusDelegate(this));

	connect(wordTable, SIGNAL(doubleClicked(const QModelIndex &) ), this, SLOT(updateStatusSel(const QModelIndex &)));

	// ������Ŧ
	QLabel *labelSelect = new QLabel("ѡ��:");
	QPushButton *allButton = new QPushButton("ȫ��");
	QPushButton *notStudyButton = new QPushButton("δѧ");
	QPushButton *studingButton = new QPushButton("ѧϰ��");
	QPushButton *masterButton = new QPushButton("����");
	//QPushButton *exportButton = new QPushButton("����...");
	QToolButton *exportButton= new QToolButton; 
	exportButton->setText("����...");
	QMenu *menu=new QMenu();
	QAction *exportQAAct = new QAction(tr("������QA��ʽ"), this);
	menu->addAction(exportQAAct);
	exportButton->setPopupMode(QToolButton::InstantPopup);
	exportButton->setMenu(menu);

	connect(allButton, SIGNAL(clicked()), model, SLOT(allButtonClick()));
	connect(notStudyButton, SIGNAL(clicked()), model, SLOT(notStudyClick()));
	connect(studingButton, SIGNAL(clicked()), model, SLOT(studingClick()));
	connect(masterButton, SIGNAL(clicked()), model, SLOT(masterClick()));

	connect(allButton, SIGNAL(clicked()), this, SLOT(updateBookSelectInfo()));
	connect(notStudyButton, SIGNAL(clicked()), this, SLOT(updateBookSelectInfo()));
	connect(studingButton, SIGNAL(clicked()), this, SLOT(updateBookSelectInfo()));
	connect(masterButton, SIGNAL(clicked()), this, SLOT(updateBookSelectInfo()));

	connect(exportQAAct, SIGNAL(triggered()), this, SLOT(exportQAClick()));




	bottomLayout->addWidget(labelSelect);
	bottomLayout->addWidget(allButton);
	bottomLayout->addWidget(notStudyButton);
	bottomLayout->addWidget(studingButton);
	bottomLayout->addWidget(masterButton);
	bottomLayout->addStretch();
	bottomLayout->addWidget(exportButton);
	mainLayout->addLayout(bottomLayout);

	// ȷ��ȡ��
	okcancelLayout->addStretch();	
	QPushButton *okButton = new QPushButton("ȷ��");
	QPushButton *cancelButton = new QPushButton("ȡ��");

	connect(okButton, SIGNAL(clicked()), this, SLOT(save()));
	connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));

	okcancelLayout->addWidget(okButton);
	okcancelLayout->addWidget(cancelButton);

	mainLayout->addLayout(okcancelLayout);

	resize(720, 520);	// 800*600�ոպ�
	updateBookSelectInfo();

	this->setWindowTitle("ѧϰ����");
}

DlgStudyMgr3::~DlgStudyMgr3()
{
	pudb->uninit();
	// ����2��ɾ����,vs����crt����⵽����й©
	delete model;
	delete wordTable;
	
}
void DlgStudyMgr3::updateStatusSel ( const QModelIndex & index )
{
	if (index.column() == 0)
	{
		model->setCheckReverse(index);
	}
}

void DlgStudyMgr3::updateBookSelectInfo()
{
	int size = model->getRowCount();
	int selCount = model->getSelCount();
	
	bookSelectInfo->setText(QString("��%1������,ѡ��%2��").arg(size).arg(selCount));
}

void DlgStudyMgr3::exportQAClick()
{
	int selCount = model->getSelCount();
	if (!(selCount > 0))
	{
		QMessageBox::information(this, "��ʾ", "����ѡ��Ҫ�����ĵ���.");
		return;
	}

	QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
		"",	tr("QA (*.txt)"));

	if (fileName.isEmpty()) return;

    this->ExportToQA(QDir::convertSeparators(fileName));

}

void DlgStudyMgr3::ExportToQA(QString fileName)
{
	ExportWordQA ewQA;
	model->MakeExportWord(&ewQA);
	//if (ewQA.count() > 0)
	ewQA.WriteFile(fileName);
	QMessageBox::information(this, "��ʾ", "�����ɹ�.");
}

void DlgStudyMgr3::save()
{
	accept();
}
