#include "handleitem.h"
#include "handlerstrategies.h"

#include <QBrush>
#include <QCursor>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include <QtMath>

QHash<int, QWeakPointer<HandlerStrategy>> GraphicsItemScaler::HandleItem::Strategies;

GraphicsItemScaler::HandleItem::HandleItem(int attachmentFlags, const QRectF &rect, GraphicsItemScaler *scaler)
    : QGraphicsRectItem(rect, scaler)
    , mScaler(scaler)
    , mHandleMoveAsResize(false)
{
    mAttachmentFlags = cleanAttachment(attachmentFlags);

    setFlag(ItemIsSelectable);
    setAcceptedMouseButtons(Qt::LeftButton);

    setBrush(QBrush(Qt::black));
    setCursor(getCursor(mAttachmentFlags));

    mStrategy = getStrategy(mAttachmentFlags);
}

GraphicsItemScaler::HandleItem::HandleItem(int attachmentFlags, const QSizeF &size, GraphicsItemScaler *scaler)
    : HandleItem(attachmentFlags, handlerRect(attachmentFlags, size), scaler)

{

}

void GraphicsItemScaler::HandleItem::alignPosition(const QRectF &resizerRect)
{
    QPointF newPos = resizerRect.center();
    mStrategy->alignHandlerPosition(&newPos, resizerRect);
    setPos(newPos);
}

GraphicsItemScaler *GraphicsItemScaler::HandleItem::scaler() const
{
    return mScaler;
}

QRectF GraphicsItemScaler::HandleItem::handlerRect(int attachment, const QSizeF& size) const
{
    double w = size.width();
    double h = size.height();
    double x = 0;
    double y = 0;

    // Handler is on the center of left or right side
    if ((attachment & HandleItem::VerticalMask) == 0)
        y = -h/2;
    // Handler is on the center of bottom or top side
    if ((attachment & HandleItem::HorizontalMask) == 0)
        x = -w/2;

    if (attachment & HandleItem::Left)
        x = -w;
    if (attachment & HandleItem::Top)
        y = -h;

    return QRectF(x, y, w, h);
}

int GraphicsItemScaler::HandleItem::cleanAttachment(int attachment)
{
    Q_ASSERT(attachment != 0);
    int valid = 0;
    if (attachment & Left)
    {
        valid |= Left;
    }
    else if (attachment & Right)
    {
        valid |= Right;
    }

    // Vertical
    if (attachment & Top)
    {
        valid |= Top;
    }
    else if (attachment & Bottom)
    {
        valid |= Bottom;
    }
    Q_ASSERT(valid != 0);
    return valid;
}

Qt::CursorShape GraphicsItemScaler::HandleItem::getCursor(int attachment)
{
    if ((attachment & VerticalMask) == 0)
    {
        return Qt::SizeHorCursor;
    }
    if ((attachment & HorizontalMask) == 0)
    {
        return Qt::SizeVerCursor;
    }

    if (attachment == (Left | Top) || attachment == (Right | Bottom))
    {
        return Qt::SizeFDiagCursor;
    }

    return Qt::SizeBDiagCursor;
}

QSharedPointer<HandlerStrategy> GraphicsItemScaler::HandleItem::getStrategy(int attachment)
{
    if (attachment == 0)
    {
        return HandlerStrategyPointer();
    }

    QWeakPointer<HandlerStrategy> &weak = Strategies[attachment];
    HandlerStrategyPointer strategy = weak.toStrongRef();
    if (strategy.isNull())
    {
        HandlerStrategy *s = nullptr;
        // Horizontal
        if (attachment & Left)
        {
            s = new LeftHandlerStrategy();
        }
        else if (attachment & Right)
        {
            s = new RightHandlerStrategy();
        }

        // Vertical
        if (attachment & Top)
        {
            s = new TopHandlerStrategy(s);
        }
        else if (attachment & Bottom)
        {
            s = new BottomHandlerStrategy(s);
        }
        Q_ASSERT(s != nullptr);
        strategy.reset(s);
        weak = strategy;
    }

    return strategy;
}


void GraphicsItemScaler::HandleItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    mHandleMoveAsResize = true;
    event->accept();
}

void GraphicsItemScaler::HandleItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (!mHandleMoveAsResize)
    {
        event->ignore();
        return;
    }

    auto offset   = event->scenePos() - event->lastScenePos();
    auto pointPos = HandlerStrategy::PointPosition(event->pos(), boundingRect());

    mStrategy->solveConstraints(scaler(), pointPos, offset);
}

void GraphicsItemScaler::HandleItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    event->setAccepted(mHandleMoveAsResize);
    mHandleMoveAsResize = false;
}
