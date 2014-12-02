#include <tetra/meta/MetaRepository.hpp>

using namespace std;
using namespace tetra;
using namespace tetra::meta;

TypeNotRegisteredException::TypeNotRegisteredException()
  : runtime_error{"Type not registered!"}
  , typeName{"not provided"}
{ }

TypeNotRegisteredException::TypeNotRegisteredException(
  const string& typeName )
  : runtime_error{"Type, " + typeName + ", not registered!"}
  , typeName{typeName}
{ }

const std::string& TypeNotRegisteredException::getTypeName() const
  noexcept
{
  return typeName;
}

MetaRepository::MetaRepository()
{
#define add( T ) addType<T>( #T );
  add(char);
  add(int);
  add(float);
  add(double);
  add(string);
  add(wstring);
#undef add
}
