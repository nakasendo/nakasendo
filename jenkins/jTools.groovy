/*
    Groovy script define reusable method for jenkins
    See
       https://stackoverflow.com/questions/55486023/reusing-groovy-script-in-jenkins-declarative-pipeline-file
*/

// Variables defined here and load by master can be seen in slaves, i.e
// env.jSLAVE_FOO="SLAVE FOO"

def dump_env_main_repo(master_groovy_file) {

    sh label : "Dump common env variables for scm", script : """
        echo env.jTARGET_REPO_HTTP=\\\"\$(python jenkins/Chainkins.py --get_http_repo --ssh_repo=${env.GIT_URL})\\\">>${master_groovy_file}
        echo env.jTARGET_BRANCH=\\\"\$(python jenkins/Chainkins.py --get_local_branch --git_branch=${env.GIT_BRANCH})\\\">>${master_groovy_file}
        echo env.jTARGET_COMMIT=\\\"${env.GIT_COMMIT}\\\">>${master_groovy_file}
        echo env.jTARGET_COMMIT_SHORT=\\\"\$(python jenkins/Chainkins.py --get_short_hash --git_hash=${env.GIT_COMMIT})\\\">>${master_groovy_file}
    """

    sh label : "Dump common env variables build", script : """
        echo env.jBUILD_DIRNAME=\\\"build\\\">>${master_groovy_file}
        echo env.jBUILD_DIRNAME_RELEASE=\\\"buildrelease\\\">>${master_groovy_file}
        echo env.jBUILD_DIRNAME_DEBUG=\\\"builddebug\\\">>${master_groovy_file}
        echo env.jPOSTBUILD_DIRNAME=\\\"buildpost\\\">>${master_groovy_file}
    """

    // Variables used for post build email
    if (!env.BITBUCKET_PAYLOAD) {// If BITBUCKET_PAYLOAD is not defined, then the build is manually triggered by a user
        wrap([$class: 'BuildUser']) {
            sh "echo env.jBUILD_TRIGGER=\\\"\$BUILD_USER\\\">>${master_groovy_file}"
                // The manual build kickoff will send email to the only user who has kickoff the build.
            sh "echo env.jEMAIL_TO_SEND=\\\"\$(python jenkins/Chainkins.py --fix_nchain_email --email=$BUILD_USER_EMAIL)\\\">>${master_groovy_file}"
        }
    }
    else{
        sh label : "Dump common env variables for email", script : """
            echo env.jBUILD_TRIGGER=\\\"Bitbucket webhook\\\">>${master_groovy_file}
            echo env.jEMAIL_TO_SEND=\\\"j.murphy@nchain.com,c.nguyen@nchain.com,j.wilden@nchain.com,c.battini@nchain.com,d.edunfunke@nchain.com,cc:r.balagourouche@nchain.com,cc:m.rae@nchain.com,cc:p.foster@nchain.com\\\">>${master_groovy_file}
        """
    }
}


def dump_buildenv_windows(win_env_groovy_file) {
    bat label : "Dump env variables for windows build", script : """
        echo env.WINDOWS_CHAINKINS_FILE=/${env.WORKSPACE}\\jenkins\\Chainkins.py/        >>${win_env_groovy_file}
        echo env.WINDOWS_BUILD_DIR=/${env.WORKSPACE}\\${env.jBUILD_DIRNAME}/             >>${win_env_groovy_file}
        echo env.WINDOWS_POSTBUILD_DIR=/${env.WORKSPACE}\\${env.jPOSTBUILD_DIRNAME}/     >>${win_env_groovy_file}
        echo env.WINDOWS_TESTRESULT_DIR_RELEASE=/${env.WORKSPACE}\\${env.jBUILD_DIRNAME}\\x64\\release/ >>${win_env_groovy_file}
        echo env.WINDOWS_TESTRESULT_DIR_DEBUG=/${env.WORKSPACE}\\${env.jBUILD_DIRNAME}\\x64\\debug/     >>${win_env_groovy_file}
    """
}

def build_on_windows() {
    bat label : "Compile Release and Debug on Windows", script : '''
        mkdir %WINDOWS_BUILD_DIR%'
        cmake -B%WINDOWS_BUILD_DIR% -H%WORKSPACE% -G"Visual Studio 15 2017" -A x64
        cmake --build %WINDOWS_BUILD_DIR% --target ALL_BUILD --config Debug --parallel 4
        cmake --build %WINDOWS_BUILD_DIR% --target ALL_BUILD --config Release --parallel 4
    '''
}

def runtest_on_windows() {
    bat label : "Run test Release and Debug on Windows", script : '''
        cmake --build %WINDOWS_BUILD_DIR% --target RUN_TESTS --config Debug
        cmake --build %WINDOWS_BUILD_DIR% --target RUN_TESTS --config Release
    '''
}

