/*
    Groovy script define reusable method for jenkins
    See
       https://stackoverflow.com/questions/55486023/reusing-groovy-script-in-jenkins-declarative-pipeline-file
*/

env.jSLAVE_FOO="SLAVE FOO"


def build_on_linux() {
    sh 'printenv'

    // Build Release/Debug -------------------------------------------
    sh 'mkdir $BUILD_DIR_RELEASE'
    sh 'mkdir $BUILD_DIR_DEBUG'
    sh 'cd $BUILD_DIR_RELEASE ; cmake $WORKSPACE -DCUSTOM_SYSTEM_OS_NAME=Ubuntu ; make -j4 ;'
    sh 'cd $BUILD_DIR_DEBUG   ; cmake $WORKSPACE -DCUSTOM_SYSTEM_OS_NAME=Ubuntu -DCMAKE_BUILD_TYPE=Debug; make -j4 ;'

    // Test Release/Debug -------------------------------------------
    sh 'cd $BUILD_DIR_RELEASE ; ctest --output-on-failure ;'
    sh 'cd $BUILD_DIR_DEBUG   ; ctest --output-on-failure ;'

}

def build_on_windows() {
    bat 'set'

    // Build Release/Debug -------------------------------------------
    bat 'mkdir %BUILD_DIR%'
    bat 'cd %BUILD_DIR%'
    bat 'cmake %WORKSPACE% -G"Visual Studio 15 2017" -A x64'
    bat 'cmake --build . --target ALL_BUILD --config Debug'
    bat 'cmake --build . --target ALL_BUILD --config Release'

    // Test Release/Debug -------------------------------------------
    bat 'ctest --output-on-failure -C Release'
    bat 'ctest --output-on-failure -C Debug'
}

return this