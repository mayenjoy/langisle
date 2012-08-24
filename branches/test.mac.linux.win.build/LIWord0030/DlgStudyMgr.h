#ifndef DLGSTUDYMGR_H
#define DLGSTUDYMGR_H

#include <QDialog>
#include <QStackedWidget>

class PageSM;

class DlgStudyMgr : public QDialog
{
	Q_OBJECT

public:
	DlgStudyMgr(QWidget *parent);
	~DlgStudyMgr();

private slots:
	void okClicked();
	void cancelClicked();
	void update(int row);
private:
	PageSM *psma;
	PageSM *psml;
	PageSM *psmk;
	PageSM *psmli;
	PageSM *psmnl;
	QStackedWidget *sw;

};
#endif // DLGSTUDYMGR_H
