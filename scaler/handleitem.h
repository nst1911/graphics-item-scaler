#ifndef HANDLEITEM_H
#define HANDLEITEM_H

#include "graphicsitemscaler.h"
#include <QGraphicsRectItem>

class HandlerStrategy;
class GraphicsItemScaler::HandleItem : public QGraphicsRectItem
{
    typedef QSharedPointer<HandlerStrategy> HandlerStrategyPointer;

public:
    enum AttachmentFlags
    {
        Left = 0x1,
        Top = 0x2,
        Bottom = 0x4,
        Right = 0x8
    };

    constexpr static int HorizontalMask = HandleItem::Left | HandleItem::Right;
    constexpr static int VerticalMask = HandleItem::Top | HandleItem::Bottom;

    explicit HandleItem(int attachmentFlags, const QRectF &rect, GraphicsItemScaler *scaler);
    explicit HandleItem(int attachmentFlags, const QSizeF &size, GraphicsItemScaler *scaler);

    int attachmentFlags() const
    {
        return mAttachmentFlags;
    }

    void alignPosition(const QRectF &resizerRect);

    // QGraphicsItem interface
protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

private:
    GraphicsItemScaler *scaler() const;

    QRectF handlerRect(int attachment, const QSizeF &size) const;

    GraphicsItemScaler *mScaler;
    int mAttachmentFlags;
    bool mHandleMoveAsResize;
    HandlerStrategyPointer mStrategy;

    static int cleanAttachment(int attachment);
    static Qt::CursorShape getCursor(int attachment);
    static HandlerStrategyPointer getStrategy(int attachment);
    static QHash<int, QWeakPointer<HandlerStrategy>> Strategies;
};

#endif // HANDLEITEM_H
