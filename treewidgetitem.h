#ifndef TREEWIDGETITEM_H
#define TREEWIDGETITEM_H

#include <QTreeWidget>
#include <QTreeWidgetItem>

class TreeWidgetItem : public QTreeWidgetItem {
public:
    TreeWidgetItem(QTreeWidget* parent);

private:
    bool operator<(const QTreeWidgetItem &other) const;
};
#endif // TREEWIDGETITEM_H
