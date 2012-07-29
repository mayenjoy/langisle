#include "stdafx.h"
#include "LIWord0030.h"
#include "GlobalVal.h"
#include "DlgSystemOption.h"
#include "DlgLogin.h"
#include "DlgImportStudyRecord.h"
#include "config.h"
#include "DlgUpdate.h"
#include "LIConfig.h"
#include "LIConfig.h"
#include "AnalyticStatisticsMgr.h"
#include "DlgStudyMgr.h"
#include "DlgSA.h"
#include "dlgstudymgr3.h"
#include "NetworkProxyMgr.h"
LIWord0030::LIWord0030(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{

	userStatics();

	setWindowTitle("语言岛智能记单词 v" VER);
	// 顶级中心窗口
	QWidget *centralWidget = new QWidget;
	// centralWidget->setMinimumSize(QSize(720, 540)); // 4:3
	// centralWidget->setMinimumSize(QSize(720, 405)); // 16:9
	// centralWidget->setMinimumSize(QSize(720, 500));
	centralWidget->setMinimumSize(QSize(540, 405));
	setCentralWidget(centralWidget);

	// stacke分页窗口
	stackeWidget = new QStackedWidget;




	// 布局
	QVBoxLayout *mainlayout = new QVBoxLayout(centralWidget);
	mainlayout->addWidget(stackeWidget);
	mainlayout->setSpacing(0);
	mainlayout->setContentsMargins(0, 0, 0, 0);

	resize(720, 520);	// 800*600刚刚好
	int iTitleBarHeight = this->style()->pixelMetric(QStyle::PM_TitleBarHeight);
	QDesktopWidget* desktop = QApplication::desktop(); // =qApp->desktop();也可以
	DBG(qDebug() << "LIWord0030::LIWord0030" << desktop->availableGeometry().width() << desktop->availableGeometry().height() << this->width() << this->height());
	move((desktop->availableGeometry().width() - this->width()) / 2, (desktop->availableGeometry().height() - this->height() - iTitleBarHeight) / 2);




	createActions();
	createMenus();
	createToolBars();
	createStatusBar();
	createTrayIcon();

	trayIcon->setIcon(QIcon(":/LIWord0030.ico"));
	trayIcon->show();

	g_pbcmgr = NULL;

//	QTimer::singleShot(0, this, SLOT(login()));


	connect(&m_um, SIGNAL(checkResult(bool)), this, SLOT(checkResult(bool)));

	connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(trayActivated(QSystemTrayIcon::ActivationReason)));

}
void LIWord0030::display()
{
    show();
    QTimer::singleShot(0, this, SLOT(login()));

	if (GlobalVal::s_pcfg->getValue("Main/autoCheckNewVer", true).toBool())
		m_um.checkNewVer();
}

LIWord0030::~LIWord0030()
{

	delete stackeWidget;	// 手动销毁,避免崩溃(不然有些窗口对象是在GlobalVal销毁之后才析构的,可能会访问无效的指针)

	if (g_pbcmgr) delete g_pbcmgr;

	
}

void LIWord0030::login(bool bDisableAutoLogin)
{

	
	bShowDlgRemind = false;
	DlgLogin dlg(this, bDisableAutoLogin);
	if (QDialog::Accepted != dlg.exec())
	{
		close();
		return;
	}
	DBG(QDateTime   start   =   QDateTime::currentDateTime());


	QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
	new BCMgr(dlg.currentUser(), this);


	pageMainBook = new PageMainBook(this);
	stackeWidget->addWidget(pageMainBook);

	pageLearn = new PageLearn(this); 
	stackeWidget->addWidget(pageLearn);

	asMgr = new AnalyticStatisticsMgr;

	connect(pageMainBook, SIGNAL(slearn()), this, SLOT(learn()));
	connect(pageLearn, SIGNAL(goback()), this, SLOT(goback()));
	connect(pageMainBook, SIGNAL(uptoolbarState()), this, SLOT(uptoolbarState()));
	connect(pageMainBook, SIGNAL(upASInfo()), this, SLOT(upASInfo()));

	
	DBG(qDebug() << "耗时" << start.msecsTo(QDateTime::currentDateTime()));
	QApplication::restoreOverrideCursor();
	bShowDlgRemind = true;

	connect(learnAct, SIGNAL(triggered()),pageMainBook, SLOT(learn()));
	connect(delAct, SIGNAL(triggered()),pageMainBook, SLOT(del()));
	connect(downAct, SIGNAL(triggered()),pageMainBook, SLOT(down()));
	connect(editBookAct, SIGNAL(triggered()),pageMainBook, SLOT(editBook()));

	connect(addCategoryAct, SIGNAL(triggered()),pageMainBook, SLOT(addCategory()));
	connect(delCategoryAct, SIGNAL(triggered()),pageMainBook, SLOT(delCategory()));
	connect(modifyCategoryAct, SIGNAL(triggered()),pageMainBook, SLOT(modifyCategory()));
	connect(addBookAct, SIGNAL(triggered()),pageMainBook, SLOT(addBook()));
	//connect(searchAct, SIGNAL(triggered()),pageMainBook, SLOT(search()));

	uptoolbarState();
	upASInfo();

	NetworkProxyMgr::inst()->UpdateProxyFromIni();

}

