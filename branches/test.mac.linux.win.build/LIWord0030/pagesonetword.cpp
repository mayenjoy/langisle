#include "stdafx.h"
#include "pagesonetword.h"
#include "NetworkProxyMgr.h"
#include "BCMgr.h"
#include "LIConfig.h"
PageSONetword::PageSONetword(QWidget *parent)
	: QWidget(parent)
{
	QVBoxLayout *mainLayout = new QVBoxLayout;
	setLayout(mainLayout);

	QGridLayout *gl = new  QGridLayout;
	gl->addWidget(new QLabel("����:"), 0, 0, 1, 1);
	gl->addWidget(new QLabel("��ַ:"), 0, 1, 1, 1);
	gl->addWidget(new QLabel("�˿�:"), 0, 2, 1, 1);

	type = new QComboBox;
	type->setFixedWidth(200);
	type->addItems(QStringList() << "��ʹ�ô���" << "HTTP����" << "SOCKS5����");
	leAddr = new QLineEdit;
	lePort = new QLineEdit(this);
	QIntValidator *v = new QIntValidator( 1, 65535, this);
	lePort->setValidator( v );

	gl->addWidget(type, 1, 0, 1, 1);
	gl->addWidget(leAddr, 1, 1, 1, 1);
	gl->addWidget(lePort, 1, 2, 1, 1);

	gl->addWidget(new QLabel("�û���:"), 2, 1, 1, 1);
	gl->addWidget(new QLabel("����:"), 2, 2, 1, 1);

	leUName = new QLineEdit;
	lePWord = new QLineEdit;

	gl->addWidget(leUName, 3, 1, 1, 1);
	gl->addWidget(lePWord, 3, 2, 1, 1);

	mainLayout->addLayout(gl);


	QPushButton *testButton = new QPushButton("����");
	QPushButton *saveButton = new QPushButton("����");
	QHBoxLayout *t = new QHBoxLayout;
	t->addStretch();
	t->addWidget(testButton);
	//t->addWidget(saveButton);

	mainLayout->addLayout(t);
	mainLayout->addStretch();

	connect(testButton, SIGNAL(clicked()), this, SLOT(testClick()));
	//connect(saveButton, SIGNAL(clicked()), this, SLOT(saveClick()));

// 	QHBoxLayout *fpButtonLayout = new QHBoxLayout;
// 
// 	QLabel *labelFP = new QLabel("����������:(��λ��,<font color='red'>�����������\',\'�ָ�,������޸�.</font>)");
// 	lineFP = new QLineEdit;
// 	QString sFp = g_pbcmgr->getUDataDB()->getConfigInfo("defaultFP");
// 	lineFP->setText(sFp);
// 	QPushButton *fpButtonApply = new QPushButton("Ӧ��");
// 	QPushButton *fpButtonRestore = new QPushButton("�ָ�Ĭ��ֵ");
// 
// 	fpButtonLayout->addWidget(fpButtonApply);
// 	fpButtonLayout->addWidget(fpButtonRestore);
// 	fpButtonLayout->addStretch();
// 
// 	mainLayout->addWidget(labelFP);
// 	mainLayout->addWidget(lineFP);
// 	mainLayout->addLayout(fpButtonLayout);
// 	mainLayout->addStretch();
// 
// 	connect(fpButtonApply, SIGNAL(clicked()), this, SLOT(applyFP()));
// 	connect(fpButtonRestore, SIGNAL(clicked()), this, SLOT(restoreFP()));

	initData();
}

PageSONetword::~PageSONetword()
{

}

void PageSONetword::save()
{
	saveClick();
}

void PageSONetword::testClick()
{
	NetworkProxyMgr *pm = NetworkProxyMgr::inst();
	QNetworkProxy::ProxyType proxyType = QNetworkProxy::NoProxy;
	if (type->currentText() == "��ʹ�ô���")
	{
		proxyType = QNetworkProxy::NoProxy;
		
	}
	else if (type->currentText() == "HTTP����")
	{
		proxyType = QNetworkProxy::HttpProxy;

	}
	else if (type->currentText() == "SOCKS5����")
	{
		proxyType = QNetworkProxy::Socks5Proxy;
	}
	QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
	if (pm->Test(proxyType, leAddr->text(), lePort->text().toInt(), leUName->text(), lePWord->text()))
	{
		QApplication::restoreOverrideCursor();
		QMessageBox::information(this, "��ʾ", "�������Ӳ��Գɹ�!");
	}
	else
	{
		QApplication::restoreOverrideCursor();
		QMessageBox::information(this, "��ʾ", "�������Ӳ���ʧ��!");
	}
}

void PageSONetword::saveClick()
{
	g_pbcmgr->getConfig()->setValue("Network/ProxyType", type->currentText());
	g_pbcmgr->getConfig()->setValue("Network/Host", leAddr->text());
	g_pbcmgr->getConfig()->setValue("Network/Port", lePort->text());
	g_pbcmgr->getConfig()->setValue("Network/UserName", leUName->text());
	g_pbcmgr->getConfig()->setValue("Network/Password", lePWord->text());
	NetworkProxyMgr::inst()->UpdateProxyFromIni();
}

void PageSONetword::initData()
{
	LIConfig * cfg = g_pbcmgr->getConfig();
	int index = type->findText(cfg->getValue("Network/ProxyType", "��ʹ�ô���").toString());
	type->setCurrentIndex(index);
	leAddr->setText(cfg->getValue("Network/Host", "").toString());
	lePort->setText(cfg->getValue("Network/Port", "").toString());
	leUName->setText(cfg->getValue("Network/UserName", "").toString());
	lePWord->setText(cfg->getValue("Network/Password", "").toString());
}
