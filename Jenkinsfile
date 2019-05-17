pipeline {


    agent { 
        dockerfile  true
    }

    triggers {
        bitBucketTrigger([[$class: 'BitBucketPPRRepositoryTriggerFilter'
                , actionFilter: [$class: 'BitBucketPPRRepositoryPushActionFilter'
                , allowedBranches: 'master'
                , triggerAlsoIfTagPush: false]]
                , [$class: 'BitBucketPPRPullRequestTriggerFilter'
                , actionFilter: [$class: 'BitBucketPPRPullRequestApprovedActionFilter'
                , triggerOnlyIfAllReviewersApproved: false]]])
    }

    options { disableConcurrentBuilds() }
    stages {

        stage ('Build') {

            steps {
                sh '/entrypoint.sh'
            }
        }
        stage ('Unit Tests') {

            steps {
                sh 'python3 unittests.py``'
            }
        }
        stage ('Functional Tests') {

            steps {
                sh 'pwd'
                    dir ('tests/functional') {
                        sh 'python3 functionaltests.py'
                    }
            }
        }
    }

    post {
        cleanup { script:  cleanWs() }
        always  { chuckNorris() }
        success { archiveArtifacts 'build/**/x64/**/*.so' }
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
