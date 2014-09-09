#pragma once
#ifndef TETRA_META_METADATA_CPP
#define TETRA_META_METADATA_CPP

#include <cstddef>
#include <map>
#include <vector>

namespace tetra
{
namespace meta
{

/**
 * Constructor function, uses type-erasure to hide the type beging constructed.
 * This is not safe to use outside of the MetaData class!
 * @templateParam T The type to be constructed
 * @return Pointer to the new instance of T
 **/
template <class T>
void* metaConstruct()
{
    return reinterpret_cast<void*>( new T );
}

/**
 * Destructor function, used type-erasure to hide the type being destroyed.
 * This is not safe to use outside fo teh MetaData class!
 * @templateParam T The type to be destroyed
 * @param object Pointer to the object to be destroyed.
 **/
template <class T>
void metaDestroy( void* object ) noexcept
{
    delete reinterpret_cast<T*>( object );
}

/**
 * Calls the class's copy operator, uses type-erasure to hide the type being
 * copied. This is not safe to use outside the MetaData class.
 * @templateParam T The type to copy
 * @param lhs The left hand side (object to be copied into)
 * @param rhs The right hand side (object to be copied from)
 **/
template <class T>
void metaCopy( void* lhs, void* rhs )
{
    *reinterpret_cast<T*>( lhs ) = *reinterpret_cast<T*>( rhs );
}

class MetaData
{
  public:
    using CtorFctn = void* ( * )();
    using DtorFctn = void ( * )( void* );
    using CopyFctn = void ( * )( void*, void* );
    using Offset = std::size_t;

    struct Member
    {
        Member() = default;
        inline Member( Offset o, const MetaData& meta )
            : offset{o}, metaData{&meta} {};

        Offset offset;
        const MetaData* metaData;
    };

  public:
    /**
     * Creates a MetaData instance.
     * @param ctor Factory function which creates an instance of the type.
     * @param dtor Function which destroys an instance of the type.
     * @param copy Function which copys an instance of the type.
     * @param size The size of the type.
     * @param members A map of names to Member structs describing this type's
     *                members.
     **/
    MetaData( CtorFctn ctor, DtorFctn dtor, CopyFctn copy, std::size_t size,
              const std::map<std::string, Member>& members = {} );

    /**
     * Calls the meta-data constructor, returns an unmanaged pointer to the
     * constructed object. This can only be safely destroyed by passing it to the
     * destructor function.
     * @return Pointer to the constructed object
     **/
    void* construct() const;

    /**
     * Calls the meta-data destructor. Assumes the pointer is to an object which
     * was created withis meta-data's constructor.
     * @param object Pointer to the object to be destroyed
     **/
    void destroy( void * object ) const noexcept;
   
    /**
     * Copies rhs into lhs.
     * @param lhs The object to be copied into.
     * @param rhs The object to be copied from.
     **/
    void copy( void* lhs, void* rhs ) const;

    /**
     * Returns the size of the type.
     **/
    std::size_t size() const noexcept;

    /**
     * Returns a vector of this type's member names.
     **/
    std::vector<std::string> members() const noexcept;

    /**
     * Retrieve information about a member.
     * @param name The name of the member to retrieve MetaData for.
     * @return The Member structure containing the member's offset and MetaData.
     * @throws MemberNotFound if the member is not present.
     **/
    Member member( const std::string& name ) const;

  private:
    const CtorFctn m_constructor;
    const DtorFctn m_destructor;
    const CopyFctn m_copy;
    const std::size_t m_size; 
    const std::map<std::string, Member> m_members;
};

/**
 * Thrown when a member lookup fails in the MetaData class.
 **/
class MemberNotFoundException : public std::runtime_error
{
  public:
    MemberNotFoundException( const std::string& memberName );

    const std::string& getMemberName() const noexcept;

  private:
    const std::string m_memberName;
};

} /* namespace meta */
} /* namespace tetra */

std::ostream& operator<<( std::ostream& out,
                          const tetra::meta::MetaData& metaData );

#endif