void LIWord0030::changeUser()
{
	disconnect(pageMainBook, SIGNAL(upASInfo()), this, SLOT(upASInfo()));
	disconnect(pageMainBook, SIGNAL(uptoolbarState()), this, SLOT(uptoolbarState()));

	disconnect(learnAct, SIGNAL(triggered()),pageMainBook, SLOT(learn()));
	disconnect(delAct, SIGNAL(triggered()),pageMainBook, SLOT(del()));
	disconnect(downAct, SIGNAL(triggered()),pageMainBook, SLOT(down()));
	disconnect(editBookAct, SIGNAL(triggered()),pageMainBook, SLOT(editBook()));

	disconnect(addCategoryAct, SIGNAL(triggered()),pageMainBook, SLOT(addCategory()));
	disconnect(delCategoryAct, SIGNAL(triggered()),pageMainBook, SLOT(delCategory()));
	disconnect(modifyCategoryAct, SIGNAL(triggered()),pageMainBook, SLOT(modifyCategory()));
	disconnect(addBookAct, SIGNAL(triggered()),pageMainBook, SLOT(addBook()));
	//disconnect(searchAct, SIGNAL(triggered()),pageMainBook, SLOT(search()));

	disconnect(pageMainBook, SIGNAL(slearn()), this, SLOT(learn()));
	disconnect(pageLearn, SIGNAL(goback()), this, SLOT(goback()));

	

	stackeWidget->removeWidget(pageMainBook);
	stackeWidget->removeWidget(pageLearn);

	
	delete asMgr;
	delete pageMainBook;
	delete pageLearn;

	if (g_pbcmgr != NULL) 
	{
		delete g_pbcmgr;
		g_pbcmgr = NULL;
	}

	login(true);
}

void LIWord0030::learn()
{
	qDebug() << "LIWord0030::learn";
	menuBar()->setVisible(false);
	categoryToolBar->setVisible(false);
	bookToolBar->setVisible(false);
	statusBar()->setVisible(false);

	bShowDlgRemind = false;
	stackeWidget->setCurrentWidget(pageLearn);
	pageLearn->beginLearn();

}

void LIWord0030::goback()
{
	pageMainBook->endlearn();
	stackeWidget->setCurrentWidget(pageMainBook);
	bShowDlgRemind = true;
	menuBar()->setVisible(true);
	categoryToolBar->setVisible(true);
	bookToolBar->setVisible(true);
	statusBar()->setVisible(true);
}

