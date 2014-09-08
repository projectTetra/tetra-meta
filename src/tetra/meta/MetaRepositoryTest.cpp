#include <tetra/meta/MetaRepository.hpp>

#include <catch.hpp>

#include <Widget.hpp>
#include <tetra/meta/MetaConstructor.hpp>

using namespace std;
using namespace tetra;
using namespace tetra::meta;

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
