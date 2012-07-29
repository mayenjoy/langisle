#ifndef UPDATEDIALOG_H
#define UPDATEDIALOG_H

#include <QtGui>
#include "../comm/LIFileDownload.h"
#include "../comm/ZipFile.h"
class UpdateDialog : public QDialog
{
	Q_OBJECT

public:
	UpdateDialog(QWidget *parent = NULL);
	~UpdateDialog();

	enum CurrentAct
	{
		caDown = 0,
		caUnzip = 1,
		caInstall = 2
	} ca;

	void start();
	void downNewVerApp();
	
	void install();

	


	QString appName;	// ������
	QString exeName;	// exe��,��:LIWord.exe
	QString updatedDir;	// ����Ŀ¼
	QString updateUrl;	// ��������
	QString newVer;	   // �°汾
	
public slots:
	void updateProgress ( qint64 current, qint64 total );
	void finished(int result);
	void cancel();
	void runApp(QString info);
private :
	bool copyDir(const QString source, const QString destination, const bool override = true);
	
private slots:
	void extract();
private:
	QLabel *labelCurrentAction;
	QProgressBar *progressBar;
	QPushButton *cancelButton;
	LIFileDownload downer;

	QString url;
	QString filePath;

	QString extDir;

	int fileCopyCount;
	volatile bool bStop;
	ZipFile zf;

protected:
	QPushButton *createButton(const QString &text, const char *member);


};

#endif // UPDATEDIALOG_H
