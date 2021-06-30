# QGraphicsItem interactive scaling class

`GraphicsItemScaler` is a `QGraphicsItem`-based container class for interactive scaling of 
`QGraphicsItem`s with 8 handle items for each direction (left-top, top, right-top, right, 
right-bottom, bottom, left-bottom, left). 

### How to use it

Copy `scaler` dir to your project. Include file `scaler/graphicsitemscaler.h`.
Create instance of `GraphicsItemScaler` class and set your target graphics item as a parent.

You can change pen and brush for handle items using `setHandleItemPen` and 
`setHandleItemBrush` methods. Pen and brush of container item bounding rect area can 
be changed with `setBoundingRectAreaPen` and `setBoundingRectAreaBrush` as well.

Use `setHandlersIgnoreTransformations` to force handler items ignore all transformations. 
For example, it would be useful when attached graphics views can be zoomed or rotated -
handlers will have the same size and relative position.

The example of using `GraphicsItemScaler` is in the constructor of `MainWindow` class in
 `mainwindow.cpp` file.

### License
Licensed under the Apache License, Version 2.0. See LICENSE for details.
