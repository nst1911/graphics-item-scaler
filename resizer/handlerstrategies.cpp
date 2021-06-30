#include "handlerstrategies.h"
#include <QtMath>
#include <QDebug>
#include <QTransform>

void HandlerStrategy::solveConstraints(GraphicsItemResizer *resizer, const HandlerStrategy::PointPosition &pos, const QPointF &offset) const
{
    Q_ASSERT(resizer);

    if (mNext != nullptr)
        mNext->solveConstraints(resizer, pos, offset);
}

void HandlerStrategy::alignHandlerPosition(QPointF *position, const QRectF &boundingRect) const
{
    Q_ASSERT(position);

    if (mNext != nullptr)
        mNext->alignHandlerPosition(position, boundingRect);
}

void HandlerStrategy::scaleTargetItem(GraphicsItemResizer *resizer, const QMarginsF &margins) const
{
    auto moveResizer = !qFuzzyIsNull(margins.left()) || !qFuzzyIsNull(margins.top());
    auto horizontal  = !qFuzzyIsNull(margins.left()) || !qFuzzyIsNull(margins.right());
    auto vertical    = !qFuzzyIsNull(margins.top()) || !qFuzzyIsNull(margins.bottom());


    if ((!horizontal && !vertical) || (horizontal && vertical))
        return;

    auto target = resizer->target();

    if (!target)
        return;

    auto oldRect = target->boundingRect();
    auto newRect = oldRect + margins;

    // To avoid dividing to 0
    if (qFuzzyIsNull(horizontal ? oldRect.width() : oldRect.height()))
        return;

    auto scaleCorrection = horizontal ? (newRect.width() / oldRect.width()) - 1
                                      : (newRect.height() / oldRect.height()) - 1;

    auto targetTransform = target->transform();
    auto m11 = horizontal ? (targetTransform.m11() + scaleCorrection) : targetTransform.m11();
    auto m12 = targetTransform.m12();
    auto m13 = targetTransform.m13();
    auto m21 = targetTransform.m21();
    auto m22 = vertical ? (targetTransform.m22() + scaleCorrection) : targetTransform.m22();
    auto m23 = targetTransform.m23();
    auto m31 = targetTransform.m31();
    auto m32 = targetTransform.m32();
    auto m33 = targetTransform.m33();

    if (m11 < 0 || m22 < 0) return; // To avoid a scaling direction inversion

    target->setTransform(QTransform(m11, m12, m13, m21, m22, m23, m31, m32, m33));

    if (moveResizer)
    {
        auto before = resizer->boundingRect().size();
        resizer->recalculate();
        auto after  = resizer->boundingRect().size();

        auto diff = QPointF(before.width() - after.width(), before.height() - after.height());
        resizer->setPos(resizer->pos() + diff);
    }
    else
        resizer->recalculate();
}

HandlerStrategy::~HandlerStrategy()
{
    delete mNext;
    mNext = nullptr;
}

void TopHandlerStrategy::solveConstraints(GraphicsItemResizer* resizer, const HandlerStrategy::PointPosition &pos, const QPointF &offset) const
{
    Q_ASSERT(resizer);
    Q_ASSERT(resizer->target());

    if (offset.y() >= 0 || !pos.yBelow())
        scaleTargetItem(resizer, QMarginsF(0, -offset.y(), 0, 0));

    HandlerStrategy::solveConstraints(resizer, pos, offset);
}

void TopHandlerStrategy::alignHandlerPosition(QPointF *position, const QRectF &boundingRect) const
{
    Q_ASSERT(position);

    position->setY(boundingRect.top());
    HandlerStrategy::alignHandlerPosition(position, boundingRect);
}

void BottomHandlerStrategy::solveConstraints(GraphicsItemResizer* resizer, const HandlerStrategy::PointPosition &pos, const QPointF &offset) const
{
    Q_ASSERT(resizer);
    Q_ASSERT(resizer->target());

    if (offset.y() < 0 || !pos.yAbove())
        scaleTargetItem(resizer, QMarginsF(0, 0, 0, offset.y()));

    HandlerStrategy::solveConstraints(resizer, pos, offset);
}

void BottomHandlerStrategy::alignHandlerPosition(QPointF *position, const QRectF &boundingRect) const
{
    Q_ASSERT(position);

    position->setY(boundingRect.bottom());
    HandlerStrategy::alignHandlerPosition(position, boundingRect);
}

void LeftHandlerStrategy::solveConstraints(GraphicsItemResizer* resizer, const HandlerStrategy::PointPosition &pos, const QPointF &offset) const
{
    Q_ASSERT(resizer);

    if (offset.x() >= 0 || !pos.xToTheRight())
        scaleTargetItem(resizer, QMarginsF(-offset.x(), 0, 0, 0));

    HandlerStrategy::solveConstraints(resizer, pos, offset);
}

void LeftHandlerStrategy::alignHandlerPosition(QPointF *position, const QRectF &boundingRect) const
{
    Q_ASSERT(position);

    position->setX(boundingRect.left());
    HandlerStrategy::alignHandlerPosition(position, boundingRect);
}

void RightHandlerStrategy::solveConstraints(GraphicsItemResizer* resizer, const HandlerStrategy::PointPosition &pos, const QPointF &offset) const
{
    Q_ASSERT(resizer);

    if (offset.x() < 0 || !pos.xToTheLeft())
        scaleTargetItem(resizer, QMarginsF(0, 0, offset.x(), 0));

    HandlerStrategy::solveConstraints(resizer, pos, offset);
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


