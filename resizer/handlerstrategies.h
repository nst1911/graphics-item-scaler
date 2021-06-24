#ifndef HANDLERSTRATEGIES_H
#define HANDLERSTRATEGIES_H

#include <QPointF>
#include <QRectF>
#include <QSizeF>
#include "graphicsitemresizer.h"

class HandlerStrategy
{
public:
    class PointPosition
    {
        enum Relation
        {
            Inside,
            OutsideBefore,
            OutsideAfter
        };
    public:
        PointPosition(const QPointF &p, const QRectF &bounds);
        bool xToTheLeft() const;
        bool xToTheRight() const;
        bool yAbove() const;
        bool yBelow() const;
    private:
        static Relation getRelation(qreal x, qreal left, qreal right);
        Relation mX;
        Relation mY;
    };

    enum TargetRectSide
    {
        Left,
        Right,
        Top,
        Bottom
    };

    explicit HandlerStrategy(HandlerStrategy *next = nullptr) : mNext(next) { }
    virtual ~HandlerStrategy();

    virtual void scaleTargetItem(GraphicsItemResizer* resizer, const PointPosition& pos, const QPointF& offset) const;
    virtual void alignHandlerPosition(QPointF *position, const QRectF &boundingRect) const;

protected:
    HandlerStrategy *mNext;
};

class TopHandlerStrategy : public HandlerStrategy
{
public:
    explicit TopHandlerStrategy(HandlerStrategy *next = nullptr) : HandlerStrategy(next) { }

    virtual void scaleTargetItem(GraphicsItemResizer* resizer, const PointPosition& pos, const QPointF& offset) const override;
    virtual void alignHandlerPosition(QPointF *position, const QRectF &boundingRect) const override;

};

class BottomHandlerStrategy : public HandlerStrategy
{
public:
    explicit BottomHandlerStrategy(HandlerStrategy *next = nullptr) : HandlerStrategy(next) { }

    virtual void scaleTargetItem(GraphicsItemResizer* resizer, const PointPosition& pos, const QPointF& offset) const override;
    virtual void alignHandlerPosition(QPointF *position, const QRectF &boundingRect) const override;
};

class LeftHandlerStrategy : public HandlerStrategy
{
public:
    explicit LeftHandlerStrategy(HandlerStrategy *next = nullptr) : HandlerStrategy(next) { }

    virtual void scaleTargetItem(GraphicsItemResizer* resizer, const PointPosition& pos, const QPointF& offset) const override;
    virtual void alignHandlerPosition(QPointF *position, const QRectF &boundingRect) const override;
};

class RightHandlerStrategy : public HandlerStrategy
{
public:
    explicit RightHandlerStrategy(HandlerStrategy *next = nullptr) : HandlerStrategy(next) { }

    virtual void scaleTargetItem(GraphicsItemResizer* resizer, const PointPosition& pos, const QPointF& offset) const override;
    virtual void alignHandlerPosition(QPointF *position, const QRectF &boundingRect) const override;
};

#endif // HANDLERSTRATEGIES_H
