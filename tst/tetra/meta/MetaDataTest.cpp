#include <tetra/meta/MetaData.hpp>
#include <test/Widget.hpp>
#include <test/VectorComponent.hpp>

#include <catch.hpp>
#include <json/json.h>

#include <iostream>
#include <typeinfo>

using namespace std;
using namespace tetra;
using namespace tetra::meta;
using test::Widget;

SCENARIO( "Creating MetaData for a VectorComponent",
          "[MetaData][Serialization]" )
{
  const MetaData& metaData =
    MetaData::get<test::VectorComponent>();

  GIVEN(
    "MetaData for a VectorComponent which supports serialization" )
  {
    REQUIRE( metaData.canSerialize() );

    THEN( "We should be able to serialize a VectorComponent" )
    {
      Json::Value root{};
      test::VectorComponent vc{1.0f, 2.0f, 3.0f};

      metaData.serializeInstance( &vc, root );
      REQUIRE( root.get( "x", 0.0f ).asFloat() == 1.0f );
      REQUIRE( root.get( "y", 0.0f ).asFloat() == 2.0f );
      REQUIRE( root.get( "z", 0.0f ).asFloat() == 3.0f );
    }

    THEN( "We should be able to deserialize a VectorComponent" )
    {
      Json::Value root{};
      root["x"] = 1.0f;
      root["y"] = 2.0f;
      root["z"] = 3.0f;

      test::VectorComponent vc{0.0f, 0.0f, 0.0f};

      metaData.deserializeInstance( &vc, root );
      REQUIRE( vc.getX() == 1.0f );
      REQUIRE( vc.getY() == 2.0f );
      REQUIRE( vc.getZ() == 3.0f );
    }
  }
}

SCENARIO( "Creating MetaData for a Widget.", "[MetaData]" )
{
  const MetaData& metaData = MetaData::get<Widget>();

  GIVEN( "MetaData for a Widget" )
  {
    THEN( "The Widget should report not supporting serialization" )
    {
      REQUIRE( !metaData.canSerialize() );
    }

    THEN( "We should be able to construct an instance of the Widget "
          "type" )
    {
      void* obj = metaData.constructInstance();
      REQUIRE( Widget::getInstanceCount() == 1 );

      Widget* pWidget = reinterpret_cast<Widget*>( obj );
      delete pWidget;
    }

    THEN(
      "We should be able to destroy an instance of the Widget type" )
    {
      REQUIRE( Widget::getInstanceCount() == 0 );
      Widget* pWidget = new Widget();

      REQUIRE( Widget::getInstanceCount() == 1 );

      metaData.destroyInstance( pWidget );

      REQUIRE( Widget::getInstanceCount() == 0 );
    }

    THEN(
      "All MetaData for a given type should be identically equal" )
    {
      const MetaData& differentMetaData = MetaData::get<Widget>();

      REQUIRE( &metaData == &differentMetaData );
      REQUIRE( metaData == differentMetaData );
    }
  }
}
