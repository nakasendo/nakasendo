#!/usr/bin/env python3

import os, sys
import json

"""
Python module helping to parse/build Bitbucket information in order  to deal with BITBUCKET rest api
In particular the environment variable BITBUCKET_PAYLOAD (json) is set when Bitbucket trigger a jenkins build by a webhook, 
it send a few informations, where most of the code  change information
stay in the BITBUCKET_PAYLOAD json string. This script help to pull out the detail of those information, i.e destination of
a pull request, source of a pull request ... etc.
Note : This script only buid and get information, assuming certain environment variables exist
"""

## From key_path_str separated by ':', return key_path in form of list
## Example 'pullrequest:source:repository:links:html:href' will return ['pullrequest','source','repository','links','html','href']
def _split_key_path(key_path_str):
    return key_path_str.split(':')

## Transform the html bitbucket link :
## From format : https://bitbucket.org/username/reponame"
## TO format   : git@bitbucket.org:username/reponame.git"
def _transform_git_html_to_ssh(html_link):
    parts = html_link.split('/')
    if parts and len(parts)>2:
        username = parts[-2]
        reponame = parts[-1]
        return 'git@bitbucket.org:{}/{}.git'.format(username,reponame)
    return None

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

## Assuming BITBUCKET_PAYLOAD environment variables is set (json str). If not, throw exception
## Get the information from the json through the key path separated by ':'
def get_BITBUCKET_PAYLOAD_info(key_path_str):
    if 'BITBUCKET_PAYLOAD' not in os.environ:
        raise OSError("Missing environment variable BITBUCKET_PAYLOAD")
    BITBUCKET_PAYLOAD_str = os.environ['BITBUCKET_PAYLOAD']
    if len(key_path_str)<1:
        return BITBUCKET_PAYLOAD_str
    json_obj = json.loads(BITBUCKET_PAYLOAD_str)
    key_path = _split_key_path(key_path_str)
    result = _get_json_data(json_obj, key_path)
    return result

## Assuming BITBUCKET_PAYLOAD environment variables is set (json str). If not, throw exception
## Get the ssh git source repository of the pull request
def get_BITBUCKET_PR_source_ssh():
    if 'BITBUCKET_PAYLOAD' not in os.environ:
        raise OSError("Missing environment variable BITBUCKET_PAYLOAD")
    BITBUCKET_PAYLOAD_str = os.environ['BITBUCKET_PAYLOAD']
    json_obj = json.loads(BITBUCKET_PAYLOAD_str)
    source_repo_html_key_path_str ='pullrequest:source:repository:links:html:href'
    source_repo_html_key_path = _split_key_path(source_repo_html_key_path_str)
    source_repo_html = _get_json_data(json_obj, source_repo_html_key_path)
    source_repo_ssh = _transform_git_html_to_ssh(source_repo_html)
    if not source_repo_ssh:
        raise Exception("Error get_BITBUCKET_PR_source_ssh")
    return source_repo_ssh

## Assuming BITBUCKET_PAYLOAD environment variables is set (json str). If not, throw exception
## Get the ssh git destination repository of the pull request
def get_BITBUCKET_PR_destination_ssh():
    if 'BITBUCKET_PAYLOAD' not in os.environ:
        raise OSError("Missing environment variable BITBUCKET_PAYLOAD")
    BITBUCKET_PAYLOAD_str = os.environ['BITBUCKET_PAYLOAD']
    json_obj = json.loads(BITBUCKET_PAYLOAD_str)
    destination_repo_html_key_path_str ='pullrequest:destination:repository:links:html:href'
    destination_repo_html_key_path = _split_key_path(destination_repo_html_key_path_str)
    destination_repo_html = _get_json_data(json_obj, destination_repo_html_key_path)
    destination_repo_ssh = _transform_git_html_to_ssh(destination_repo_html)
    if not destination_repo_ssh:
        raise Exception("Error get_BITBUCKET_PR_destination_ssh")
    return destination_repo_ssh














