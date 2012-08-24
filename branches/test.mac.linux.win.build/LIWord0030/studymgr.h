#ifndef STUDYMGR_H
#define STUDYMGR_H

#include <QObject>
#include <QVariant>
class UDataDB;
class StudyMgr : public QObject
{
	Q_OBJECT

public:
	StudyMgr(QObject *parent);
	~StudyMgr();
public slots:
	QList<QVariant> getTest();
	void logOut(QString slog);


private:
	UDataDB *pudb;
};

#endif // STUDYMGR_H