void LIWord0030::createActions()
{
	
	studyMgrAct = new QAction(QIcon(Global::makeRelativeFilePath("/html/images/ico/studymgr.ico")),tr("学习管理(&S)..."), this);
	studyMgrAct->setStatusTip(tr("学习中的课程管理."));
	connect(studyMgrAct, SIGNAL(triggered()), this, SLOT(studyMgr()));
	
	optionAction = new QAction(/*QIcon(Global::makeRelativeFilePath("/html/images/ico/tools.ico")), */tr("选项(&O)..."), this);
	optionAction->setStatusTip(tr("调整一些可选的工作参数."));
	connect(optionAction, SIGNAL(triggered()), this, SLOT(option()));

	exitAct = new QAction(/*QIcon(Global::makeRelativeFilePath("/html/images/ico/shut_down.ico")), */tr("退出(&X)"), this);
	
	connect(exitAct, SIGNAL(triggered()), qApp, SLOT(quit()));

	changeUserAct = new QAction(/*QIcon(Global::makeRelativeFilePath("/html/images/ico/users.ico")),*/ tr("切换用户(&C)..."), this);
	changeUserAct->setStatusTip(tr("换一个用户登录."));
	connect(changeUserAct, SIGNAL(triggered()),
		this, SLOT(changeUser()));

	importStudyRecordAct = new QAction(/*QIcon(Global::makeRelativeFilePath("/html/images/ico/upload.ico")), */tr("导入学习记录(&I)..."), this);
	importStudyRecordAct->setStatusTip(tr("导入低版本的学习记录."));
	connect(importStudyRecordAct, SIGNAL(triggered()),
		this, SLOT(importStudyRecord()));

	ASAct = new QAction(QIcon(Global::makeRelativeFilePath("/html/images/ico/chart_up.ico")), "统计分析(&S)", this);
	connect(ASAct, SIGNAL(triggered()),
		this, SLOT(AS()));
	
	//helpAct = new QAction(QIcon(Global::makeRelativeFilePath("/html/images/ico/help.ico")), tr("帮助(&H)"), this);
	helpAct = new QAction(tr("帮助(&H)"), this);
	connect(helpAct, SIGNAL(triggered()),
		this, SLOT(help()));

	//donatAct = new QAction(QIcon(Global::makeRelativeFilePath("/html/images/ico/donation.ico")), tr("捐助我们(&D)"), this);
	donatAct = new QAction(tr("捐助我们(&D)"), this);
	connect(donatAct, SIGNAL(triggered()),
		this, SLOT(donat()));

	aboutAct = new QAction(tr("关于(&A)"), this);
	connect(aboutAct, SIGNAL(triggered()),
		this, SLOT(about()));


	// 词库
	learnAct = new QAction(QIcon(Global::makeRelativeFilePath("/html/images/ico/learn.ico")), tr("学习(&S)"), this);
	learnAct->setStatusTip(tr("学习选中的词库."));
	delAct = new QAction(QIcon(Global::makeRelativeFilePath("/html/images/ico/delete.ico")), tr("删除(&R)"), this);
	delAct->setStatusTip(tr("删除选中的词库."));
	downAct = new QAction(QIcon(Global::makeRelativeFilePath("/html/images/ico/download.ico")), tr("下载(&D)"), this);
	downAct->setStatusTip(tr("下载安装选中的词库."));
	editBookAct = new QAction(QIcon(Global::makeRelativeFilePath("/html/images/ico/pencil.ico")), tr("编辑(&E)"), this);
	editBookAct->setStatusTip(tr("编辑制作选中的词库(可导入第三方词库)."));

	// 分类
 	addCategoryAct = new QAction(QIcon(Global::makeRelativeFilePath("/html/images/ico/add.ico")), tr("添加分类(&A)"), this);
	addCategoryAct->setStatusTip(tr("在选中的分类下添加一个子分类."));

 	delCategoryAct = new QAction(QIcon(Global::makeRelativeFilePath("/html/images/ico/delete.ico")), tr("删除分类(&R)"), this);
	delCategoryAct->setStatusTip(tr("删除选中的分类."));
	
 	modifyCategoryAct  = new QAction(QIcon(Global::makeRelativeFilePath("/html/images/ico/edit2.ico")), tr("修改分类名(&E)"), this);
	modifyCategoryAct->setStatusTip(tr("修改选中的分类名称."));
	
 	addBookAct  = new QAction(QIcon(Global::makeRelativeFilePath("/html/images/ico/add2.ico")), tr("添加词库(&E)"), this);
	addBookAct->setStatusTip(tr("在选中的分类下添加一个空词库."));

	
// 	searchAct  = new QAction(QIcon(Global::makeRelativeFilePath("/html/images/ico/search.ico")), tr("搜索(&S)"), this);

	learnAct->setEnabled(false);
	delAct->setEnabled(false);
	downAct->setEnabled(false);
	editBookAct->setEnabled(false);
	studyMgrAct->setEnabled(false);

	addCategoryAct->setEnabled(false);
	delCategoryAct->setEnabled(false);
	modifyCategoryAct->setEnabled(false);
	addBookAct->setEnabled(false);


	restoreAction = new QAction(tr("显示"), this);
	connect(restoreAction, SIGNAL(triggered()), this, SLOT(showNormal()));

	quitAction = new QAction(tr("退出"), this);
	connect(quitAction, SIGNAL(triggered()), this, SLOT(quit()));

}
void LIWord0030::quit()
{
	if (stackeWidget->currentWidget() == pageLearn)
	{
		QMessageBox::question(this, "提示信息", "请先退出学习状态.");
		return;
	}
	 qApp->quit();
}
void LIWord0030::createMenus()
{
	fileMenu = menuBar()->addMenu("文件(&F)");
	fileMenu->addAction(changeUserAct);
	fileMenu->addSeparator();
	fileMenu->addAction(exitAct);

	bookMenu = menuBar()->addMenu("词库(&B)");
	bookMenu->addAction(learnAct);
	bookMenu->addAction(delAct);
	bookMenu->addAction(downAct);
	bookMenu->addAction(editBookAct);
	bookMenu->addSeparator();
	bookMenu->addAction(addCategoryAct);
	bookMenu->addAction(delCategoryAct);
	bookMenu->addAction(modifyCategoryAct);
	bookMenu->addAction(addBookAct);

	bookTableMenu = new QMenu("词库列表");
	bookTableMenu->addAction(learnAct);
	bookTableMenu->addAction(delAct);
	bookTableMenu->addAction(downAct);
	bookTableMenu->addAction(editBookAct);
	bookTableMenu->addAction(studyMgrAct);

	cateTreeMenu = new QMenu("分类树");
	cateTreeMenu->addAction(addCategoryAct);
	cateTreeMenu->addAction(delCategoryAct);
	cateTreeMenu->addAction(modifyCategoryAct);
	cateTreeMenu->addAction(addBookAct);

	toolsMenu = menuBar()->addMenu("工具(&T)");
	toolsMenu->addAction(importStudyRecordAct);
	toolsMenu->addAction(studyMgrAct);
	toolsMenu->addAction(optionAction);
	toolsMenu->addAction(ASAct);

	helpMenu = menuBar()->addMenu("帮助(&H)");
	helpMenu->addAction(helpAct);
	helpMenu->addAction(donatAct);
	helpMenu->addAction(aboutAct);


	


	

}

