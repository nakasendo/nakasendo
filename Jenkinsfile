pipeline {
    agent none

    triggers {
        bitbucketPush()
    }

    stages {

        stage ('Build') {

            agent { 
                dockerfile  true 
            }

            steps {
                sh '/entrypoint.sh'
                    archiveArtifacts '**/*.so'
            }
        }
    }

    post {
        failure {
            emailext body: "$DEFAULT_CONTENT"
            to: "$DEFAULT_RECIPIENTS"
            subject: "$DEFAULT_SUBJECT"
        }
    }
}
