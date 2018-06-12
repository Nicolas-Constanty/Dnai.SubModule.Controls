#include <QQmlApplicationEngine>
#include <QMetaObject>
#include <QQmlContext>

#include "searchablemenu.h"

namespace dnai {
namespace controls {
SearchableMenu::SearchableMenu(QQuickItem *parent) : QQuickItem(parent), m_menu(nullptr), m_fuzzyMenu(nullptr), m_model(nullptr)
{
}

SearchableMenu::SearchableMenu(QAbstractItemModel *model, QQuickItem *parent) : QQuickItem(parent), m_menu(nullptr), m_fuzzyMenu(nullptr), m_model(nullptr)
{
    setModel(model);
}

QQuickItem *SearchableMenu::menu() const
{
    return m_menu;
}

QQuickItem *SearchableMenu::fuzzyField() const
{
    return m_fuzzyMenu;
}

QAbstractItemModel *SearchableMenu::model() const
{
    return m_model;
}

int SearchableMenu::itemHeight() const
{
    return m_itemHeight;
}

int SearchableMenu::itemWidth() const
{
    return m_itemWidth;
}

QVariant SearchableMenu::matchedList() const
{
    return QVariant::fromValue(m_matchedList);
}

void SearchableMenu::setMenu(QQuickItem *menu)
{
    if (m_menu == menu)
        return;
    m_menu = menu;
    emit menuChanged(menu);
}

void SearchableMenu::setFuzzyField(QQuickItem *menu)
{
    if (m_fuzzyMenu == menu)
        return;
    m_fuzzyMenu = menu;
    emit fuzzyFieldChanged(menu);
}

void SearchableMenu::setModel(QAbstractItemModel *model)
{
    if (model == m_model)
        return;
    m_model = model;
    emit modelChanged(model);
}

void SearchableMenu::setItemHeight(int h)
{
    if (h == m_itemHeight)
        return;
    m_itemHeight = h;
    emit itemHeightChanged(h);
}

void SearchableMenu::setItemWidth(int w)
{
    if (w == m_itemWidth)
        return;
    m_itemWidth = w;
    emit itemWidthChanged(w);
}

QObject *SearchableMenu::getActionObj(const QString &key)
{
    auto actionQml =  qvariant_cast<QObject *>(m_actions[key]->property("action"));
    return qvariant_cast<QObject *>(actionQml->property("model"));
}

void SearchableMenu::forEach(QQuickItem *menuParent, QAbstractItemModel* model, const QString& path, QModelIndex parent)
{
    QHash<QByteArray, int> reverseHash;
    const auto hash = model->roleNames();
    for (auto key : hash.keys())
    {
        reverseHash[hash[key]] = key;
    }
    for(int r = 0; r < model->rowCount(parent); ++r) {
        QModelIndex index = model->index(r, 0, parent);
        QVariant name = model->data(index, reverseHash["name"]);
        QString fullpath = path.isEmpty() ? name.toString() : path + " / " + name.toString();
        if( model->hasChildren(index) )
        {
            QQmlComponent component(m_engine, QUrl::fromLocalFile(":BaseMenu.qml"));
            auto menu = dynamic_cast<QQuickItem*>(component.create());
            qvariant_cast<QObject *>(menu->property("menu"))->setProperty("title", name);
            if (menuParent)
            {
                menu->setParentItem(menuParent);
                QMetaObject::invokeMethod(menuParent, "addMenu", Q_ARG(QVariant, menu->property("menu")));
            }
            else
            {
                m_menu->setParentItem(this);
                menu->setParentItem(m_menu);
                QMetaObject::invokeMethod(m_menu, "addMenu", Q_ARG(QVariant, menu->property("menu")));
            }
            forEach(menu, model, fullpath, index);
        }
        else
        {
            QQmlComponent component(m_engine, QUrl::fromLocalFile(":BaseAction.qml"));
            auto action = dynamic_cast<QQuickItem*>(component.create());
            QVariant item = model->data(index, reverseHash["item"]);
            auto actionQml =  qvariant_cast<QObject *>(action->property("action"));
            actionQml->setProperty("text", name);
            actionQml->setProperty("model", item);
            action->setParentItem(menuParent);
            action->setProperty("triggeredAction", property("triggeredAction"));
            m_actions[fullpath] = action;
            QMetaObject::invokeMethod(menuParent, "addAction", Q_ARG(QVariant, action->property("action")));
        }
    }
}

void SearchableMenu::searchFuzzy(const QString &search)
{
    m_matchedList.clear();
    if (!search.isEmpty())
    {
        QMap<int, QStringList> map;
        for(const auto &key : m_actions.keys())
        {
            const auto lsearch = search.toLower();
            const auto lkey = key.toLower();
            auto out = 0;
            fts::fuzzy_match(lsearch.toLatin1().constData(), lkey.toLatin1().constData(), out);
            if (lkey.contains(lsearch))
                out += 50;
            if (out > 80)
                map[out].append(key);
        }
        for (const auto &key : map.keys())
        {
            for (const auto &val : map[key])
                m_matchedList.insert(0, val);
        }
    }
    emit matchedListChanged(QVariant::fromValue(m_matchedList));
}

bool SearchableMenu::fuzzyMatch(const QString& fuzzy, const QString& text) {
    // empty fuzzy string matches any text
    if(fuzzy.isEmpty())
        return true;

    // counter of matched characters
    int counter = 0;

    // iterate over matched string
    auto fuzziness = 1;
    if (fuzziness > fuzzy.length())
        fuzziness = 1;
    const auto lfuzzy = fuzzy.toLower();
    const auto ltext = text.toLower();
    for(auto& c : ltext) {
        // try to match a single character from the fuzzy string
        if(c == lfuzzy[counter])
        {
            ++counter;
        }
        // if all characters of the fuzzy string were matched, return success
        if(counter == lfuzzy.length())
            return true;
    }

    // reached the end of the matched string without matching all fuzzy
    //   characters - failure
    return false;
}

void SearchableMenu::componentComplete()
{
    QQuickItem::componentComplete();
    if (m_model)
    {
        m_engine = dynamic_cast<QQmlApplicationEngine *>(QQmlApplicationEngine::contextForObject(this)->engine());
        forEach(nullptr, m_model, "");
    }
}
}
}
