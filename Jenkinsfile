pipeline {
    agent { 
        dockerfile  true
    }

    triggers {
        bitBucketTrigger([[$class: 'BitBucketPPRRepositoryTriggerFilter'
                , actionFilter: [$class: 'BitBucketPPRRepositoryPushActionFilter'
                , allowedBranches: ''
                , triggerAlsoIfTagPush: false]]
                , [$class: 'BitBucketPPRPullRequestTriggerFilter'
                , actionFilter: [$class: 'BitBucketPPRPullRequestApprovedActionFilter'
                , triggerOnlyIfAllReviewersApproved: false]]])
    }

    options { disableConcurrentBuilds() }
    stages {

        stage ('Build') {

            steps {
                bitbucketStatusNotify(buildState: 'INPROGRESS')
                sh '/entrypoint.sh'
            }
        }
        stage ('Tests') {

            steps {
                bitbucketStatusNotify(buildState: 'TESTING')

                dir ('releasebuild') {
                    sh 'ctest'
                }   
            }
        }
        stage ('Pack') {
            steps {
                dir ('releasebuild') {
                    sh 'cpack -G TGZ'
                }   
            }
        }
    }

    post {
        cleanup { script:  cleanWs() }
        always  { 
                  chuckNorris() 
                  }
        success { 
                  bitbucketStatusNotify(buildState: 'SUCCESSFUL')
                  archiveArtifacts '**/SDKLibraries-*-Release.tar.gz, **/release-notes.txt'
                  }
        failure {
                  bitbucketStatusNotify(buildState: 'FAILED')
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
