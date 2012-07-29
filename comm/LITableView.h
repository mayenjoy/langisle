#ifndef LITABLEVIEW_H
#define LITABLEVIEW_H

#include <QTableView>

class LITableView : public QTableView
{
	Q_OBJECT

public:
	LITableView(QWidget *parent = 0);
	~LITableView();

	void selectAllRow();
protected:
	void leaveEvent ( QEvent * event );
	void mouseMoveEvent ( QMouseEvent * event);
	bool eventFilter (QObject* object,QEvent* event);
	void wheelEvent ( QWheelEvent * event );
	void updateRow(int row);
private:
	int n_orow;
};

#endif // LITABLEVIEW_H
