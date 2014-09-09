#include <algorithm>

#include <tetra/meta/MetaConstructor.hpp>

#include <catch.hpp>
#include <Widget.hpp>

using namespace std;
using namespace tetra;
using namespace tetra::meta;

SCENARIO( "Create MetaData with the MetaConstructor",
          "[MetaConstructor][MetaData]" )
{
    const MetaData& data = metaData<Widget>();
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

class MyTypeWithFields
{
  public:
    int myInt;
    float myFloat;
    string myString;
};

BEGIN_META_REGISTER( MyTypeWithFields )
{
    META_MEMBER( myInt )
    META_MEMBER( myFloat )
    META_MEMBER( myString )
}
END_META_REGISTER()

BASE_META_REGISTER( int );
BASE_META_REGISTER( float );
BASE_META_REGISTER( string );

SCENARIO( "A type with fields has been registered with the MetaData system",
          "[MetaConstructor][MetaData]" )
{
    const MetaData& myTypeWithFieldsMeta = metaData<MyTypeWithFields>();

    GIVEN( "MetaData for the registered type" )
    {
        THEN( "The type's members should exist" )
        {
            auto contains = []( const vector<string>& members,
                                const string& member )
            {
                return find( begin( members ), end( members ), member ) !=
                       end( members );
            };

            auto members = myTypeWithFieldsMeta.members();

            REQUIRE( contains( members, "myInt" ) );
            REQUIRE( contains( members, "myFloat" ) );
            REQUIRE( contains( members, "myString" ) );
        }

        THEN( "The type's members should have the proper MetaData and offsets" )
        {
            auto myInt = myTypeWithFieldsMeta.member( "myInt" );
            auto myFloat = myTypeWithFieldsMeta.member( "myFloat" );
            auto myString = myTypeWithFieldsMeta.member( "myString" );

            REQUIRE( myInt.offset == memberOffset( &MyTypeWithFields::myInt ) );
            REQUIRE( myFloat.offset ==
                     memberOffset( &MyTypeWithFields::myFloat ) );
            REQUIRE( myString.offset ==
                     memberOffset( &MyTypeWithFields::myString ) );

            REQUIRE( myInt.metaData == &metaData<int>() );
            REQUIRE( myFloat.metaData == &metaData<float>() );
            REQUIRE( myString.metaData == &metaData<string>() );
        }
    }
}

