#include "tetra/meta/MetaData.hpp"

#include <iostream>

using namespace tetra;
using namespace tetra::meta;

MetaData::MetaData( MetaConstructor constructor,
                    MetaDestructor destructor,
                    const std::type_info& typeInfo,
                    const std::string& typeName )
  : typeName{typeName}
  , typeConstructor{constructor}
  , typeDestructor{destructor}
  , typeInfo{typeInfo}
{
}

bool MetaData::operator==( const MetaData& metaData ) const noexcept
{
  return this->getTypeInfo() == metaData.getTypeInfo();
}

const std::string& MetaData::getTypeName() const noexcept
{
  return this->typeName;
}

const std::type_info& MetaData::getTypeInfo() const noexcept
{
  return this->typeInfo;
}

void* MetaData::constructInstance() const noexcept
{
  return this->typeConstructor();
}

void MetaData::destroyInstance( void* obj ) const noexcept
{
  this->typeDestructor( obj );
}
