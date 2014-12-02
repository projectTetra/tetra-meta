#include "tetra/meta/MetaData.hpp"

#include <iostream>

using namespace tetra;
using namespace tetra::meta;

MetaData::MetaData( MetaConstructor constructor,
                    MetaDestructor destructor )
  : typeConstructor{constructor}, typeDestructor{destructor}
{
}

bool MetaData::operator==( const MetaData& metaData ) const noexcept
{
  return this == &metaData;
}

void* MetaData::constructInstance() const noexcept
{
  return this->typeConstructor();
}

void MetaData::destroyInstance( void* obj ) const noexcept
{
  this->typeDestructor( obj );
}
