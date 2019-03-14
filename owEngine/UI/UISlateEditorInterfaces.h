#pragma once

struct IUISlateConnectionable
{
    enum LineDefaultDirection
    {
        DirectionLeft,
        DirectionRight,
    };

    virtual vec2 GetConnectPoint() const = 0;
    virtual BoundingRect GetConnectRectangle() const = 0;
    virtual LineDefaultDirection GetConnectDirection() const = 0;
};