#pragma once
#ifndef TETRA_META_METAREPOSITORY_HPP
#define TETRA_META_METAREPOSITORY_HPP

#include <tetra/meta/MetaData.hpp>

#include <string>
#include <unordered_map>

namespace tetra
{
namespace meta
{

/**
 * The MetaRepository maintains a map of type-names to MetaData instances.
 * Actual pieces of MetaData should _only_ be inserted through the META_REGISTER
 * macro.
 **/
class MetaRepository
{
  public:
    /**
     * Adds a type and its MetaData to the repository.
     * @param name The name of the type, this can be used to look up the data
     *             later
     * @param metaData A non-owning pointer to the type's MetaData
     **/
    static void addType( const std::string& name,
                         const MetaData& metaData ) noexcept;

    /**
     * Looks up a type's MetaData using the name used to originally add the type.
     * @param name The name of the type to get MetaData for
     * @return A constant reference to the type's MetaData
     * @throws std::out_of_range if the name is not in the map.
     **/
    static const MetaData& lookupMetaData( const std::string& name );

  private:
    static std::unordered_map<std::string, const MetaData*>& getMetaDataMap();
};

} /* namespace meta */
} /* namespace tetra */

#endif
