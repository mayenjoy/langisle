#include "stdafx.h"
#include <QtGui/QApplication>
#include "UIGDialog.h"
#include <QXmlStreamWriter>

void test()
{ 

	//xsw.
}

int main(int argc, char *argv[])
{
	Global::setCodec("GB18030");

	QApplication a(argc, argv);

	//test();

	UIGDialog uigd;
	uigd.show();
	return a.exec();
}
