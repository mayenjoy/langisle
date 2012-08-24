#include "stdafx.h"
#include "PageSODelUser.h"
#include "GlobalVal.h"
#include "UserMgr.h"

PageSODelUser::PageSODelUser(QWidget *parent,const QString &username)
	: QDialog(parent),username(username)
{
	this->setWindowTitle("ɾ���û�");
	QVBoxLayout *mainLayout = new QVBoxLayout;
	setLayout(mainLayout);

	QFormLayout *formLayout = new QFormLayout;
	lineEditUserName = new QLineEdit;
	lineEditPassword = new QLineEdit;
	lineEditPassword->setEchoMode(QLineEdit::Password);

	formLayout->setWidget(0, QFormLayout::LabelRole, new QLabel("�û���:"));
	formLayout->setWidget(0, QFormLayout::FieldRole, lineEditUserName);
	lineEditUserName->setText(username);
	lineEditUserName->setReadOnly(true);

	formLayout->setWidget(1, QFormLayout::LabelRole, new QLabel("����:"));
	formLayout->setWidget(1, QFormLayout::FieldRole, lineEditPassword);

	QHBoxLayout *bottomLayout = new QHBoxLayout;
	bottomLayout->addStretch();
	QPushButton *pushButtonCreate = new QPushButton("ȷ��");
	QPushButton *pushButtonCancel = new QPushButton("ȡ��");
	
	bottomLayout->addWidget(pushButtonCreate);
	bottomLayout->addWidget(pushButtonCancel);

	mainLayout->addLayout(formLayout);
	mainLayout->addLayout(bottomLayout);
	mainLayout->addStretch();

	connect(pushButtonCreate, SIGNAL(clicked()), 
		this, SLOT(delClicked()));

	connect(pushButtonCancel, SIGNAL(clicked()), 
		this, SLOT(reject()));
}

PageSODelUser::~PageSODelUser()
{

}


void PageSODelUser::delClicked()
{
	QString userName = lineEditUserName->text();
	QString password = lineEditPassword->text();

	if (userName.isEmpty())
	{
		QMessageBox::warning(this, tr("����"), tr("�û�������Ϊ��."), QMessageBox::Ok);
		lineEditUserName->setFocus();
		return;
	}

	if (!GlobalVal::s_um->verifyPsw(userName,password))
	{
		QMessageBox::warning(this, tr("����"), tr("���벻��."), QMessageBox::Ok);
		lineEditPassword->setFocus();
		lineEditPassword->selectAll();
		return;
	}else
	{
		accept();
	
	}

}
