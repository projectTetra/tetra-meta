#include <catch.hpp>

#include <Widget.hpp>
#include <tetra/meta/Variant.hpp>

using namespace tetra;
using namespace tetra::meta;

SCENARIO(
  "Variants should be safely hold data using a type's MetaData",
  "[Variant]" )
{
  GIVEN( "A Variant constructed with a type's MetaData" )
  {
    {
      Variant variant = Variant{MetaData::create<Widget>()};

      THEN(
        "We should see the side-effects of the object being created" )
      {
        REQUIRE( Widget::getInstanceCount() == 1 );
      }

      THEN( "The MetaData stored in the Variant should match the "
            "MetaData for the Widget class" )
      {
        REQUIRE( variant.getMetaData() ==
                 MetaData::create<Widget>() );
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
      Variant toMove{MetaData::create<Widget>()};
      Variant variant{MetaData::create<int>()};

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

