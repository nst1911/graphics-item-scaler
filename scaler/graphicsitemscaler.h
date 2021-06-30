#ifndef GRAPHICSITEMRESIZER_H
#define GRAPHICSITEMRESIZER_H

#include <QObject>
#include <QGraphicsItem>
#include <QPen>
#include <QGraphicsView>

class GraphicsItemScaler : public QGraphicsObject
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)
public:
    explicit GraphicsItemScaler(QGraphicsItem *target, QGraphicsItem *parent = nullptr);
    ~GraphicsItemScaler();

    inline QBrush handleItemBrush() const;
    void setHandleItemBrush(const QBrush &brush);

    QPen handleItemPen() const;
    void setHandleItemPen(const QPen &pen);

    QBrush boundingRectAreaBrush() const;
    void setBoundingRectAreaBrush(const QBrush &brush);

    QPen boundingRectAreaPen() const;
    void setBoundingRectAreaPen(const QPen &pen);

    bool boundingRectAreaVisible() const;
    void setBoundingRectAreaVisible(bool visible);

    QGraphicsItem* target() const;
    QRectF targetBoundingRect() const;

    bool handlersIgnoreTransformations() const;
    // If true, handler items ignore all transformations e.g. zooming the view etc
    void setHandlersIgnoreTransformations(bool ignore);

    // QGraphicsItem interface
public:
    virtual QRectF boundingRect() const override;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

public slots:
    void recalculate();

private:
    class HandleItem;

    void updateHandleItemPositions();
    void updateBoundingRectSize(const QSizeF &size);

    QList<HandleItem *> mHandleItems;
    QPen mHandleItemPen;
    QBrush mHandleItemBrush;

    QGraphicsItem* mTarget;
    QRectF mBounds;

    bool mBoundingRectAreaVisible;
    QPen mBoundingRectAreaPen;
    QBrush mBoundingRectAreaBrush;

    bool mHandlersIgnoreTransformations;
};

#endif // GRAPHICSITEMRESIZER_H
