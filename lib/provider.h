/*
    This file is part of KDE.

    Copyright (c) 2008 Cornelius Schumacher <schumacher@kde.org>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301,
    USA.
*/

#ifndef ATTICA_PROVIDER_H
#define ATTICA_PROVIDER_H

#include <QtCore/QExplicitlySharedDataPointer>
#include <QtCore/QString>

#include <QUrl>

#include "atticaclient_export.h"
#include "category.h"
#include "listjob.h"


class QUrl;
class QDate;
class QNetworkReply;
class QAuthenticator;

namespace Attica {

class Activity;
class Content;
class ContentJob;
class Event;
class EventJob;
class Folder;
class KnowledgeBaseEntry;
class KnowledgeBaseEntryJob;
class KnowledgeBaseListJob;
class Message;
class Person;
class PersonJob;
class PostJob;
class ProviderInitJob;


/**
  Open Collaboration Services API.
*/
class ATTICA_EXPORT Provider :public QObject
{
    Q_OBJECT
    
  public:
    Provider(QObject* parent = 0);
    Provider(const Provider& other, QObject* parent = 0);
    Provider& operator=(const Provider& other);
    ~Provider();
    
    bool isValid() const;
    QUrl baseUrl() const;
    QString name() const;
    QString id() const;

    enum SortMode {
        Newest,
        Alphabetical,
        Rating,
        Downloads
    };

    static ProviderInitJob* byId(const QString& id);

    // Person part of OCS

    PersonJob* requestPerson(const QString& id);
    PersonJob* requestPersonSelf();
    ListJob<Person>* requestPersonSearchByName(const QString& name);
    ListJob<Person>* requestPersonSearchByLocation(qreal latitude, qreal longitude, qreal distance, int page = 0, int pageSize = 100);
    PostJob* postLocation(qreal latitude, qreal longitude, const QString& city = QString(), const QString& country = QString());

    // Friend part of OCS

    ListJob<Person>* requestFriends(const QString& id, int page = 0, int pageSize = 100);
    PostJob* postFriendInvitation(const QString& to, const QString& message);

    // Message part of OCS

    ListJob<Folder>* requestFolders();
    ListJob<Message>* requestMessages(const Folder& folder);
    PostJob* postMessage(const Message& message);

    // Activity part of OCS

    ListJob<Activity>* requestActivities();
    PostJob* postActivity(const QString& message);

    // Content part of OCS

    ListJob<Category>* requestCategories();
    ListJob<Content>* searchContents(const Category::List& categories, const QString& search, SortMode mode);
    ContentJob* requestContent(const QString& id);

    PostJob* addNewContent(const Category& category, const Content& newContent);

    // KnowledgeBase part of OCS

    KnowledgeBaseEntryJob* requestKnowledgeBaseEntry(const QString& id);
    KnowledgeBaseListJob* searchKnowledgeBase(const Content& content, const QString& search, SortMode, int page, int pageSize);

    // Event part of OCS

    EventJob* requestEvent(const QString& id);
    ListJob<Event>* requestEvent(const QString& country, const QString& search, const QDate& startAt, SortMode mode, int page, int pageSize);

  private Q_SLOTS:
    void authenticate(QNetworkReply*,QAuthenticator*);
    
  protected:
    QUrl createUrl(const QString& path);
  
    PersonJob* doRequestPerson(const QUrl& url);
    ListJob<Person>* doRequestPersonList(const QUrl& url);
    ListJob<Activity>* doRequestActivityList(const QUrl& url);
    ListJob<Folder>* doRequestFolderList(const QUrl& url);
    ListJob<Message>* doRequestMessageList(const QUrl& url);

  private:
    class Private;
    QExplicitlySharedDataPointer<Private> d;
    Provider(const QString& id, const QUrl& baseUrl, const QString& name);
    friend class ProviderInitJob;
};

}

#endif
