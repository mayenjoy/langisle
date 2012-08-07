#include "stdafx.h"
#include "PageSOGeneral.h"
#include "LIWord0030.h"
#include "GlobalVal.h"
#include "LIConfig.h"
#include "DlgUpdate.h"
PageSOGeneral::PageSOGeneral(QWidget *parent)
	: QWidget(parent)
{
	QVBoxLayout *mainLayout = new QVBoxLayout;
	setLayout(mainLayout);
	
	QHBoxLayout *verlayout = new QHBoxLayout;
	QCheckBox *autoCheckNewVer = new QCheckBox("�Զ�������");
	checkUpdateButton = new QPushButton("����°汾");
	verlayout->addWidget(autoCheckNewVer);
	verlayout->addWidget(checkUpdateButton);
	verlayout->addStretch();


	QCheckBox *checkBoxEnableTray =new QCheckBox("�رյ�����");


	mainLayout->addLayout(verlayout);
	mainLayout->addWidget(checkBoxEnableTray);
	mainLayout->addStretch();

	connect(checkUpdateButton, SIGNAL(clicked()), this, SLOT(checkUpdate()));
	connect(&m_um, SIGNAL(checkResult(bool)), this, SLOT(checkResult(bool)));

	autoCheckNewVer->setCheckState(GlobalVal::s_pcfg->getValue("Main/autoCheckNewVer", true).toBool() ? Qt::Checked : Qt::Unchecked);
	checkBoxEnableTray->setCheckState(GlobalVal::s_pcfg->getValue("Main/enableTray", true).toBool() ? Qt::Checked : Qt::Unchecked);

	connect(autoCheckNewVer, SIGNAL(stateChanged (int)), this, SLOT(stateChanged (int)));
	connect(checkBoxEnableTray, SIGNAL(stateChanged (int)), this, SLOT(enableTraystateChanged (int)));
}

PageSOGeneral::~PageSOGeneral()
{

}

void PageSOGeneral::save()
{
	
}

void PageSOGeneral::checkUpdate()
{
	checkUpdateButton->setEnabled(false);
	m_um.checkNewVer();
}	

void PageSOGeneral::checkResult(bool bHaveNewVer)
{
	checkUpdateButton->setEnabled(true);
	if (bHaveNewVer)
	{
// 		DBG(qDebug() << "PageSOGeneral::checkResult ���°汾");
// 		if (QMessageBox::Yes ==QMessageBox::question(this, "����°汾", "��⵽�°汾,�����ڸ�����?", QMessageBox::Yes | QMessageBox::No))
// 		{
// 			m_um.applyNewVer();
// 			GlobalVal::s_pmw->close ();
// 		}
		DlgUpdate dlg(this);
		dlg.setHtml(m_um.updateNote());
		if (QDialog::Accepted == dlg.exec())
		{
			m_um.applyNewVer();
			qApp->quit();
		}
	}
	else
	{
		DBG(qDebug() << m_um.getResultInfo());
		QMessageBox::information(this, "����°汾", m_um.getResultInfo());
	}
	
}

void PageSOGeneral::stateChanged ( int state )
{
	GlobalVal::s_pcfg->setValue("Main/autoCheckNewVer", state);
}	


void PageSOGeneral::enableTraystateChanged( int state )
{
	GlobalVal::s_pcfg->setValue("Main/enableTray", state);
}