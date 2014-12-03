#pragma once
#ifndef TETRA_META_METADATA_HPP
#define TETRA_META_METADATA_HPP

#include <json/json-forwards.h>

#include <typeinfo>
#include <string>

namespace tetra
{
namespace meta
{

template <class T>
struct HasSerializer;

template <class T>
struct HasDeserializer;

/**
 * Holds function pointers which facilitate the safe construction and
 * destruction of a given class. Instances are unique for given types.
 **/
class MetaData
{
  using MetaConstructor = void* ( * )();
  using MetaDestructor = void ( * )( void* );
  using MetaSerializer = void ( * )( void*, Json::Value& );
  using MetaDeserializer = void (*) ( void*, Json::Value& );
  
  const bool supportsSerialization{false};
  const MetaConstructor typeConstructor;
  const MetaDestructor typeDestructor;
  const MetaSerializer typeSerializer{nullptr};
  const MetaDeserializer typeDeserializer{nullptr};

  template <class T, bool b>
  struct MetaDataConstructor;

public:
  /**
   * Creates a MetaData instance to represent the templated
   * type.
   * @templateParam The type to be represented
   * @return MetaData for the template parameter, has program-life
   *         lifetime, and is unique for this type.
   **/
  template <typename T>
  static const MetaData& create()
  {
    return MetaDataConstructor < T,
           HasSerializer<T>::value &&
             HasDeserializer<T>::value > ::get();
  }
  
  /**
   * MetaData cannot be copied or moved, instances returned by
   * the create method are unique.
   **/
  MetaData( const MetaData& metaData ) = delete;
  MetaData& operator=( const MetaData& metaData ) = delete;
  
  /**
   * Compares two instances of MetaData, true only if they
   * describe the same type.
   **/
  bool operator==( const MetaData& metaData ) const noexcept;

  /**
   * Constructs an instance of the class that this MetaData
   * represents.
   * @return Unmanaged pointer to the freestore allocated instance
   **/
  void* constructInstance() const;

  /**
   * Destroys a freestore allocated instance of the class that
   * this MetaData represents.
   * @param obj An unmanaged pointer to the instance to be destroyed
   **/
  void destroyInstance( void* obj ) const noexcept;

  /**
   * Returns true if this type can be serialized/deserialized. If not,
   * then calling serialize/deserialize will likely result in
   * segfaults or worse. (no checking here, you have to be careful
   * bub).
   **/
  bool canSerialize() const noexcept;

  /**
   * Serializes the object into the Json::Value node.
   * It is an extremely bad idea to call this without checking
   * canSerialize() for this type first!
   * @param obj The object to serialize
   * @param root The Json::Value to serialize the object into.
   **/
  void serializeInstance( void* obj, Json::Value& root ) const;

  /**
   * Deserializes the object from the Json::Value node.
   * It is an extremely bad idea to call this without checking
   * canSerialize() for this type first!
   * @param obj The object to deserialize into
   * @param root The Json::Value node to deserialize from.
   **/
  void deserializeInstance( void* obj, Json::Value& root ) const;

private:
  MetaData( MetaConstructor constructor, MetaDestructor destructor );
  MetaData( MetaConstructor constructor, MetaDestructor destructor,
            MetaSerializer serializer,
            MetaDeserializer deserializer );

  template <class T>
  struct MetaDataConstructor<T, true>
  {
    static const MetaData& get()
    {
      static MetaData metaData( metaConstructor<T>, metaDestructor<T>,
                                metaSerialize<T>,
                                metaDeserialize<T> );
      return metaData;
    }
  };

  template <class T>
  struct MetaDataConstructor<T, false>
  {
    static const MetaData& get()
    {
      static MetaData metaData( metaConstructor<T>,
                                metaDestructor<T> );
      return metaData;
    }
  };

  template <typename T>
  static void metaSerialize( void* obj, Json::Value& root )
  {
    serialize( *reinterpret_cast<T*>( obj ), root );
  }

  template <typename T>
  static void metaDeserialize( void* obj, Json::Value& root )
  {
    deserialize( *reinterpret_cast<T*>( obj ), root );
  }

  template <typename T>
  static void* metaConstructor()
  {
    return reinterpret_cast<void*>( new T{} );
  }

  template <typename T>
  static void metaDestructor( void* obj )
  {
    delete reinterpret_cast<T*>( obj );
  }
};

/**
 * Uses SFINAE to detect the presence of a serialize override with the
 * following signature: bool serialize( const Type& obj, Json::Value&
 * root )
 * - Note: the override should be in the same namespace as the type
 *   being serialized, then ADL will find it. -
 **/
template <class T>
struct HasSerializer
{
  template <class Type>
  static std::true_type hasSerializer(
    decltype( serialize( *reinterpret_cast<Type*>( 0 ),
                         *reinterpret_cast<Json::Value*>( 0 ) ) ) );
  template <class Type>
  static std::false_type hasSerializer( ... );

  constexpr static bool value = std::is_same<
    std::true_type, decltype( hasSerializer<T>( false ) )>::value;
};

/**
 * Uses SFINAE to detect the presence of a deserialize method with the
 * following signature: bool deserialize( Type& object, const
 * Json::Value& root );
 * - Note: the deserialize override should be in the same namespace as
 *   the type being deserialized, then it will be found with ADL.
 **/
template <class T>
struct HasDeserializer
{
  template <class Type>
  static std::true_type hasDeserializer(
    decltype( deserialize( *reinterpret_cast<Type*>( 0 ),
                           *reinterpret_cast<Json::Value*>( 0 ) ) ) );
  template <class Type>
  static std::false_type hasDeserializer( ... );

  constexpr static bool value = std::is_same<
    std::true_type, decltype( hasDeserializer<T>( false ) )>::value;
};

} /* namespace meta */
} /* namespace tetra */

#endif
