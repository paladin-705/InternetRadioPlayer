#ifndef LISTVIEWSTREAMMODEL_H
#define LISTVIEWSTREAMMODEL_H

#include <QAbstractListModel>
#include <QList>
#include <QVariant>
#include <QModelIndex>

class StreamModel
{
public:
    StreamModel(int id, QString name, QString url, int state, int hash)
    {
        id_ = id;
        name_ = name;
        url_ = url;
        state_ = state;
        hash_ = hash;
    }
    QString getName() const
    {
        return name_;
    }
    QString getUrl() const
    {
        return url_;
    }
    int getId() const
    {
        return id_;
    }
    int getHash() const
    {
        return hash_;
    }
    int getState() const
    {
        return state_;
    }

private:
    int id_;
    QString name_;
    QString url_;
    int state_;
    int hash_;
};

class ListViewStreamModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit ListViewStreamModel(QObject *parent = 0);

    enum streamRoles
    {
        NameRole = Qt::UserRole + 1,
        UrlRole,
        StreamIdRole,
        StateRole
    };

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    int rowCount(const QModelIndex &parent) const;

    QHash<int, QByteArray> roleNames() const;

public slots:
    void addStream(StreamModel stream);

private:
    QList<StreamModel> streams;

    QHash<int, QByteArray> roles;
};

#endif // LISTVIEWSTREAMMODEL_H
