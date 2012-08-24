#include "stdafx.h"
#include "LIWord0030.h"
#include <QtGui/QApplication>

#include "GlobalVal.h"
#include "DlgOBookDown.h"
#include "NetworkProxyMgr.h"
#include "../comm/qtsingleapplication/qtsingleapplication.h"
SharedTools::QtSingleApplication *  pApp;

void test()
{
// 	QDir directory("E:\\Langisle\\v0.2\\src\\bin\\udata\\prehisfle");
// 
// 	if (!directory.exists()) 
// 	{
// 		return;
// 	}
// 
// 	QStringList files = directory.entryList(QStringList("*.ldb"), QDir::AllEntries | QDir::NoDotAndDotDot | QDir::Hidden);
// 	DBG(qDebug() << files);

// 	QNetworkProxy proxy;
// 	proxy.setType(QNetworkProxy::HttpProxy);
// 	proxy.setHostName("192.168.1.2");
// 	proxy.setPort(808);
// 	proxy.setUser("username");
// 	proxy.setPassword("password");
// 	QNetworkProxy::setApplicationProxy(proxy);

//	exit(0);
}

int main(int argc, char *argv[])
{
	QDateTime   start   =   QDateTime::currentDateTime();
	SharedTools::QtSingleApplication app("langisle liword0.3", argc, argv);
	pApp = &app;
	pApp->setApplicationName("LIWord");

	if (pApp->isRunning())
	{
		pApp->sendMessage( "RaiseWindow" );
		return  0;
	}

// 	QTranslator translator( 0 ); 
// 	if (translator.load(Global::makePathStr("/translations/qt_zh_CN")))
// 	{
// 		app.installTranslator( &translator );
// 	}
	

	DBG(qInstallMsgHandler(MyMsgHandler));
	Global::setCodec("GB18030");	

    //test();
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));


	GlobalVal::init();
	
    QIcon logo = QIcon(":/LIWord0030.ico");
    QApplication::setWindowIcon(logo);
	


	QFontDatabase database;
	QStringList fl = database.families();
	//qDebug() << fl;
	QFont oFont = QApplication::font();
	if (fl.contains(QObject::tr("Î¢ÈíÑÅºÚ")))
	{
		DBG(qDebug() << QObject::tr("Î¢ÈíÑÅºÚ"));
		QApplication::setFont(QFont(QObject::tr("Î¢ÈíÑÅºÚ"), oFont.pointSize()));
		// QApplication::setFont(QFont("Î¢ÈíÑÅºÚ"));
	}
	else if (fl.contains(QObject::tr("ËÎÌå")))
	{
		DBG(qDebug() << QObject::tr("ËÎÌå"));
		QApplication::setFont(QFont(QObject::tr("ËÎÌå"), oFont.pointSize()));
		// QApplication::setFont(QFont("ËÎÌå"));
	}
	else
	{
		QApplication::setFont(QFont("simsun", 10)); 

	}

	// DBG(qDebug() << QApplication::font());

	//a.setQuitOnLastWindowClosed(true);
	LIWord0030 *pw = new LIWord0030;

//	pw->show();

	GlobalVal::s_pmw = pw;

	QObject::connect( pApp, SIGNAL( messageReceived( const QString & ) ),
		pw, SLOT(vMessageReceivedFromOtherInst( const QString & ) ) );

	
	
	QApplication::restoreOverrideCursor();
	//Global::sleep(2500 - start.msecsTo(QDateTime::currentDateTime()));
        GlobalVal::s_pSplash->finish(pw);
        pw->display();


	
	int nRet = app.exec();
	delete pw;
	GlobalVal::uninit();
	return nRet;
}

