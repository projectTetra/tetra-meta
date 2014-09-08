#include <Widget.hpp>

int Widget::count{0};

Widget::Widget()
{
    ++Widget::count;
}

Widget::~Widget()
{
    --Widget::count;
}
