env = Environment()
env['CPPPATH'] = [ './inc' ]
env['LIBPATH'] = [ './bin' ]
env['CXX'] = 'clang++';
env['CXXFLAGS'] = "-std=c++11"

buildLib = env.Library('./bin/tetraMeta', Glob('src/*/*/*.cpp'))

env['LIBS'] = [ 'tetraMeta' ]
buildTests = env.Program('./bin/catchTests', Glob('tst/*.cpp'));
Depends(buildTests, buildLib)

runTests = Command( target = "runTests"
                  , source = "./bin/catchTests"
                  , action = [ "./bin/catchTests" ]
                  )

Depends(runTests, buildTests)
Default(runTests)
