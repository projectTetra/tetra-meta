#include <tetra/meta/MetaRepository.hpp>

#include <catch.hpp>

#include <Widget.hpp>
#include <tetra/meta/MetaConstructor.hpp>

using namespace std;
using namespace tetra;
using namespace tetra::meta;

class MyType
{
  public:
    MyType()
    {
        ++MyType::count;
    }

    ~MyType()
    {
        --MyType::count;
    }

  public:
    static int count;
};

int MyType::count{0};

META_REGISTER( MyType );

SCENARIO( "The MetaRepository should support adding and looking up MetaData "
          "instances",
          "[MetaRepository][MetaData]" )
{
    WHEN( "The Widget MetaData is added" )
    {
        MetaRepository::addType( "widget",
                                 MetaConstructor<Widget>::getMetaData() );

        THEN( "The MetaData returned when looked up should match the stored "
              "MetaData." )
        {
            const MetaData& meta = MetaRepository::lookupMetaData( "widget" );
            REQUIRE( &meta == &MetaConstructor<Widget>::getMetaData() );
        }

        THEN( "The MetaRepository throws a std::out_of_range if the wrong type "
              "name is used for lookup" )
        {
            CHECK_THROWS_AS( MetaRepository::lookupMetaData( "WIDGET" ),
                             std::out_of_range );
        }
    }
}

SCENARIO( "The MetaRepository should support looking up registered types",
          "[MetaRepository][MetaData][Registered]" )
{
    WHEN( "A type is registered with META_REGISTER" )
    {
        const MetaData& myTypeMeta = MetaRepository::lookupMetaData( "MyType" );
        void* myType = myTypeMeta.construct();

        THEN( "The type's constructor should be callable" )
        {
            REQUIRE( MyType::count == 1 );
        }

        myTypeMeta.destroy( myType );
        THEN( "The type's destructor should be callable" )
        {
            REQUIRE( MyType::count == 0 );
        }

        THEN( "The MetaData should have the right size stored" )
        {
            REQUIRE( myTypeMeta.size() == sizeof( MyType ) );
        }
    }
}

