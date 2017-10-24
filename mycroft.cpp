/******************************************************************************
 *  Copyright (C) 2016 – 2018 by Aditya Mehra <aix.m@outlook.com>             *
 *                                                                            *
 *  This library is free software; you can redistribute it and/or modify      *
 *  it under the terms of the GNU Lesser General Public License as published  *
 *  by the Free Software Foundation; either version 2 of the License or (at   *
 *  your option) any later version.                                           *
 *                                                                            *
 *  This library is distributed in the hope that it will be useful,           *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU         *
 *  Library General Public License for more details.                          *
 *                                                                            *
 *  You should have received a copy of the GNU Lesser General Public License  *
 *  along with this library; see the file COPYING.LIB.                        *
 *  If not, see <http://www.gnu.org/licenses/>.                               *
 *****************************************************************************/

#include "mycroft.h"

#include <KLocalizedString>
#include <QIcon>
#include <QApplication>
#include <QClipboard>
#include <QDebug>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonObject>

static QString sendQuery;

Mycroft::Mycroft(QObject *parent, const QVariantList &args)
    : Plasma::AbstractRunner(parent, args)
{
    Q_UNUSED(args);
    setObjectName(QLatin1String("Mycroft"));
    QIcon icon = QIcon::fromTheme(QStringLiteral("mycroft-plasma-appicon"));
    setIgnoredTypes(Plasma::RunnerContext::Directory |
                    Plasma::RunnerContext::File |
                    Plasma::RunnerContext::NetworkLocation);
    setSpeed(AbstractRunner::SlowSpeed);
    setPriority(HighestPriority);
    setDefaultSyntax(Plasma::RunnerSyntax(QString(":q:").arg(" "),i18n("Ask Mycroft")));
}

Mycroft::~Mycroft()
{
}

void Mycroft::match(Plasma::RunnerContext &context)
{
    const QString query = context.query();

if ((query.length() < 3) || !context.isValid()) {
        return;
    }
else {
    Plasma::QueryMatch match(this);
    match.setText(query);
    match.setIcon(icon());
    context.addMatch(match);
    }
}

void Mycroft::run(const Plasma::RunnerContext &context, const Plasma::QueryMatch &match)
{
    Q_UNUSED(context);
    QApplication::clipboard()->setText(match.text());
    sendQuery = match.text();
    m_webSocket = new QWebSocket;
    m_webSocket->setParent(this);
    connect(m_webSocket, &QWebSocket::connected, this, &Mycroft::onConnected);
    connect(m_webSocket, &QWebSocket::disconnected, this, &Mycroft::onDisconnected);
    QUrl url = QUrl("ws://0.0.0.0:8181/core");
    m_webSocket->open(url);
}

void Mycroft::onConnected()
{
    QString sendinArray = sendQuery;
    QJsonObject socketObject;
    socketObject.insert("type", QJsonValue::fromVariant("recognizer_loop:utterance"));
    QJsonObject dataObject;
    QJsonArray utteranceArray;
    utteranceArray.push_front(sendQuery);
    dataObject.insert("lang", "en-us");
    dataObject.insert("utterances", QJsonValue::fromVariant(utteranceArray));
    socketObject.insert("data", dataObject);
    
    QJsonDocument doc;
    doc.setObject(socketObject);
    QString jsend = doc.toJson(QJsonDocument::Compact);
    m_webSocket->sendTextMessage(jsend);
    connect(m_webSocket, &QWebSocket::textMessageReceived, this, &Mycroft::onTextMessageReceived);
}

void Mycroft::onDisconnected()
{

}

void Mycroft::onTextMessageReceived(QString message)
{
  m_webSocket->close();
}

#include "moc_mycroft.cpp"
