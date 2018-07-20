#ifndef SEARCHABLEMENU_H
#define SEARCHABLEMENU_H

#include <QQuickItem>
#include <QAbstractItemModel>
#include <QQmlApplicationEngine>
#include <QQmlListProperty>
#include <QQmlIncubator>

namespace dnai {
namespace controls {
class SearchableMenu : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(QQuickItem* menu READ menu WRITE setMenu NOTIFY menuChanged)
    Q_PROPERTY(QQuickItem* fuzzyField READ fuzzyField WRITE setFuzzyField NOTIFY fuzzyFieldChanged)
    Q_PROPERTY(QAbstractItemModel *model READ model WRITE setModel NOTIFY modelChanged)
    Q_PROPERTY(int itemHeight READ itemHeight WRITE setItemHeight NOTIFY itemHeightChanged)
    Q_PROPERTY(int itemWidth READ itemWidth WRITE setItemWidth NOTIFY itemWidthChanged)

    Q_PROPERTY(QVariant matchedList READ matchedList NOTIFY matchedListChanged)

public:
    SearchableMenu(QQuickItem *parent = nullptr);
    SearchableMenu(QAbstractItemModel *model, QQuickItem *parent = nullptr);
    QQuickItem *menu() const;
    QQuickItem *fuzzyField() const;
    QAbstractItemModel *model() const;
    int itemHeight() const;
    int itemWidth() const;
    QVariant matchedList() const;

public:
    void setMenu(QQuickItem *menu);
    void setFuzzyField(QQuickItem *menu);
    void setModel(QAbstractItemModel *model);
    void setItemHeight(int h);
    void setItemWidth(int w);
    void setMatchedList(const QVariant& list) const;
    Q_INVOKABLE QObject *getActionObj(const QString &key);

public slots:
    void searchFuzzy(const QString &search);

signals:
    void matched(const QString &str);
    void menuChanged(QQuickItem *menu);
    void fuzzyFieldChanged(QQuickItem *menu);
    void modelChanged(QAbstractItemModel *model);
    void itemHeightChanged(int h);
    void itemWidthChanged(int w);
    void matchedListChanged(const QVariant& list);

protected:
    void componentComplete() override;

private slots:
    void createSubMenu(QQmlIncubator::Status status);
    void createMenu(QQmlIncubator::Status status);

private:
    QQuickItem *m_menu;
    QQuickItem *m_fuzzyMenu;
    QAbstractItemModel *m_model;
    QQmlApplicationEngine *m_engine;
    int m_itemHeight;
    int m_itemWidth;
    void forEach(QQuickItem *menuParent, QAbstractItemModel *model, const QString& path, QModelIndex parent= QModelIndex());
    bool fuzzyMatch(const QString &fuzzy, const QString &text);
    void clearModel();
    QMap<QString, QObject *> m_actions;
    QStringList m_matchedList;
    QList<QQuickItem*> m_rootMenus;
};
}
}


#endif // SEARCHABLEMENU_H
