/*
    Groovy script define reusable method for jenkins
    See
       https://stackoverflow.com/questions/55486023/reusing-groovy-script-in-jenkins-declarative-pipeline-file
*/

// Variables defined here and load by master can be seen in slaves, i.e
// env.jSLAVE_FOO="SLAVE FOO"

def build_on_linux() {
    sh 'mkdir $BUILD_DIR_RELEASE'
    sh 'mkdir $BUILD_DIR_DEBUG'
    sh 'cmake -B$BUILD_DIR_RELEASE -H$WORKSPACE -DCUSTOM_SYSTEM_OS_NAME=Ubuntu'
    sh 'cmake --build $BUILD_DIR_RELEASE --target all --parallel 4'
    sh 'cmake -B$BUILD_DIR_DEBUG -H$WORKSPACE -DCUSTOM_SYSTEM_OS_NAME=Ubuntu -DCMAKE_BUILD_TYPE=Debug'
    sh 'cmake --build $BUILD_DIR_DEBUG --target all --parallel 4'
}

def runtest_on_linux() {
    sh 'cmake --build $BUILD_DIR_DEBUG --target test'
    sh 'cmake --build $BUILD_DIR_RELEASE --target test'
}

def pack_on_linux() {
    sh 'cpack --config $BUILD_DIR_DEBUG/CPackConfig.cmake -G TGZ'
    sh 'cpack --config $BUILD_DIR_RELEASE/CPackConfig.cmake -G TGZ'
    sh 'mv SDKLibraries-v*Ubuntu*.tar.gz $POSTBUILD_DIR'
    sh 'ls -Ss1pq --block-size=M $POSTBUILD_DIR/*.tar.gz'
}

def build_on_windows() {
    bat 'mkdir %BUILD_DIR%'
    bat 'cmake -B%BUILD_DIR% -H%WORKSPACE% -G"Visual Studio 15 2017" -A x64'
    bat 'cmake --build %BUILD_DIR% --target ALL_BUILD --config Debug --parallel 4'
    bat 'cmake --build %BUILD_DIR% --target ALL_BUILD --config Release --parallel 4'
}

def runtest_on_windows() {
    bat 'cmake --build %BUILD_DIR% --target RUN_TESTS --config Debug'
    bat 'cmake --build %BUILD_DIR% --target RUN_TESTS --config Release'
}

def pack_on_windows() {
    bat 'cpack --config %BUILD_DIR%\\CPackConfig.cmake -G NSIS -C Debug'
    bat 'cpack --config %BUILD_DIR%\\CPackConfig.cmake -G NSIS -C Release'
    bat 'move SDKLibraries-v*Windows*.exe %POSTBUILD_DIR%'
    bat 'dir %POSTBUILD_DIR%\\*.exe'
}

return this