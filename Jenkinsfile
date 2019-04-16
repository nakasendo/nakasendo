pipeline {
    agent { 
        dockerfile {
            label 'linux' && 'slave'
        }
    }
    stages {
        stage('Clean workspace') {
            steps {
                deleteDir()
            }
        }
        stage('Checkout') {
            steps {
                checkout([$class: 'GitSCM' \
                        , branches: [[name: '*/master']] \
                        , doGenerateSubmoduleConfigurations: false \
                        , extensions: [] \
                        , submoduleCfg: [] \
                        , userRemoteConfigs: [[credentialsId: 'c6291d6c-0a47-4c3f-8cc1-83e9a9da2f0b' \
                        , url: 'https://bitbucket.org/nch-atlassian/sdklibraries/src/master/']]])
            }
        }
    }
}
