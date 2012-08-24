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
	QPushButton *buttonBrowerFile = new QPushButton("���...");
	lineImportFilePath = new QLineEdit;
	lineImportFilePath->setReadOnly(true);
	bookPosLayout->addWidget(lineImportFilePath);
	bookPosLayout->addWidget(buttonBrowerFile);
	connect(buttonBrowerFile, SIGNAL(clicked()), this, SLOT(importFile()));
	QFormLayout *fLayout = new QFormLayout;
	comboBoxType = new QComboBox;
	comboBoxType->addItems(bcm.getConvertTypeName());
	fLayout->addRow("����:", comboBoxType);
	fLayout->addRow("λ��:", bookPosLayout);
	//fLayout->addRow("�ʿ�����:", new QLineEdit);
	//fLayout->addRow("�ʿⱸע:", new QTextEdit);



	QDialogButtonBox *buttonBox = new QDialogButtonBox;
	buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

	QObject::connect(buttonBox, SIGNAL(accepted()), this, SLOT(ok()));
	QObject::connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

	mainLayout->addLayout(fLayout);
	mainLayout->addWidget(buttonBox);

	setWindowTitle("����ʿ�");

	//resize(300, sizeHint().height());
	
}

DlgImportUBook::~DlgImportUBook()
{

}


void DlgImportUBook::importFile()
{
	QString fileName = QFileDialog::getOpenFileName(this,
		tr("��ѡ����Ҫ����Ĵʿ�"));

	if (!fileName.isEmpty()) 
	{
		lineImportFilePath->setText(QDir::convertSeparators(fileName));
	}

}


void DlgImportUBook::ok()
{
// 	if (!QFile::exists(lineImportFilePath))
// 	{
// 		QMessageBox::information(this, "��ʾ", "��ѡ��Ĵʿⲻ����,������")
// 	}
	if (!bcm.Convert(mbdb, comboBoxType->currentIndex(), lineImportFilePath->text()))
	{
		DBG(qDebug() << "DlgImportUBook::ok Convert faild");
		return;
	}
	accept();
}
