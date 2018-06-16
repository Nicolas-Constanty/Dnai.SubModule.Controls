#include "colorpicker.h"
#include <QPixmap>
#include <QGuiApplication>
#include <QScreen>
#include <QtMath>

namespace dnai
{
namespace controls
{
ColorPicker::ColorPicker(QQuickItem *parent) : QQuickItem(parent)
{
    m_pointingColor = QColor(130, 255, 0);
    m_color = QColor(130, 255, 0);
    m_rotateFocus = 0;
}

float ColorPicker::sign (const QPointF &p1, const QPointF & p2, const QPointF & p3)
{
    return (p1.x() - p3.x()) * (p2.y() - p3.y()) - (p2.x() - p3.x()) * (p1.y() - p3.y());
}

QColor ColorPicker::pointingColor() const
{
    return m_pointingColor;
}

bool ColorPicker::pointInTriangle (const QPointF &pt, const QPointF & v1, const QPointF & v2, const QPointF & v3)
{
    bool b1, b2, b3;

    b1 = sign(pt, v1, v2) < 0.0f;
    b2 = sign(pt, v2, v3) < 0.0f;
    b3 = sign(pt, v3, v1) < 0.0f;

    return ((b1 == b2) && (b2 == b3));
}

void ColorPicker::setRadius(qreal radius)
{
    qWarning("Floating point comparison needs context sanity check");
    if (qFuzzyCompare(m_radius, radius))
        return;

    m_radius = radius;
    emit radiusChanged(m_radius);
}

void ColorPicker::setInnerPercent(float innerPercent)
{
    qWarning("Floating point comparison needs context sanity check");
    if (qFuzzyCompare(m_innerPercent, innerPercent))
        return;

    m_innerPercent = innerPercent;
    emit innerPercentChanged(m_innerPercent);
}

void ColorPicker::setColor(QColor color)
{
    if (m_color == color)
        return;

    m_color = color;
    emit colorChanged(m_color);
}

void ColorPicker::getColor(QWindow *w, int xt, int yt)
{
    if (xt < 0 || yt < 0)
        return;
    auto cx = width() / 2;
    auto cy = height() / 2;
    auto dy = yt - cy;
    auto dx = xt - cx;
    auto at = m_rotateFocus / (180 / M_PI);
    auto d = sqrt( dx * dx + dy * dy );

    auto innerRadius = (m_innerPercent + 0.025) * m_radius;
    const auto a = float(2 * M_PI) / 3.0;
    QPointF p1(cx + (innerRadius - 0.1) * qFastCos(0 + at), cy + (innerRadius - 0.1) * qFastSin(0 + at));
    QPointF p2(cx + (innerRadius - 0.1) * qFastCos(a * 1 + at), cy + (innerRadius - 0.1) * qFastSin(a * 1+ at));
    QPointF p3(cx + (innerRadius - 0.1) * qFastCos(a * 2 + at), cy + (innerRadius - 0.1) * qFastSin(a * 2+ at));
    QPoint p(xt, yt);
    bool isInTriangle = pointInTriangle(p, p1, p2, p3);
    if ((d < m_radius - 0.1 && d > innerRadius) || isInTriangle)
    {
        auto pixmap = QGuiApplication::screenAt(p)->grabWindow(w->winId(), xt + position().x(), yt + position().y(), 1, 1);
        QRgb pixelValue = pixmap.toImage().pixel(0, 0);
        QColor col(pixelValue);
        if (d < m_radius && d > innerRadius)
        {
            m_color = col;
            m_rotateFocus = atan2(dy, dx); // range (-PI, PI]
            m_rotateFocus *= 180 / M_PI;
            emit rotateFocusChanged(m_rotateFocus);
            emit colorChanged(m_color);
        }
        m_pointingColor = col;
        emit pointingColorChanged(m_pointingColor);
    }
}

const QColor &ColorPicker::color() const
{
    return m_color;
}

float ColorPicker::rotateFocus() const
{
    return m_rotateFocus;
}

qreal ColorPicker::radius() const
{
    return m_radius;
}

float ColorPicker::innerPercent() const
{
    return m_innerPercent;
}
}
}
