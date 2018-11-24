#ifndef COLORPICKER_H
#define COLORPICKER_H

#include <QQuickItem>
#include <QWindow>

namespace dnai
{
namespace controls
{
class ColorPicker : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
    Q_PROPERTY(qreal rotateFocus READ rotateFocus NOTIFY rotateFocusChanged)
    Q_PROPERTY(qreal radius READ radius WRITE setRadius NOTIFY radiusChanged)
    Q_PROPERTY(qreal innerPercent READ innerPercent WRITE setInnerPercent NOTIFY innerPercentChanged)
    Q_PROPERTY(QColor pointingColor READ pointingColor NOTIFY pointingColorChanged)

public:
    ColorPicker(QQuickItem *parent = nullptr);
    Q_INVOKABLE void getColor(QWindow *w, int x, int y);
    const QColor &color() const;

    qreal rotateFocus() const;

    qreal radius() const;

    qreal innerPercent() const;

    bool pointInTriangle(const QPointF &pt, const QPointF &v1, const QPointF &v2, const QPointF &v3);


    qreal sign(const QPointF &p1, const QPointF &p2, const QPointF &p3);
    QColor pointingColor() const;

public slots:
    void setRadius(qreal radius);

    void setInnerPercent(qreal innerPercent);

    void setColor(const QColor &color);

signals:
    void colorChanged(const QColor &color);

    void rotateFocusChanged(qreal rotateFocus);

    void radiusChanged(qreal radius);

    void innerPercentChanged(qreal innerPercent);

    void pointingColorChanged(const QColor &pointingColor);

private:
    QColor m_color;
    qreal m_rotateFocus;
    qreal m_radius;
    qreal m_innerPercent;
    QColor m_pointingColor;
};
}
}
#endif // COLORPICKER_H
