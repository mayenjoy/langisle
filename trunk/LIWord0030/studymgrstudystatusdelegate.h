#ifndef STUDYMGRSTUDYSTATUSDELEGATE_H
#define STUDYMGRSTUDYSTATUSDELEGATE_H

#include <QItemDelegate>

class StudyMgrStudyStatusDelegate : public QItemDelegate
{
	Q_OBJECT

public:
	StudyMgrStudyStatusDelegate(QObject *parent = 0);
public:

	QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
		const QModelIndex &index) const;

	void setEditorData(QWidget *editor, const QModelIndex &index) const;
	void setModelData(QWidget *editor, QAbstractItemModel *model,
		const QModelIndex &index) const;

	void updateEditorGeometry(QWidget *editor,
		const QStyleOptionViewItem &option, const QModelIndex &index) const;
protected:
	virtual void drawFocus ( QPainter * painter, const QStyleOptionViewItem & option, const QRect & rect ) const{};

private:
	
};

#endif // STUDYMGRSTUDYSTATUSDELEGATE_H
