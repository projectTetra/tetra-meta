# tetra-meta
---

Tetra-Meta is a very simple C++ meta-data system which facilitates safe type-erasure to hold objects in generic containers.

Tetra-Meta also uses the jsoncpp library to facilitate simple and generic serialization of objects.

Finally, Tetra-Meta also provides utilities for creating type-dictonaries which dramatically simplify the process
of serializing and deserializing objects.


## Contents

* [**Compiling**](#compiling)
* [**Getting Started**](#getting-started)
  * [*MetaData*](#metadata)
  * [*Variant*](#variant)
  * [*MetaRepository*](#metarepository)
* [**Conclusion**](#conclusion)
* [**Future Work**](#future-work)

---

## Compiling

As with all tetra libraries, the provided build uses SCons.
The SConstruct script does assume that you have clang installed, if that is 
an issue at some point then I can make this configurable.

Tetra-Meta does not have any external dependencies, so it can be downloaded
and compiled out-of-box without you needing to install any new software.

build tools:

- [scons] (http://www.scons.org/)
- [clang] (http://clang.llvm.org/)

Instructions:

1. clone repo
2. run scons
3. ~~profit~~

=========

## Getting Started

### MetaData

MetaData holds information about how to safely create, destroy, and optionally 
serialize and deserialize instances of a class.
MetaData instances are unique to the type that they are created for, and live
for the duration of the program. Getting access to an instance is as simple
as:

```C++
const MetaData& metaData = MetaData::create<Widget>();
```
This gives us a reference to the unique MetaData instance which describes
the Widget type.

We can now create and destroy instances of Widgets using the Widget meta data:

```C++
void* pWidget = metaData.constructInstance();
// use widget
metaData.destroyInstance(pWidget);
```
We can even serialize the widget if it supports it!

```C++
Json::Value root; // from jsoncpp, included with tetra-meta

metaData.serializeInstance(pWidget, root); // write Widget to root
```
This might produce json like:

```Javascript
{
  "name": "scroll-bar",
  "state": "active",
  ...
}
```

Which we can deserialize like this:

```C++
metaData.deserializeInstanec(pWidget, root); // read Widget from root
```

Using MetaData directly like this is tedious and unsafe, the better option
is to use a Variant.

### Variant

```C++
Variant v = Variant::create<Widget>();

v.getObject<Widget>().setName( "kitty" );
```

Variants carry object MetaData with them so it is possible check the type of a variant at runtime.

```C++
vector<Variant> messages; // assume this is populated elsewhere
for (const auto& msg : messages)
{
  const MetaData& msgMeta = msg.getMetaData();

  if (msgMeta == MetaData::create<Foo>())
  {
    Foo& foo = msg.getObject<Foo>();
    // do foo stuff
  }
  else if(msgMeta == MetaData::create<Bar>())
  {
    // do bar stuff
  }
}
```

Variants support serialization too!

```C++
Variant point = Variant::create( Widget{} );

Json::Value root; // from jsoncpp
point.serialize( root );
```
This might result in the following JSON

```Javascript
{
  "name": "scroll-bar",
  "state": "active",
  ...
}
```

Which we could deserialize with:

```C++
point.deserialize( root );
```

### MetaRepository

At this point, it is worth noting that no class names have been 
serialized. This means that deserializing a JSON blob is only possible
if you already know what type the blob represents!

The solution is to use the MetaRepository!

Setting up is simple enough:

```C++
MetaRepository repository{};
repository.addType<Widget>( "widget" );
```
Now we can serialize widget variants and record the typename.

```C++
Variant widget = Variant::create<Widget>();
Json::Value root;

repository.serialize( widget, root );
```

This would result in the following JSON:

```Javascript
{
  "type": "widget",
  "object": {
    "name": "scroll-bar",
    "state": "active",
    ...
  }
}
```
Now we know the class-type that the Variant's json represents!
This is even better when we deserialize the same json:

```C++
Variant someObject = repository.deserialize( root );
// now we can use the awesome functionality of the Variant to discover its type

if (someObject.getMetaData() == MetaData::create<Widget>())
  // do widget stuff
```

## Conclusion

The real value of having this MetaData available is in what it enables.
Check out the tetra-message and tetra-framework libraries for some 
applications of the tetra-meta library.

tetra-message is a publish-subscribe messaging system built on the idea of
holding messages in Variants and using MetaData to dispatch messages to
their subscribed listeners.

tetra-framework is an entity-component system as described by t-machine.
Basically the idea is to have entities hold components in Variants, this
makes authoring components super easy and simple.

## Future Work

My tentative next step (after doing some benchmarking) is to add a
small-object optimization to the Variant class. This is fairly common
for implementations of std::string, and could improve performance by
reducing freestore allocations. 

Although, I probably will not get to this until I see some real usage-based
evidence that performance needs to be improved.













