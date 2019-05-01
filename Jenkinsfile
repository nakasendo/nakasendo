pipeline {
    agent none

    environment {
        EMAILto = "$DEFAULT_CONTENT"
        EMAILsubj = "$DEFAULT_SUBJECT"
        EMAILbody = "$DEFAULT_RECIPIENTS"
    }

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
            emailext to: $EMAILto
            subject: $EMAILsubj
            body: $EMAILbody
        }
    }
}