//学习管理
void LIWord0030::studyMgr()
{
	if (pageMainBook->getSelSize() == 0)
	{
		pageMainBook->selAll();
	}
	bShowDlgRemind = false;

	DlgStudyMgr3 dlgStudyMgr(this);
	dlgStudyMgr.exec();

	pageMainBook->update();

	bShowDlgRemind = true;


}

void LIWord0030::option()
{
	bShowDlgRemind = false;
	DlgSystemOption dlgSystemOption(this);
	dlgSystemOption.exec();
	pageMainBook->update();
	bShowDlgRemind = true;

}
void LIWord0030::topShow()
{
	activateWindow();
	setFocus( Qt::OtherFocusReason );
	show();
	setWindowState( Qt::WindowActive );
	update();
	repaint();
	setVisible( true );
	raise();
}

void  LIWord0030::vMessageReceivedFromOtherInst( const QString &  msg )
{
	if (msg == "RaiseWindow")
	{
		topShow();
	}
}

void LIWord0030::importStudyRecord()
{
	DlgImportStudyRecord dlg(this);
	dlg.exec();
	pageMainBook->update();
}


void LIWord0030::checkResult(bool bHaveNewVer)
{
	if (bHaveNewVer && pageMainBook == stackeWidget->currentWidget())
	{
		DlgUpdate dlg(this);
		dlg.setHtml(m_um.updateNote());
		if (QDialog::Accepted == dlg.exec())
		{
			m_um.applyNewVer();
			qApp->quit();
		}
	}

}

void LIWord0030::help()
{
	QDesktopServices::openUrl(QUrl("http://bbs.langisle.com/"));
}

void LIWord0030::donat()
{
	QDesktopServices::openUrl(QUrl("http://www.langisle.com/donation.html"));
}

