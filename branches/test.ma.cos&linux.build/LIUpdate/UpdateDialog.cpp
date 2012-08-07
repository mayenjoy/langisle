#include "stdafx.h"
#include "UpdateDialog.h"


UpdateDialog::UpdateDialog(QWidget *parent)
	: QDialog(parent)
{
 

	QVBoxLayout *mainLaylout = new QVBoxLayout;
	setLayout(mainLaylout);

	labelCurrentAction =  new QLabel("��������"); 
	progressBar = new QProgressBar;

	QHBoxLayout *buttomLayout = new QHBoxLayout;
	cancelButton = createButton("ȡ��", SLOT(cancel()));
	buttomLayout->addStretch();
	buttomLayout->addWidget(cancelButton);

	mainLaylout->addWidget(labelCurrentAction);	
	mainLaylout->addWidget(progressBar);
	mainLaylout->addStretch();
	mainLaylout->addLayout(buttomLayout);

	setMinimumWidth(320);

	progressBar->setRange(0, 100);

	fileCopyCount = 0;
	bStop = false;

	Global::createRelExeDir("/temp");

	connect(&zf, SIGNAL(currentProgress(qint64,qint64)),
		this, SLOT(updateProgress(qint64,qint64)));
	connect(&zf, SIGNAL(finished(int)),
		this, SLOT(finished(int)));

}

UpdateDialog::~UpdateDialog()
{

}


void UpdateDialog::start()
{
	setWindowTitle(appName + "���³���");
	

	url = updateUrl;
	filePath = Global::makePathStr("/temp/" + QFileInfo(url).fileName());
	extDir = Global::makePathStr("/temp/" + QFileInfo(filePath).completeBaseName());

	downNewVerApp();


}

void UpdateDialog::downNewVerApp()
{
	progressBar->setValue(0);
	ca = caDown;
	connect(&downer, SIGNAL(downloadProgress(qint64,qint64)),
		this, SLOT(updateProgress(qint64,qint64)));
	connect(&downer, SIGNAL(downloaded(int)),
		this, SLOT(finished(int)));

	labelCurrentAction->setText("�������� " + appName + newVer);
	DBG(qDebug() << "UpdateDialog::downNewVerApp" << filePath);
	downer.download(url, filePath);
}

void UpdateDialog::updateProgress ( qint64 current, qint64 total )
{
	//if (total > 0) // >2000����Ϊ,�ļ�������ʱ,ʧ����,����ȴ����,ֻ����ʾ�Ľ��մ�����Ϣ�Ĵ�С, unzip,�͸����ļ�ʱ������
	{
		progressBar->setRange(0, total);
		progressBar->setValue(current);
	}
	//Global::sleep(1); // xxxxxxxxxx����ܺ��˰�,����crash,��Ϊ�п��������ﴦ��"ȡ��"��Ϣ,������������һ���߳���
	//qDebug() << current << total;
}



void UpdateDialog::finished(int result)
{
	if (bStop)
		return;
	switch (ca)
	{
	case caDown:
		{
			if (result != 0 )
			{
				DBG(qDebug() << "����ʧ��" << result);
				progressBar->setRange(0, 0);
				QString info = QString("�����°汾ʧ��, �������[%1],").arg(result);
				QMessageBox::warning(this, appName + "���³���", info, QMessageBox::Yes);
				QTimer::singleShot(0, this, SLOT(cancel()));
				return;
			}

			qDebug() << "UpdateDialog::finished" <<  "caDown" << result;
			QTimer::singleShot(10, this, SLOT(extract()));
			
		}
		break;
	case caUnzip:
		{
// 			if (0 ==  result)
// 			{
// 				qDebug() << "��ѹ��ʱ" << result;
// 				return;
// 			}
// 			qDebug() << "UpdateDialog::finished" <<  "caUnzip" << result << unzip->fileCount();
// 			if (unzip->fileCount() < 0)
// 			{
// 				qDebug() << "��ѹʧ�� unzip->fileCount()" << unzip->fileCount();
// 				return;
// 			}
			


			progressBar->setRange(0, zf.getfileCount());
			install();
			
		}
		break;
	case caInstall:
		{
			qDebug() << "caInstall" << result;
		}
	}
	
}



