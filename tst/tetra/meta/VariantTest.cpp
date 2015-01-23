#include <tetra/meta/Variant.hpp>

#include <catch.hpp>
#include <json/json.h>
#include <test/Widget.hpp>
#include <test/VectorComponent.hpp>

using namespace tetra;
using namespace tetra::meta;
using test::Widget;
using test::VectorComponent;

SCENARIO( "Serializing and Deserializing Variants",
          "[Variant][Serialization]" )
{
  GIVEN( "A Variant constructed from a Widget" )
  {
    Variant variant = Variant::create( Widget{} );

    THEN( "Serializing and Deserializing the object should return "
          "false (not supported)" )
    {
      Json::Value root{};

      REQUIRE( variant.serialize( root ) == false );
      REQUIRE( variant.deserialize( root ) == false );
    }
  }

  GIVEN( "A Variant constructed from a VectorComponent" )
  {
    Variant variant =
      Variant::create( VectorComponent{3.0f, 1.0f, 2.0f} );

    THEN( "Serializing the variant should be successfull" )
    {
      Json::Value root{};
      REQUIRE( variant.serialize( root ) );

      REQUIRE( root.get( "x", 0.0f ).asFloat() == 3.0f );
      REQUIRE( root.get( "y", 0.0f ).asFloat() == 1.0f );
      REQUIRE( root.get( "z", 0.0f ).asFloat() == 2.0f );
    }

    THEN( "Deserializing the variant should be successful" )
    {
      Json::Value root{};
      root["x"] = -4.0f;
      root["y"] = 1000.0f;
      root["z"] = 23.5f;

      REQUIRE( variant.deserialize( root ) );

      auto& vectorComponent = variant.getObject<VectorComponent>();
      REQUIRE( vectorComponent.getX() == -4.0f );
      REQUIRE( vectorComponent.getY() == 1000.0f );
      REQUIRE( vectorComponent.getZ() == 23.5f );
    }
  }
}

SCENARIO(
  "Variants should be safely hold data using a type's MetaData",
  "[Variant]" )
{
  GIVEN( "A Variant constructed with a type's MetaData" )
  {
    {
      Variant variant = Variant{MetaData::get<Widget>()};

      THEN(
        "We should see the side-effects of the object being created" )
      {
        REQUIRE( Widget::getInstanceCount() == 1 );
      }

      THEN( "The MetaData stored in the Variant should match the "
            "MetaData for the Widget class" )
      {
        REQUIRE( variant.getMetaData() ==
                 MetaData::get<Widget>() );
      }

      THEN( "We should be able to retrieve a reference to the actual "
            "Widget object" )
      {
        REQUIRE( variant.getObject<Widget>().getMyName() ==
                 "Widget{1}" );
      }
    }

    THEN( "When the Variant goes out of scope, whe should see the "
          "side-effects of the object being destroyed" )
    {
      REQUIRE( Widget::getInstanceCount() == 0 );
    }
  }

  GIVEN( "A Variant which was move-constructed from an already "
         "existing Variant" )
  {
    {
      Variant toMove{MetaData::get<Widget>()};
      Variant variant{MetaData::get<int>()};

      variant = std::move( toMove );

      THEN( "There should only be one of the payload object in "
            "existence" )
      {
        REQUIRE( Widget::getInstanceCount() == 1 );
      }

      THEN(
        "The Variant that was moved to should be valid and support "
        "the getObject method" )
      {
        REQUIRE( variant.getObject<Widget>().getMyName() ==
                 "Widget{1}" );
      }

      THEN( "The Variant that was moved from should always throw a "
            "TypeCastException when getObject is called." )
      {
        REQUIRE_THROWS_AS( toMove.getObject<Widget>(),
                           TypeCastException );
      }
    }

    THEN(
      "The side-effects of deleting the Variants should be present" )
    {
      REQUIRE( Widget::getInstanceCount() == 0 );
    }
  }
}

