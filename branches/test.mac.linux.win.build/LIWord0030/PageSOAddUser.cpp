#include "stdafx.h"
#include "PageSOAddUser.h"
#include "GlobalVal.h"
#include "UserMgr.h"

PageSOAddUser::PageSOAddUser(QWidget *parent)
	: QDialog(parent)
{
	this->setWindowTitle("����û�");
	QVBoxLayout *mainLayout = new QVBoxLayout;
	setLayout(mainLayout);
	
	QFormLayout *formLayout = new QFormLayout;
	lineEditUserName = new QLineEdit;
	lineEditPassword = new QLineEdit;
	lineEditPassword->setEchoMode(QLineEdit::Password);
	lineEditVerifyPsw = new QLineEdit;
	lineEditVerifyPsw->setEchoMode(QLineEdit::Password);
	formLayout->setWidget(0, QFormLayout::LabelRole, new QLabel("�û���:"));
	formLayout->setWidget(0, QFormLayout::FieldRole, lineEditUserName);

	formLayout->setWidget(1, QFormLayout::LabelRole, new QLabel("����:"));
	formLayout->setWidget(1, QFormLayout::FieldRole, lineEditPassword);

	formLayout->setWidget(2, QFormLayout::LabelRole, new QLabel("ȷ������:"));
	formLayout->setWidget(2, QFormLayout::FieldRole, lineEditVerifyPsw);

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
		QMessageBox::warning(this, tr("����"), tr("�û�������Ϊ��."), QMessageBox::Ok);
		lineEditUserName->setFocus();
		return;
	}

	if (password != verifyPsw)
	{
		QMessageBox::warning(this, tr("����"), tr("�����������벻һ��."), QMessageBox::Ok);
		lineEditPassword->setFocus();
		return;
	}

	if (GlobalVal::s_um->exists(userName))
	{
		QMessageBox::warning(this, tr("����"), tr("���û��Ѵ���."), QMessageBox::Ok);
		lineEditUserName->setFocus();
		lineEditUserName->selectAll();
		return;
	}

	QMap<QString, QVariant> ui;
	ui["userName"] = userName;
	ui["password"] = verifyPsw;

	GlobalVal::s_um->addUser(ui);
    /*
	if (QMessageBox::Ok == QMessageBox::information(this, tr("��ʾ"), tr("�û������ɹ�,�����������û���?"), QMessageBox::Ok | QMessageBox::Cancel))
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
