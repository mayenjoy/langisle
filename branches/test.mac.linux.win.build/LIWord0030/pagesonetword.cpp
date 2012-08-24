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
	gl->addWidget(new QLabel("类型:"), 0, 0, 1, 1);
	gl->addWidget(new QLabel("地址:"), 0, 1, 1, 1);
	gl->addWidget(new QLabel("端口:"), 0, 2, 1, 1);

	type = new QComboBox;
	type->setFixedWidth(200);
	type->addItems(QStringList() << "不使用代理" << "HTTP代理" << "SOCKS5代理");
	leAddr = new QLineEdit;
	lePort = new QLineEdit(this);
	QIntValidator *v = new QIntValidator( 1, 65535, this);
	lePort->setValidator( v );

	gl->addWidget(type, 1, 0, 1, 1);
	gl->addWidget(leAddr, 1, 1, 1, 1);
	gl->addWidget(lePort, 1, 2, 1, 1);

	gl->addWidget(new QLabel("用户名:"), 2, 1, 1, 1);
	gl->addWidget(new QLabel("密码:"), 2, 2, 1, 1);

	leUName = new QLineEdit;
	lePWord = new QLineEdit;

	gl->addWidget(leUName, 3, 1, 1, 1);
	gl->addWidget(lePWord, 3, 2, 1, 1);

	mainLayout->addLayout(gl);


	QPushButton *testButton = new QPushButton("测试");
	QPushButton *saveButton = new QPushButton("保存");
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
// 	QLabel *labelFP = new QLabel("遗忘点设置:(单位秒,<font color='red'>各遗忘点间以\',\'分隔,请谨慎修改.</font>)");
// 	lineFP = new QLineEdit;
// 	QString sFp = g_pbcmgr->getUDataDB()->getConfigInfo("defaultFP");
// 	lineFP->setText(sFp);
// 	QPushButton *fpButtonApply = new QPushButton("应用");
// 	QPushButton *fpButtonRestore = new QPushButton("恢复默认值");
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
	if (type->currentText() == "不使用代理")
	{
		proxyType = QNetworkProxy::NoProxy;
		
	}
	else if (type->currentText() == "HTTP代理")
	{
		proxyType = QNetworkProxy::HttpProxy;

	}
	else if (type->currentText() == "SOCKS5代理")
	{
		proxyType = QNetworkProxy::Socks5Proxy;
	}
	QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
	if (pm->Test(proxyType, leAddr->text(), lePort->text().toInt(), leUName->text(), lePWord->text()))
	{
		QApplication::restoreOverrideCursor();
		QMessageBox::information(this, "提示", "网络连接测试成功!");
	}
	else
	{
		QApplication::restoreOverrideCursor();
		QMessageBox::information(this, "提示", "网络连接测试失败!");
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
	int index = type->findText(cfg->getValue("Network/ProxyType", "不使用代理").toString());
	type->setCurrentIndex(index);
	leAddr->setText(cfg->getValue("Network/Host", "").toString());
	lePort->setText(cfg->getValue("Network/Port", "").toString());
	leUName->setText(cfg->getValue("Network/UserName", "").toString());
	lePWord->setText(cfg->getValue("Network/Password", "").toString());
}
