#include "treewidgetitem.h"

TreeWidgetItem::TreeWidgetItem(QTreeWidget *parent) : QTreeWidgetItem(parent)
{

}

bool TreeWidgetItem::operator<(const QTreeWidgetItem &other) const
{
    int column = treeWidget()->sortColumn();
    return text(column).toInt() < other.text(column).toInt();
}

