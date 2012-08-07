#include "stdafx.h"
#include "studymgrstudystatusdelegate.h"
#include "studymgrmodel.h"

StudyMgrStudyStatusDelegate::StudyMgrStudyStatusDelegate(QObject *parent)
	: QItemDelegate(parent)
{
	
}

QWidget *StudyMgrStudyStatusDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option,
					  const QModelIndex &index) const
{
	QComboBox *cb = new QComboBox(parent);
	cb->setFrame(false);
	cb->addItem("δѧ");
	cb->addItem("������");
	return cb;
}

void StudyMgrStudyStatusDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
	QComboBox *cb = (QComboBox *)editor;
	int value = ((StudyMgrModel *)index.model())->record(index.row()).value("status").toInt();
	switch (value)
	{
	case 0:
	case 1:
		cb->setCurrentIndex(0);
		break;
	case 2:
		cb->setCurrentIndex(1);
	}
	cb->showPopup();


}
void StudyMgrStudyStatusDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
				  const QModelIndex &index) const
{
	
	QComboBox *cb = (QComboBox *)editor;
	if (cb->currentText().compare("δѧ") == 0)
	{
		((StudyMgrModel *)model)->setNotStudyStatus(index);
	}
	if (cb->currentText().compare("������") == 0)
	{
		((StudyMgrModel *)model)->setKnowStatus(index);
	}
	
}

void StudyMgrStudyStatusDelegate::updateEditorGeometry(QWidget *editor,
						  const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	editor->setGeometry(option.rect);
}
