#include <iostream>

#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#include <tetra/meta/MetaData.hpp>
#include <Widget.hpp>


using namespace std;
using namespace tetra;
using namespace tetra::meta;

SCENARIO( "Create MetaData", "[MetaData]" )
{
    GIVEN( "A Widget class to construct MetaData from" )
    {
        auto data = MetaData{&metaConstruct<Widget>, &metaDestroy<Widget>,
                             sizeof( Widget )};
        auto* widget = data.construct();

        THEN( "The constructor should have side effects" )
        {
            REQUIRE( Widget::count == 1 );
        }

        data.destroy( widget );
        THEN( "The destructor should have side effects" )
        {
            REQUIRE( Widget::count == 0 );
        }

        THEN( "The MetaData size should match the Widget's size" )
        {
            REQUIRE( data.size() == sizeof( Widget ) );
        }
    }
}

