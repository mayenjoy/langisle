#ifndef LITREEWIDGET_H
#define LITREEWIDGET_H

#include <QTreeWidget>
// �ڵ�ѡ��,�ڽڵ�ˮƽ�������޷�ѡ�������
class LITreeWidget : public QTreeWidget
{
	Q_OBJECT

public:
	LITreeWidget(QWidget *parent);
	~LITreeWidget();

private:
	
protected:
	virtual void	resizeEvent ( QResizeEvent * event );
};

#endif // LITREEWIDGET_H
