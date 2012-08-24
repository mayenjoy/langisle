#include "StdAfx.h"
#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	g_pbcmgr = new BCMgr("tester", this);

	ui.setupUi(this);

	g_pbcmgr->addOTreeItem(ui.treeWidget_3);
}

MainWindow::~MainWindow()
{
	delete g_pbcmgr;
}