void LIWord0030::about()
{


		QString translatedTextAboutQtCaption;
// 		translatedTextAboutQtCaption = QMessageBox::tr(
// 			"<h3>About Qt</h3>"
// 			"<p>This program uses Qt version %1.</p>"
// 			).arg(QLatin1String(QT_VERSION_STR));
		translatedTextAboutQtCaption = QMessageBox::tr("<h3>语言岛智能记单词 " VER "</h3>");
		QString translatedTextAboutQtText;
		translatedTextAboutQtText = QMessageBox::tr(
			"<p>Copyright (C) 2010-2011 语言岛工作室</p>"
			"<p><a href='http://www.langisle.com'>语言岛主页</a></p>"
			"<p><a href='http://bbs.langisle.com'>语言岛论坛</a></p>"
			);
// 		translatedTextAboutQtText = QMessageBox::tr(
// 			"<p>Qt is a C++ toolkit for cross-platform application "
// 			"development.</p>"
// 			"<p>Qt provides single-source portability across MS&nbsp;Windows, "
// 			"Mac&nbsp;OS&nbsp;X, Linux, and all major commercial Unix variants. "
// 			"Qt is also available for embedded devices as Qt for Embedded Linux "
// 			"and Qt for Windows CE.</p>"
// 			"<p>Qt is available under three different licensing options designed "
// 			"to accommodate the needs of our various users.</p>"
// 			"<p>Qt licensed under our commercial license agreement is appropriate "
// 			"for development of proprietary/commercial software where you do not "
// 			"want to share any source code with third parties or otherwise cannot "
// 			"comply with the terms of the GNU LGPL version 2.1 or GNU GPL version "
// 			"3.0.</p>"
// 			"<p>Qt licensed under the GNU LGPL version 2.1 is appropriate for the "
// 			"development of Qt applications (proprietary or open source) provided "
// 			"you can comply with the terms and conditions of the GNU LGPL version "
// 			"2.1.</p>"
// 			"<p>Qt licensed under the GNU General Public License version 3.0 is "
// 			"appropriate for the development of Qt applications where you wish to "
// 			"use such applications in combination with software subject to the "
// 			"terms of the GNU GPL version 3.0 or where you are otherwise willing "
// 			"to comply with the terms of the GNU GPL version 3.0.</p>"
// 			"<p>Please see <a href=\"http://qt.nokia.com/products/licensing\">qt.nokia.com/products/licensing</a> "
// 			"for an overview of Qt licensing.</p>"
// 			"<p>Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).</p>"
// 			"<p>Qt is a Nokia product. See <a href=\"http://qt.nokia.com/\">qt.nokia.com</a> "
// 			"for more information.</p>"
// 			);
		QMessageBox *msgBox = new QMessageBox(this);
		msgBox->setAttribute(Qt::WA_DeleteOnClose);
		msgBox->setWindowTitle("关于语言岛智能记单词");
		msgBox->setText(translatedTextAboutQtCaption);
		msgBox->setInformativeText(translatedTextAboutQtText);

		QPixmap pm(":/LIWord0030.ico");
		if (!pm.isNull())
			msgBox->setIconPixmap(pm);

		msgBox->exec();

}


void LIWord0030::userStatics()
{
	int count = GlobalVal::s_pcfg->getValue("Other/statics", 0).toInt();
	QWebView *view = new QWebView(this);
#if defined(Q_WS_WIN)
	view->load(QUrl(QString("http://www.langisle.com/statics.html?win_%1_%2").arg(VER).arg(count + 1)));
#endif
#if defined(Q_WS_X11)
	view->load(QUrl(QString("http://www.langisle.com/statics.html?linux_%1_%2").arg(VER).arg(count + 1)));
#endif
	
	view->hide();
	GlobalVal::s_pcfg->setValue("Other/statics", count + 1);
}


void LIWord0030::closeEvent(QCloseEvent *event)
{
	if (stackeWidget->currentWidget() == pageLearn)
	{
		QMessageBox::question(this, "提示信息", "你现在处于学习状态,请先\"返回\"后再退出.");
		event->ignore();
		return;
	}

	bool bETary = GlobalVal::s_pcfg->getValue("Main/enableTray", true).toBool() ? Qt::Checked : Qt::Unchecked;

	if (bETary && trayIcon->isVisible()) 
	{
		hide();
		event->ignore();
	}

// 	if (maybeSave()) {
// 		writeSettings();
// 		event->accept();
// 	} else {
// 		event->ignore();
// 	}
}

