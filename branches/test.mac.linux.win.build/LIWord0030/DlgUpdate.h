#ifndef DLGUPDATE_H
#define DLGUPDATE_H

#include <QDialog>
#include <QWebView>
class DlgUpdate : public QDialog
{
	Q_OBJECT

public:
	DlgUpdate(QWidget *parent);
	~DlgUpdate();

	void setHtml(const QString &html);

private:
	QWebView *webView;
};

#endif // DLGUPDATE_H
