#pragma once
#ifndef WIDGET_HPP
#define WIDGET_HPP

/**
 * Used in testing to verify that MetaData is properly constructed.
 **/
class Widget
{
  public:
    /**
     * Increments count by one.
     **/
    Widget();

    /**
     * Decrements count by one.
     **/
    ~Widget();

  public:
    static int count;
};

#endif
