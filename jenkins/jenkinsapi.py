#!/usr/bin/env python3

import os, sys
import json
import requests
from requests.auth import HTTPBasicAuth
import urllib3
urllib3.disable_warnings(urllib3.exceptions.InsecureRequestWarning) #https://stackoverflow.com/questions/27981545/suppress-insecurerequestwarning-unverified-https-request-is-being-made-in-pytho

"""
Python script module helping to parse/build jenkins information in order  to deal with jenkins rest api
"""

## From key_path_str separated by ':', return key_path in form of list
## Example 'pullrequest:source:repository:links:html:href' will return ['pullrequest','source','repository','links','html','href']
def _split_key_path(key_path_str):
    return key_path_str.split(':')

##  dict_obj        is a dictionary parsed from json
##  key_path        is the key path in form of a list
##  key_path_index  is the index of the current key request in key_path.
##  The algorithm go recursively untill the key_path_index reach the end of the key_path_str. It will get the data from that point
def _get_json_data(dict_obj, key_path, key_path_index=0):
    key_path_len = len(key_path)
    request_key = key_path[key_path_index]
    if key_path_index == key_path_len -1:
        if request_key in dict_obj:
            return dict_obj[request_key]
        else:
            return None
    else:
        if request_key in dict_obj:
            return _get_json_data(dict_obj[request_key], key_path, key_path_index+1)
        else:
            return None

## Use request to get jenkins json api from a provided build url
## Use credential sdklibraries@nchain.com:11f2803d6dd9c72048328b2c451a383ef9
## curl -k -X GET build_url_str -u sdklibraries@nchain.com:11f2803d6dd9c72048328b2c451a383ef9
def _get_build_api_json_str(build_url_str):
    json_api_str = ''
    response = requests.get(build_url_str, auth=HTTPBasicAuth('sdklibraries@nchain.com', '11f2803d6dd9c72048328b2c451a383ef9'), verify=False)
    json_api_str = response.content.decode('utf-8')
    return json_api_str

## Assuming BUILD_URL environment variables is set. If not, throw exception
## Use https request to request the build json api
def get_current_build_api_json_str():
    if 'BUILD_URL' not in os.environ:
        raise OSError("Missing environment variable BUILD_URL")
    BUILD_URL_str = os.environ['BUILD_URL']
    return _get_build_api_json_str(BUILD_URL_str)

## Assuming BUILD_URL environment variables is set. If not, throw exception
## Use https request to request the build json api
def get_current_build_api_json_str():
    if 'BUILD_URL' not in os.environ:
        raise OSError("Missing environment variable BUILD_URL")
    BUILD_URL_str = os.environ['BUILD_URL']
    return _get_build_api_json_str(BUILD_URL_str)

## Assuming BUILD_URL environment variables is set. If not, throw exception
## Use https request to request the build json api then get the build status part
def get_current_build_status_str():
    if 'BUILD_URL' not in os.environ:
        raise OSError("Missing environment variable BUILD_URL")
    BUILD_URL_str = os.environ['BUILD_URL']
    BUILD_URL__API_JSON_str = BUILD_URL_str + '/api/json'
    build_api_json_str=_get_build_api_json_str(BUILD_URL__API_JSON_str)
    json_obj = json.loads(build_api_json_str)
    key_path = ['result']
    current_result = _get_json_data(json_obj, key_path)
    return current_result

###########################################################################################################################
###########################################################################################################################
### Tests locally. This part run only when execute this script
def _get_example_BUILD_URL():
    example_BUILD_URL='https://142.93.35.114/job/SDKLibraries/job/testjobs/job/pr_pipeline/12'
    return example_BUILD_URL

