#ifndef PICTUREMGR_H
#define PICTUREMGR_H

#include <QObject>

class PictureMgr : public QObject
{
	Q_OBJECT

public:
	PictureMgr(QObject *parent = 0);
	~PictureMgr();
	
	QString getPPath(const QString &word);
	void setPath(const QString &path)
	{
		m_path = path;
	}
	void reset();
private:
	QString m_path;
};

#endif // PICTUREMGR_H
