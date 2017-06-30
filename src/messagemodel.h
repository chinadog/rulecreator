#ifndef MESSAGEMODEL_H
#define MESSAGEMODEL_H

#include <QAbstractListModel>
#include <TDB/structs.h>

class MessageModel : public QAbstractListModel
{
public:
    MessageModel();

    bool addRow(const Event &event, int row = -1);
    Event getEvent(const QModelIndex &index);
    bool lowerOneStep(int row);
    bool raiseOneStep(int row);
    bool removeRow(int row, const QModelIndex &parent = QModelIndex());
    bool updateRow(const Event &event, int row);
protected:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
private:
    QList<Event> m_values;
};

#endif // MESSAGEMODEL_H
