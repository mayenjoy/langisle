#include "stdafx.h"
#include "PageSORemind.h"
#include "BCMgr.h"
PageSORemind::PageSORemind(QWidget *parent)
	: QWidget(parent)
{
	QFormLayout *mainLayout = new QFormLayout;
	setLayout(mainLayout);
	mainLayout->setFieldGrowthPolicy(QFormLayout::FieldsStayAtSizeHint);

	QCheckBox *checkEnableRemind = new QCheckBox("启用提醒功能");
	QCheckBox *checkEnableHightLight =new QCheckBox("高亮标示我的词库");

	QComboBox *comboThreshold = new QComboBox;
	comboThreshold->addItems(QStringList() << "5" << "20" << "30" << "40" << "50" << "60");
	mainLayout->addRow(checkEnableRemind);
	mainLayout->addRow("当待复习单词数超过多少个时提醒我:", comboThreshold);
	mainLayout->addRow(checkEnableHightLight);

	checkEnableRemind->setCheckState(g_pbcmgr->getRemindMgr()->isEnableRemind() ? Qt::Checked : Qt::Unchecked);
	checkEnableHightLight->setCheckState(g_pbcmgr->getRemindMgr()->isEnableHightLight() ? Qt::Checked : Qt::Unchecked);
	comboThreshold->setCurrentIndex(comboThreshold->findText(QString::number(g_pbcmgr->getRemindMgr()->getThreshold())));

	connect(checkEnableRemind, SIGNAL(stateChanged (int)), this, SLOT(checkEnableRemindstateChanged (int)));
	connect(checkEnableHightLight, SIGNAL(stateChanged (int)), this, SLOT(checkEnableHightLightChanged (int)));
	connect(comboThreshold, SIGNAL(currentIndexChanged ( const QString &  )), this, SLOT(currentIndexChangedcomboThreshold( const QString &  )));

}

PageSORemind::~PageSORemind()
{

}

void PageSORemind::save()
{

}

void PageSORemind::checkEnableRemindstateChanged ( int state )
{
	g_pbcmgr->getRemindMgr()->setEnableRemind(Qt::Checked == state);	
}

void PageSORemind::checkEnableHightLightChanged ( int state )
{
	g_pbcmgr->getRemindMgr()->setEnableHightLight(Qt::Checked == state);	
}

void PageSORemind::currentIndexChangedcomboThreshold ( const QString & text )
{
	g_pbcmgr->getRemindMgr()->setThreshold(text.toInt());
}
