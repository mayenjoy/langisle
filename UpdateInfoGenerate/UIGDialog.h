#ifndef UIGDIALOG_H
#define UIGDIALOG_H

#include <QDialog>
#include <QWebView>
#include <QLineEdit>
#include <QTextEdit>
class UIGDialog : public QDialog
{
	Q_OBJECT

public:
	UIGDialog(QWidget *parent = NULL);
	~UIGDialog();

	QPushButton *createButton(const QString &text, const char *member);
private:
	QLineEdit *lineOutputDir;
	QTextEdit *textEditUpdateNotes;
	QLineEdit *lineDownurl;
	QLineEdit *lineEditVer;
	QWebView *webView;
private slots:
	void browse();	
	void generate();	
	void preview();
};

#endif // UIGDIALOG_H
