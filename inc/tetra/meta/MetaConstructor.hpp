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
    using MetaType = T;

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
        static MetaData data{&metaConstruct<T>, &metaDestroy<T>, &metaCopy<T>,
                             sizeof( T ),       getMembers()};
        return data;
    }

  private:
    /**
     * Used during construction of the MetaData instance. Defined by the
     * META_REGISTER macro.
     **/
    static std::map<std::string, MetaData::Member> getMembers();
};

/**
 * Helper function which uses the MetaConstructor to retrieve a unique instance
 * of a class's MetaData.
 **/
template <class T>
const MetaData& metaData()
{
    return MetaConstructor<T>::getMetaData();
}

/**
 * Helper function used in the META_MEMBER macro for registering members.
 * @param mbr Pointer to member variable of some class.
 * @templateParam Type The class
 * @templateParam MemberType The type of the member
 * @return MetaData::Member structure describing the member.
 **/
template <class Type, class MemberType>
MetaData::Member constructMember( MemberType( Type::*mbr ) )
{
    auto offset = reinterpret_cast<std::size_t>(
        &( reinterpret_cast<Type*>( 0 )->*mbr ) );

    return MetaData::Member{offset, metaData<MemberType>()};
}

} /* namespace meta */
} /* namespace tetra */

#define TETRA_CONCAT_IMPL( x, y ) x##y
#define TETRA_CONCAT( x, y ) TETRA_CONCAT_IMPL( x, y )
#define TETRA_GENERATED_NAME TETRA_CONCAT( TETRA_GENERATED, __LINE__ )

/**
 * Registers a type with the MetaRepository so that its MetaData can be looked
 * up by name. This should only be called once per type (you will get a compile
 * fail which babbles about too many template specializations if you try to call
 * this more than once for a type). Basically, just stick this macro in your
 * type's implementation file (.cpp) and you should be in good shape.
 * The unit tests for the MetaConstructor and the MetaRepository both give
 * examples of how to use this macro.
 **/
#define BEGIN_META_REGISTER( type )                                            \
    tetra::meta::MetaConstructor<type> TETRA_GENERATED_NAME( #type );          \
    template <>                                                                \
    std::map<std::string, tetra::meta::MetaData::Member>                       \
    tetra::meta::MetaConstructor<type>::getMembers()                           \
    {                                                                          \
    return

#define END_META_REGISTER() ;};

#define META_MEMBER( name ) { #name, constructMember( &MetaType::name ) },

#define BASE_META_REGISTER( type )                                             \
    BEGIN_META_REGISTER( type )                                                \
    {                                                                          \
    }                                                                          \
    END_META_REGISTER()

#endif
