#include <tetra/meta/MetaRepository.hpp>

#include <json/json.h>
#include <catch.hpp>
#include <test/Widget.hpp>
#include <test/VectorComponent.hpp>

#include <iostream>

using namespace std;
using namespace tetra;
using namespace tetra::meta;
using test::Widget;
using test::VectorComponent;

SCENARIO(
  "Using the MetaRepository to serialize and deserialize objects",
  "[MetaRepository][Serialization]" )
{
  GIVEN( "A MetaRepository with VectorComponents registered" )
  {
    MetaRepository metaRepository{};
    const std::string vectorComponentTypeName{"vector3d"};
    metaRepository.addType<VectorComponent>(
      vectorComponentTypeName );

    THEN( "Serializing an unregistered type should throw" )
    {
      Variant widget = Variant::create( Widget{} );
      Json::Value root{};
      REQUIRE_THROWS_AS( metaRepository.serialize( widget, root ),
                         TypeNotRegisteredException );
    }

    THEN( "deserializing an unregistered type should throw" )
    {
      Json::Value root{};
      root["type"] = "Widget";
      root["object"] = Json::Value{};

      REQUIRE_THROWS_AS( metaRepository.deserialize( root ),
                         TypeNotRegisteredException );
    }

    THEN( "We should be able to serialize a Variant of a registered "
          "type" )
    {
      Variant vector3d =
        Variant::create( VectorComponent{1.0f, 2.0f, 3.0f} );
      Json::Value root{};

      REQUIRE( metaRepository.serialize( vector3d, root ) );
      REQUIRE( root.get( "type", "" ).asString() ==
               vectorComponentTypeName );
      
      Json::Value component = root.get( "object", "" );
      REQUIRE( component.isObject() );
    }

    THEN( "We should be able to deserialize a Variant of a "
          "registered type" )
    {
      Json::Value root{};
      root["type"] = vectorComponentTypeName;

      Json::Value component{};
      component["x"] = 1.0f;
      component["y"] = 2.0f;
      component["z"] = 1000.0f;

      root["object"] = component;

      Variant var = metaRepository.deserialize( root );
      REQUIRE( var.getMetaData() == MetaData::create<VectorComponent>() );
      
      const auto& vectorComp = var.getObject<VectorComponent>();
      REQUIRE( vectorComp.getX() == 1.0f );
      REQUIRE( vectorComp.getY() == 2.0f );
      REQUIRE( vectorComp.getZ() == 1000.0f );
    }
  }
}

SCENARIO(
  "Using the MetaRepository to map names to MetaData instances",
  "[MetaRepository]" )
{
  GIVEN( "A MetaRepository" ) 
  {
    MetaRepository metaRepository{};

    THEN( "The basic types, char, int , float, double, string, and "
          "wstring, should all be added by default" )
    {
#define check( T )                                                   \
  REQUIRE( metaRepository.getMetaData( #T ) ==                       \
           MetaData::create<T>() );
      check(char);
      check(int);
      check(float);
      check(double);
      check(string);
      check(wstring);
#undef check
    }

    THEN( "The getMetaData method should throw when unregistered "
          "types are requested" )
    {
      REQUIRE_THROWS_AS( metaRepository.getMetaData( "Widget" ),
                         TypeNotRegisteredException );
    }

    THEN( "The getTypeName method should throw when unregistered "
          "types are requested" )
    {
      REQUIRE_THROWS_AS(
        metaRepository.getTypeName( MetaData::create<Widget>() ),
        TypeNotRegisteredException );
    }

    THEN( "The getMetaData method should return the correct MetaData "
          "when a registered type name is requested" )
    {
      REQUIRE( MetaData::create<int>() ==
               metaRepository.getMetaData( "int" ) );
    }

    THEN( "the getTypeName method should return the correct string "
          "when given a registered MetaData instance" )
    {
      REQUIRE( metaRepository.getTypeName(
                 MetaData::create<float>() ) == "float" );
    }

    THEN( "creating an instance of an unregistered type should throw "
          "a TypeNotRegisteredException" )
    {
      REQUIRE_THROWS_AS( metaRepository.createInstance( "Widget" ),
                         TypeNotRegisteredException );
    }

    THEN(
      "We should be able to create an instance of a registered type" )
    {
      metaRepository.addType<Widget>( "Widget" );

      Variant widgetVariant =
        metaRepository.createInstance( "Widget" );

      REQUIRE( widgetVariant.getMetaData() ==
               MetaData::create<Widget>() );
      REQUIRE( Widget::getInstanceCount() == 1 );
    }
  }
}

