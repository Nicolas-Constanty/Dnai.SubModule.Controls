#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <QQuickItem>
#include <QSGNode>


namespace dnai
{
namespace controls
{

class Triangle : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(QColor fillColor READ fillColor WRITE setFillColor NOTIFY fillColorChanged)
    Q_PROPERTY(int nbSegments READ nbSegments WRITE setNbSegments NOTIFY nbSegmentsChanged)
    Q_PROPERTY(QColor borderColor READ borderColor WRITE setBorderColor NOTIFY borderColorChanged)
    Q_PROPERTY(int radius READ radius WRITE setRadius NOTIFY radiusChanged)
    Q_PROPERTY(int borderWidth READ borderWidth WRITE setBorderWidth NOTIFY borderWidthChanged)

public:
    Triangle(QQuickItem *parent = nullptr);


    QSGNode *updatePaintNode(QSGNode *, UpdatePaintNodeData *) override;
    QColor fillColor() const;
    int borderWidth() const;

    int radius() const;

    QColor borderColor() const;

    int nbSegments() const;

public slots:
    void setFillColor(const QColor &fillColor);
    void setBorderWidth(int borderWidth);
    void setRadius(int radius);
    void setBorderColor(const QColor &borderColor);
    void setNbSegments(int nbSegments);

signals:
    void fillColorChanged(const QColor &fillColor);
    void borderWidthChanged(int borderWidth);
    void radiusChanged(int radius);
    void borderColorChanged(const QColor &borderColor);
    void nbSegmentsChanged(int nbSegments);

private:
    QColor m_fillColor;

    int m_borderWidth;
    int m_radius;
    QColor m_borderColor;
    int m_nbSegments;
};
}
}

#endif // TRIANGLE_H
