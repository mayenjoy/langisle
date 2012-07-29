#ifndef DLGOBOOKDOWN_H
#define DLGOBOOKDOWN_H

#include <QtGui>
#include "../comm/LIFileDownload.h"
class DlgOBookDown : public QDialog
{
	Q_OBJECT

public:
	DlgOBookDown(int bid, QWidget *parent = 0);
	~DlgOBookDown();

	
	
public slots:
	void start();
	void updateProgress ( qint64 current, qint64 total );
	void finished(int result);
	void cancel();
private:
	QPushButton *createButton(const QString &text, const char *member);
private:
	LIFileDownload *downer;

	QLabel *labelCurrentAction;
	QProgressBar *progressBar;
	QPushButton *cancelButton;

	QString obPath;
	int m_bid;
};

#endif // DLGOBOOKDOWN_H
