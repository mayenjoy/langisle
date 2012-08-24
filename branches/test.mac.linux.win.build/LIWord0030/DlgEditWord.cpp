#include "stdafx.h"
#include "DlgEditWord.h"
#include "BookDB.h"


DlgEditWord::DlgEditWord(QWidget *parent, BookDB *bdb, DlgEditWordType type)
	: QDialog(parent), mbdb(bdb), mtype(type)
{
	QVBoxLayout *mainLayout = new QVBoxLayout;
	setLayout(mainLayout);

	wordEdit = new QLineEdit;
	meanEdit = new QTextEdit;
	meanEdit->setTabChangesFocus(true);
	QFormLayout *fLayout = new QFormLayout;
	fLayout->addRow("单词:", wordEdit);
	fLayout->addRow("释义:", meanEdit);

	if (type == DEEdit)
	{
		setWindowTitle("修改单词");
		wordEdit->setReadOnly(true);
	}
	else
	{
		setWindowTitle("添加单词");
	}

	QDialogButtonBox *buttonBox = new QDialogButtonBox;
	buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

	QObject::connect(buttonBox, SIGNAL(accepted()), this, SLOT(ok()));
	QObject::connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

	mainLayout->addLayout(fLayout);
	mainLayout->addWidget(buttonBox);
}

DlgEditWord::~DlgEditWord()
{

}

void DlgEditWord::ok()
{
	if (mtype == DEAdd)
	{
		if (mbdb->isWordExist(word()))
		{
			if (QMessageBox::Ok == QMessageBox::question(this, "提示", QString("[%1]已经存在,覆盖现有单词吗?").arg(word()), QMessageBox::Ok | QMessageBox::Cancel))
			{
				mbdb->addEditWord(word(), mean());
				accept();
			}
		}
		else
		{
			mbdb->addEditWord(word(), mean());
			accept();
		}
	}
	else if (mtype == DEEdit)
	{
		mbdb->addEditWord(word(), mean());
		accept();
	}

	
	
}
