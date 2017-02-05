#include "includes/ListViewStreamModel.h"

ListViewStreamModel::ListViewStreamModel(QObject *parent) :
    QAbstractListModel(parent)
{
    roles[NameRole] = "name";
    roles[UrlRole] = "phone";
    roles[StreamIdRole] = "age";
}

QVariant ListViewStreamModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() > streams.count())
        return QVariant();
    const StreamModel & stream = streams[index.row()];
        if (role == NameRole)
            return stream.getName();
        else if (role == UrlRole)
            return stream.getUrl();
        else if (role == StreamIdRole)
            return stream.getId();
        else if (role == StateRole)
            return stream.getState();
        return QVariant();
}

void ListViewStreamModel::addStream(StreamModel stream)
{
   streams << stream;
}

int ListViewStreamModel::rowCount(const QModelIndex &parent) const
{
    return streams.size();
}

QHash<int, QByteArray> ListViewStreamModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[NameRole] = "name";
    roles[UrlRole] = "url";
    roles[StreamIdRole] = "streamId";
    roles[StateRole] = "state";

    return roles;
}
