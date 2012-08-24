#include "stdafx.h"
#include "PageSOAddUser.h"
#include "GlobalVal.h"
#include "UserMgr.h"

PageSOAddUser::PageSOAddUser(QWidget *parent)
	: QDialog(parent)
{
	this->setWindowTitle("添加用户");
	QVBoxLayout *mainLayout = new QVBoxLayout;
	setLayout(mainLayout);
	
	QFormLayout *formLayout = new QFormLayout;
	lineEditUserName = new QLineEdit;
	lineEditPassword = new QLineEdit;
	lineEditPassword->setEchoMode(QLineEdit::Password);
	lineEditVerifyPsw = new QLineEdit;
	lineEditVerifyPsw->setEchoMode(QLineEdit::Password);
	formLayout->setWidget(0, QFormLayout::LabelRole, new QLabel("用户名:"));
	formLayout->setWidget(0, QFormLayout::FieldRole, lineEditUserName);

	formLayout->setWidget(1, QFormLayout::LabelRole, new QLabel("密码:"));
	formLayout->setWidget(1, QFormLayout::FieldRole, lineEditPassword);

	formLayout->setWidget(2, QFormLayout::LabelRole, new QLabel("确认密码:"));
	formLayout->setWidget(2, QFormLayout::FieldRole, lineEditVerifyPsw);

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
		this, SLOT(createClicked()));

	connect(pushButtonCancel, SIGNAL(clicked()), 
		this, SLOT(reject()));
}

PageSOAddUser::~PageSOAddUser()
{

}


void PageSOAddUser::createClicked()
{
	QString userName = lineEditUserName->text();
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
		QMessageBox::warning(this, tr("错误"), tr("两次输入密码不一致."), QMessageBox::Ok);
		lineEditPassword->setFocus();
		return;
	}

	if (GlobalVal::s_um->exists(userName))
	{
		QMessageBox::warning(this, tr("错误"), tr("该用户已存在."), QMessageBox::Ok);
		lineEditUserName->setFocus();
		lineEditUserName->selectAll();
		return;
	}

	QMap<QString, QVariant> ui;
	ui["userName"] = userName;
	ui["password"] = verifyPsw;

	GlobalVal::s_um->addUser(ui);
    /*
	if (QMessageBox::Ok == QMessageBox::information(this, tr("提示"), tr("用户创建成功,继续创建新用户吗?"), QMessageBox::Ok | QMessageBox::Cancel))
	{
		lineEditUserName->clear();
		lineEditPassword->clear();
		lineEditVerifyPsw->clear();
		lineEditUserName->setFocus();
	}
	else
	*/
		accept();
}
