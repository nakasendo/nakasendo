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
}
