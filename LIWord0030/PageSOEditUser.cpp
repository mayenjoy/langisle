#include "stdafx.h"
#include "PageSOEditUser.h"
#include "GlobalVal.h"
#include "UserMgr.h"

PageSOEditUser::PageSOEditUser(QWidget *parent,const QString &username)
: QDialog(parent),username(username)
{
	this->setWindowTitle("修改密码");
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
	formLayout->setWidget(0, QFormLayout::LabelRole, new QLabel("用户名:"));
	formLayout->setWidget(0, QFormLayout::FieldRole, lineEditUserName);
	lineEditUserName->setText(username);
	lineEditUserName->setReadOnly(true);

	formLayout->setWidget(1, QFormLayout::LabelRole, new QLabel("旧密码:"));
	formLayout->setWidget(1, QFormLayout::FieldRole, lineEditOldPassword);

	formLayout->setWidget(2, QFormLayout::LabelRole, new QLabel("新密码:"));
	formLayout->setWidget(2, QFormLayout::FieldRole, lineEditPassword);

	formLayout->setWidget(3, QFormLayout::LabelRole, new QLabel("确认新密码:"));
	formLayout->setWidget(3, QFormLayout::FieldRole, lineEditVerifyPsw);

	QHBoxLayout *bottomLayout = new QHBoxLayout;
	bottomLayout->addStretch();
	QPushButton *pushButtonCreate = new QPushButton("确定");
	QPushButton *pushButtonCancel = new QPushButton("取消");

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
		QMessageBox::warning(this, tr("错误"), tr("用户名不能为空."), QMessageBox::Ok);
		lineEditUserName->setFocus();
		return;
	}

	if (password != verifyPsw)
	{
		QMessageBox::warning(this, tr("错误"), tr("两次输入新密码不一致."), QMessageBox::Ok);
		lineEditPassword->setFocus();
		return;
	}

	if (!GlobalVal::s_um->verifyPsw(userName,oldPassword))
	{
		QMessageBox::warning(this, tr("错误"), tr("用户旧密码不正确."), QMessageBox::Ok);
		lineEditUserName->setFocus();
		lineEditUserName->selectAll();
		return;
	}
	GlobalVal::s_um->updatePassword(username,password);
	accept();
}