void LIWord0030::createToolBars()
{
	categoryToolBar = addToolBar("分类");
	categoryToolBar->addAction(addCategoryAct);
	categoryToolBar->addAction(addBookAct);
	categoryToolBar->addAction(modifyCategoryAct);
	categoryToolBar->addAction(delCategoryAct);
	//categoryToolBar->addAction(searchAct);
	

	bookToolBar = addToolBar("词库");
	bookToolBar->addAction(downAct);
	bookToolBar->addAction(learnAct);
	bookToolBar->addAction(editBookAct);
	bookToolBar->addAction(delAct);
	bookToolBar->addAction(studyMgrAct);
	bookToolBar->addAction(ASAct);
	
	

}

void  LIWord0030::uptoolbarState()
{
	
	learnAct->setEnabled(g_pbcmgr->isShowStudyButton() && pageMainBook->getBookCount() > 0);
	delAct->setEnabled(g_pbcmgr->isShowDelButton());
	downAct->setEnabled(g_pbcmgr->isShowDownButton());
	editBookAct->setEnabled(g_pbcmgr->isShowEdit());
    studyMgrAct->setEnabled(g_pbcmgr->isShowStudyMgrButton() && pageMainBook->getBookCount() > 0);

	
	addCategoryAct->setEnabled(g_pbcmgr->isCanAddUCategory());
	delCategoryAct->setEnabled(g_pbcmgr->isCanDelCategory());
	modifyCategoryAct->setEnabled(g_pbcmgr->isCanModifyUCategory());
	addBookAct->setEnabled(g_pbcmgr->isCanAddUBook());
	//searchAct->setEnabled(g_pbcmgr->isCanAddUCategory());
}

void LIWord0030::createStatusBar()
{ 
	//statusBar()->showMessage(tr("Ready"));
	statusLabel = new QLabel();
	statusBar()->addWidget(statusLabel);
	//statusLabel->setText("从学习开始至今的<b>106</b>天中,总共学习单词<b>188</b>个,掌握单词<b>24</b>个. 平均每天学习<b>56</b>个,掌握<b>10</b>个 总体学习进度:<b>16%</b>");
	//statusBar()->addPermanentWidget(new QLabel("addPermanentWidget"));
	//connect(statusBar(), SIGNAL(messageChanged ( const QString &)), this,  SLOT(statusBarMessageChanged ( const QString &)));
}

void LIWord0030::statusBarMessageChanged ( const QString & message )
{
	if (message.isEmpty())
	{
		//statusBar()->showMessage(tr("Ready"));
	}
	DBG(qDebug() << "LIWord0030::statusBarMessageChanged" << message);
}



void LIWord0030::upASInfo()
{
	int days = asMgr->getUseDays();
	int scount = asMgr->getStudyCount();
	int kcount = asMgr->getKnowWellCount();
	QString info;
	if (1 == days)
	{
		info = QString("<b>%4</b>,今天共学习单词<b>%1</b>个,掌握单词<b>%2</b>个,总体学习进度:<b>%3%</b>")
			.arg(scount)
			.arg(kcount)
			.arg(QString::number(asMgr->getProgress(), 'f', 2))
			.arg(g_pbcmgr->getCurUserName())
			;

	}
	else
	{
		info = QString("<b>%7</b>,您在至今的<b>%1</b>天中,总共学习单词<b>%2</b>个,掌握单词<b>%3</b>个. 平均每天学习<b>%4</b>个,掌握<b>%5</b>个 总体学习进度:<b>%6%</b>")
			.arg(days)
			.arg(scount)
			.arg(kcount)
			.arg(days > 0 ? scount / days : 0)
			.arg(days > 0 ? kcount / days : 0)
			.arg(QString::number(asMgr->getProgress(), 'f', 2))
			.arg(g_pbcmgr->getCurUserName());
	}

				
	statusLabel->setText(info);
}

void LIWord0030::createTrayIcon()
{
	trayIconMenu = new QMenu(this);
// 	trayIconMenu->addAction(minimizeAction);
// 	trayIconMenu->addAction(maximizeAction);
	trayIconMenu->addAction(restoreAction);
	trayIconMenu->addSeparator();
	trayIconMenu->addAction(quitAction);

	trayIcon = new QSystemTrayIcon(this);
	trayIcon->setContextMenu(trayIconMenu);
}

void LIWord0030::trayActivated ( QSystemTrayIcon::ActivationReason reason )
{
	DBG(qDebug() << "LIWord0030::trayActivated" << reason);
	if (QSystemTrayIcon::DoubleClick == reason)
	{
		topShow();
	}
}

void LIWord0030::AS()
{

	DlgSA dlg(this);
	dlg.exec();
}
