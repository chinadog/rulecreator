#include "messagemodel.h"
#include <QDebug>

MessageModel::MessageModel()
{
    Event ev;
    ev.msg = "TEST1";
    ev.time = QDateTime::currentDateTime();
    m_values.push_back(ev);
    ev.msg = "TEST2";
    ev.time = QDateTime::currentDateTime();
    m_values.push_back(ev);
}

Event MessageModel::getEvent(const QModelIndex &index)
{
    if(index.row() >= 0 && index.row() < m_values.size())
    {
        return m_values[index.row()];
    }
    return Event();
}

bool MessageModel::removeRow(int row, const QModelIndex &parent)
{
    Q_UNUSED(parent)
    if(row >= 0 && row < m_values.size())
    {
        beginRemoveRows(QModelIndex(), row, row);
        m_values.removeAt(row);
        endRemoveRows();
        return true;
    }
    return false;
}

bool MessageModel::addRow(const Event &value, int row)
{
    if(row > m_values.size())
        return false;
    if(row == -1)
    {
        beginInsertRows(QModelIndex(), m_values.size(), m_values.size());
        m_values.push_back(value);
        endInsertRows();
    }
    else
    {
        beginInsertRows (QModelIndex(), row, row);
        m_values.insert(row, value);
        endInsertRows();
    }
    return true;
}

bool MessageModel::updateRow(const Event &value, int row)
{
    if(row < 0 || row >= m_values.size())
        return false;
    m_values[row] = value;
    emit dataChanged(index(row,0),index(row,0));
    return true;
}

int MessageModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_values.size();
}

QVariant MessageModel::data(const QModelIndex &index, int role) const
{
    Event event;
    QString result;
    switch (role) {
    case Qt::DisplayRole:
        event = m_values[index.row()];
        result = event.time.toString("dd.MM.yyyy hh:mm:ss")+"\n"+event.msg;
        return result;
        break;
    default:
        break;
    }
    return QVariant();
}