def _get_example_JENKINS_JSON_API():
    example_JENKINS_JSON_API ='''
{  
   "_class":"org.jenkinsci.plugins.workflow.job.WorkflowRun",
   "actions":[  
      {  
         "_class":"hudson.model.CauseAction",
         "causes":[  
            {  
               "_class":"io.jenkins.plugins.bitbucketpushandpullrequest.cause.pullrequest.cloud.BitBucketPPRPullRequestCreatedCause",
               "shortDescription":"Started by user Chi Thanh NGUYEN: Bitbucket PPR: new pull request created"
            }
         ]
      },
      {  

      },
      {  

      },
      {  

      },
      {  

      },
      {  
         "_class":"hudson.plugins.git.util.BuildData",
         "buildsByBranchName":{  
            "refs/remotes/origin/master":{  
               "_class":"hudson.plugins.git.util.Build",
               "buildNumber":12,
               "buildResult":null,
               "marked":{  
                  "SHA1":"7ce4dfbd92f2436b601bc425a57079c71fbf6764",
                  "branch":[  
                     {  
                        "SHA1":"7ce4dfbd92f2436b601bc425a57079c71fbf6764",
                        "name":"refs/remotes/origin/master"
                     }
                  ]
               },
               "revision":{  
                  "SHA1":"7ce4dfbd92f2436b601bc425a57079c71fbf6764",
                  "branch":[  
                     {  
                        "SHA1":"7ce4dfbd92f2436b601bc425a57079c71fbf6764",
                        "name":"refs/remotes/origin/master"
                     }
                  ]
               }
            }
         },
         "lastBuiltRevision":{  
            "SHA1":"7ce4dfbd92f2436b601bc425a57079c71fbf6764",
            "branch":[  
               {  
                  "SHA1":"7ce4dfbd92f2436b601bc425a57079c71fbf6764",
                  "name":"refs/remotes/origin/master"
               }
            ]
         },
         "remoteUrls":[  
            "git@bitbucket.org:nch-atlassian/sdklibraries.git"
         ],
         "scmName":""
      },
      {  
         "_class":"hudson.plugins.git.GitTagAction"
      },
      {  

      },
      {  

      },
      {  
         "_class":"hudson.plugins.git.util.BuildData",
         "buildsByBranchName":{  
            "refs/remotes/origin/master":{  
               "_class":"hudson.plugins.git.util.Build",
               "buildNumber":12,
               "buildResult":null,
               "marked":{  
                  "SHA1":"c522ae3469c4000c47d357950d7837db48912744",
                  "branch":[  
                     {  
                        "SHA1":"c522ae3469c4000c47d357950d7837db48912744",
                        "name":"refs/remotes/origin/master"
                     }
                  ]
               },
               "revision":{  
                  "SHA1":"c522ae3469c4000c47d357950d7837db48912744",
                  "branch":[  
                     {  
                        "SHA1":"c522ae3469c4000c47d357950d7837db48912744",
                        "name":"refs/remotes/origin/master"
                     }
                  ]
               }
            }
         },
         "lastBuiltRevision":{  
            "SHA1":"c522ae3469c4000c47d357950d7837db48912744",
            "branch":[  
               {  
                  "SHA1":"c522ae3469c4000c47d357950d7837db48912744",
                  "name":"refs/remotes/origin/master"
               }
            ]
         },
         "remoteUrls":[  
            "git@bitbucket.org:nch-atlassian/sdklibraries.git"
         ],
         "scmName":""
      },
      {  

      },
      {  

      },
      {  

      },
      {  

      },
      {  
         "_class":"org.jenkinsci.plugins.pipeline.modeldefinition.actions.RestartDeclarativePipelineAction"
      },
      {  

      },
      {  
         "_class":"org.jenkinsci.plugins.workflow.job.views.FlowGraphAction"
      },
      {  

      },
      {  

      },
      {  

      }
   ],
   "artifacts":[  

   ],
   "building":false,
   "description":null,
   "displayName":"#12",
   "duration":478032,
   "estimatedDuration":168090,
   "executor":null,
   "fullDisplayName":"SDKLibraries » testjobs » pr_pipeline #12",
   "id":"12",
   "keepLog":false,
   "number":12,
   "queueId":273,
   "result":"SUCCESS",
   "timestamp":1564661182385,
   "url":"https://142.93.35.114/job/SDKLibraries/job/testjobs/job/pr_pipeline/12/",
   "changeSets":[  
      {  
         "_class":"hudson.plugins.git.GitChangeSetList",
         "items":[  
            {  
               "_class":"hudson.plugins.git.GitChangeSet",
               "affectedPaths":[  
                  "jenkins/pull-request.pipeline"
               ],
               "commitId":"c522ae3469c4000c47d357950d7837db48912744",
               "timestamp":1564660986000,
               "author":{  
                  "absoluteUrl":"https://142.93.35.114/user/c.nguyen",
                  "fullName":"c.nguyen"
               },
               "authorEmail":"c.nguyen@nchain.com",
               "comment":"improve pipeline file for pull request\n",
               "date":"2019-08-01 13:03:06 +0100",
               "id":"c522ae3469c4000c47d357950d7837db48912744",
               "msg":"improve pipeline file for pull request",
               "paths":[  
                  {  
                     "editType":"edit",
                     "file":"jenkins/pull-request.pipeline"
                  }
               ]
            }
         ],
         "kind":"git"
      }
   ],
   "culprits":[  
      {  
         "absoluteUrl":"https://142.93.35.114/user/c.nguyen",
         "fullName":"c.nguyen"
      }
   ],
   "nextBuild":null,
   "previousBuild":{  
      "number":11,
      "url":"https://142.93.35.114/job/SDKLibraries/job/testjobs/job/pr_pipeline/11/"
   }
}
    '''
    return example_JENKINS_JSON_API

