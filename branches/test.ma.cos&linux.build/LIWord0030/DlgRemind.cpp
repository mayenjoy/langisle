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

	setWindowTitle("语言岛学习提醒");
	web = new QWebView(this);
	QPushButton *buttonStudy = new QPushButton("现在学习");
	QPushButton *buttonAgain = new QPushButton("再提醒我");
	QPushButton *buttonCancel = new QPushButton("知道了");
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
	QString html = QString("您现在有<b><font color='red'>%1</font></b>个单词需要复习,建议您现在开始学习以保证最佳的学习效果!").arg(nNeedReviewCount);
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
