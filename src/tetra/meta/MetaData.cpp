#include "tetra/meta/MetaData.hpp"

#include <iostream>

using namespace tetra;
using namespace tetra::meta;

MetaData::MetaData( MetaConstructor constructor,
                    MetaDestructor destructor, MetaCopy copy )
  : typeCopy{copy}
  , typeConstructor{constructor}
  , typeDestructor{destructor}
{
}

MetaData::MetaData( MetaConstructor constructor,
                    MetaDestructor destructor, MetaCopy copy,
                    MetaSerializer serializer,
                    MetaDeserializer deserializer )
  : supportsSerialization{true}
  , typeCopy{copy}
  , typeConstructor{constructor}
  , typeDestructor{destructor}
  , typeSerializer{serializer}
  , typeDeserializer{deserializer}
{
}

bool MetaData::operator==( const MetaData& metaData ) const noexcept
{
  return this == &metaData;
}

void* MetaData::constructInstance() const
{
  return this->typeConstructor();
}

void MetaData::destroyInstance( void* obj ) const noexcept
{
  this->typeDestructor( obj );
}

void MetaData::copyInstance( void* lhs, void* rhs ) const noexcept
{
  this->typeCopy( lhs, rhs );
}

bool MetaData::canSerialize() const noexcept
{
  return this->supportsSerialization;
}

bool MetaData::serializeInstance( void* obj, Json::Value& root ) const
{
  return this->typeSerializer( obj, root );
}

bool MetaData::deserializeInstance( void* obj,
                                    Json::Value& root ) const
{
  return this->typeDeserializer( obj, root );
}
