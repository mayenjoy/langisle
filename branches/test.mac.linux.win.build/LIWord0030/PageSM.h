#ifndef PAGESM_H
#define PAGESM_H

#include <QDialog>
#include <QModelIndex>
#include <QLabel>
#include <QItemSelection>

class StudyMgrBCDBModel;
class  LITableView;

class PageSM : public QDialog
{
	Q_OBJECT
public:
	PageSM(QWidget *parent,const QString &type );
	~PageSM();
	StudyMgrBCDBModel *getModel(){return model;};
	LITableView *getWordTable(){return wordTable;};

	QLabel *getLabelWordtotal(){return lableWordtotal;};

	void update();


public slots:

		//void tableDoubleClicked( const QModelIndex & index );
		//void	selectionChanged ( const QItemSelection & selected, const QItemSelection & deselected );
	    void	clicked ( const QModelIndex & index );
		void modifyStatus();

private:
	QPushButton *buttonModifyStatus;
	QLabel *lableWordtotal;


	StudyMgrBCDBModel *model;
	LITableView *wordTable;
	int bid;
	QString type;

};
#endif 
