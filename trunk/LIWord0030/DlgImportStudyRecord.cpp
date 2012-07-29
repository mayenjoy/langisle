#include "stdafx.h"
#include "DlgImportStudyRecord.h"
#include "BCMgr.h"
DlgImportStudyRecord::DlgImportStudyRecord(QWidget *parent)
	: QDialog(parent)
{
	QVBoxLayout *mainLayout = new QVBoxLayout;
	setLayout(mainLayout);

	QHBoxLayout *browerLayout = new QHBoxLayout;
	QPushButton *buttonBrowerFile = new QPushButton("���...");
	lineImportFilePath = new QLineEdit;
	lineImportFilePath->setReadOnly(true);
	browerLayout->addWidget(lineImportFilePath);
	browerLayout->addWidget(buttonBrowerFile);
	connect(buttonBrowerFile, SIGNAL(clicked()), this, SLOT(browerPath()));
	QFormLayout *fLayout = new QFormLayout;

	comboBoxType = new QComboBox;
	comboBoxType->addItems(isrm.getConvertTypeName());
	fLayout->addRow("����:", comboBoxType);
	fLayout->addRow("λ��:", browerLayout);



	QDialogButtonBox *buttonBox = new QDialogButtonBox;
	buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

	QObject::connect(buttonBox, SIGNAL(accepted()), this, SLOT(ok()));
	QObject::connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

	mainLayout->addLayout(fLayout);
	mainLayout->addWidget(buttonBox);

	setWindowTitle("����ѧϰ��¼");
}

DlgImportStudyRecord::~DlgImportStudyRecord()
{

}

void DlgImportStudyRecord::browerPath()
{
	QString path = QFileDialog::getExistingDirectory(this,tr("��ѡ��Ҫ������û���¼�ļ���:"));

	if (!path.isEmpty()) 
	{
		lineImportFilePath->setText(QDir::convertSeparators(path));
	}
}

void DlgImportStudyRecord::ok()
{
	if (lineImportFilePath->text().isEmpty())
	{
		return;
	}
	if (!isrm.StartImport(g_pbcmgr->getUDataDB(), comboBoxType->currentIndex(), lineImportFilePath->text()))
	{
		DBG(qDebug() << "DlgImportStudyRecord::ok Convert faild");
		return;
	}
	accept();
}
