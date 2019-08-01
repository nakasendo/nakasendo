#!/usr/bin/env python3

import os, sys
import argparse
import json
import bitbucketapi

"""
Python script helping to parse Bitbucket BITBUCKET_PAYLOAD (json) and get the desired information
When Bitbucket trigger a jenkins build by a webhook, it send a few informations, where most of the code  change information
stay in the BITBUCKET_PAYLOAD json string. This script help to pull out the detail of those information, i.e destination of
a pull request, source of a pull request ... etc.
Note : This script assume the system variable BITBUCKET_PAYLOAD is set and exist
Usage :
    BitbucketPayloadGet --key_path=repository:scm # generic query
    BitbucketPayloadGet --source_repository       # give out the git link of the source repository
    BitbucketPayloadGet --destination_repository  # give out the git link of the destination repository

Example of info_path : pullrequest_source_repository_links_html_href : this get the html link of the source repository in the BITBUCKET_PAYLOAD
To use with Windows Batch script :
    for /f "delims=" %A in ('python BitbucketPayloadGet.py --source_repository') do set "BITBUCKET_PR_SOURCE_REPO=%A"  # this will retreive and set BITBUCKET_PR_SOURCE_REPO
    for /f "delims=" %A in ('python BitbucketPayloadGet.py --destination_repository') do set "BITBUCKET_PR_DESTINATION_REPO=%A"  # this will retreive and set BITBUCKET_PR_DESTINATION_REPO
    for /f "delims=" %A in ('python BitbucketPayloadGet.py --key_path=pullrequest:id') do set "BITBUCKET_PR_ID=%A"  # this will retreive and set BITBUCKET_PR_ID
Note that for Windows, if use http git glone, then use 
  --key_path=pullrequest:source:repository:links:html:href
  --key_path=pullrequest:destination:repository:links:html:href
To get the repo url with https protocol
To use with Linux shell script :
    BITBUCKET_PR_SOURCE_REPO=$(python BitbucketPayloadGet.py --source_repository)
    BITBUCKET_PR_DESTINATION_REPO=$(python BitbucketPayloadGet.py --destination_repository)
    BITBUCKET_PR_ID(python BitbucketPayloadGet.py --key_path=pullrequest:id)

Do remember that the script BitbucketPayloadGet.py is in $SDKLIBRARIES_ROOT/jenkins
"""

if 'BITBUCKET_PAYLOAD' not in os.environ:
    print("Error missing environment variable BITBUCKET_PAYLOAD")
    sys.exit(2)

parser = argparse.ArgumentParser(description='BITBUCKET_PAYLOAD parser')
parser.add_argument('-k','--key_path'              , help='Key path \':\'-separated to query the JSON string')
parser.add_argument('-s','--source_repository'     , dest='source_repository', action='store_true'      , help='Get the git source repository of the pull request. It cancel out other arguments')
parser.add_argument('-d','--destination_repository', dest='destination_repository', action='store_true' , help='Get the git destination repository of the pull request. It cancel out other arguments')

args = parser.parse_args()

if args.source_repository:
    args.key_path=None
    args.destination_repository=None
if args.destination_repository:
    args.key_path=None
    args.source_repository=None

BITBUCKET_PAYLOAD_str = os.environ['BITBUCKET_PAYLOAD']
bbPAYLOAD = json.loads(BITBUCKET_PAYLOAD_str)

if args.key_path is not None:
    key_path_str = args.key_path
    result = bitbucketapi.get_BITBUCKET_PAYLOAD_info(key_path_str)
    print(result)
    sys.exit(0)

if args.source_repository:
    source_repo_ssh = bitbucketapi.get_BITBUCKET_PR_source_ssh()
    print(source_repo_ssh)
    sys.exit(0)

if args.destination_repository:
    destination_repo_ssh = bitbucketapi.get_BITBUCKET_PR_destination_ssh()
    print(destination_repo_ssh)
    sys.exit(0)