#ifndef DLGSTUDYMGR2_H
#define DLGSTUDYMGR2_H

#include <QDialog>
#include <QWebView>
#include "studymgr.h"
class DlgStudyMgr2 : public QDialog
{
	Q_OBJECT

public:
	DlgStudyMgr2(QWidget *parent = 0);
	~DlgStudyMgr2();
public slots:
		void populateJavaScriptWindowObject();
private:
	QWebView *web;
	StudyMgr *sm;
};

#endif // DLGSTUDYMGR2_H
