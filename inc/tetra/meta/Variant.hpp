#pragma once
#ifndef TETRA_META_VARIANT_HPP
#define TETRA_META_VARIANT_HPP

#include <tetra/meta/MetaData.hpp>

#include <stdexcept>

namespace tetra
{
namespace meta
{

/**
 * Thrown by the Variant when there is a type mismatch of some sort.
 **/
class TypeCastException : public std::runtime_error
{
public:
  inline TypeCastException()
    : std::runtime_error{"Error while attempting to cast types!"}
  { }
};

/**
 * Uses MetaData to safely hold a freestore allocated instance
 * of the class that the MetaData describes.
 **/
class Variant
{
  const MetaData* metaData{nullptr};
  void* pObj{nullptr};

public:
  /**
   * Creates a new Variant which holds an instance of the class
   * that the provided MetaData describes.
   **/
  Variant( const MetaData& metaData ) noexcept;

  /**
   * Creates a payload object using the inferred type's MetaData,
   * then stores the given object into the payload. Only works
   * if the type has a copy or move operator.
   * @param toStore The object to store.
   **/
  template <typename T>
  static Variant create( T&& toStore ) noexcept
  {
    Variant v{MetaData::create<T>()};
    v.getObject<T>() = toStore;

    return v;
  }

  /**
   * Safely deletes the object using its MetaData.
   **/
  ~Variant();

  /**
   * Variants are not copyable, if you must copy
   * a variant then use the Variant::create method along
   * with the getObject method. (e.g. create(v.getObject()))
   **/
  Variant( const Variant& ) = delete;
  Variant& operator=( const Variant& ) = delete;

  /**
   * Variants are moveable, the object that is left behind
   * holds no data and will always throw when getObject is called.
   **/
  Variant( Variant&& variant ) noexcept;
  Variant& operator=( Variant&& variant ) noexcept;

  /**
   * Returns the MetaData which describes this Variant's payload.
   **/
  const MetaData& getMetaData() const noexcept;

  /**
   * Safely casts the Variant's payload to the type requested
   * and returns a reference to it.
   * @throws TypeCastException if the type requested is incompatable
   *         with the type of the Variant's payload.
   * @templateParam T The type to cast the payload to.
   * @return Reference to the Variant's payload, cast to the templated
   *         type
   **/
  template <typename T>
  T& getObject() const throw( TypeCastException )
  {
    if ( metaData != &MetaData::create<T>() )
    {
      throw TypeCastException{};
    }

    return *reinterpret_cast<T*>( pObj );
  }
};

} /* namespace meta */
} /* namespace tetra */

#endif