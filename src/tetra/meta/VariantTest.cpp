#include <catch.hpp>

#include <Widget.hpp>
#include <tetra/meta/Variant.hpp>

using namespace std;
using namespace tetra;
using namespace tetra::meta;

SCENARIO( "Constructing Variant objects using type MetaData.",
          "[Variant][MetaData]" )
{
    GIVEN( "A variant constructed with the explicit createVariant method." )
    {
        {
            Variant v = Variant::createVariant<Widget>();

            THEN( "The type's constructor should be called correctly" )
            {
                REQUIRE( Widget::count == 1 );
            }
        }

        THEN( "The type's destructor shoud be called when the Variant is "
              "destroyed" )
        {
            REQUIRE( Widget::count == 0 );
        }
    }

    GIVEN( "A variant constructed with the createVariant( name ) method." )
    {
        {
            Variant v = Variant::createVariant( "Widget" );

            THEN( "The type's constructor should be called correctly" )
            {
                REQUIRE( Widget::count == 1 );
            }
        }

        THEN( "The type's destructor should be called when the Variant is "
              "destroyed" )
        {
            REQUIRE( Widget::count == 0 );
        }
    }
}

SCENARIO( "The Variant get method should be type-safe.", "[Variant]" )
{
    GIVEN( "A Variant containing an int" )
    {
        auto var = Variant::createVariant<int>();

        THEN( "Setting the value should work." )
        {
            var.get<int>() = 100;
            REQUIRE( var.get<int>() == 100 );
        }

        THEN( "The get method should throw if the wrong type is requested" )
        {
            CHECK_THROWS_AS( var.get<float>(), TypeMismatchError );
        }
    }
}

SCENARIO( "Copying a Variant object should perform a deep-copy" )
{
    GIVEN( "A Variant holding an integer" )
    {
        auto var = Variant::createVariant<int>();

        var.get<int>() = 100;
        REQUIRE( var.get<int>() == 100 );

        THEN( "Copying the Variant should perform a deep-copy of the int" )
        {
            auto var2 = var;
            REQUIRE( var2.get<int>() == 100 );
            REQUIRE( &var2.get<int>() != &var.get<int>() );
        }
    }
}







