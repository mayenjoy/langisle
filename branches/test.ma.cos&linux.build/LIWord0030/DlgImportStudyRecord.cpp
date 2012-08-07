#include "stdafx.h"
#include "DlgImportStudyRecord.h"
#include "BCMgr.h"
DlgImportStudyRecord::DlgImportStudyRecord(QWidget *parent)
	: QDialog(parent)
{
	QVBoxLayout *mainLayout = new QVBoxLayout;
	setLayout(mainLayout);

	QHBoxLayout *browerLayout = new QHBoxLayout;
	QPushButton *buttonBrowerFile = new QPushButton("浏览...");
	lineImportFilePath = new QLineEdit;
	lineImportFilePath->setReadOnly(true);
	browerLayout->addWidget(lineImportFilePath);
	browerLayout->addWidget(buttonBrowerFile);
	connect(buttonBrowerFile, SIGNAL(clicked()), this, SLOT(browerPath()));
	QFormLayout *fLayout = new QFormLayout;

	comboBoxType = new QComboBox;
	comboBoxType->addItems(isrm.getConvertTypeName());
	fLayout->addRow("类型:", comboBoxType);
	fLayout->addRow("位置:", browerLayout);



	QDialogButtonBox *buttonBox = new QDialogButtonBox;
	buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

	QObject::connect(buttonBox, SIGNAL(accepted()), this, SLOT(ok()));
	QObject::connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

	mainLayout->addLayout(fLayout);
	mainLayout->addWidget(buttonBox);

	setWindowTitle("导入学习记录");
}

DlgImportStudyRecord::~DlgImportStudyRecord()
{

}

void DlgImportStudyRecord::browerPath()
{
	QString path = QFileDialog::getExistingDirectory(this,tr("请选择要导入的用户记录文件夹:"));

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
