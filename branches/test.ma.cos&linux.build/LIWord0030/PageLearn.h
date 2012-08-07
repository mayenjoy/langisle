#ifndef PAGEDEFAULTLEARN_H
#define PAGEDEFAULTLEARN_H

#include <QWidget>
#include <QWebView>
#include "LearnMgr.h"
class PageLearn : public QWidget
{
	Q_OBJECT

public:
	PageLearn(QWidget *parent);
	~PageLearn();

	void beginLearn();
	void endLearn();
signals:
	void goback();
public slots:
	void mgoback();
	void populateJavaScriptWindowObject();
private:
	QWebView *webView;
	LearnMgr *learnMgr;
};

#endif // PAGEDEFAULTLEARN_H
