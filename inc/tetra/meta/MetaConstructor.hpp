#pragma once
#ifndef TETRA_META_METACONSTRUCTOR_HPP
#define TETRA_META_METACONSTRUCTOR_HPP

#include <tetra/meta/MetaData.hpp>

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
     * Retrieve the MetaData associated with this type.
     * @return The MetaData for T
     **/
    static const MetaData& getMetaData() noexcept
    {
        static MetaData data{&metaConstruct<T>, &metaDestroy<T>, sizeof( T )};
        return data;
    }
};

} /* namespace meta */
} /* namespace tetra */

#endif
