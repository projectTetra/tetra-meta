#include <tetra/meta/MetaRepository.hpp>

using namespace std;
using namespace tetra;
using namespace tetra::meta;

const MetaData& tetra::meta::metaData( const string& typeName )
{
    return MetaRepository::lookupMetaData( typeName );
}

void MetaRepository::addType( const string& name,
                              const MetaData& metaData ) noexcept
{
    getMetaDataMap().emplace( name, &metaData );
}

const MetaData& MetaRepository::lookupMetaData( const string& name )
{
    return *getMetaDataMap().at( name );
}

unordered_map<string, const MetaData*>& MetaRepository::getMetaDataMap()
{
    static unordered_map<string, const MetaData*> metaMap;
    return metaMap;
}
