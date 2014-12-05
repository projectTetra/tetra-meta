tetra-meta
==========

Tetra-Meta is a very simple C++ meta-data system which facilitates safe type-erasure to hold objects in generic containers.

Tetra-Meta also uses the jsoncpp library to facilitate simple and generic serialization of objects.

Finally, Tetra-Meta also provides utilities for creating type-dictonaries which dramatically simplify the process
of serializing and deserializing objects.

==========

Example:

Assuming you have previously declared some class called Widget...

```
Variant v = Variant::create<Widget>();

v.getObject\<Widget\>().setName( "kitty" );
```

Variants carry object MetaData with them so it is possible check the type of a variant at runtime.

```
if (variant.getMetaData() == MetaData::create\<Widget\>())
  // Do something Widget related with the variant
  
if (variant.getMetaData() == MetaData::create\<Foo\>())
  // Do something Foo related with the variant
```

See the tetra-message library or the tetra-framework library for furthur usage examples, as both libraries use the
tetra-meta library.
