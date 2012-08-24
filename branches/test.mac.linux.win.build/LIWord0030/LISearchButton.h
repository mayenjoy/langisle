#ifndef LISEARCHBUTTON_H
#define LISEARCHBUTTON_H

#include <QAbstractButton>

class LISearchButton : public QAbstractButton
{
	Q_OBJECT

public:
	LISearchButton(QWidget *parent = 0);
	~LISearchButton();
	void paintEvent(QPaintEvent *event);

private:
protected:
	void mousePressEvent(QMouseEvent *event);	
};

#endif // LISEARCHBUTTON_H
