#include <tetra/meta/MetaConstructor.hpp>

#include <catch.hpp>
#include <Widget.hpp>

using namespace std;
using namespace tetra;
using namespace tetra::meta;

SCENARIO( "Create MetaData with the MetaConstructor",
          "[MetaConstructor][MetaData]" )
{
    const MetaData& data = MetaConstructor<Widget>::getMetaData();
    WHEN( "The MetaConstructor is used to get MetaData for the Widget class" )
    {
        void* widget = data.construct();
        THEN( "The widget's constructor should have its proper side effects" )
        {
            REQUIRE( Widget::count == 1 );
        }

        data.destroy( widget );
        THEN( "The widget's destructor should have its proper side effets" )
        {
            REQUIRE( Widget::count == 0 );
        }

        THEN( "The MetaData for the Widget class should properly record the "
              "Widget's size" )
        {
            REQUIRE( data.size() == sizeof( Widget ) );
        }
    }
};
