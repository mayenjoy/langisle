#include "stdafx.h"
#include "PageSOAdvanced.h"
#include "BCMgr.h"
PageSOAdvanced::PageSOAdvanced(QWidget *parent)
	: QWidget(parent)
{
	QVBoxLayout *mainLayout = new QVBoxLayout;
	setLayout(mainLayout);

	QHBoxLayout *fpButtonLayout = new QHBoxLayout;

	QLabel *labelFP = new QLabel("遗忘点设置:(单位秒,<font color='red'>各遗忘点间以\',\'分隔,请谨慎修改.</font>)");
	lineFP = new QLineEdit;
	QString sFp = g_pbcmgr->getUDataDB()->getConfigInfo("defaultFP");
	lineFP->setText(sFp);
	QPushButton *fpButtonApply = new QPushButton("应用");
	QPushButton *fpButtonRestore = new QPushButton("恢复默认值");

	fpButtonLayout->addWidget(fpButtonApply);
	fpButtonLayout->addWidget(fpButtonRestore);
	fpButtonLayout->addStretch();

	mainLayout->addWidget(labelFP);
	mainLayout->addWidget(lineFP);
	mainLayout->addLayout(fpButtonLayout);
	mainLayout->addStretch();

	connect(fpButtonApply, SIGNAL(clicked()), this, SLOT(applyFP()));
	connect(fpButtonRestore, SIGNAL(clicked()), this, SLOT(restoreFP()));
}

PageSOAdvanced::~PageSOAdvanced()
{

}

void PageSOAdvanced::save()
{

}

void PageSOAdvanced::applyFP()
{
	DBG(qDebug() << "PageSOAdvanced::applyFP" << lineFP->text());
	QStringList sl = lineFP->text().split(',');
	if (sl.size() < 3)
	{
		QMessageBox::information(this, "提示信息", "至少给3个遗忘点吧!");
		return;
	}
	foreach (QString s, sl)
	{
		if (!s.toInt())
		{
			QMessageBox::information(this, "提示信息", "请正确设置遗忘点!");
			return;
		}
	}
	g_pbcmgr->getUDataDB()->setConfigInfo("defaultFP", lineFP->text());
	g_pbcmgr->getUDataDB()->initFP();
	g_pbcmgr->getUDataDB()->updateStudyProgress();
}

void PageSOAdvanced::restoreFP()
{
	lineFP->setText("30,120,300,1800,43200,86400,172800,345600,604800,1296000");
}
