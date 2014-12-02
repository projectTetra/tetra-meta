#pragma once
#ifndef TETRA_META_METADATA_HPP
#define TETRA_META_METADATA_HPP

#include <typeinfo>
#include <string>

namespace tetra
{
namespace meta
{

/**
 * Holds function pointers which facilitate the safe construction and
 * destruction of a given class. Instances are unique for given types.
 **/
class MetaData
{
  using MetaConstructor = void* ( * )();
  using MetaDestructor = void ( * )( void* );

  const MetaConstructor typeConstructor;
  const MetaDestructor typeDestructor;

public:
  /**
   * Creates a MetaData instance to represent the templated
   * type.
   * @templateParam The type to be represented
   * @return MetaData for the template parameter, has program-life
   *         lifetime, and is unique for this type.
   **/
  template <typename T>
  static const MetaData& create()
  {
    static MetaData metaData( metaConstructor<T>, metaDestructor<T> );
    return metaData;
  }

  /**
   * MetaData cannot be copied or moved, instances returned by
   * the create method are unique.
   **/
  MetaData( const MetaData& metaData ) = delete;
  MetaData& operator=( const MetaData& metaData ) = delete;
  
  /**
   * Compares two instances of MetaData, true only if they
   * describe the same type.
   **/
  bool operator==( const MetaData& metaData ) const noexcept;

  /**
   * Constructs an instance of the class that this MetaData
   * represents.
   * @return Unmanaged pointer to the freestore allocated instance
   **/
  void* constructInstance() const noexcept;

  /**
   * Destroys a freestore allocated instance of the class that
   * this MetaData represents.
   * @param obj An unmanaged pointer to the instance to be destroyed
   **/
  void destroyInstance( void* obj ) const noexcept;

private:
  MetaData( MetaConstructor constructor, MetaDestructor destructor );

  template <typename T>
  static void* metaConstructor()
  {
    return reinterpret_cast<void*>( new T{} );
  }

  template <typename T>
  static void metaDestructor( void* obj )
  {
    delete reinterpret_cast<T*>( obj );
  }
};

} /* namespace meta */
} /* namespace tetra */

#endif
