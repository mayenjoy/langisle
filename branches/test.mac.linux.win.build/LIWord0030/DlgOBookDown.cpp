#include "stdafx.h"
#include "DlgOBookDown.h"
#include "config.h"
DlgOBookDown::DlgOBookDown(int bid, QWidget *parent)
	: QDialog(parent), m_bid(bid)
{
	QVBoxLayout *mainLaylout = new QVBoxLayout;
	setLayout(mainLaylout);

	setWindowTitle("下载官方课程");

	labelCurrentAction =  new QLabel;
	progressBar = new QProgressBar;

	QHBoxLayout *buttomLayout = new QHBoxLayout;
	cancelButton = createButton("取消", SLOT(cancel()));
	buttomLayout->addStretch();
	buttomLayout->addWidget(cancelButton);

	mainLaylout->addWidget(labelCurrentAction);	
	mainLaylout->addWidget(progressBar);
	mainLaylout->addStretch();
	mainLaylout->addLayout(buttomLayout);

	setMinimumWidth(360);

	Global::createRelExeDir(TEMP_DIR);

	downer = new LIFileDownload;

	connect(downer, SIGNAL(downloadProgress(qint64,qint64)),
		this, SLOT(updateProgress(qint64,qint64)));
	connect(downer, SIGNAL(downloaded(int)),
		this, SLOT(finished(int)));

	QTimer::singleShot(0, this, SLOT(start()));

}

DlgOBookDown::~DlgOBookDown()
{	downer->cancel();
	delete downer;
}

void DlgOBookDown::updateProgress ( qint64 current, qint64 total )
{
	progressBar->setRange(0, total);
	progressBar->setValue(current);
}



void DlgOBookDown::finished(int result)
{
	if (result != QNetworkReply::NoError)
	{
		QString info;
		if (result == QNetworkReply::OperationCanceledError)
		{
			info = "下载课程失败, 用户中止.";
		}
		else
		{
			info = QString("下载课程失败, 错误代码[%1],").arg(result);
			QMessageBox::warning(this, "错误", info, QMessageBox::Yes);
		}
		reject();
		
		return;
	}


	QString obookPath = Global::makePathStr(O_BOOK_PATH);
	ZipFile zf;
	zf.extract(obPath, obookPath);
	accept();
}

void DlgOBookDown::start()
{
	QString url = QString(OBOOK_DOWN_URL "%1.zip").arg(m_bid);
	labelCurrentAction->setText(QString("正在下载%1").arg(url));
	obPath = Global::makePathStr(QString(TEMP_DIR "/%1.zip").arg(m_bid));
	
	downer->download(url, obPath);
}


QPushButton *DlgOBookDown::createButton(const QString &text, const char *member)
{
	QPushButton *button = new QPushButton(text);
	connect(button, SIGNAL(clicked()), this, member);
	return button;
}

void DlgOBookDown::cancel()
{
	reject();
}

