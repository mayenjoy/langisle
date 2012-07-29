#include "stdafx.h"
#include "UpdateDialog.h"
#include <QtGui/QApplication>
#include "../comm/qtsingleapplication/qtsingleapplication.h"

int main(int argc, char *argv[])
{


 
	//SharedTools::QtSingleApplication *app = new SharedTools::QtSingleApplication("langisle liword", argc, argv);

	//int count = 0;
	//while (app->isRunning() && count <= 50) 
	//{
	//	app->sendMessage( "Close" );
	//	Global::sleep(100);
	//	count ++;
	//}
	//
	//QStringList args = QCoreApplication::instance()->arguments();
	//args.takeFirst();           // skip the first argument, which is the program's name
	//if (args.size() < 4)
	//{
	//	delete app;
	//	return 0;
	//}
	//qDebug() << args;
	//GlobalVal::gVal.appName = args[0];
	//GlobalVal::gVal.exeName = args[1];
	//GlobalVal::gVal.updatedDir = args[2];
	//GlobalVal::gVal.updateUrl = args[3];
	//

	//if (count > 50)
	//{
	//	QMessageBox::warning(NULL, "错误!", QString("%1无法关闭, 更新失败.").arg(GlobalVal::gVal.appName));
	//	delete app;
	//	return 0;
	//}
	//delete app;


	QApplication a(argc, argv);

	Global::setCodec("GB18030");
	DBG(qInstallMsgHandler(MyMsgHandler));

	UpdateDialog ud;



	QStringList args = QCoreApplication::instance()->arguments();


	DBG(qDebug() << args);

	args.takeFirst();           // skip the first argument, which is the program's name
	if (args.size() < 5)
	{
		//delete app;
		return 0;
	}

	ud.appName = args[0];
	ud.exeName = args[1];
	ud.updatedDir = args[2];
	ud.updateUrl = args[3];
	ud.newVer = args[4];

	ud.start();
	ud.show();
	return a.exec();
}