def test__split_key_path():
    example_key_path = 'pullrequest:source:repository:links:html:href'
    split_key_path = _split_key_path(example_key_path)
    assert(len(split_key_path) == 6)
    assert (split_key_path[0] == 'pullrequest')
    assert (split_key_path[5] == 'href')
    #print(split_key_path)

def test__get_json_data():
    example_json_str = '{"foo":{"bar":"hello bar","identity":{"name":"hello foo","age":"12"}}}'
    json_obj = json.loads(example_json_str)
    example_test_path_str1 = 'foo:bar'
    test_path1 = _split_key_path(example_test_path_str1)
    result1 = _get_json_data(json_obj, test_path1)
    assert(result1 =='hello bar')
    example_test_path_str2 = 'foo:identity:name'
    test_path2 = _split_key_path(example_test_path_str2)
    result2 = _get_json_data(json_obj, test_path2)
    assert(result2 =='hello foo')

def test_access_build_api_json_connection():
    # https://stackoverflow.com/questions/33419568/accessing-https-sites-with-ip-address
    test_BUILD_URL = _get_example_BUILD_URL() + '/api/json'
    response_content_str = _get_build_api_json_str(test_BUILD_URL)
    assert ('result' in response_content_str)
    #print(response_content_str)

def test_get_current_build_api_json_str():
    build_info_json_str=''
    ## When environment variable BUILD_URL does not exist, it should throw OSError when calling get_current_build_api_json_str
    try:
        build_info_json_str = get_current_build_api_json_str()
    except OSError:
        build_info_json_str=''
    ## Set environment variable BUILD_URL and test
    os.environ['BUILD_URL']=_get_example_BUILD_URL() + '/api/json'
    build_info_json_str = get_current_build_api_json_str()
    assert ('result' in build_info_json_str )

def test_get_current_build_status_str():
    current_build_status=''
    ## When environment variable BUILD_URL does not exist, it should throw OSError when calling get_current_build_status_str
    try:
        current_build_status = get_current_build_status_str()
    except OSError:
        current_build_status=''
    ## Set environment variable BUILD_URL and test
    os.environ['BUILD_URL']=_get_example_BUILD_URL() + '/api/json'
    current_build_status = get_current_build_status_str()
    assert (current_build_status == 'SUCCESS' )

if __name__ == '__main__':
    test__split_key_path()
    test__get_json_data()

    test_access_build_api_json_connection()
    test_get_current_build_api_json_str()
    test_get_current_build_status_str()