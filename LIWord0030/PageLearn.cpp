#include "stdafx.h"
#include "PageLearn.h"
#include <QWebFrame>
#include "ThemeMgr.h"
#include "GlobalVal.h"
#include "LIConfig.h"
PageLearn::PageLearn(QWidget *parent)
	: QWidget(parent)
{
// 	QPushButton *buttonGoback = new QPushButton("返回");
// 	connect(buttonGoback, SIGNAL(clicked()), this, SLOT(mgoback()));

	webView = new QWebView(this);
	webView->settings()->setAttribute(QWebSettings::JavascriptEnabled, true);
	webView->settings()->setAttribute(QWebSettings::PluginsEnabled, true);

	QHBoxLayout *bottomLayout = new QHBoxLayout;
	bottomLayout->addStretch();
//	bottomLayout->addWidget(buttonGoback);

	QVBoxLayout *mainLayout = new QVBoxLayout;
	setLayout(mainLayout);

	mainLayout->addWidget(webView);
	// mainLayout->addStretch();
	mainLayout->addLayout(bottomLayout);
	

	mainLayout->setSpacing(0);
	mainLayout->setContentsMargins(0, 0, 0, 0);

/*#if !defined(__DBG)*/
//	webView->setContextMenuPolicy(Qt::NoContextMenu);
/*#endif*/

#if defined(__DBG)
	QWebSettings* defaultSettings = QWebSettings::globalSettings();
// 	defaultSettings->setAttribute(QWebSettings::OfflineStorageDatabaseEnabled, true);
// 	defaultSettings->setAttribute(QWebSettings::JavascriptEnabled, true);
	defaultSettings->setAttribute(QWebSettings::DeveloperExtrasEnabled, true);
// 	QString dbDir = QDesktopServices::storageLocation(QDesktopServices::DataLocation);
// 	__DBG(qDebug() << dbDir);
// 	QWebSettings::setOfflineStoragePath(dbDir);
// 	QWebSettings::setOfflineStorageDefaultQuota(500000);
#endif
	
	 //webView->setUrl(Global::makeRelativeFilePath("/html/learnmgr.html"));
	

	// 缓存
	QNetworkDiskCache *diskCache = new QNetworkDiskCache(0);
	diskCache->setCacheDirectory(Global::makePathStr("/cache/webkit")); 
	webView->page()->networkAccessManager()->setCache(diskCache ); 

	if (!g_pbcmgr->getConfig()->getThemeID().isEmpty())
	{
		webView->setUrl(Global::makeRelativeFilePath("/html/theme" + GlobalVal::s_ptm->getCurrentThemeHtmlPath(g_pbcmgr->getConfig()->getThemeID())));
	}
	else
	{
		webView->setUrl(Global::makeRelativeFilePath("/html/theme/li1/li1.html"));
	}



	connect(webView->page()->mainFrame(), SIGNAL(javaScriptWindowObjectCleared()),
		this, SLOT(populateJavaScriptWindowObject()));
	webView->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);
	//connect(this, SIGNAL(test(QMap<QString, QVariant>)),
	//	learnMgr, SIGNAL(test(QMap<QString, QVariant>)));
	
	learnMgr = NULL;
}

PageLearn::~PageLearn()
{

}

void PageLearn::mgoback()
{
	DBG(qDebug() << "PageDefaultLearn::mgoback");
	//learnMgr->testSignal();
	endLearn();
	emit goback();
}

void PageLearn::populateJavaScriptWindowObject()
{
	if (NULL != learnMgr)
	{
		DBG(qDebug() << "PageLearn::populateJavaScriptWindowObject");

		webView->page()->mainFrame()->addToJavaScriptWindowObject("learnMgr", learnMgr);
		
		//QVariant tt = webView->page()->mainFrame()->evaluateJavaScript("new Array(1, 2, 3);");
	}
	
}


void PageLearn::beginLearn()
{
	QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
	if (NULL == learnMgr)
	{


		learnMgr = new LearnMgr(this);
		connect(learnMgr, SIGNAL(goback()), this, SLOT(mgoback()));
		//connect(webView->page(), SIGNAL(linkClicked(const QUrl &)), learnMgr, SLOT(linkClicked(const QUrl &)));
		
	}
	else
        {
		DBG(qDebug() << "PageLearn::beginLearn 重复开始");
        }
	webView->reload();
	QApplication::restoreOverrideCursor();
}

void PageLearn::endLearn()
{
	if (NULL != learnMgr)
	{
		disconnect(learnMgr, SIGNAL(goback()), this, SLOT(mgoback()));
		delete learnMgr;
		learnMgr = NULL;
	}
}
