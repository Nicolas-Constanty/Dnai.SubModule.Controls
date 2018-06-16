#include "triangle.h"

#include <QSGVertexColorMaterial>
#include <QtMath>

namespace dnai
{
namespace controls
{
Triangle::Triangle(QQuickItem *parent) : QQuickItem(parent)
{
    setFlag(ItemHasContents, true);
    connect(this, SIGNAL(fillColorChanged(const QColor &)),
            this, SLOT(update()));
    m_fillColor = QColor(130, 255, 0);
}

QSGNode *Triangle::updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *data)
{
    m_nbSegments = 3;
    const char r = m_fillColor.red();
    const char g = m_fillColor.green();
    const char b = m_fillColor.blue();
    const char alpha = m_fillColor.alpha();

    const auto aa = antialiasing();
    QSGGeometryNode *node;
    QSGGeometry *geometry;

    setWidth((m_radius + m_radius / 2.5f) * 2);
    setHeight((m_radius + m_radius / 2.5f) * 2);
    const uint nbVertices = (aa ? 21 : 3);
    if (!oldNode) {
        node = new QSGGeometryNode;
        geometry = new QSGGeometry(QSGGeometry::defaultAttributes_ColoredPoint2D(), nbVertices);
        geometry->setLineWidth(1);
        geometry->setDrawingMode(QSGGeometry::DrawTriangles);
        node->setGeometry(geometry);
        node->setFlag(QSGNode::OwnsGeometry);
        auto material = new QSGVertexColorMaterial;
        material->setFlag(QSGMaterial::Blending);
        node->setMaterial(material);
        node->setFlag(QSGNode::OwnsMaterial);
    }
    else {
        node = static_cast<QSGGeometryNode *>(oldNode);
        geometry = node->geometry();
        geometry->allocate(nbVertices);
    }
    const auto cx = width() / 2;
    const auto cy = height() / 2;
    const auto a = float(2 * M_PI) / m_nbSegments;
    const qreal aaoffset = 3;
    const auto vertices = geometry->vertexDataAsColoredPoint2D();
    auto idx = -1;
    int i = 0;
    vertices[++idx].set(cx + m_radius * qFastCos(a * i), cy + m_radius * qFastSin(a * i), r, g, b, 255);
    ++i;
    vertices[++idx].set(cx + m_radius * qFastCos(a * i), cy + m_radius * qFastSin(a * i), 255, 255, 255, 255);
    ++i;
    vertices[++idx].set(cx + m_radius * qFastCos(a * i), cy + m_radius * qFastSin(a * i), 0, 0, 0, 255);
    i = 0;

    if (aa)
    {
        vertices[++idx].set(cx + m_radius * qFastCos(a * i), cy + m_radius * qFastSin(a * i), r, g, b, 255);
        vertices[++idx].set(cx + (m_radius + aaoffset) * qFastCos(a * i), cy + (m_radius + aaoffset) * qFastSin(a * i), 0, 0, 0, 0);
        vertices[++idx].set(cx + m_radius * qFastCos(a * (i+1)), cy + m_radius * qFastSin(a * (i+1)), 255, 255, 255, 255);

        vertices[++idx].set(cx + m_radius * qFastCos(a * (i+1)), cy + m_radius * qFastSin(a * (i+1)), 255, 255, 255, 255);
        vertices[++idx].set(cx + (m_radius + aaoffset) * qFastCos(a * (i+1)), cy + (m_radius + aaoffset) * qFastSin(a * (i+1)), 0, 0, 0, 0);
        vertices[++idx].set(cx + (m_radius + aaoffset) * qFastCos(a * i), cy + (m_radius + aaoffset) * qFastSin(a * i), 0, 0, 0, 0);
        ++i;

        vertices[++idx].set(cx + m_radius * qFastCos(a * i), cy + m_radius * qFastSin(a * i), 255, 255, 255, 255);
        vertices[++idx].set(cx + (m_radius + aaoffset) * qFastCos(a * i), cy + (m_radius + aaoffset) * qFastSin(a * i), 0, 0, 0, 0);
        vertices[++idx].set(cx + m_radius * qFastCos(a * (i+1)), cy + m_radius * qFastSin(a * (i+1)), 0, 0, 0, 255);

        vertices[++idx].set(cx + m_radius * qFastCos(a * (i+1)), cy + m_radius * qFastSin(a * (i+1)), 0, 0, 0, 255);
        vertices[++idx].set(cx + (m_radius + aaoffset) * qFastCos(a * (i+1)), cy + (m_radius + aaoffset) * qFastSin(a * (i+1)), 0, 0, 0, 0);
        vertices[++idx].set(cx + (m_radius + aaoffset) * qFastCos(a * i), cy + (m_radius + aaoffset) * qFastSin(a * i), 0, 0, 0, 0);
        ++i;

        vertices[++idx].set(cx + m_radius * qFastCos(a * i), cy + m_radius * qFastSin(a * i), 0, 0, 0, 255);
        vertices[++idx].set(cx + (m_radius + aaoffset) * qFastCos(a * i), cy + (m_radius + aaoffset) * qFastSin(a * i), 0, 0, 0, 0);
        vertices[++idx].set(cx + m_radius * qFastCos(a * 0), cy + m_radius * qFastSin(a * 0), r, g, b, 255);

        vertices[++idx].set(cx + m_radius * qFastCos(a * 0), cy + m_radius * qFastSin(a * 0), r, g, b, 255);
        vertices[++idx].set(cx + (m_radius + aaoffset) * qFastCos(a * 0), cy + (m_radius + aaoffset) * qFastSin(a * 0), 0, 0, 0, 0);
        vertices[++idx].set(cx + (m_radius + aaoffset) * qFastCos(a * i), cy + (m_radius + aaoffset) * qFastSin(a * i), 0, 0, 0, 0);
    }

    if (m_borderWidth > 0)
        Q_ASSERT(static_cast<uint>(idx + 1) == nbVertices);
    node->markDirty(QSGNode::DirtyGeometry);
    return node;
}

QColor Triangle::fillColor() const
{
    return m_fillColor;
}

int Triangle::borderWidth() const
{
    return m_borderWidth;
}

int Triangle::radius() const
{
    return m_radius;
}

QColor Triangle::borderColor() const
{
    return m_borderColor;
}

int Triangle::nbSegments() const
{
    return m_nbSegments;
}

void Triangle::setFillColor(QColor fillColor)
{
    if (m_fillColor == fillColor)
        return;

    m_fillColor = fillColor;
    emit fillColorChanged(m_fillColor);
}

void Triangle::setBorderWidth(int borderWidth)
{
    if (m_borderWidth == borderWidth)
        return;

    m_borderWidth = borderWidth;
    emit borderWidthChanged(m_borderWidth);
}

void Triangle::setRadius(int radius)
{
    if (m_radius == radius)
        return;

    m_radius = radius;
    emit radiusChanged(m_radius);
}

void Triangle::setBorderColor(QColor borderColor)
{
    if (m_borderColor == borderColor)
        return;

    m_borderColor = borderColor;
    emit borderColorChanged(m_borderColor);
}

void Triangle::setNbSegments(int nbSegments)
{
    if (m_nbSegments == nbSegments)
        return;

    m_nbSegments = nbSegments;
    emit nbSegmentsChanged(m_nbSegments);
}

}
}

