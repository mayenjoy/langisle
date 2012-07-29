#ifndef STUDYMGRMODEL_H
#define STUDYMGRMODEL_H

#include "LISqlQueryModel.h"
#include <QTableView>
class ExportWordBase;
class StudyMgrModel : public LISqlQueryModel
{
	Q_OBJECT

public:
	StudyMgrModel(QObject *parent, QTableView *view);
	~StudyMgrModel();

	static void initC2hmap();
	static void initN2size();

	void updateHeaderName();

	Qt::ItemFlags flags(const QModelIndex &index) const;
	QVariant data ( const QModelIndex & item, int role)  const;
	bool setData(const QModelIndex &index, const QVariant &value, int role);
	void setCheck(const QModelIndex &index, const QVariant &value1);
	void setCheckReverse(const QModelIndex &index); // 反向选择
	QVariant getCheck(const QModelIndex &index) const;
	void setNotStudyStatus(const QModelIndex &index);// 设置为未学
	void setKnowStatus(const QModelIndex &index); // 设置为掌握

	int getSizeSection(const QString &sname);
	int getSelCount();
	void MakeExportWord(ExportWordBase *ewb);
signals:
	void updateCheck();
public slots:
	void allButtonClick();
	void notStudyClick();
	void studingClick();
	void masterClick();
private:
	QString getHeaderNameByColname(QString colname);
	
	QTableView *view;
	bool bAllSel;
	bool bnotStudySel;
	bool bstudingSel;
	bool bmasterSel;

};

#endif // STUDYMGRMODEL_H
