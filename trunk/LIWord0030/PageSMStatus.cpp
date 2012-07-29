#include "stdafx.h"
#include "PageSMStatus.h"
#include "BCMgr.h"

PageSMStatus::PageSMStatus(QWidget *parent,int mmid,const QString &word,const QString &status)
: QDialog(parent),mmid(mmid),word(word),status(status)
{
	this->setWindowTitle("�޸�״̬");
	QVBoxLayout *mainLayout = new QVBoxLayout;
	setLayout(mainLayout);

	QFormLayout *formLayout = new QFormLayout;
	lineEditWord = new QLineEdit;
	lineEditOldStatus = new QLineEdit;
	comboBoxStatusList = new QComboBox;

	formLayout->setWidget(0, QFormLayout::LabelRole, new QLabel("����:"));
	formLayout->setWidget(0, QFormLayout::FieldRole, lineEditWord);
	lineEditWord->setText(word);
	lineEditWord->setReadOnly(true);

	formLayout->setWidget(1, QFormLayout::LabelRole, new QLabel("ԭʼ״̬:"));
	formLayout->setWidget(1, QFormLayout::FieldRole, lineEditOldStatus);
	lineEditOldStatus->setText(status);
	lineEditOldStatus->setReadOnly(true);

	formLayout->setWidget(2, QFormLayout::LabelRole, new QLabel("�޸�״̬:"));
	formLayout->setWidget(2, QFormLayout::FieldRole, comboBoxStatusList);


	QHBoxLayout *bottomLayout = new QHBoxLayout;
	bottomLayout->addStretch();
	QPushButton *pushButtonModify = new QPushButton("ȷ��");
	QPushButton *pushButtonCancel = new QPushButton("ȡ��");

	bottomLayout->addWidget(pushButtonModify);
	bottomLayout->addWidget(pushButtonCancel);


	mainLayout->addLayout(formLayout);
	mainLayout->addLayout(bottomLayout);
	mainLayout->addStretch();

	initComboBoxStatusList();

	connect(pushButtonModify, SIGNAL(clicked()), 
		this, SLOT(modifyClicked()));

	connect(pushButtonCancel, SIGNAL(clicked()), 
		this, SLOT(reject()));
		
}

PageSMStatus::~PageSMStatus()
{
}



bool PageSMStatus::initComboBoxStatusList()
{
	comboBoxStatusList->clear();

	QStringList statusList ;
	statusList<<tr("δѧ")<<tr("����");
	QStringListIterator javaStyleIterator(statusList);
	while (javaStyleIterator.hasNext())
	{
		comboBoxStatusList->addItem(javaStyleIterator.next());
	}
	return true;
}


void PageSMStatus::modifyClicked()
{
	QString status = comboBoxStatusList->currentText();

	int mid = mmid;

	if (status=="δѧ")
	{	
		g_pbcmgr->getUDataDB()->clearBookStudyInfo(mid);
	}

	if (status=="����")
	{
		g_pbcmgr->getUDataDB()->updateBookStudyInfoknown(mid);
	}
	accept();
}
