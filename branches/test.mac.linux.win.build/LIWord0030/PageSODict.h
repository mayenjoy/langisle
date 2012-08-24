#ifndef PAGESODICT_H
#define PAGESODICT_H

#include <QtGui/QtGui>

class PageSODict : public QWidget
{
	Q_OBJECT

public:
	PageSODict(QWidget *parent);
	~PageSODict();

	void save();
private slots:
	void add();
	void del();
	void up();
	void down();
	void itemSelectionChanged();
private:
	QTableWidget *tableWidget;
	QPushButton *upButton;
	QPushButton *downButton;
};

#endif // PAGESODICT_H



