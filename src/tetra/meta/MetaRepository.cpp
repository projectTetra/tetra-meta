#include <tetra/meta/MetaRepository.hpp>

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