void UpdateDialog::extract()
{
	progressBar->setValue(0);
	ca = caUnzip;
	labelCurrentAction->setText("���ڽ�ѹ" + filePath);
	qDebug() << "UpdateDialog::extract" << "���ڽ�ѹ" + filePath;


	qDebug() << "UpdateDialog::extract deleteDir" << extDir;
	
	Global::deleteDir(extDir);

	qDebug() << "UpdateDialog::extract 2";

	zf.extract(filePath, extDir);
	qDebug() << "UpdateDialog::extract 3";
	//qDebug() << "UpdateDialog::extract deleteDir ok";
 
// #if defined(Q_WS_WIN)
//        unzip->extract(filePath, extDir, QDir::convertSeparators(updatedDir + "/unzip.exe"));
// #endif
// #if defined(Q_WS_X11)
//         unzip->extract(filePath, extDir, QDir::convertSeparators(updatedDir + "/unzip"));
// #endif
}

void UpdateDialog::install()
{
	ca = caInstall;
	labelCurrentAction->setText("���ڰ�װ����");
	DBG(qDebug() << "UpdateDialog::install" << extDir << updatedDir);
	if (!copyDir(extDir, updatedDir))
		return;
	//progressBar->setValue(zf.getfileCount());	// xp���޷�ö��.lnk�ļ�
	labelCurrentAction->setText("�ɹ�Ӧ���°汾");

	//qDebug() << "�ɹ�Ӧ���°汾" << fileCopyCount++;
	
	
	runApp("�ɹ����µ�"  + newVer + ", �Ƿ������°汾?");


	
}

bool UpdateDialog::copyDir(const QString source, const QString destination, const bool override) 
{
	if (bStop)
	{
		DBG(qDebug() << "UpdateDialog::copyDir stoped.");
		return false;
	}

	QDir directory(source);
	bool error = false;

	if (!directory.exists()) 
	{
		return false;
	}


	QStringList files = directory.entryList(QDir::AllDirs | QDir::Files | QDir::NoDotAndDotDot | QDir::Hidden); // xp���޷�ö��.lnk�ļ�


	QList<QString>::iterator f = files.begin();


	for (; f != files.end(); ++f) 
	{
		QString filePath = QDir::convertSeparators(directory.path() + '/' + (*f));
		QString dPath = QDir::convertSeparators(destination + "/" + directory.relativeFilePath(filePath));
		QFileInfo fi(filePath);
		//if (fi.isSymLink())
		//{
		//	DBG(qDebug() << "UpdateDialog::copyDir 1" << filePath);
		//}
		if (fi.isFile() || fi.isSymLink())
		{
			if (override)
			{
				QFile::setPermissions(dPath, QFile::WriteOwner);
				QFile::remove(dPath);
			}
			if (!QFile::copy(filePath, dPath))
			{
				qDebug() << "�����ļ�ʧ��" << filePath << dPath;
			}
			progressBar->setValue(fileCopyCount);
			Global::sleep(1); // ��ֹȡ����ť��Ч

		}
		else if (fi.isDir())
		{
			
			QDir ddir(dPath);
			ddir.mkpath(dPath);
			if (!copyDir(filePath, dPath, override))
			{
				error = true;
			}
		}
		else
		{
			DBG(qDebug() << "UpdateDialog::copyDir 2" << filePath);
		}
		
		fileCopyCount++;

	}

	Global::sleep(1); // ��ֹȡ����ť��Ч
	progressBar->setValue(fileCopyCount);

// 	if (fileCopyCount == unzip->fileCount())
// 	{
// 		DBG(qDebug() << "UpdateDialog::copyDir 2");
// 	}

	return !error;
}

QPushButton *UpdateDialog::createButton(const QString &text, const char *member)
{
	QPushButton *button = new QPushButton(text);
	connect(button, SIGNAL(clicked()), this, member);
	return button;
}

void UpdateDialog::cancel()
{
	bStop = true;
	downer.cancel();
	zf.cancel();
	runApp("������ֹ, �Ƿ�������оɰ汾?");
}

void UpdateDialog::runApp(QString info)
{
	if (QMessageBox::Yes == QMessageBox::information(this, appName + "���³���", info, QMessageBox::Yes | QMessageBox::No))
	{
		QProcess::startDetached(QDir::convertSeparators(updatedDir + "/" + exeName), QStringList(), updatedDir);
                qDebug()<<tr("ִ�г�����:")<<exeName<<tr("ִ�г���·��")<<updatedDir;
	}
	close();
}
