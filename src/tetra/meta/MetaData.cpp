#include <tetra/meta/MetaData.hpp>

using namespace std;
using namespace tetra;
using namespace tetra::meta;

MetaData::MetaData( CtorFctn ctor, DtorFctn dtor, CopyFctn copy, size_t size )
    : m_constructor{ctor}, m_destructor{dtor}, m_copy{copy}, m_size{size} {};

void* MetaData::construct() const
{
    return m_constructor();
}

void MetaData::destroy( void* object ) const noexcept
{
    m_destructor( object );
}

void MetaData::copy( void* lhs, void* rhs ) const
{
    m_copy( lhs, rhs );
}

size_t MetaData::size() const noexcept
{
    return m_size;
}