def consolidate_test_on_windows() {
    bat label : "Consolidate tests results for post build", script : '''
        python %WINDOWS_CHAINKINS_FILE% --consolidate_junit --indir_debug=%WINDOWS_TESTRESULT_DIR_DEBUG% --indir_release=%WINDOWS_TESTRESULT_DIR_RELEASE% --outdir=%WINDOWS_POSTBUILD_DIR%
        python %WINDOWS_CHAINKINS_FILE% --consolidate_html --indir_debug=%WINDOWS_TESTRESULT_DIR_DEBUG% --indir_release=%WINDOWS_TESTRESULT_DIR_RELEASE% --outdir=%WINDOWS_POSTBUILD_DIR%
        python %WINDOWS_CHAINKINS_FILE% --dump_mainrepo_email_html --indir_debug=%WINDOWS_TESTRESULT_DIR_DEBUG% --indir_release=%WINDOWS_TESTRESULT_DIR_RELEASE% --outdir=%WINDOWS_POSTBUILD_DIR%
    '''
}

def pack_on_windows() {
    bat label : "Pack Release and Debug on Windows", script : '''
        if not exist %WINDOWS_POSTBUILD_DIR% mkdir %WINDOWS_POSTBUILD_DIR%
        cpack --config %WINDOWS_BUILD_DIR%\\CPackConfig.cmake -G NSIS -C Debug
        cpack --config %WINDOWS_BUILD_DIR%\\CPackConfig.cmake -G NSIS -C Release
        move SDKLibraries-v*Windows*.exe %WINDOWS_POSTBUILD_DIR%
        dir %WINDOWS_POSTBUILD_DIR%\\*.exe
    '''
}



def dump_buildenv_linux(linux_env_groovy_file) {
    sh label : "Dump env variables for linux build", script : """
        echo env.LINUX_CHAINKINS_FILE=\\\"${env.WORKSPACE}/jenkins/Chainkins.py\\\"              >> ${linux_env_groovy_file}
        echo env.LINUX_BUILD_DIR_RELEASE=\\\"${env.WORKSPACE}/${env.jBUILD_DIRNAME_RELEASE}\\\"  >> ${linux_env_groovy_file}
        echo env.LINUX_BUILD_DIR_DEBUG=\\\"${env.WORKSPACE}/${env.jBUILD_DIRNAME_DEBUG}\\\"      >> ${linux_env_groovy_file}
        echo env.LINUX_POSTBUILD_DIR=\\\"${env.WORKSPACE}/${env.jPOSTBUILD_DIRNAME}\\\"          >> ${linux_env_groovy_file}
        echo env.LINUX_TESTRESULT_DIR_RELEASE=\\\"${env.WORKSPACE}/${env.jBUILD_DIRNAME_RELEASE}/x64/release\\\" >> ${linux_env_groovy_file}
        echo env.LINUX_TESTRESULT_DIR_DEBUG=\\\"${env.WORKSPACE}/${env.jBUILD_DIRNAME_DEBUG}/x64/debug\\\"       >> ${linux_env_groovy_file}
    """
}

def build_on_linux() {
    sh label : "Compile Release and Debug on Linux", script : '''
        mkdir $LINUX_BUILD_DIR_RELEASE
        mkdir $LINUX_BUILD_DIR_DEBUG
        cmake -B$LINUX_BUILD_DIR_RELEASE -H$WORKSPACE -DCUSTOM_SYSTEM_OS_NAME=Ubuntu
        cmake --build $LINUX_BUILD_DIR_RELEASE --target all --parallel 4
        cmake -B$LINUX_BUILD_DIR_DEBUG -H$WORKSPACE -DCUSTOM_SYSTEM_OS_NAME=Ubuntu -DCMAKE_BUILD_TYPE=Debug
        cmake --build $LINUX_BUILD_DIR_DEBUG --target all --parallel 4
    '''
}

def runtest_on_linux() {
    sh label : "Run test Release and Debug on Linux", script : '''
       cmake --build $LINUX_BUILD_DIR_DEBUG --target test
       cmake --build $LINUX_BUILD_DIR_RELEASE --target test
    '''
}

def consolidate_test_on_linux() {
    sh label : "Consolidate tests results for post build", script : '''
        python $LINUX_CHAINKINS_FILE --consolidate_junit --indir_debug=$LINUX_TESTRESULT_DIR_DEBUG --indir_release=$LINUX_TESTRESULT_DIR_RELEASE --outdir=$LINUX_POSTBUILD_DIR
        python $LINUX_CHAINKINS_FILE --consolidate_html --indir_debug=$LINUX_TESTRESULT_DIR_DEBUG --indir_release=$LINUX_TESTRESULT_DIR_RELEASE --outdir=$LINUX_POSTBUILD_DIR
        python $LINUX_CHAINKINS_FILE --dump_mainrepo_email_html --indir_debug=$LINUX_TESTRESULT_DIR_DEBUG --indir_release=$LINUX_TESTRESULT_DIR_RELEASE --outdir=$LINUX_POSTBUILD_DIR
    '''
}

def pack_on_linux() {
    sh label : "Pack Release and Debug on Linux", script : '''
        mkdir -p $LINUX_POSTBUILD_DIR
        cpack --config $LINUX_BUILD_DIR_DEBUG/CPackConfig.cmake -G TGZ
        cpack --config $LINUX_BUILD_DIR_RELEASE/CPackConfig.cmake -G TGZ
        mv SDKLibraries-v*Ubuntu*.tar.gz $LINUX_POSTBUILD_DIR
        ls -Ss1pq --block-size=M $LINUX_POSTBUILD_DIR/*.tar.gz
    '''
}

return this