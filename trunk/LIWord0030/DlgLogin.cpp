#include "stdafx.h"
#include "DlgLogin.h"
#include "GlobalVal.h"
#include "UserMgr.h"
#include "LIConfig.h"
#include "DlgCreateUser.h"
DlgLogin::DlgLogin(QWidget *parent, bool bDisableAutoLogin)
	: QDialog(parent), m_bDisableAutoLogin(bDisableAutoLogin)
{
	QVBoxLayout *mainLayout = new QVBoxLayout;
	setLayout(mainLayout);

	QFormLayout *formLayout = new QFormLayout;
	formLayout->setFormAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
	QLabel *labelUserName = new QLabel("�û���:");
	comboBoxUserList = new QComboBox;
	formLayout->setWidget(0, QFormLayout::LabelRole, labelUserName);
	formLayout->setWidget(0, QFormLayout::FieldRole, comboBoxUserList);

	lineEditPassword = new QLineEdit;
	lineEditPassword->setEchoMode(QLineEdit::Password);
	formLayout->setWidget(1, QFormLayout::LabelRole, new QLabel("����:"));
	formLayout->setWidget(1, QFormLayout::FieldRole, lineEditPassword);


	QVBoxLayout *buttonLayout = new QVBoxLayout;
	QPushButton *createUserButton = new QPushButton("�½��û�");
	QPushButton *loginButton = new QPushButton("��¼");
	buttonLayout->addWidget(createUserButton);
	buttonLayout->addWidget(loginButton);

	QHBoxLayout *topLayout = new QHBoxLayout;
	topLayout->addLayout(formLayout);
	topLayout->addLayout(buttonLayout);


	QHBoxLayout *bottomLayout = new QHBoxLayout;
	checkBoxRememberPsw = new QCheckBox("��ס����");
	checkBoxAutoLogin = new QCheckBox("�Զ���¼");
	bottomLayout->addSpacing(labelUserName->sizeHint().rwidth() + 6);
	//bottomLayout->addItem(new QSpacerItem(labelUserName->sizeHint().rwidth() + 6, checkBoxAutoLogin->sizeHint().height()));
	bottomLayout->addWidget(checkBoxRememberPsw);
	bottomLayout->addWidget(checkBoxAutoLogin);
	bottomLayout->addStretch();

	mainLayout->addLayout(topLayout);
	mainLayout->addLayout(bottomLayout);
	
	mainLayout->addStretch();

	initComboBoxUserList();

	connect(loginButton, SIGNAL(clicked()), 
		this, SLOT(loginClicked()));

	connect(createUserButton, SIGNAL(clicked()), 
		this, SLOT(newUserClicked()));

	connect(checkBoxRememberPsw, SIGNAL(clicked (bool)), 
		this, SLOT(rememberPswClicked(bool)));

	connect(checkBoxAutoLogin, SIGNAL(clicked (bool)), 
		this, SLOT(autoLoginClicked(bool)));

	connect(comboBoxUserList, SIGNAL(currentIndexChanged ( const QString &)), this, SLOT(userChanged(const QString &)));

	// ��������¼���û�
	QString userName = GlobalVal::s_pcfg->getLastLoginUser();
	int index = comboBoxUserList->findText(userName);
	if (-1 != index)
	{
		comboBoxUserList->setCurrentIndex(index);
		userChanged(userName);
		// �����Զ���¼��ť
		bool bAutoLogin = GlobalVal::s_pcfg->getAutoLogin();
		checkBoxAutoLogin->setCheckState(bAutoLogin ? Qt::Checked : Qt::Unchecked);

		autoLoginClicked(bAutoLogin);

		if (bAutoLogin && !bDisableAutoLogin)
			QTimer::singleShot(50, this, SLOT(loginClicked()));
	}

	lineEditPassword->setFocus();


}

DlgLogin::~DlgLogin()
{

}

bool DlgLogin::initComboBoxUserList()
{
	comboBoxUserList->clear();

	QStringList userList = GlobalVal::s_um->getUserList();
	QStringListIterator javaStyleIterator(userList);
	while (javaStyleIterator.hasNext())
	{
		comboBoxUserList->addItem(javaStyleIterator.next());
	}
	return true;
}

void DlgLogin::userChanged(QString userName)
{
	bool bSavePsw = GlobalVal::s_um->getUserInfo(QString("savePsw"), userName).toInt();
	if (bSavePsw)
	{
		lineEditPassword->setText(GlobalVal::s_um->getUserInfo(QString("password"), userName).toString());
	}
	else
	{
		lineEditPassword->setText("");
	}
	checkBoxRememberPsw->setCheckState(bSavePsw ? Qt::Checked : Qt::Unchecked);
	checkBoxAutoLogin->setCheckState(Qt::Unchecked);
}

void DlgLogin::loginClicked()
{
	qDebug() << comboBoxUserList->currentText() << lineEditPassword->text();
	QString userName = comboBoxUserList->currentText();
	QString password = lineEditPassword->text();
	if (GlobalVal::s_um->verifyPsw(userName, password))
	{
		qDebug() << "������ȷ";
		GlobalVal::s_pcfg->setLastLoginUser(userName);
		GlobalVal::s_um->updateLoginTime(userName);
		GlobalVal::s_um->updateSavePsw(userName, checkBoxRememberPsw->checkState());
		GlobalVal::s_pcfg->setAutoLogin(checkBoxAutoLogin->checkState());

		accept();
	}
	else
	{
		QMessageBox::warning(this, tr("����"), tr("�������(ע���Сд),����������."));
		lineEditPassword->setFocus();
	}
}

void DlgLogin::newUserClicked()
{
	DlgCreateUser cu(this);
	if (QDialog::Accepted == cu.exec())
	{
		

		checkBoxAutoLogin->setCheckState(Qt::Unchecked);
		checkBoxRememberPsw->setCheckState(Qt::Unchecked);

		lineEditPassword->setFocus();
	}
	initComboBoxUserList();
}

void DlgLogin::rememberPswClicked(bool checked)
{
	QString userName = comboBoxUserList->currentText();
	GlobalVal::s_um->updateSavePsw(userName, checked);
	if (!checked)
		checkBoxAutoLogin->setCheckState(Qt::Unchecked);
}

void DlgLogin::autoLoginClicked(bool checked)
{
	if (checked)
	{
		checkBoxRememberPsw->setCheckState(Qt::Checked);
	}

}


void DlgLogin::reject()
{
	if (QMessageBox::Yes == QMessageBox::information(this, "��ʾ��Ϣ", "��ȷ��Ҫ�˳�������?", QMessageBox::Yes | QMessageBox::No))
	{
		QDialog::reject();
	}

}
