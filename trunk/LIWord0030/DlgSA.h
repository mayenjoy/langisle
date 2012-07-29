#ifndef DLGSA_H
#define DLGSA_H

#include <QDialog>
#include "qchartviewer.h"
class DlgSA : public QDialog
{
	Q_OBJECT

public:
	DlgSA(QWidget *parent);
	~DlgSA();

private:
	QChartViewer *viewer;
private slots:
	void okClicked();
protected:
	virtual void	resizeEvent ( QResizeEvent * event );
};

#endif // DLGSA_H
