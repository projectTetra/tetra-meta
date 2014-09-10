#include <tetra/meta/Variant.hpp>

using namespace std;
using namespace tetra;
using namespace tetra::meta;

Variant::Deleter::Deleter( const MetaData& m ) : m_metaData{&m}
{
}

void Variant::Deleter::operator()( void* p ) const
{
    m_metaData->destroy( p );
}

Variant Variant::createVariant( const string& name )
{
    return Variant{tetra::meta::metaData( name )};
}

Variant::Variant( const MetaData& meta )
    : m_payload{meta.construct(), Deleter{meta}}, m_metaData{&meta}
{}

Variant::Variant( const Variant& v ) : m_metaData{&v.metaData()}
{
    
}
