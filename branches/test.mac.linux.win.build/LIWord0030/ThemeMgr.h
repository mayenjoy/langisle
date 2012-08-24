#ifndef THEMEMGR_H
#define THEMEMGR_H

#include <QObject>
#include <QMap>

typedef QMap<QString, QVariant> TInfo;
class ThemeMgr : public QObject
{
	Q_OBJECT

public:
	ThemeMgr(QObject *parent = 0);
	~ThemeMgr();

	QList<QVariant> getThemeList();
	QString getCurrentThemeHtmlPath(const QString &id);
	void reset();
private:
	QList<QVariant> m_tl;
};

#endif // THEMEMGR_H
