env = Environment()
env['CPPPATH'] = [ './inc' ]
env['LIBPATH'] = [ './bin' ]
env['CXX'] = 'clang++';
env['CXXFLAGS'] = "-std=c++11"

buildLib = env.Library('./bin/tetraMeta', 
                       Glob('src/*/*/*.cpp') + Glob('src/*/*.cpp'))

env['CPPPATH'] += ['./tst']
env['LIBS'] = [ 'tetraMeta' ]
buildTests = env.Program('./bin/catchTests.out',
                         Glob('tst/*.cpp') + 
                         Glob('tst/test/*.cpp') + 
                         Glob('tst/*/*/*.cpp'));
Depends(buildTests, buildLib)

runTests = Command( target = "runTests"
                  , source = "./bin/catchTests.out"
                  , action = [ "./bin/catchTests.out" ]
                  )

Depends(runTests, buildTests)
Default(runTests)
