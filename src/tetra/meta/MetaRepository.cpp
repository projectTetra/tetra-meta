#include <tetra/meta/MetaRepository.hpp>

#include <json/json.h>

using namespace std;
using namespace tetra;
using namespace tetra::meta;

TypeNotRegisteredException::TypeNotRegisteredException()
  : runtime_error{"Type not registered!"}
  , typeName{"not provided"}
{ }

TypeNotRegisteredException::TypeNotRegisteredException(
  const string& typeName )
  : runtime_error{"Type, " + typeName + ", not registered!"}
  , typeName{typeName}
{ }

const std::string& TypeNotRegisteredException::getTypeName() const
  noexcept
{
  return typeName;
}

MetaRepository::MetaRepository()
{
#define add( T ) addType<T>( #T );
  add(char);
  add(int);
  add(float);
  add(double);
  add(string);
  add(wstring);
#undef add
}

bool MetaRepository::serialize( const Variant& obj,
                                Json::Value& root ) const
{
  string typeName = getTypeName( obj.getMetaData() );

  Json::Value object{};
  bool success = obj.serialize( object );

  if ( !success ) return false; // do nothing to root when failed

  root["type"] = typeName;
  root["object"] = object;

  return true;
}

Variant MetaRepository::deserialize( Json::Value& root ) const
{
  string typeName = root.get( "type", "" ).asString();
  const MetaData& typeMetaData = getMetaData( typeName );
  Json::Value object = root.get( "object", 0 );

  if ( !object.isObject() ) throw TypeNotRegisteredException{};

  Variant var{ typeMetaData };
  var.deserialize( object );

  return var;
}

const MetaData&
MetaRepository::getMetaData( const string& typeName ) const
{
  auto iter = nameToMetaMap.find( typeName );
  if ( iter == nameToMetaMap.end() )
    throw TypeNotRegisteredException{typeName};

  return *( iter->second );
}

const string& MetaRepository::getTypeName( const MetaData& metaData ) const
{
  auto iter = metaToNameMap.find(&metaData);
  if ( iter == metaToNameMap.end() )
    throw TypeNotRegisteredException{};

  return iter->second;
}

Variant MetaRepository::createInstance( const string& typeName ) const
{
  return {getMetaData( typeName )};
}

void MetaRepository::addType( const MetaData& metaData,
                              const string& typeName )
{
  if ( metaToNameMap.find( &metaData ) != metaToNameMap.end() )
    return;

  metaToNameMap[&metaData] = typeName;
  nameToMetaMap[typeName] = &metaData;
}

