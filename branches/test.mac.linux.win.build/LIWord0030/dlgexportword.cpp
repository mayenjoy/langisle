#include "stdafx.h"
#include "dlgexportword.h"

DlgExportWord::DlgExportWord(QWidget *parent)
	: QDialog(parent)
{
	QVBoxLayout *mainLayout = new QVBoxLayout;
	setLayout(mainLayout);

	QHBoxLayout *browerLayout = new QHBoxLayout;
	QPushButton *buttonBrowerFile = new QPushButton("浏览...");
	lineExportFilePath = new QLineEdit;
	lineExportFilePath->setReadOnly(true);
	browerLayout->addWidget(lineExportFilePath);
	browerLayout->addWidget(buttonBrowerFile);
	connect(buttonBrowerFile, SIGNAL(clicked()), this, SLOT(browerPath()));
	QFormLayout *fLayout = new QFormLayout;

	comboBoxType = new QComboBox;
	//comboBoxType->addItems(isrm.getConvertTypeName());
	fLayout->addRow("类型:", comboBoxType);
	fLayout->addRow("位置:", browerLayout);



	QDialogButtonBox *buttonBox = new QDialogButtonBox;
	buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

	QObject::connect(buttonBox, SIGNAL(accepted()), this, SLOT(ok()));
	QObject::connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

	mainLayout->addLayout(fLayout);
	mainLayout->addWidget(buttonBox);

	setWindowTitle("导入单词");
}

DlgExportWord::~DlgExportWord()
{

}

void DlgExportWord::browerPath()
{
	QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
		"",	tr("QA (*.txt)"));

	if (fileName.isEmpty()) return;

	lineExportFilePath->setText(QDir::convertSeparators(fileName));

}

void DlgExportWord::ok()
{
	if (lineExportFilePath->text().isEmpty())
	{
		return;
	}
// 	if (!isrm.StartImport(g_pbcmgr->getUDataDB(), comboBoxType->currentIndex(), lineImportFilePath->text()))
// 	{
// 		DBG(qDebug() << "DlgImportStudyRecord::ok Convert faild");
// 		return;
// 	}
	accept();
}
