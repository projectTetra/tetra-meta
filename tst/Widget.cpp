#include <Widget.hpp>

int Widget::instanceCount{0};

Widget::Widget()
{
  ++instanceCount;
}

Widget::~Widget()
{
  --instanceCount;
}

int Widget::getInstanceCount()
{
  return instanceCount;
}
