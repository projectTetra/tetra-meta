#include <tetra/meta/Variant.hpp>

using namespace tetra;
using namespace tetra::meta;

Variant::Variant( const MetaData& metaData ) noexcept
  : metaData{&metaData},
    pObj{metaData.constructInstance()}
{
}

Variant::~Variant()
{
  if ( pObj != nullptr && metaData != nullptr )
  {
    metaData->destroyInstance( pObj );
    pObj = nullptr;
    metaData = nullptr;
  }
}

Variant::Variant( Variant&& variant ) noexcept
  : metaData{&variant.getMetaData()},
    pObj{variant.pObj}
{
  variant.metaData = nullptr;
  variant.pObj = nullptr;
}

Variant& Variant::operator=( Variant&& variant ) noexcept
{
  if ( pObj != nullptr && metaData != nullptr )
  {
    metaData->destroyInstance( pObj );
  }

  // in-place new to call the move c'tor
  new ( this ) Variant{std::move( variant )};

  return *this;
}

void Variant::copy( const Variant& variant ) noexcept
{
  if ( pObj != nullptr && metaData != nullptr )
  {
    metaData->destroyInstance( pObj );
  }

  metaData = variant.metaData;
  pObj     = metaData->constructInstance();

  metaData->copyInstance( pObj, variant.pObj );
}

bool Variant::serialize( Json::Value& root ) const
{
  if (!getMetaData().canSerialize())
    return false;

  return getMetaData().serializeInstance( this->pObj, root );
}

bool Variant::deserialize( Json::Value& root )
{
  if (!getMetaData().canSerialize())
    return false;

  return getMetaData().deserializeInstance( this->pObj, root );
}

const MetaData& Variant::getMetaData() const noexcept
{
  return *metaData;
}
