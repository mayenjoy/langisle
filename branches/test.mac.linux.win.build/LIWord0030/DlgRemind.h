#ifndef DLGREMIND_H
#define DLGREMIND_H

#include <QDialog>
class QWebView;
class RemindMgr;
class DlgRemind : public QDialog
{
	Q_OBJECT

public:
	DlgRemind(QWidget *parent, RemindMgr *rmgr);
	~DlgRemind();
	void updateWeb(int nNeedReviewCount);
public slots:
	void Study();
	void Again();
	void Cancel();
	void topShow();
private:
	QWebView *web;
	RemindMgr *m_rmgr;
};

#endif // DLGREMIND_H
