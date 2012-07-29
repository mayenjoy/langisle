#include "stdafx.h"
#include "dlgstudymgr2.h"
#include <QWebFrame>
#include "GlobalVal.h"
#include "BCMgr.h"
DlgStudyMgr2::DlgStudyMgr2(QWidget *parent)
	: QDialog(parent)
{
	QVBoxLayout *mainLaylout = new QVBoxLayout;
	setLayout(mainLaylout);

	setWindowTitle("语言岛学习管理");
	web = new QWebView(this);

	mainLaylout->setSpacing(0);
	mainLaylout->setContentsMargins(0, 0, 0, 0);

	mainLaylout->addWidget(web);

	web->setUrl(Global::makeRelativeFilePath("/html/studymgr.html"));
	resize(720, 520);	// 800*600刚刚好

	UDataDB *udb = g_pbcmgr->getUDataDB();

	connect(web->page()->mainFrame(), SIGNAL(javaScriptWindowObjectCleared()),
		this, SLOT(populateJavaScriptWindowObject()));
	sm = new StudyMgr(0);


	QWebSettings* defaultSettings = QWebSettings::globalSettings();
	// 	defaultSettings->setAttribute(QWebSettings::OfflineStorageDatabaseEnabled, true);
	// 	defaultSettings->setAttribute(QWebSettings::JavascriptEnabled, true);
	defaultSettings->setAttribute(QWebSettings::DeveloperExtrasEnabled, true);
	// 	QString dbDir = QDesktopServices::storageLocation(QDesktopServices::DataLocation);
	// 	__DBG(qDebug() << dbDir);
	// 	QWebSettings::setOfflineStoragePath(dbDir);
	// 	QWebSettings::setOfflineStorageDefaultQuota(500000);

}

DlgStudyMgr2::~DlgStudyMgr2()
{
	delete sm;
}

void DlgStudyMgr2::populateJavaScriptWindowObject()
{
	web->page()->mainFrame()->addToJavaScriptWindowObject("StudyMgr",sm);
}