###########################################################################################################################
###########################################################################################################################
### Tests locally. This part run only when execute this script
def _get_example_BITBUCKET_PAYLOAD():
    example_BITBUCKET_PAYLOAD ='''
    {  
       "repository":{  
          "scm":"git",
          "website":"",
          "name":"SDKLibraries",
          "links":{  
             "html":{  
                "href":"https://bitbucket.org/nch-atlassian/sdklibraries"
             },
             "self":{  
                "href":"https://api.bitbucket.org/2.0/repositories/nch-atlassian/sdklibraries"
             }
          },
          "owner":{  
             "type":"user",
             "display_name":"nch-atlassian",
             "uuid":"{63caba29-aa02-4cda-bda1-4ae94a4757eb}",
             "links":{  
                "html":{  
                   "href":"https://bitbucket.org/%7B63caba29-aa02-4cda-bda1-4ae94a4757eb%7D/"
                },
                "self":{  
                   "href":"https://api.bitbucket.org/2.0/users/%7B63caba29-aa02-4cda-bda1-4ae94a4757eb%7D"
                }
             },
             "nickname":"nch-atlassian"
          },
          "type":"repository",
          "is_private":true,
          "uuid":"{12b08376-cba1-4690-8a09-6af8e33077e3}"
       },
       "actor":{  
          "display_name":"Chi Thanh NGUYEN",
          "account_id":"5b7a840eb8e3cb589585e5e5",
          "type":"user",
          "links":{  
             "html":{  
                "href":"https://bitbucket.org/%7Bfdec1edd-a9e6-4cfa-b51a-7b59d908514d%7D/"
             },
             "self":{  
                "href":"https://api.bitbucket.org/2.0/users/%7Bfdec1edd-a9e6-4cfa-b51a-7b59d908514d%7D"
             }
          },
          "nickname":"Chi Thanh NGUYEN",
          "uuid":"{fdec1edd-a9e6-4cfa-b51a-7b59d908514d}"
       },
       "pullrequest":{  
          "id":"60",
          "title":"IGNORE this pull request, it\u0027s just for test purpose. DO NOT MERGE IT",
          "description":"",
          "state":"OPEN",
          "author":{  
             "display_name":"Chi Thanh NGUYEN",
             "account_id":"5b7a840eb8e3cb589585e5e5",
             "type":"user",
             "links":{  
                "html":{  
                   "href":"https://bitbucket.org/%7Bfdec1edd-a9e6-4cfa-b51a-7b59d908514d%7D/"
                },
                "self":{  
                   "href":"https://api.bitbucket.org/2.0/users/%7Bfdec1edd-a9e6-4cfa-b51a-7b59d908514d%7D"
                }
             },
             "nickname":"Chi Thanh NGUYEN",
             "uuid":"{fdec1edd-a9e6-4cfa-b51a-7b59d908514d}"
          },
          "created_on":"Jun 19, 2019 1:01:44 PM",
          "updated_on":"Jul 16, 2019 10:26:59 AM",
          "source":{  
             "branch":{  
                "name":"feature/jenkins_pr"
             },
             "commit":{  
                "hash":"2ae1cad6933f"
             },
             "repository":{  
                "name":"SDKLibraries-chi",
                "links":{  
                   "html":{  
                      "href":"https://bitbucket.org/cnguyennChain/sdklibraries-chi"
                   },
                   "self":{  
                      "href":"https://api.bitbucket.org/2.0/repositories/cnguyennChain/sdklibraries-chi"
                   }
                },
                "type":"repository",
                "uuid":"{99b5db93-2442-4c57-af6e-a38ff356ad76}"
             }
          },
          "destination":{  
             "branch":{  
                "name":"master"
             },
             "commit":{  
                "hash":"ea063804c3cd"
             },
             "repository":{  
                "name":"SDKLibraries",
                "links":{  
                   "html":{  
                      "href":"https://bitbucket.org/nch-atlassian/sdklibraries"
                   },
                   "self":{  
                      "href":"https://api.bitbucket.org/2.0/repositories/nch-atlassian/sdklibraries"
                   }
                },
                "type":"repository",
                "uuid":"{12b08376-cba1-4690-8a09-6af8e33077e3}"
             }
          },
          "participants":[  

          ],
          "type":"pullrequest",
          "reason":"",
          "links":{  
             "html":{  
                "href":"https://bitbucket.org/nch-atlassian/sdklibraries/pull-requests/60"
             },
             "self":{  
                "href":"https://api.bitbucket.org/2.0/repositories/nch-atlassian/sdklibraries/pullrequests/60"
             }
          }
       }
    }
    '''
    return example_BITBUCKET_PAYLOAD

