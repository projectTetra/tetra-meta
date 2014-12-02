#include <catch.hpp>

#include <Widget.hpp>
#include <tetra/meta/MetaRepository.hpp>

using namespace std;
using namespace tetra;
using namespace tetra::meta;

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

