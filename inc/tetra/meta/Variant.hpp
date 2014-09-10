#pragma once
#ifndef TETRA_META_VARIANT_HPP
#define TETRA_META_VARIANT_HPP

#include <tetra/meta/MetaConstructor.hpp>
#include <memory>

namespace tetra
{
namespace meta
{

class TypeMismatchError : public std::runtime_error
{
  public:
    inline TypeMismatchError() : std::runtime_error{"TypeMismatchError"}
    { }
};

/**
 * The Variant class is able to safely hold any type which is registered with
 * the MetaData system.
 **/
class Variant
{
  private:
    /**
     * The deleter is responsible for correctly calling a type's destructor
     * using its MetaData. (Used by the std::unique_ptr)
     **/
    struct Deleter
    {
        Deleter() = default;
        Deleter( const Deleter& ) = default;
        Deleter( const MetaData& m );

        void operator()( void* p ) const;

        const MetaData* m_metaData;
    };
    using VPtr = std::unique_ptr<void, Deleter>;

  public:
    /**
     * Creates a variant which holds a T.
     **/
    template <typename T>
    static Variant createVariant()
    {
        return Variant{metaData<T>()};
    }

    /**
     * Creates a Variant which holds an instance of the object with the type
     * name provided.
     * @param name Passed to the metaData(string) method to lookup a type's
     *             MetaData.
     **/
    static Variant createVariant( const std::string& name );

  public:
    Variant( const Variant& );
    Variant& operator=( const Variant& );

    Variant( Variant&& ) = default;
    Variant& operator=( Variant&& ) = default;

    /**
     * Retrieves a reference to the Variant's payload, safely cast to the type
     * specified.
     * @templateParam T The type to cast the payload reference to.
     * @throws TypeMismatchError If the payload type does not match T's type.
     **/
    template <class T>
    T& get()
    {
        const MetaData& meta = metaData<T>();
        if ( &meta != m_metaData ) throw TypeMismatchError{};

        return *reinterpret_cast<T*>( m_payload.get() );
    }

    /**
     * Return's the current type's MetaData.
     **/
    const MetaData& metaData() const noexcept;

  private:
    Variant( const MetaData& meta );

  private:
    VPtr m_payload;
    const MetaData* m_metaData;
};

} /* namespace meta */
} /* namespace tetra */

#endif