def test__split_key_path():
    example_key_path = 'pullrequest:source:repository:links:html:href'
    split_key_path = _split_key_path(example_key_path)
    assert(len(split_key_path) == 6)
    assert (split_key_path[0] == 'pullrequest')
    assert (split_key_path[5] == 'href')
    #print(split_key_path)

def test__transform_git_html_to_ssh():
    example_git_http = 'https://bitbucket.org/username/reponame'
    git_ssh=_transform_git_html_to_ssh(example_git_http)
    assert (git_ssh == 'git@bitbucket.org:username/reponame.git')
    #print(git_ssh)

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

def test_get_BITBUCKET_PAYLOAD_info():
    test_bb_key_path_str = 'repository:scm'
    result_repo_scm=''
    ## When environment variable BITBUCKET_PAYLOAD does not exist, it should throw OSError when calling get_BITBUCKET_PAYLOAD_info
    try:
        result_repo_scm = get_BITBUCKET_PAYLOAD_info(test_bb_key_path_str)
    except OSError:
        result_repo_scm=''
    ## Set environment variable BITBUCKET_PAYLOAD and test
    example_BITBUCKET_PAYLOAD = _get_example_BITBUCKET_PAYLOAD()
    os.environ['BITBUCKET_PAYLOAD']=example_BITBUCKET_PAYLOAD
    result_repo_scm = get_BITBUCKET_PAYLOAD_info(test_bb_key_path_str)
    assert(result_repo_scm =='git')

def test_get_BITBUCKET_PR_source_ssh():
    source_ssh_repo=''
    ## When environment variable BITBUCKET_PAYLOAD does not exist, it should throw OSError when calling get_BITBUCKET_PR_source_ssh
    try:
        source_ssh_repo = get_BITBUCKET_PR_source_ssh()
    except OSError:
        source_ssh_repo=''
    ## Set environment variable BITBUCKET_PAYLOAD and test
    example_BITBUCKET_PAYLOAD = _get_example_BITBUCKET_PAYLOAD()
    os.environ['BITBUCKET_PAYLOAD']=example_BITBUCKET_PAYLOAD
    source_ssh_repo = get_BITBUCKET_PR_source_ssh()
    assert(source_ssh_repo =='git@bitbucket.org:cnguyennChain/sdklibraries-chi.git')

def test_get_BITBUCKET_PR_destination_ssh():
    destination_ssh_repo=''
    ## When environment variable BITBUCKET_PAYLOAD does not exist, it should throw OSError when calling get_BITBUCKET_PR_destination_ssh
    try:
        destination_ssh_repo = get_BITBUCKET_PR_destination_ssh()
    except OSError:
        destination_ssh_repo=''
    ## Set environment variable BITBUCKET_PAYLOAD and test
    example_BITBUCKET_PAYLOAD = _get_example_BITBUCKET_PAYLOAD()
    os.environ['BITBUCKET_PAYLOAD']=example_BITBUCKET_PAYLOAD
    destination_ssh_repo = get_BITBUCKET_PR_destination_ssh()
    assert(destination_ssh_repo =='git@bitbucket.org:nch-atlassian/sdklibraries.git')

if __name__ == '__main__':
    test__split_key_path()
    test__transform_git_html_to_ssh()
    test__get_json_data()
    test_get_BITBUCKET_PAYLOAD_info()
    test_get_BITBUCKET_PR_source_ssh()
    test_get_BITBUCKET_PR_destination_ssh()