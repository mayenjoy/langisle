#ifndef DLGEDITWORD_H
#define DLGEDITWORD_H

#include <QtGui>

enum DlgEditWordType
{
	DEAdd,
	DEEdit
};
class BookDB;
class DlgEditWord : public QDialog
{
	Q_OBJECT

public:
	DlgEditWord(QWidget *parent, BookDB *bdb, DlgEditWordType type);
	~DlgEditWord();


	void setWord(const QString &word)
	{
		wordEdit->setText(word);
	}
	void setMean(const QString &mean)
	{
		meanEdit->setPlainText(mean);
	}

	QString word() {return wordEdit->text();};
	QString mean() {return meanEdit->document()->toPlainText();};

	void setwid(int wid) {mwid = wid;};
public slots:
	void ok();
private:
	QLineEdit *wordEdit;
	QTextEdit *meanEdit;

	BookDB *mbdb;
	DlgEditWordType mtype;
	int mwid; // ”√”⁄ÃÊªª
};

#endif // DLGEDITWORD_H
