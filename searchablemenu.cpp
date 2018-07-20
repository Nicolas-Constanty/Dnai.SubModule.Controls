#include <QQmlApplicationEngine>
#include <QMetaObject>
#include <QQmlContext>

#include "searchablemenu.h"
#include "callbackincubator.h"

#define FTS_FUZZY_MATCH_IMPLEMENTATION
#include "fuzzymatch.h"

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

void SearchableMenu::clearModel()
{
    m_actions.clear();
    for (auto menu : m_rootMenus)
    {
        QMetaObject::invokeMethod(m_menu, "removeMenu", Q_ARG(QVariant, menu->property("menu")));
    }
    m_rootMenus.clear();
}

void SearchableMenu::setModel(QAbstractItemModel *model)
{
    if (m_model && isComponentComplete())
    {
        clearModel();
    }
    m_model = model;
    if (m_model && isComponentComplete())
    {
        m_engine = dynamic_cast<QQmlApplicationEngine *>(QQmlApplicationEngine::contextForObject(this)->engine());
        forEach(nullptr, m_model, "");
    }
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

void SearchableMenu::createSubMenu(QQmlIncubator::Status status)
{
    if (status == QQmlIncubator::Status::Ready)
    {
        auto incubator = dynamic_cast<CallBackIncubator*>(sender());
        auto menu = dynamic_cast<QQuickItem*>(incubator->object());
        qvariant_cast<QObject *>(menu->property("menu"))->setProperty("title", incubator->name);
        if (incubator->menuParent)
        {
            menu->setParentItem(incubator->menuParent);
            QMetaObject::invokeMethod(incubator->menuParent, "addMenu", Q_ARG(QVariant, menu->property("menu")));
        }
        else
        {
            m_menu->setParentItem(this);
            menu->setParentItem(m_menu);
            m_rootMenus.append(menu);
            QMetaObject::invokeMethod(m_menu, "addMenu", Q_ARG(QVariant, menu->property("menu")));
        }
        forEach(menu, incubator->model, incubator->fullpath, incubator->index);
    }
}

void SearchableMenu::createMenu(QQmlIncubator::Status status)
{
    if (status == QQmlIncubator::Status::Ready)
    {
        auto incubator = dynamic_cast<CallBackIncubator*>(sender());
        auto action = dynamic_cast<QQuickItem*>(incubator->object());
        auto actionQml =  qvariant_cast<QObject *>(action->property("action"));
        actionQml->setProperty("text", incubator->name);
        actionQml->setProperty("model", incubator->item);
        action->setParentItem(incubator->menuParent);
        action->setProperty("triggeredAction", property("triggeredAction"));
        m_actions[incubator->fullpath] = action;
        QMetaObject::invokeMethod(incubator->menuParent, "addAction", Q_ARG(QVariant, action->property("action")));
    }
}

void SearchableMenu::forEach(QQuickItem *menuParent, QAbstractItemModel* model, const QString& path, QModelIndex parent)
{
    QHash<QByteArray, int> reverseHash;
    QList<CallBackIncubator *> incubators;
    const auto hash = model->roleNames();
    for (auto key : hash.keys())
    {
        reverseHash[hash[key]] = key;
    }
    for(int r = 0; r < model->rowCount(parent); ++r) {
        QModelIndex index = model->index(r, 0, parent);
        QVariant name = model->data(index, reverseHash["name"]);
        QVariant item = model->data(index, reverseHash["item"]);
        QString fullpath = path.isEmpty() ? name.toString() : path + " / " + name.toString();
        const auto incubator = new CallBackIncubator();
        incubator->name = name;
        incubator->menuParent = menuParent;
        incubator->fullpath = fullpath;
        incubator->index = index;
        incubator->item = item;
        incubator->model = model;
        incubators.append(incubator);
        if( model->hasChildren(index) )
        {
            QQmlComponent component(m_engine, QUrl(QStringLiteral("qrc:/BaseMenu.qml")));
            QObject::connect(incubator, SIGNAL(statusAsChanged(QQmlIncubator::Status)),
                             this, SLOT(createSubMenu(QQmlIncubator::Status)));

            component.create(*incubator);
        }
        else
        {
            QQmlComponent component(m_engine, QUrl(QStringLiteral("qrc:/BaseAction.qml")));
            QObject::connect(incubator, SIGNAL(statusAsChanged(QQmlIncubator::Status)),
                             this, SLOT(createMenu(QQmlIncubator::Status)));
            component.create(*incubator);
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
