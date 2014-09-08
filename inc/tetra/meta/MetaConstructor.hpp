#pragma once
#ifndef TETRA_META_METACONSTRUCTOR_HPP
#define TETRA_META_METACONSTRUCTOR_HPP

#include <tetra/meta/MetaData.hpp>
#include <tetra/meta/MetaRepository.hpp>

namespace tetra
{
namespace meta
{

/**
 * Helper class which creates a single instance of MetaData for each type used.
 **/
template <class T>
class MetaConstructor
{
  public:
    /**
     * Register's the generated MetaData with the MetaRepository.
     * @param name The name of the type to add to the repository
     **/
    MetaConstructor( const std::string& name )
    {
        MetaRepository::addType( name, getMetaData() );
    }

    /**
     * Retrieve the MetaData associated with this type.
     * @return The MetaData for T
     **/
    static const MetaData& getMetaData() noexcept
    {
        static MetaData data{&metaConstruct<T>, &metaDestroy<T>, sizeof( T )};
        return data;
    }
};

template <class T>
const MetaData& meta()
{
    return MetaConstructor<T>::getMetaData();
}

} /* namespace meta */
} /* namespace tetra */

#define TETRA_CONCAT_IMPL( x, y ) x##y
#define TETRA_CONCAT( x, y ) TETRA_CONCAT_IMPL( x, y )
#define TETRA_GENERATED_NAME TETRA_CONCAT( TETRA_GENERATED, __LINE__ )

/**
 * Registers a type with the MetaRepository so that its MetaData can be looked
 * up by name. This should only be called once per type (no harm if it is called
 * to many times, though it could make the resulting executable larger).
 * Basically, just stick this macro in your type's implementation file (.cpp)
 * and you should be in good shape.
 **/
#define META_REGISTER( type )                                                  \
    MetaConstructor<type> TETRA_GENERATED_NAME( #type );

#endif
