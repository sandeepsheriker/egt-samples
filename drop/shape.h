/*
 * Copyright (C) 2018 Microchip Technology Inc.  All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef SHAPE_H
#define SHAPE_H

#include <Box2D/Box2D.h>
#include <cairo.h>
#include <egt/ui>
#include <iostream>
#include <map>

using namespace egt;
using namespace std;

inline double to_meter(double x)
{
    return x * .01;
}

inline double from_meter(double x)
{
    return x * 100.;
}

/**
 * A special widget that supports rotating a rectangle within it.
 */
class RotateRectangleWidget : public Widget
{
public:

    explicit RotateRectangleWidget(const Color& fill)
    {
	set_border(2);
	set_theme(theme());
	set_color(Palette::ColorId::bg, fill);
    }

    void set_angle(double angle)
    {
      if (detail::change_if_diff<>(m_angle, angle))
	damage();
    }

    virtual void draw(Painter& painter, const Rect&) override
    {
        auto cr = painter.context().get();
        cairo_translate(cr, center().x, center().y);
        cairo_rotate(cr, m_angle);
        cairo_translate(cr, -center().x, -center().y);

	Rect drawbox(Point(), m_shapesize);
	drawbox.move_to_center(center());

	theme().draw_box(painter,
			 Theme::boxtype::blank | Theme::boxtype::solid,
			 drawbox,
			 color(Palette::ColorId::border),
			 color(Palette::ColorId::bg),
			 border(),
			 margin());
    }

protected:
    double m_angle{0.0};
    Size m_shapesize;
};

class Shape : public RotateRectangleWidget
{
public:
    Shape(b2World& world, const Point& point);

    std::unique_ptr<Widget> clone()
    {
        return std::unique_ptr<Widget>();
    }

    void update();

    bool is_alive() const
    {
        return true;
    }

    void toss()
    {
	m_body->SetLinearVelocity(b2Vec2(6,2));
	m_body->SetAngularVelocity(-4.);
    }

    virtual ~Shape();

protected:

    b2World& m_world;

    b2Body* createBoxElement(const b2Vec2& size,
                             const b2Vec2& position);

private:

    b2Body* m_body;
};

#endif
