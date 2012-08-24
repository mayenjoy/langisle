#ifndef PAGESOADVANCED_H
#define PAGESOADVANCED_H

#include <QtGui>

class PageSOAdvanced : public QWidget
{
	Q_OBJECT

public:
	PageSOAdvanced(QWidget *parent);
	~PageSOAdvanced();

	void save();
public slots:
	void applyFP();
	void restoreFP();
private:
	QLineEdit *lineFP;
};

#endif // PAGESOADVANCED_H
