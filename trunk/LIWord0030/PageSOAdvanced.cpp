#include "stdafx.h"
#include "PageSOAdvanced.h"
#include "BCMgr.h"
PageSOAdvanced::PageSOAdvanced(QWidget *parent)
	: QWidget(parent)
{
	QVBoxLayout *mainLayout = new QVBoxLayout;
	setLayout(mainLayout);

	QHBoxLayout *fpButtonLayout = new QHBoxLayout;

	QLabel *labelFP = new QLabel("����������:(��λ��,<font color='red'>�����������\',\'�ָ�,������޸�.</font>)");
	lineFP = new QLineEdit;
	QString sFp = g_pbcmgr->getUDataDB()->getConfigInfo("defaultFP");
	lineFP->setText(sFp);
	QPushButton *fpButtonApply = new QPushButton("Ӧ��");
	QPushButton *fpButtonRestore = new QPushButton("�ָ�Ĭ��ֵ");

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
		QMessageBox::information(this, "��ʾ��Ϣ", "���ٸ�3���������!");
		return;
	}
	foreach (QString s, sl)
	{
		if (!s.toInt())
		{
			QMessageBox::information(this, "��ʾ��Ϣ", "����ȷ����������!");
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
