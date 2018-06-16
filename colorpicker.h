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
    Q_PROPERTY(float rotateFocus READ rotateFocus NOTIFY rotateFocusChanged)
    Q_PROPERTY(qreal radius READ radius WRITE setRadius NOTIFY radiusChanged)
    Q_PROPERTY(float innerPercent READ innerPercent WRITE setInnerPercent NOTIFY innerPercentChanged)
    Q_PROPERTY(QColor pointingColor READ pointingColor NOTIFY pointingColorChanged)

public:
    ColorPicker(QQuickItem *parent = nullptr);
    Q_INVOKABLE void getColor(QWindow *w, int x, int y);
    const QColor &color() const;

    float rotateFocus() const;

    qreal radius() const;

    float innerPercent() const;

    bool pointInTriangle(const QPointF &pt, const QPointF &v1, const QPointF &v2, const QPointF &v3);


    float sign(const QPointF &p1, const QPointF &p2, const QPointF &p3);
    QColor pointingColor() const;

public slots:
    void setRadius(qreal radius);

    void setInnerPercent(float innerPercent);

    void setColor(QColor color);

signals:
    void colorChanged(const QColor &color);

    void rotateFocusChanged(float rotateFocus);

    void radiusChanged(qreal radius);

    void innerPercentChanged(float innerPercent);

    void pointingColorChanged(QColor pointingColor);

private:
    QColor m_color;
    float m_rotateFocus;
    qreal m_radius;
    float m_innerPercent;
    QColor m_pointingColor;
};
}
}
#endif // COLORPICKER_H
