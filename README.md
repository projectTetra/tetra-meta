tetra-meta
==========

Tetra-Meta is a very simple C++ meta-data system which facilitates safe type-erasure to hold objects in generic containers.

==========

Example:

Assuming you have previously declared some class called Widget...

<code>
Variant v = Variant::create<Widget>();

<code>
v.getObject\<Widget\>().setName( "kitty" );

Variants carry object MetaData with them so it is possible check the type of a variant at runtime.

<code>
  if (variant.getMetaData() == MetaData::create\<Widget\>())
    // Do something Widget related with the variant
  
<code>
  if (variant.getMetaData() == MetaData::create\<Foo\>())
    // Do something Foo related with the variant

See the tetra-message library or the tetra-framework library for furthur usage examples, as both libraries use the
tetra-meta library.
