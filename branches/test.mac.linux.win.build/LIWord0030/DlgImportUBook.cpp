#include "stdafx.h"
#include "DlgImportUBook.h"
#include "BookDB.h"
#include "BookConvertMgr.h"
DlgImportUBook::DlgImportUBook(QWidget *parent, BookDB *bdb)
	: QDialog(parent), mbdb(bdb)
{
	QVBoxLayout *mainLayout = new QVBoxLayout;
	setLayout(mainLayout);

	QHBoxLayout *bookPosLayout = new QHBoxLayout;
	QPushButton *buttonBrowerFile = new QPushButton("浏览...");
	lineImportFilePath = new QLineEdit;
	lineImportFilePath->setReadOnly(true);
	bookPosLayout->addWidget(lineImportFilePath);
	bookPosLayout->addWidget(buttonBrowerFile);
	connect(buttonBrowerFile, SIGNAL(clicked()), this, SLOT(importFile()));
	QFormLayout *fLayout = new QFormLayout;
	comboBoxType = new QComboBox;
	comboBoxType->addItems(bcm.getConvertTypeName());
	fLayout->addRow("类型:", comboBoxType);
	fLayout->addRow("位置:", bookPosLayout);
	//fLayout->addRow("词库名称:", new QLineEdit);
	//fLayout->addRow("词库备注:", new QTextEdit);



	QDialogButtonBox *buttonBox = new QDialogButtonBox;
	buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

	QObject::connect(buttonBox, SIGNAL(accepted()), this, SLOT(ok()));
	QObject::connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

	mainLayout->addLayout(fLayout);
	mainLayout->addWidget(buttonBox);

	setWindowTitle("导入词库");

	//resize(300, sizeHint().height());
	
}

DlgImportUBook::~DlgImportUBook()
{

}


void DlgImportUBook::importFile()
{
	QString fileName = QFileDialog::getOpenFileName(this,
		tr("请选择需要导入的词库"));

	if (!fileName.isEmpty()) 
	{
		lineImportFilePath->setText(QDir::convertSeparators(fileName));
	}

}


void DlgImportUBook::ok()
{
// 	if (!QFile::exists(lineImportFilePath))
// 	{
// 		QMessageBox::information(this, "提示", "您选择的词库不存在,请重新")
// 	}
	if (!bcm.Convert(mbdb, comboBoxType->currentIndex(), lineImportFilePath->text()))
	{
		DBG(qDebug() << "DlgImportUBook::ok Convert faild");
		return;
	}
	accept();
}
