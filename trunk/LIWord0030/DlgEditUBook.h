#ifndef DLGEDITUBOOK_H
#define DLGEDITUBOOK_H

#include <QDialog>
#include <QModelIndex>
#include <QItemSelection>
class BookDB;
class WordViewBCDBModel;
class  LITableView;
class DlgEditUBook : public QDialog
{
	Q_OBJECT

public:
	DlgEditUBook(QWidget *parent);
	~DlgEditUBook();
public slots:
	void tableDoubleClicked( const QModelIndex & index );

	void importData();
	void exportData();
	void add();
	void del();
	void edit();

// 	void currentRowChanged (const QModelIndex & current, const QModelIndex & previous);
// 	void	clicked ( const QModelIndex & index );
	void	selectionChanged ( const QItemSelection & selected, const QItemSelection & deselected );
	BookDB *getbdb() {return bdb;};
private:
	QPushButton *buttonImport;
	QPushButton *buttonExport;	

	QPushButton *buttonAdd;		
	QPushButton *buttonDel;		
	QPushButton *buttonEdit;	
	QPushButton *buttonCancel;

	BookDB *bdb;
	WordViewBCDBModel *model;
	LITableView *wordTable;
	int bid;
	
};

#endif // DLGEDITUBOOK_H
