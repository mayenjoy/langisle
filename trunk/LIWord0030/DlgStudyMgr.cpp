#include "stdafx.h"
#include "DlgStudyMgr.h"
#include "StudyMgrBCDBModel.h"
#include "LITableView.h"
#include "qframe.h"
#include "BCMgr.h"
#include "PageSM.h"


DlgStudyMgr::DlgStudyMgr(QWidget *parent)
: QDialog(parent)
{
	DBG(QDateTime   start   =   QDateTime::currentDateTime());
	QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
	QVBoxLayout *mainLayout = new QVBoxLayout;
	QHBoxLayout *topLayout = new QHBoxLayout;
	QHBoxLayout *bottomLayout = new QHBoxLayout;
	mainLayout->addLayout(topLayout);
	mainLayout->addLayout(bottomLayout);
	setLayout(mainLayout);

	QListWidget *lw = new QListWidget;
	sw = new QStackedWidget;
	sw->setFrameShape(QFrame::StyledPanel);
	//sw->setFrameStyle(QFrame::Plain);
	sw->setContentsMargins(-1, -1, -1, 1);
	// sw->setStyleSheet("border: 2px solid green;	border-radius: 4px;padding: 2px;");
	//sw->setStyleSheet("border-width:1px; border: 1px solid white;");



	topLayout->addWidget(lw);
	topLayout->addWidget(sw);
	bottomLayout->addStretch();	
	QPushButton *okButton = new QPushButton("ȷ��");
	QPushButton *cancelButton = new QPushButton("ȡ��");

	bottomLayout->addWidget(okButton);
	bottomLayout->addWidget(cancelButton);

	lw->setMaximumWidth(120);

	lw->addItem("���е���");
	lw->addItem("��ѧ����");
	lw->addItem("ѧϰ�е���");
	lw->addItem("���յ���");
	lw->addItem("δѧ����");
	

	psma = new PageSM(this,QString("all"));
	psml = new PageSM(this,QString("learned"));
	psmli = new PageSM(this,QString("learning"));
	psmk = new PageSM(this,QString("known"));
	psmnl = new PageSM(this,QString("notlearn"));


	sw->addWidget(psma);
	sw->addWidget(psml);
	sw->addWidget(psmli);
	sw->addWidget(psmk);
	sw->addWidget(psmnl);

	
	//connect(lw, SIGNAL(currentRowChanged(int)), sw, SLOT(setCurrentIndex(int)));
	connect(lw, SIGNAL(currentRowChanged(int)), sw, SLOT(setCurrentIndex(int)));
	connect(lw, SIGNAL(currentRowChanged(int)), this, SLOT(update(int)));
	connect(okButton, SIGNAL(clicked()), this, SLOT(okClicked()));
	connect(cancelButton, SIGNAL(clicked()), this, SLOT(cancelClicked()));
	resize(720, 520);	// 800*600�ոպ�

	Qt::WindowFlags flags=Qt::Dialog;
	flags |= Qt::WindowMaximizeButtonHint;
	flags |= Qt::WindowMinimizeButtonHint;
	setWindowFlags(flags);
	
	setWindowTitle(QString("ѧϰ����"));
	DBG(qDebug() << "DlgStudyMgr::DlgStudyMgr ��ʱ" << start.msecsTo(QDateTime::currentDateTime()));
	QApplication::restoreOverrideCursor();

}

DlgStudyMgr::~DlgStudyMgr()
{
}

void DlgStudyMgr::okClicked()
{
	g_pbcmgr->getUDataDB()->updateStudyProgress();	
	accept();
}

void DlgStudyMgr::cancelClicked()
{
	g_pbcmgr->getUDataDB()->updateStudyProgress();	
	reject();
}

void DlgStudyMgr::update(int row)
{
	DBG(QDateTime   start   =   QDateTime::currentDateTime());
	((PageSM*)sw->widget(row))->update();
    DBG(qDebug() << "DlgStudyMgr::update::~DlgStudyMgr::update ��ʱ" << start.msecsTo(QDateTime::currentDateTime()));
}
