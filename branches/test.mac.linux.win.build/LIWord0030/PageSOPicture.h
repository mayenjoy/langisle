#ifndef PAGESOPICTURE_H
#define PAGESOPICTURE_H

#include <QtGui>

class PageSOPicture : public QWidget
{
	Q_OBJECT

public:
	PageSOPicture(QWidget *parent);
	~PageSOPicture();
	void save();
private slots:
	void browerPath();
	void showPic();
private:
	QLineEdit *editPath;
	QLabel *picLabel;
	QLineEdit *lineTestWord;
};

#endif // PAGESOPICTURE_H
