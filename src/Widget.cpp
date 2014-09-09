#include <Widget.hpp>

#include <tetra/meta/MetaConstructor.hpp>

BASE_META_REGISTER( Widget );

int Widget::count{0};

Widget::Widget()
{
    ++Widget::count;
}

Widget::~Widget()
{
    --Widget::count;
}
