#pragma once
#ifndef TETRA_META_METADATA_CPP
#define TETRA_META_METADATA_CPP

#include <cstddef>

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

  public:
    /**
     * Creates a MetaData instance.
     * @param ctor Factory function which creates an instance of the type.
     * @param dtor Function which destroys an instance of the type.
     * @param copy Function which copys an instance of the type.
     * @param size The size of the type.
     **/
    MetaData( CtorFctn ctor, DtorFctn dtor, CopyFctn copy, std::size_t size );

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

  private:
    const CtorFctn m_constructor;
    const DtorFctn m_destructor;
    const CopyFctn m_copy;
    const std::size_t m_size; 
};

} /* namespace meta */
} /* namespace tetra */

#endif
