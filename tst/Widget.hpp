#pragma once
#ifndef TETRA_META_TEST_WIDGET_HPP
#define TETRA_META_TEST_WIDGET_HPP

/**
 * Used by the various MetaData tests.
 **/
class Widget
{
  static int instanceCount;

public:
  static int getInstanceCount();

public:
  Widget();
  ~Widget();
};

#endif
