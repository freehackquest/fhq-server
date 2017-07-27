#include "log.h"

#include <QTextStream>
#include <QFile>
#include <QDebug>
#include <QByteArray>
#include <QDateTime>
#include <QDir>
#include <QThread>
#include <QMutexLocker>

void Log::info(QString tag, QString msg){
	Log::add("INFO",tag, msg);
}

void Log::err(QString tag, QString msg){
	Log::add("ERR",tag, msg);
}

void Log::warn(QString tag, QString msg){
	Log::add("WARN",tag, msg);
}

void Log::add(QString type, QString tag, QString msg){
	QMutexLocker locker(&LOG_MUTEX);
	QString sThreadID = "0x" + QString::number((long long)QThread::currentThreadId(), 16);
	msg = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz") + ", " + sThreadID + " [" + type + "] " + tag + ": " + msg;
	qDebug() << msg;
	LAST_LOG_MESSAGES << msg;
	while(LAST_LOG_MESSAGES.size() > 50){
		LAST_LOG_MESSAGES.removeLast();
	}
}

/*
// use this function for problems with database connection
void Errors::WriteServerError(QString errorInfo){
	QDir dir("/var/log/freehackquestd/errors");
	if(!dir.exists()){
		qDebug().nospace() << dir.absolutePath() << " did not found";
		qDebug().nospace() << errorInfo;
		return;
	}
	QString date = QDateTime::currentDateTime().toString("yyyy-MM-dd");
	if(!dir.exists(date) && !dir.mkdir(date)){
		qDebug().nospace() << dir.absolutePath() << " could not create dir " << date;
		qDebug().nospace() << errorInfo;
		return;
	}
	dir.cd(date);
	
	QString time = QDateTime::currentDateTime().toString("HHmmss.zzz");
    QFile errorf(dir.absolutePath() + '/' + time);
    errorf.open(QIODevice::WriteOnly | QIODevice::Text);

    if(!errorf.isOpen()){
        qDebug().nospace() << dir.absolutePath() + '/' + time << " could not openfile ";
		qDebug().nospace() << errorInfo;
		return;
    }
    QTextStream outStream(&errorf);
    outStream << errorInfo.toUtf8();
    errorf.close();
}

*/
