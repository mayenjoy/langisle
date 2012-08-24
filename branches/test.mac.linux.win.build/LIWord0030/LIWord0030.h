#ifndef LIWORD0030_H
#define LIWORD0030_H

#include <QtGui/QMainWindow>
#include <QStackedWidget>
#include "BCMgr.h"
#include "PageMainBook.h"
#include "PageLearn.h"
#include "../comm/UpdateMgr.h"
class AnalyticStatisticsMgr;
class LIWord0030 : public QMainWindow
{
	Q_OBJECT

public:
	LIWord0030(QWidget *parent = 0, Qt::WFlags flags = 0);
	~LIWord0030();


	void userStatics();
	PageMainBook * getPageMainBook() {return pageMainBook;};
	bool isShowDlgRemind() {return bShowDlgRemind;};

	QMenu *getBookTableMenu() {return bookTableMenu;};
	QMenu *getCateTreeMenu() {return cateTreeMenu;};
public slots:
	void learn();
	void display();
	void goback();
	void login(bool bDisableAutoLogin = false);
	void changeUser();
	void importStudyRecord();
	void AS();
	void help();
	void donat();
	void about();
	void topShow();
	void quit();

	void uptoolbarState();
	void upASInfo();
private slots:
	void studyMgr();
	void option();
	void  vMessageReceivedFromOtherInst( const QString &  msg );
	void checkResult(bool bHaveNewVer);
	void statusBarMessageChanged ( const QString & message );

	void	trayActivated ( QSystemTrayIcon::ActivationReason reason );
private:
	void createActions();
	void createMenus();
	void createToolBars();
	void createStatusBar();
	void createTrayIcon();
protected:
	void closeEvent(QCloseEvent *event);
protected:
	QStackedWidget *stackeWidget;
	PageMainBook *pageMainBook;
	PageLearn *pageLearn;

	
    

	
	QAction *exitAct;
	QAction *changeUserAct;
	
	QAction *helpAct;
	QAction *donatAct;
	QAction *aboutAct;

	// 工具
	QAction *studyMgrAct; // 学习管理
	QAction *optionAction;
	QAction *importStudyRecordAct;
	QAction *ASAct;	// 统计分析

	// 词库
	QAction *learnAct;
	QAction *delAct;
	QAction *downAct;
	QAction *editBookAct;
	// 分类
	QAction *addCategoryAct;
	QAction *delCategoryAct;
	QAction *modifyCategoryAct;
	QAction *addBookAct;
	QAction *searchAct;

	// 拖盘
	QAction *quitAction;
	QAction *restoreAction;

	QMenu *fileMenu;
	QMenu *bookMenu;
	QMenu *bookTableMenu;
	QMenu *cateTreeMenu;
	QMenu *toolsMenu;
	QMenu *helpMenu;

	QToolBar *categoryToolBar;
	QToolBar *bookToolBar;
	UpdateMgr m_um;
	bool bShowDlgRemind;
	QLabel *statusLabel;

	QSystemTrayIcon *trayIcon;
	QMenu *trayIconMenu;
protected:
	AnalyticStatisticsMgr *asMgr;
};

#endif // LIWORD0030_H
