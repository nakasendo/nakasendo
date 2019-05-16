pipeline {
    agent { 
        dockerfile  true
    }

    triggers {
        bitbucketPush()
    }

    stages {

        stage ('Build') {

            steps {
                sh '/entrypoint.sh'
            }
        }
    }

    post {
        cleanup { script:  cleanWs() }
        success { archiveArtifacts '**/*.so' }
        failure {
script: emailext (
                subject: "FAILED: Job '${env.JOB_NAME} [${env.BUILD_NUMBER}]'",
                body: """<p>FAILED: Job '${env.JOB_NAME} [${env.BUILD_NUMBER}]':</p>
                <p>Check console output (account needed) at &QUOT;<a href='${env.BUILD_URL}'>${env.JOB_NAME} [${env.BUILD_NUMBER}]</a>&QUOT;</p>""",
                recipientProviders: [[$class: 'CulpritsRecipientProvider'],
                [$class: 'DevelopersRecipientProvider'],
                [$class: 'RequesterRecipientProvider'], 
                [$class: 'FailingTestSuspectsRecipientProvider'],
                [$class: 'FirstFailingBuildSuspectsRecipientProvider'],
                [$class: 'UpstreamComitterRecipientProvider']]
                )
        }
    }
}
