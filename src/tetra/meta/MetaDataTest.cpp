#include <iostream>
#include <algorithm>

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

class MyTypeWithFields
{
  public:
    int myInt;
    float myFloat;
    string myString;
};

template <class Type, typename MemberType>
std::size_t MemberOffset( MemberType( Type::*mbr ) )
{
    auto ptr = &( reinterpret_cast<Type*>( 0 )->*mbr );
    return reinterpret_cast<size_t>( ptr );
}

SCENARIO( "MetaData should be able to record member locations and their "
          "associated MetaData.",
          "[MetaData]" )
{
    auto intMeta = MetaData{&metaConstruct<int>, &metaDestroy<int>,
                            &metaCopy<int>,      sizeof( int )};

    auto floatMeta = MetaData{&metaConstruct<float>, &metaDestroy<float>,
                              &metaCopy<float>,      sizeof( float )};

    auto stringMeta = MetaData{&metaConstruct<string>, &metaDestroy<string>,
                               &metaCopy<string>,      sizeof( string )};

    map<string, MetaData::Member> members = {
        {"myInt", {MemberOffset( &MyTypeWithFields::myInt ), intMeta}},
        {"myFloat", {MemberOffset( &MyTypeWithFields::myFloat ), floatMeta}},
        {"myString",
         {MemberOffset( &MyTypeWithFields::myString ), stringMeta}}};

    auto myTypeWithFieldsMeta = MetaData{
        &metaConstruct<MyTypeWithFields>, &metaDestroy<MyTypeWithFields>,
        &metaCopy<MyTypeWithFields>,      sizeof( MyTypeWithFields ),
        members};

    GIVEN( "MetaData for a type with fields" )
    {
        auto members = myTypeWithFieldsMeta.members();
        auto contains = []( const vector<string> memberList,
                            const string & member )
        {
            return find( begin( memberList ), end( memberList ), member ) !=
                   end( memberList );
        };

        THEN(
            "The MetaData should contain member information for those fields" )
        {
            REQUIRE( contains( members, "myInt" ) );
            REQUIRE( contains( members, "myFloat" ) );
            REQUIRE( contains( members, "myString" ) );
        }

        THEN( "The MetaData for each member should exist and be correct" )
        {
            REQUIRE( myTypeWithFieldsMeta.member( "myInt" ).metaData ==
                     &intMeta );

            REQUIRE( myTypeWithFieldsMeta.member( "myInt" ).offset ==
                     MemberOffset( &MyTypeWithFields::myInt ) );

            REQUIRE( myTypeWithFieldsMeta.member( "myFloat" ).metaData ==
                     &floatMeta );

            REQUIRE( myTypeWithFieldsMeta.member( "myFloat" ).offset ==
                     MemberOffset( &MyTypeWithFields::myFloat ) );

            REQUIRE( myTypeWithFieldsMeta.member( "myString" ).metaData ==
                     &stringMeta );

            REQUIRE( myTypeWithFieldsMeta.member( "myString" ).offset ==
                     MemberOffset( &MyTypeWithFields::myString ) );
        }
    }
}

