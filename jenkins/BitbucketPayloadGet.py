#!/usr/bin/env python3

import os, sys
import argparse
import json

"""
Python script helping to parse Bitbucket BITBUCKET_PAYLOAD (json) and get the desired information
When Bitbucket trigger a jenkins build by a webhook, it send a few informations, where most of the code  change information
stay in the BITBUCKET_PAYLOAD json string. This script help to pull out the detail of those information, i.e destination of
a pull request, source of a pull request ... etc.
Note : This script assume the system variable BITBUCKET_PAYLOAD is set and exist
Usage :
    BitbucketPayloadGet --key_path=repository_scm # generic query
    BitbucketPayloadGet --source_repository       # give out the git link of the source repository
    BitbucketPayloadGet --destination_repository  # give out the git link of the destination repository

Example of info_path : pullrequest_source_repository_links_html_href : this get the html link of the source repository in the BITBUCKET_PAYLOAD
To use with Windows Batch script :
    for /f "delims=" %A in ('python BitbucketPayloadGet.py --source_repository') do set "BITBUCKET_PR_SOURCE=%A"  # this will retreive and set BITBUCKET_PR_SOURCE
    for /f "delims=" %A in ('python BitbucketPayloadGet.py --destination_repository') do set "BITBUCKET_PR_DESTINATION=%A"  # this will retreive and set BITBUCKET_PR_DESTINATION
    for /f "delims=" %A in ('python BitbucketPayloadGet.py --key_path=pullrequest_id') do set "BITBUCKET_PR_ID=%A"  # this will retreive and set BITBUCKET_PR_ID
To use with Linux shell script :
    BITBUCKET_PR_SOURCE=$(python BitbucketPayloadGet.py --source_repository)
    BITBUCKET_PR_DESTINATION$(python BitbucketPayloadGet.py --destination_repository)
    BITBUCKET_PR_ID(python BitbucketPayloadGet.py --key_path=pullrequest_id)
"""

example_BITBUCKET_PAYLOAD = '''
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
example_INFO_PATH = 'pullrequest_source_repository_links_html_href'

#print(example_BITBUCKET_PAYLOAD)

## From key_path_str separated by '_', return key_path in form of list
def _get_key_path_list(key_path_str):
    return key_path_str.split('_')

##  dict_obj        is a dictionary
##  key_path        is the key path in form of a list
##  key_path_index  is the index of the current key request in key_path.
##  The algorithm go recursively untill the key_path_index reach the end of the key_path. It will get the data from that point
def _get_json_data(dict_obj, key_path, key_path_index):
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

## Transform the html bitbucket link :
## From format : https://bitbucket.org/username/reponame"
## TO format   : git@bitbucket.org:username/reponame.git"
def _transform_git_html_to_ssh(html_link):
    parts = html_link.split('/')
    if parts and len(parts)>2:
        username = parts[-2]
        reponame = parts[-1]
        return 'git@bitbucket.org:{}/{}.git'.format(username,reponame)
    return ''

parser = argparse.ArgumentParser(description='BITBUCKET_PAYLOAD parser')
parser.add_argument('-k','--key_path'              , help='Key path \'_\'-separated to query the JSON string')
parser.add_argument('-s','--source_repository'     , dest='source_repository', action='store_true'      , help='Get the git source repository of the pull request. It cancel out other arguments')
parser.add_argument('-d','--destination_repository', dest='destination_repository', action='store_true' , help='Get the git destination repository of the pull request. It cancel out other arguments')

args = parser.parse_args()
if 'BITBUCKET_PAYLOAD' not in os.environ:
    print("Error environment variable BITBUCKET_PAYLOAD does not exist. The parser assume the system has set this variable")
    parser.print_help()
    sys.exit(2)

if args.source_repository:
    args.key_path=None
    args.destination_repository=None
if args.destination_repository:
    args.key_path=None
    args.source_repository=None

BITBUCKET_PAYLOAD_str = os.environ['BITBUCKET_PAYLOAD'] #example_BITBUCKET_PAYLOAD
bbPAYLOAD = json.loads(BITBUCKET_PAYLOAD_str)

if args.key_path is not None:
    key_path_str = args.key_path#example_INFO_PATH
    key_path_list = _get_key_path_list(key_path_str)
    result = _get_json_data(bbPAYLOAD, key_path_list, 0)
    print(result)
    sys.exit(0)

if args.source_repository:
    key_path_str_for_source_repo = 'pullrequest_source_repository_links_html_href'
    key_path_str = key_path_str_for_source_repo
    key_path_list = _get_key_path_list(key_path_str)
    source_repo_html = _get_json_data(bbPAYLOAD, key_path_list, 0)
    source_repo_ssh = _transform_git_html_to_ssh(source_repo_html)
    print(source_repo_ssh)
    sys.exit(0)

if args.destination_repository:
    key_path_str_for_destination_repo = 'pullrequest_destination_repository_links_html_href'
    key_path_str = key_path_str_for_destination_repo
    key_path_list = _get_key_path_list(key_path_str)
    destination_repo_html = _get_json_data(bbPAYLOAD, key_path_list, 0)
    destination_repo_ssh = _transform_git_html_to_ssh(destination_repo_html)
    print(destination_repo_ssh)
    sys.exit(0)