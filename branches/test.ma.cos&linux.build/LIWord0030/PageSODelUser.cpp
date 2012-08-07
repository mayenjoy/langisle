#include "stdafx.h"
#include "PageSODelUser.h"
#include "GlobalVal.h"
#include "UserMgr.h"

PageSODelUser::PageSODelUser(QWidget *parent,const QString &username)
	: QDialog(parent),username(username)
{
	this->setWindowTitle("删除用户");
	QVBoxLayout *mainLayout = new QVBoxLayout;
	setLayout(mainLayout);

	QFormLayout *formLayout = new QFormLayout;
	lineEditUserName = new QLineEdit;
	lineEditPassword = new QLineEdit;
	lineEditPassword->setEchoMode(QLineEdit::Password);

	formLayout->setWidget(0, QFormLayout::LabelRole, new QLabel("用户名:"));
	formLayout->setWidget(0, QFormLayout::FieldRole, lineEditUserName);
	lineEditUserName->setText(username);
	lineEditUserName->setReadOnly(true);

	formLayout->setWidget(1, QFormLayout::LabelRole, new QLabel("密码:"));
	formLayout->setWidget(1, QFormLayout::FieldRole, lineEditPassword);

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
		QMessageBox::warning(this, tr("错误"), tr("用户名不能为空."), QMessageBox::Ok);
		lineEditUserName->setFocus();
		return;
	}

	if (!GlobalVal::s_um->verifyPsw(userName,password))
	{
		QMessageBox::warning(this, tr("错误"), tr("密码不对."), QMessageBox::Ok);
		lineEditPassword->setFocus();
		lineEditPassword->selectAll();
		return;
	}else
	{
		accept();
	
	}

}
