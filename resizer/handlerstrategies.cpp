#include "handlerstrategies.h"
#include <QtMath>
#include <QDebug>
#include <QTransform>

void HandlerStrategy::scaleTargetItem(GraphicsItemResizer *resizer, const HandlerStrategy::PointPosition &pos, const QPointF &offset) const
{
    Q_ASSERT(resizer);

    if (mNext != nullptr)
        mNext->scaleTargetItem(resizer, pos, offset);
}

void HandlerStrategy::alignHandlerPosition(QPointF *position, const QRectF &boundingRect) const
{
    Q_ASSERT(position);

    if (mNext != nullptr)
        mNext->alignHandlerPosition(position, boundingRect);
}

HandlerStrategy::~HandlerStrategy()
{
    delete mNext;
    mNext = nullptr;
}

void TopHandlerStrategy::scaleTargetItem(GraphicsItemResizer* resizer, const HandlerStrategy::PointPosition &pos, const QPointF &offset) const
{
    Q_ASSERT(resizer);
    Q_ASSERT(resizer->target());

    if (offset.y() >= 0 || !pos.yBelow())
    {
        auto target  = resizer->target();

        auto newRect = target->boundingRect().adjusted(0, offset.y(), 0, 0);

        Q_ASSERT(target->boundingRect().height());
        auto scaleFactor    = newRect.height() / target->boundingRect().height();
        auto scaleTransform = target->transform().scale(1, scaleFactor);

        target->setTransform(scaleTransform);

        auto heightBefore = resizer->boundingRect().height();
        resizer->recalculate();
        auto heightAfter  = resizer->boundingRect().height();

        resizer->setPos(resizer->pos() + QPointF(0, heightBefore - heightAfter));
    }

    HandlerStrategy::scaleTargetItem(resizer, pos, offset);
}

void TopHandlerStrategy::alignHandlerPosition(QPointF *position, const QRectF &boundingRect) const
{
    Q_ASSERT(position);

    position->setY(boundingRect.top());
    HandlerStrategy::alignHandlerPosition(position, boundingRect);
}

void BottomHandlerStrategy::scaleTargetItem(GraphicsItemResizer* resizer, const HandlerStrategy::PointPosition &pos, const QPointF &offset) const
{
    Q_ASSERT(resizer);
    Q_ASSERT(resizer->target());

    if (offset.y() < 0 || !pos.yAbove())
    {
        auto target  = resizer->target();

        auto newRect = target->boundingRect().adjusted(0, 0, 0, offset.y());

        Q_ASSERT(target->boundingRect().height());
        auto scaleFactor    = newRect.height() / target->boundingRect().height();
        auto scaleTransform = target->transform().scale(1, scaleFactor);

        target->setTransform(scaleTransform);

        resizer->recalculate();
    }

    HandlerStrategy::scaleTargetItem(resizer, pos, offset);
}

void BottomHandlerStrategy::alignHandlerPosition(QPointF *position, const QRectF &boundingRect) const
{
    Q_ASSERT(position);

    position->setY(boundingRect.bottom());
    HandlerStrategy::alignHandlerPosition(position, boundingRect);
}

void LeftHandlerStrategy::scaleTargetItem(GraphicsItemResizer* resizer, const HandlerStrategy::PointPosition &pos, const QPointF &offset) const
{
    Q_ASSERT(resizer);
    Q_ASSERT(resizer->target());

    if (offset.x() >= 0 || !pos.xToTheRight())
    {
        auto target  = resizer->target();

        auto newRect = target->boundingRect().adjusted(offset.x(), 0, 0, 0);

        Q_ASSERT(target->boundingRect().width());
        auto scaleFactor    = newRect.width() / target->boundingRect().width();
        auto scaleTransform = target->transform().scale(scaleFactor, 1);

        target->setTransform(scaleTransform);

        auto widthBefore = resizer->boundingRect().width();
        resizer->recalculate();
        auto widthAfter  = resizer->boundingRect().width();

        resizer->setPos(resizer->pos() + QPointF(widthBefore - widthAfter, 0));
    }

    HandlerStrategy::scaleTargetItem(resizer, pos, offset);
}

void LeftHandlerStrategy::alignHandlerPosition(QPointF *position, const QRectF &boundingRect) const
{
    Q_ASSERT(position);

    position->setX(boundingRect.left());
    HandlerStrategy::alignHandlerPosition(position, boundingRect);
}

void RightHandlerStrategy::scaleTargetItem(GraphicsItemResizer* resizer, const HandlerStrategy::PointPosition &pos, const QPointF &offset) const
{
    Q_ASSERT(resizer);
    Q_ASSERT(resizer->target());

    if (offset.x() < 0 || !pos.xToTheLeft())
    {
        auto target  = resizer->target();

        auto newRect = target->boundingRect().adjusted(0, 0, offset.x(), 0);

        Q_ASSERT(target->boundingRect().width());
        auto scaleFactor    = newRect.width() / target->boundingRect().width();
        auto scaleTransform = target->transform().scale(scaleFactor, 1);

        target->setTransform(scaleTransform);

        resizer->recalculate();
    }

    HandlerStrategy::scaleTargetItem(resizer, pos, offset);
}

void RightHandlerStrategy::alignHandlerPosition(QPointF *position, const QRectF &boundingRect) const
{
    Q_ASSERT(position);

    position->setX(boundingRect.right());
    HandlerStrategy::alignHandlerPosition(position, boundingRect);
}

HandlerStrategy::PointPosition::PointPosition(const QPointF &p, const QRectF &bounds)
    : mX(getRelation(p.x(), bounds.left(), bounds.right()))
    , mY(getRelation(p.y(), bounds.top(), bounds.bottom()))
{
}

bool HandlerStrategy::PointPosition::xToTheLeft() const
{
    return mX == OutsideBefore;
}

bool HandlerStrategy::PointPosition::xToTheRight() const
{
    return mX == OutsideAfter;
}

bool HandlerStrategy::PointPosition::yAbove() const
{
    return mY == OutsideBefore;
}

bool HandlerStrategy::PointPosition::yBelow() const
{
    return mY == OutsideAfter;
}

HandlerStrategy::PointPosition::Relation HandlerStrategy::PointPosition::getRelation(qreal x, qreal left, qreal right)
{
    if (x < left)
    {
        return OutsideBefore;
    }
    if (x > right)
    {
        return OutsideAfter;
    }
    return Inside;
}


