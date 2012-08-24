#ifndef TABBOOKDETAIL_H
#define TABBOOKDETAIL_H

#include <QWidget>
#include <QLabel>

class TabBookDetail : public QWidget
{
	Q_OBJECT

public:
	TabBookDetail(QWidget *parent = 0);
	~TabBookDetail();
public:
	void setBookName(const QString &bookName);
	void setCommentInfo(const QString &commentInfo);
	void setWordCountInfo(int wcount);
	void clear();
private:
	QLabel *m_bookName;
	QLabel *m_verInfo;
	QLabel *m_commentInfo;
	QLabel *m_wordCountInfo;
};

#endif // TABBOOKDETAIL_H
