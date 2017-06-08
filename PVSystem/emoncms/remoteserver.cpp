/*******************************************************
 * Copyright (C) 2017-2018 Marco Rossi marco.rossi1337@gmail.com
 *
 * This file is part of PVSystem.
 *
 * PVSystem can not be copied and/or distributed without the express
 * permission of Marco Rossi
 *******************************************************/

#include "remoteserver.h"
#include <QNetworkAccessManager>
#include <QObject>
#include <QCoreApplication>
#include <QDebug>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QUrlQuery>

RemoteServer::RemoteServer()
{

}

QNetworkReply *sendRequest()
{

    QEventLoop eventLoop;
    QNetworkAccessManager mgr;
    QObject::connect(&mgr, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));

    QNetworkRequest req( QUrl( QString("http://ip.jsontest.com/")));
    QNetworkReply *reply = mgr.get(req);
    eventLoop.exec();

    return reply;
}

QString output()
{
    QNetworkReply *reply = sendRequest();
    return "";
}
