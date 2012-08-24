#include "stdafx.h"
#include "PageSOEditUser.h"
#include "GlobalVal.h"
#include "UserMgr.h"

PageSOEditUser::PageSOEditUser(QWidget *parent,const QString &username)
: QDialog(parent),username(username)
{
	this->setWindowTitle("�޸�����");
	QVBoxLayout *mainLayout = new QVBoxLayout;
	setLayout(mainLayout);

	QFormLayout *formLayout = new QFormLayout;
	lineEditUserName = new QLineEdit;
	lineEditOldPassword = new QLineEdit;
	lineEditOldPassword->setEchoMode(QLineEdit::Password);
	lineEditPassword = new QLineEdit;
	lineEditPassword->setEchoMode(QLineEdit::Password);
	lineEditVerifyPsw = new QLineEdit;
	lineEditVerifyPsw->setEchoMode(QLineEdit::Password);
	formLayout->setWidget(0, QFormLayout::LabelRole, new QLabel("�û���:"));
	formLayout->setWidget(0, QFormLayout::FieldRole, lineEditUserName);
	lineEditUserName->setText(username);
	lineEditUserName->setReadOnly(true);

	formLayout->setWidget(1, QFormLayout::LabelRole, new QLabel("������:"));
	formLayout->setWidget(1, QFormLayout::FieldRole, lineEditOldPassword);

	formLayout->setWidget(2, QFormLayout::LabelRole, new QLabel("������:"));
	formLayout->setWidget(2, QFormLayout::FieldRole, lineEditPassword);

	formLayout->setWidget(3, QFormLayout::LabelRole, new QLabel("ȷ��������:"));
	formLayout->setWidget(3, QFormLayout::FieldRole, lineEditVerifyPsw);

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
		this, SLOT(editClicked()));

	connect(pushButtonCancel, SIGNAL(clicked()), 
		this, SLOT(reject()));
}

PageSOEditUser::~PageSOEditUser()
{

}


void PageSOEditUser::editClicked()
{
	QString userName = lineEditUserName->text();
	QString oldPassword = lineEditOldPassword->text();
	QString password = lineEditPassword->text();
	QString verifyPsw = lineEditVerifyPsw->text();

	if (userName.isEmpty())
	{
		QMessageBox::warning(this, tr("����"), tr("�û�������Ϊ��."), QMessageBox::Ok);
		lineEditUserName->setFocus();
		return;
	}

	if (password != verifyPsw)
	{
		QMessageBox::warning(this, tr("����"), tr("�������������벻һ��."), QMessageBox::Ok);
		lineEditPassword->setFocus();
		return;
	}

	if (!GlobalVal::s_um->verifyPsw(userName,oldPassword))
	{
		QMessageBox::warning(this, tr("����"), tr("�û������벻��ȷ."), QMessageBox::Ok);
		lineEditUserName->setFocus();
		lineEditUserName->selectAll();
		return;
	}
	GlobalVal::s_um->updatePassword(username,password);
	accept();
}
