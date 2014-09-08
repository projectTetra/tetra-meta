#include <tetra/meta/MetaData.hpp>

using namespace std;
using namespace tetra;
using namespace tetra::meta;

MetaData::MetaData( CtorFctn ctor, DtorFctn dtor, size_t size )
    : m_constructor{ctor}, m_destructor{dtor}, m_size{size} {};

void* MetaData::construct() const
{
    return m_constructor();
}

void MetaData::destroy( void* object ) const noexcept
{
    m_destructor( object );
}

size_t MetaData::size() const noexcept
{
    return m_size;
}

