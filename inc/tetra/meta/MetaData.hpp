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
 * Holds MetaData for a class including the class's name, the
 * class's size, and methods for constructing/destroying instances
 * of the class.
 **/
class MetaData
{
  using MetaConstructor = void* ( * )();
  using MetaDestructor = void ( * )( void* );

  const std::string typeName;
  const MetaConstructor typeConstructor;
  const MetaDestructor typeDestructor;
  const std::type_info& typeInfo;

public:
  /**
   * Creates a MetaData instance to represent the templated
   * type.
   * @templateParam The type to be represented
   * @return MetaData for the template parameter
   **/
  template <typename T>
  static MetaData create( const std::string& typeName )
  {
    return MetaData( metaConstructor<T>, metaDestructor<T>,
                     typeid( T ), typeName );
  }

  bool operator==(const MetaData& metaData) const noexcept;

  const std::string& getTypeName() const noexcept;

  const std::type_info& getTypeInfo() const noexcept;

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
  MetaData( MetaConstructor constructor, MetaDestructor destructor,
            const std::type_info& typeInfo,
            const std::string& typeName );

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
