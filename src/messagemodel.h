#ifndef MESSAGEMODEL_H
#define MESSAGEMODEL_H

#include <QAbstractListModel>
#include <TDB/structs.h>

class MessageModel : public QAbstractListModel
{
public:
    MessageModel();

    Event getEvent(const QModelIndex &index);
    bool addRow(const Event &event, int row = -1);
    bool updateRow(const Event &event, int row);
protected:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
private:
    QList<Event> m_values;
};

#endif // MESSAGEMODEL_H
