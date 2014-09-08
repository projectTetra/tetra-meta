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
                             &metaCopy<Widget>,      sizeof( Widget )};
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

    GIVEN(
        "Two strings constructed with the MetaData for the std::string class." )
    {
        auto data = MetaData{&metaConstruct<string>, &metaDestroy<string>,
                             &metaCopy<string>,      sizeof( string )};
        void* str = data.construct();
        void* str2 = data.construct();

        auto asString = []( void * s )->string &
        {
            return *reinterpret_cast<string*>( s );
        };

        asString( str ) = "hello world";

        THEN( "The MetaData's copy operator should call the actual copy "
              "operator for the string class." )
        {
            data.copy( str2, str );
            REQUIRE( asString( str2 ) == "hello world" );
        }

        data.destroy( str );
        data.destroy( str2 );
    }
}

