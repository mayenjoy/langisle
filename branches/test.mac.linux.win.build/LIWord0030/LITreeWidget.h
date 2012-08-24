#ifndef LITREEWIDGET_H
#define LITREEWIDGET_H

#include <QTreeWidget>
// 节点选择,在节点水平区域外无法选择的问题
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
