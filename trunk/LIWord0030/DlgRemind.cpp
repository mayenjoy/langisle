#include "stdafx.h"
#include "DlgRemind.h"
#include <QWebView>
#include "RemindMgr.h"
#include "GlobalVal.h"
#include "LIWord0030.h"
DlgRemind::DlgRemind(QWidget *parent, RemindMgr *rmgr)
	: QDialog(parent), m_rmgr(rmgr)
{
	QVBoxLayout *mainLaylout = new QVBoxLayout;
	setLayout(mainLaylout);

	mainLaylout->setSpacing(0);
	mainLaylout->setContentsMargins(0, 0, 0, 0);

	setWindowTitle("���Ե�ѧϰ����");
	web = new QWebView(this);
	QPushButton *buttonStudy = new QPushButton("����ѧϰ");
	QPushButton *buttonAgain = new QPushButton("��������");
	QPushButton *buttonCancel = new QPushButton("֪����");
	QHBoxLayout *buttonLayout = new QHBoxLayout;
	buttonLayout->addStretch();
	buttonLayout->addWidget(buttonStudy);
	buttonLayout->addWidget(buttonAgain);
	buttonLayout->addWidget(buttonCancel);


	mainLaylout->addWidget(web);
	mainLaylout->addLayout(buttonLayout);

	connect(buttonStudy, SIGNAL(clicked()), this, SLOT(Study()));
	connect(buttonAgain, SIGNAL(clicked()), this, SLOT(Again()));
	connect(buttonCancel, SIGNAL(clicked()), this, SLOT(Cancel()));

	resize(480, 320);

	setAttribute(Qt::WA_QuitOnClose, false);
}

DlgRemind::~DlgRemind()
{

}


void DlgRemind::Study()
{
	DBG(qDebug() << "DlgRemind::Study begin");
	accept();
	DBG(qDebug() << "DlgRemind::Study end1.");
	GlobalVal::s_pmw->getPageMainBook()->learnAllBook();
	DBG(qDebug() << "DlgRemind::Study end2.");
	
}
void DlgRemind::Again()
{
	DBG(qDebug() << "DlgRemind::Again begin");
	accept();
	DBG(qDebug() << "DlgRemind::Again end");
}
void DlgRemind::Cancel()
{
	DBG(qDebug() << "DlgRemind::Cancel Cancel");
	m_rmgr->tempDisableRemind();
	DBG(qDebug() << "DlgRemind::Cancel end1.");
	accept();
	DBG(qDebug() << "DlgRemind::Cancel end2.");
}

void DlgRemind::updateWeb(int nNeedReviewCount)
{
	QString html = QString("��������<b><font color='red'>%1</font></b>��������Ҫ��ϰ,���������ڿ�ʼѧϰ�Ա�֤��ѵ�ѧϰЧ��!").arg(nNeedReviewCount);
	web->setHtml(html);
}

void DlgRemind::topShow()
{
	activateWindow();
	setFocus( Qt::OtherFocusReason );
	show();
	setWindowState( Qt::WindowActive );
	update();
	repaint();
	setVisible( true );
	raise();
}
