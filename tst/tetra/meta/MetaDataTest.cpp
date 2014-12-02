#include <Widget.hpp>
#include <tetra/meta/MetaData.hpp>

#include <catch.hpp>

#include <typeinfo>

using namespace tetra;
using namespace tetra::meta;

SCENARIO( "Creating MetaData for the a Widget.", "[MetaData]" )
{
  const MetaData& metaData = MetaData::create<Widget>();

  GIVEN( "MetaData for a Widget" )
  {
    THEN( "We should be able to construct an instance of the Widget "
          "type" )
    {
      void* obj = metaData.constructInstance();
      REQUIRE( Widget::getInstanceCount() == 1 );

      Widget* pWidget = reinterpret_cast<Widget*>( obj );
      delete pWidget;
    }

    THEN(
      "We should be able to destroy an instance of the Widget type" )
    {
      REQUIRE( Widget::getInstanceCount() == 0 );
      Widget* pWidget = new Widget();

      REQUIRE( Widget::getInstanceCount() == 1 );

      metaData.destroyInstance( pWidget );

      REQUIRE( Widget::getInstanceCount() == 0 );
    }

    THEN(
      "All MetaData for a given type should be identically equal" )
    {
      const MetaData& differentMetaData = MetaData::create<Widget>();
      
      REQUIRE( &metaData == &differentMetaData );
      REQUIRE( metaData == differentMetaData );
    }
  }
}
