#ifndef PAGESADEFAULT_H
#define PAGESADEFAULT_H

#include <QFrame>
#include <QtGui>
class QChartViewer;
class PageSADefault : public QFrame
{
	Q_OBJECT

public:
	PageSADefault(QWidget *parent);
	~PageSADefault();

	void updateChartViewer();
	void testViewer();
	void updateData();
	void clear();
	void updateChart();
private:
	QChartViewer *viewer;
	QScrollArea *scrollArea;
	QCheckBox *swcountCheckBox;
	QCheckBox *snwcountCheckBox; 
	QCheckBox *rscountCheckBox;	
	QCheckBox *kncountCheckBox;	
	QCheckBox *sTimesCheckBox;

	QCheckBox *stwcountCheckBox;	
	QCheckBox *stnwcountCheckBox;

	QDateEdit *sdateEdit;
	QDateEdit *edateEdit;

	double *swc0Y;
	double *snwc0Y;
	double *rsc0Y;
	double *knc0Y;
	double *stw0Y;
	double *stnw0Y;
	double *sTimes0Y;

	int dSize;

	bool bswcount;
	bool bsnwcount;
	bool brscount;
	bool bkncount;
	bool bstwcount;
	bool bstnwcount;
	bool bsTimes0Y;

	double data1Start;
	double data1End;
private slots:
	void makeClick();
protected:
	virtual void	resizeEvent ( QResizeEvent * event );
};

#endif // PAGESADEFAULT_H
