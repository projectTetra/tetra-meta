#pragma once
#ifndef TETRA_META_VARIANT_HPP
#define TETRA_META_VARIANT_HPP

#include <tetra/meta/MetaData.hpp>

#include <json/json-forwards.h>

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
   * Creates an empty variant that always throws a TypeCastException
   * when getObject is called.
   **/
  Variant() = default;

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
    using Type = typename std::remove_reference<T>::type;
    Variant v{MetaData::get<Type>()};
    v.getObject<Type>() = std::forward<T&&>( toStore );

    return v;
  }

  /**
   * Safely deletes the object using its MetaData.
   **/
  ~Variant();

  /**
   * Variants do not support copy operations. (this avoid ambiguity as
   * moving a variant is fundamentally different from copyng the
   * contents of a variant.)
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
   * Copies the payload and type of the provided variant.
   * @param variant - The variant to copy from.
   **/
  void copy( const Variant& variant ) noexcept;

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
    if ( metaData != &MetaData::get<T>() )
    {
      throw TypeCastException{};
    }

    return *reinterpret_cast<T*>( pObj );
  }

  /**
   * Serializes the object into the Json::Value node.
   * If the object does not support serialization
   * (metaData.canSerialize() == false) then this is just a no-op.
   * @param root The root Json::Value node to serialize the object
   *             into.
   * @return false if the object does not support serialization,
   *         otherwise this is the value returned by the object's
   *         MetaData's serialize method.
   **/
  bool serialize( Json::Value& root ) const;

  /**
   * Deserializes the object from the Json::Value node.
   * If the object does not support serialization then this is a
   * no-op.
   * @param root The root Json::Value node to deserialize the
   *             object from.
   * @return false if the object does not support serialization,
   *         otherwise this is the value returned by the object's
   *         MetaData's deserialize method.
   **/
  bool deserialize( Json::Value& root );
};

} /* namespace meta */
} /* namespace tetra */

#endif
