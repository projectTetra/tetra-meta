#pragma once
#ifndef TETRA_META_METAREPOSITORY_HPP
#define TETRA_META_METAREPOSITORY_HPP

#include <tetra/meta/Variant.hpp>

#include <stdexcept>
#include <unordered_map>

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

  const std::string& getTypeName();
};

/**
 * Maintains a mapping of MetaData to plain-text class names.
 **/
class MetaRepository
{
  std::unordered_map<std::string, const MetaData*> nameToMetaMap;
  std::unordered_map<const MetaData*, const std::string*>
    metaToNameMap;

public:
  /**
   * Adds MetaData for the basic primitive types: int, float, double,
   * char,std::string, std::wstring.
   **/
  MetaRepository();

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
  const MetaData& getMetaData( const std::string& typeName );

  /**
   * Returns the name for the registered type.
   * @throws TypeNotRegistered if the type was not registered.
   * @param metaData MetaData for the type.
   * @return The typename associated with the metaData.
   **/
  const std::string& getTypeName( const MetaData& metaData );

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
