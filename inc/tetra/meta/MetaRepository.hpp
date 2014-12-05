#pragma once
#ifndef TETRA_META_METAREPOSITORY_HPP
#define TETRA_META_METAREPOSITORY_HPP

#include <tetra/meta/Variant.hpp>

#include <json/json-forwards.h>

#include <stdexcept>
#include <map>

namespace tetra
{
namespace meta
{

/**
 * Thrown by the MetaRepository when an action is taken on a type
 * which has not been added with the addType method.
 **/
class TypeNotRegisteredException : public std::runtime_error
{
  const std::string typeName;

public:
  TypeNotRegisteredException();
  TypeNotRegisteredException( const std::string& typeName );

  const std::string& getTypeName() const noexcept;
};

/**
 * Maintains a mapping of MetaData to plain-text class names.
 **/
class MetaRepository
{
  std::map<std::string, const MetaData*> nameToMetaMap;
  std::map<const MetaData*, std::string> metaToNameMap;

public:
  /**
   * Adds MetaData for the basic primitive types: int, float, double,
   * char, string, wstring.
   **/
  MetaRepository();
  
  // Copy and Move operations are defaulted
  MetaRepository( const MetaRepository& ) = default;
  MetaRepository( MetaRepository&& ) = default;
  MetaRepository& operator=( const MetaRepository& ) = default;
  MetaRepository& operator=( MetaRepository&& ) = default;

  /**
   * Adds a new type to the MetaRepository, does nothing if T is
   * already registered.
   **/
  template <typename T>
  void addType( const std::string& typeName )
  {
    addType( MetaData::create<T>(), typeName );
  }
  
  /**
   * Returns MetaData for the type which was registered with the 
   * given typeName.
   * @throws TypeNotRegistered if the type was not registered.
   * @param typeName The name of the type to look up.
   * @return const ref to the MetaData for the type.
   **/
  const MetaData& getMetaData( const std::string& typeName ) const;

  /**
   * Returns the name for the registered type.
   * @throws TypeNotRegistered if the type was not registered.
   * @param metaData MetaData for the type.
   * @return The typename associated with the metaData.
   **/
  const std::string& getTypeName( const MetaData& metaData ) const;

  /**
   * Uses the registered MetaData to create an instance of the
   * Variant.
   * @throws TypeNotRegistered if the requested type has not been
   *         registered.
   * @param typeName The registered typeName of the type that you want
   *        to create.
   * @return A Variant containing an instance of the type that you
   *         requested.
   **/
  Variant createInstance( const std::string& typeName ) const;

  /**
   * Uses the Variant's serialize method to serialize the object into
   * the root node.
   * Even if the Variant does not support serialization, this will
   * still write the Variant type to the Json node.
   * @throws TypeNotRegistered if the Variant contains an unregistered
   *         type
   * @param obj The object to serialize.
   * @param root The Json::Value to serialize into.
   **/
  void serialize( const Variant& obj, Json::Value& root ) const;

  /**
   * Deserializes the object described by the Json::Value into a new
   * Variant. Throws a TypeNotRegistered exception if the
   * deserialization fails.
   * @throws TypeNotRegistered if deserialization fails.
   * @param root The Json::Value to deserialize from.
   * @return A variant containing the deserialized object.
   **/
  Variant deserialize( Json::Value& root ) const;

private:
  /**
   * Adds a new type to the MetaRepository, does nothing if T is
   * already registered.
   **/
  void addType( const MetaData& metaData,
                const std::string& typeName );
};

} /* namespace meta */
} /* namespace tetra */

#endif
