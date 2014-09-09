#include <tetra/meta/MetaData.hpp>
#include <iostream>

using namespace std;
using namespace tetra;
using namespace tetra::meta;

std::ostream& operator<<( std::ostream& out, const MetaData& metaData )
{
    out << metaData.size();
    return out;
}

MetaData::MetaData( CtorFctn ctor, DtorFctn dtor, CopyFctn copy, size_t size,
                    const map<string, Member>& members )
    : m_constructor{ctor}
    , m_destructor{dtor}
    , m_copy{copy}
    , m_size{size}
    , m_members{members} {};

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

vector<string> MetaData::members() const noexcept
{
    auto keys = vector<string>{};
    for ( const auto& member : m_members )
    {
        keys.push_back( member.first );
    }

    return keys;
}



