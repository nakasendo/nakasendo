This directory contains all files/tools helping to make a smooth CI.

jenkins files are for build jenkins pipeline
jenkinsapi.py   is python tool to interact with jenkins through rest api
bitbucketapi.py is python tool to interact with jenkins through rest api
junithelper.py  is python tool to aggregate/generate nice junit test reports
Chainkins.py    is the wrapper that use all python tools above and can be used in command line

TODO :
Investigate some more interesting jenkins plugins :
Kubernetes Plugin : a must for opensource
bootstraped-multi-test-results-report : generate nice test report view
Build Monitor plugin : nice dashboard of all builds
Monitor Pro Plugin : looks very nice but seems free version stopped supporting
Test Result Analyzer Plugin