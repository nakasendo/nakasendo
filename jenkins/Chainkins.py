#!/usr/bin/env python3

import os, sys
import argparse
import pathlib
import jenkinsapi
import bitbucketapi
import junithelper

import requests
from requests.auth import HTTPBasicAuth
import urllib3
urllib3.disable_warnings(urllib3.exceptions.InsecureRequestWarning) #https://stackoverflow.com/questions/27981545/suppress-insecurerequestwarning-unverified-https-request-is-being-made-in-pytho

'''
Python script helping jenkins build.
It can parse additional hidden information from bitbucket api, jenkins api
It can post status update to jenkins, bitbucket
Usage :
    Chainkins --get_jenkins_build_status    # give out the current status of the build assuming the environment variable BUILD_URL is set

Do remember that the script Chainkins.py is in $SDKLIBRARIES_ROOT/jenkins

Note : Use Environment Injector to pass environment variable from build job to post build actions : https://plugins.jenkins.io/envinject
Create a directory in ${CMAKE_BINARY_DIR}/postbuild
Inject values that need for post build to ${CMAKE_BINARY_DIR}/postbuild/env.properties
At build step, add step "Inject environment variables" after the executing the command populating ${CMAKE_BINARY_DIR}/postbuild/env.properties
See : https://stackoverflow.com/questions/38039633/sending-email-in-jenkins-with-body-from-content-in-file
'''

parser = argparse.ArgumentParser(description='Chainkins parser')
parser.add_argument('-b','--get_jenkins_build_status'     , dest='get_jenkins_build_status'     , action='store_true' , help='Get the current build status of the jenkins job')
parser.add_argument('-s','--get_pr_source_repository'     , dest='get_pr_source_repository'     , action='store_true' , help='Get the bitbucket source repository of the pull request')
parser.add_argument('-d','--get_pr_destination_repository', dest='get_pr_destination_repository', action='store_true' , help='Get the bitbucket destination repository of the pull request')

parser.add_argument(     '--get_bbpayload_info'           , dest='get_bbpayload_info'           , action='store_true' , help='Get info from json BITBUCKET_PAYLOAD. Requires --key_path')
parser.add_argument(     '--key_path' , help='Key path \':\'-separated to query the JSON string')

parser.add_argument(     '--get_short_hash'                , dest='get_short_hash'               , action='store_true' , help='Get first 8 chars from git commit hash. Requires --git_hash')
parser.add_argument(     '--git_hash' , help='Full Git Hash')

parser.add_argument(     '--get_local_branch'              , dest='get_local_branch'               , action='store_true' , help='Get local branch from remote branch. Requires --git_branch')
parser.add_argument(     '--git_branch' , help='git branch name')

parser.add_argument(     '--get_http_repo'                 , dest='get_http_repo'               , action='store_true' , help='Get http repo url from ssh repo url. Requires --ssh_repo')
parser.add_argument(     '--ssh_repo' , help='Repository ssh url')

parser.add_argument(     '--fix_nchain_email'              , dest='fix_nchain_email'               , action='store_true' , help='Fix nchain email. Requires --email')
parser.add_argument(     '--email' , help='Email address that might contain error username@nchain.com@nchain.com')

parser.add_argument(     '--consolidate_junit'            , dest='consolidate_junit'            , action='store_true' , help='Consolidate all xml junit test results. Requires --indir_debug --indir_release --outdir')
parser.add_argument(     '--consolidate_html'             , dest='consolidate_html'             , action='store_true' , help='Consolidate all xml junit test results into html content. Requires --indir_debug --indir_release --outdir')
parser.add_argument(     '--dump_pr_email_html'           , dest='dump_pr_email_html'           , action='store_true' , help='Write all xml junit test results into html email. Requires --indir_debug --indir_release --outdir')
parser.add_argument(     '--dump_mainrepo_email_html'     , dest='dump_mainrepo_email_html'     , action='store_true' , help='Write all xml junit test results into html email. Requires --indir_debug --indir_release --outdir')
parser.add_argument(     '--indir_debug'   , help='Directory containing all JUnit xml test results for debug build')
parser.add_argument(     '--indir_release' , help='Directory containing all JUnit xml test results for release build')
parser.add_argument(     '--outdir'        , help='Output directory where <all_junit_release.xml,all_junit_debug.xml> or  all_junit.html will be store')

parser.add_argument(     '--update_bitbucket_build_status' , dest='update_bitbucket_build_status', action='store_true' , help='Update bitbucket build status through rest API. Requires --bb_username --bb_password --target_repo --target_commit --jenkins_status')
parser.add_argument(     '--bb_username'   , help='Bitbucket credential username')
parser.add_argument(     '--bb_password'   , help='Bitbucket credential password')
parser.add_argument(     '--target_repo'   , help='Bitbucket http repository url')
parser.add_argument(     '--target_commit' , help='Bitbucket commit')
parser.add_argument(     '--jenkins_status', help='Jenkins build status. It will be transformed to the appropriated Bitbucket build status')
parser.add_argument(     '--build_href'    , help='Optional parameter forcing the url link of the build')

args = parser.parse_args()

if args.get_jenkins_build_status is not None and args.get_jenkins_build_status:
    ## Assume environment variable BUILD_URL is set
    ##   python Chainkins.py --get_jenkins_build_status
    current_build_status = jenkinsapi.get_current_build_status_str()
    print(current_build_status)
    sys.exit(0)

if args.get_pr_source_repository is not None and args.get_pr_source_repository:
    ## Assume environment variable BITBUCKET_PAYLOAD is set
    ##   python Chainkins.py --get_pr_source_repository
    source_repo_ssh = bitbucketapi.get_BITBUCKET_PR_source_ssh()
    print(source_repo_ssh)
    sys.exit(0)

if args.get_pr_destination_repository is not None and args.get_pr_destination_repository:
    ## Assume environment variable BITBUCKET_PAYLOAD is set
    ##   python Chainkins.py --get_pr_destination_repository
    destination_repo_ssh = bitbucketapi.get_BITBUCKET_PR_destination_ssh()
    print(destination_repo_ssh)
    sys.exit(0)

if args.get_bbpayload_info is not None and args.get_bbpayload_info:
    ## Assume environment variable BITBUCKET_PAYLOAD is set
    ##   python Chainkins.py --get_bbpayload_info --key_path=repository:scm
    key_path_str = ''
    if args.key_path is not None:
        key_path_str = args.key_path
    result = bitbucketapi.get_BITBUCKET_PAYLOAD_info(key_path_str)
    print(result)
    sys.exit(0)

if args.get_short_hash is not None and args.get_short_hash:
    ##   python Chainkins.py --get_short_hash --git_hash=f6a940645029272ff94a96d8b321691805c29f8b
    if args.git_hash is None:
        print("Get short git hash requires --git_hash")
        parser.print_help()
        sys.exit(2)
    long_git_hash = args.git_hash
    if len(long_git_hash)<8:
        print(long_git_hash)
        sys.exit(0)
    short_git_hash=long_git_hash[0:8]
    print(short_git_hash)
    sys.exit(0)

if args.get_local_branch is not None and args.get_local_branch:
    ##   python Chainkins.py --get_local_branch --git_branch=origin/feature/SL-123
    if args.git_branch is None:
        print("Get local branch requires --git_branch")
        parser.print_help()
        sys.exit(2)
    _local_branch = args.git_branch.replace('origin/', '')
    print(_local_branch)
    sys.exit(0)

if args.fix_nchain_email is not None and args.fix_nchain_email:
    ##   python Chainkins.py --fix_nchain_email --email=username@nchain.com@nchain.com
    if args.email is None:
        print("Fix email require --email")
        parser.print_help()
        sys.exit(2)
    error_email = args.email
    parts = error_email.split('@')
    if len(parts)<3: ## This is the good email
        print(error_email)
        sys.exit(0)
    print('{}@{}'.format(parts[0],parts[1]))
    sys.exit(0)

if args.get_http_repo is not None and args.get_http_repo:
    ##   python Chainkins.py --get_http_repo --ssh_repo=git@bitbucket.org:username/reponame.git
    if args.ssh_repo is None:
        print("Get http repo requires --ssh_repo")
        parser.print_help()
        sys.exit(2)
    _ssh_repo = args.ssh_repo
    _http_repo = bitbucketapi.transform_git_ssh_to_http(_ssh_repo)
    print(_http_repo)
    sys.exit(0)

if args.consolidate_junit is not None and args.consolidate_junit:
    ##   python Chainkins.py --consolidate_junit --indir_debug=$build_dir/x64/debug --indir_release=$build_dir/x64/release --outdir=$build_dir/testresult/junit
    if args.indir_debug is None or args.indir_release is None or args.outdir is None:
        print("Consolidate all xml junit test results requires --indir_debug --indir_release --outdir")
        parser.print_help()
        sys.exit(2)
    ## create output directory if not exist
    pathlib.Path(args.outdir).mkdir(parents=True, exist_ok=True)
    out_dir = pathlib.Path(args.outdir)
    ## aggregate all test results in debug mode
    test_result_dir_debug = pathlib.Path(args.indir_debug)
    xml_debug = junithelper.get_consolidated_junitxml(test_result_dir_debug, 'd')
    all_test_debug_file = out_dir / 'all_junit_debug.xml'
    xml_debug.write(all_test_debug_file)
    print('Chainkins successfully consolidate all junit debug tests result to {}'.format(str(all_test_debug_file)))
    ## aggregate all test results in release mode
    test_result_dir_release = pathlib.Path(args.indir_release)
    xml_release = junithelper.get_consolidated_junitxml(test_result_dir_release)
    all_test_release_file = out_dir / 'all_junit_release.xml'
    xml_release.write(all_test_release_file)
    print('Chainkins successfully consolidate all junit release tests result to {}'.format(str(all_test_release_file)))
    sys.exit(0)

if args.consolidate_html is not None and args.consolidate_html:
    ##   python Chainkins.py --consolidate_html --indir_debug=$build_dir/x64/debug --indir_release=$build_dir/x64/release --outdir=$build_dir/testresult/html
    if args.indir_debug is None or args.indir_release is None or args.outdir is None:
        print('Consolidate all xml junit test results into html content requires --indir_debug --indir_release --outdir')
        parser.print_help()
        sys.exit(2)
    ## create output directory if not exist
    pathlib.Path(args.outdir).mkdir(parents=True, exist_ok=True)
    out_dir = pathlib.Path(args.outdir)
    ## aggregate all test results in debug mode
    test_result_dir_debug = pathlib.Path(args.indir_debug)
    xml_debug = junithelper.get_consolidated_junitxml(test_result_dir_debug, 'd')
    ## aggregate all test results in release mode
    test_result_dir_release = pathlib.Path(args.indir_release)
    xml_release = junithelper.get_consolidated_junitxml(test_result_dir_release)
    test_result_html_content = junithelper.get_consolidated_html(xml_release, xml_debug)
    out_file = out_dir / 'all_junit.html'
    with out_file.open("w", encoding="utf-8") as f:
        f.write(test_result_html_content)
    print('Chainkins successfully consolidate all junit tests result to {}'.format(str(out_file)))
    sys.exit(0)

if args.dump_pr_email_html is not None and args.dump_pr_email_html:
    ## Assume environment variable BITBUCKET_PAYLOAD is set
    ##   python Chainkins.py --dump_pr_email_html --indir_debug=$build_dir/x64/debug --indir_release=$build_dir/x64/release --outdir=$build_dir/testresult/html
    if args.indir_debug is None or args.indir_release is None or args.outdir is None:
        print('Consolidate all xml junit test results into html content requires --indir_debug --indir_release --outdir')
        parser.print_help()
        sys.exit(2)
    ## create output directory if not exist
    pathlib.Path(args.outdir).mkdir(parents=True, exist_ok=True)
    out_dir = pathlib.Path(args.outdir)

    ## Get environment variables    
    jPR_BITBUCKET_ACTOR = os.environ['jPR_BITBUCKET_ACTOR'] if 'jPR_BITBUCKET_ACTOR' in os.environ else 'Unknown'
    jPR_BITBUCKET_TITLE = os.environ['jPR_BITBUCKET_TITLE'] if 'jPR_BITBUCKET_TITLE' in os.environ else 'Unknown'
    jTARGET_REPO_HTTP = os.environ['jTARGET_REPO_HTTP'] if 'jTARGET_REPO_HTTP' in os.environ else 'Unknown'
    jTARGET_BRANCH = os.environ['jTARGET_BRANCH'] if 'jTARGET_BRANCH' in os.environ else 'Unknown'
    jTARGET_COMMIT = os.environ['jTARGET_COMMIT'] if 'jTARGET_COMMIT' in os.environ else 'Unknown'
    jRUN_DISPLAY_URL = os.environ['RUN_DISPLAY_URL'] if 'RUN_DISPLAY_URL' in os.environ else 'Unknown URL'
    jBUILD_NUMBER = os.environ['BUILD_NUMBER'] if 'BUILD_NUMBER' in os.environ else 'Unknown'
    jBITBUCKET_PULL_REQUEST_LINK = os.environ['BITBUCKET_PULL_REQUEST_LINK'] if 'BITBUCKET_PULL_REQUEST_LINK' in os.environ else 'Unknown URL'
    jBITBUCKET_PULL_REQUEST_ID = os.environ['BITBUCKET_PULL_REQUEST_ID'] if 'BITBUCKET_PULL_REQUEST_ID' in os.environ else 'N/A'
    jBUILD_URL = os.environ['BUILD_URL'] if 'BUILD_URL' in os.environ else 'Unknown URL'

    ### Building email content ################################################
    html_email_content=''
    html_email_content += 'Pull Request author : <b>{}</b><br><br>\n\n'.format(jPR_BITBUCKET_ACTOR)
    html_email_content += 'Code review <a href={}>{}</a><br>\n'.format(jBITBUCKET_PULL_REQUEST_LINK, jPR_BITBUCKET_TITLE)
    html_email_content += 'Build log <a href={}>pipeline #{}</a><br><br>\n\n'.format(jRUN_DISPLAY_URL, jBUILD_NUMBER)

    ## aggregate all test results in debug mode
    test_result_dir_debug = pathlib.Path(args.indir_debug)
    xml_debug = junithelper.get_consolidated_junitxml(test_result_dir_debug, 'd')
    ## aggregate all test results in release mode
    test_result_dir_release = pathlib.Path(args.indir_release)
    xml_release = junithelper.get_consolidated_junitxml(test_result_dir_release)
    html_email_content += junithelper.get_consolidated_html(xml_release, xml_debug)
    html_email_content += '<br>\nSource repository : {}<br>\n'.format(jTARGET_REPO_HTTP)
    html_email_content += 'Source branch : {}<br>\n'.format(jTARGET_BRANCH)
    html_email_content += 'Source commit : <a href={}/commits/branch/{}>{}</a><br>\n'.format(jTARGET_REPO_HTTP,jTARGET_BRANCH,jTARGET_COMMIT)
    html_email_content += '<a href="{}/consoleFull">Jenkins full log build #{}</a><br><br>\n\n'.format(jBUILD_URL, jBUILD_NUMBER)
    out_file = out_dir / 'email.html'
    with out_file.open("w", encoding="utf-8") as f:
        f.write(html_email_content)
    print('Chainkins successfully dump pull request email test results file {}'.format(str(out_file)))
    sys.exit(0)

if args.dump_mainrepo_email_html is not None and args.dump_mainrepo_email_html:
    ## Assume environment variable BITBUCKET_PAYLOAD is set
    ##   python Chainkins.py --dump_mainrepo_email_html --indir_debug=$build_dir/x64/debug --indir_release=$build_dir/x64/release --outdir=$build_dir/testresult/html
    if args.indir_debug is None or args.indir_release is None or args.outdir is None:
        print('Consolidate all xml junit test results into html content requires --indir_debug --indir_release --outdir')
        parser.print_help()
        sys.exit(2)
    ## create output directory if not exist
    pathlib.Path(args.outdir).mkdir(parents=True, exist_ok=True)
    out_dir = pathlib.Path(args.outdir)

    ## Get other environment variables
    jTARGET_BRANCH = os.environ['jTARGET_BRANCH'] if 'jTARGET_BRANCH' in os.environ else 'Unable to define branch'
    jBUILD_TRIGGER = os.environ['jBUILD_TRIGGER'] if 'jBUILD_TRIGGER' in os.environ else 'Unknown'
    jRUN_DISPLAY_URL = os.environ['RUN_DISPLAY_URL'] if 'RUN_DISPLAY_URL' in os.environ else 'Unknown URL'
    jBUILD_NUMBER = os.environ['BUILD_NUMBER'] if 'BUILD_NUMBER' in os.environ else 'Unknown'
    jJENKINS_SLAVE_OS = os.environ['JENKINS_SLAVE_OS'] if 'JENKINS_SLAVE_OS' in os.environ else 'Unknown'
    jTARGET_REPO_HTTP = os.environ['jTARGET_REPO_HTTP'] if 'jTARGET_REPO_HTTP' in os.environ else 'Unknown URL'
    jTARGET_COMMIT = os.environ['jTARGET_COMMIT'] if 'jTARGET_COMMIT' in os.environ else 'Unknown'
    jBUILD_URL = os.environ['BUILD_URL'] if 'BUILD_URL' in os.environ else 'Unknown URL'

    ### Building email content ################################################
    html_email_content=''
    html_email_content += 'Build branch [<b>{}</b>] triggered by : <i>{}</i><br><br>\n\n'.format(jTARGET_BRANCH, jBUILD_TRIGGER)
    html_email_content += 'Build log <a href={}>pipeline #{}</a><br>\n'.format(jRUN_DISPLAY_URL, jBUILD_NUMBER)
    html_email_content += 'Commit : <a href={}/commits/branch/{}>{}</a><br><br>\n\n'.format(jTARGET_REPO_HTTP,jTARGET_BRANCH,jTARGET_COMMIT)

    ## aggregate all test results in debug mode
    test_result_dir_debug = pathlib.Path(args.indir_debug)
    xml_debug = junithelper.get_consolidated_junitxml(test_result_dir_debug, 'd')
    ## aggregate all test results in release mode
    test_result_dir_release = pathlib.Path(args.indir_release)
    xml_release = junithelper.get_consolidated_junitxml(test_result_dir_release)
    html_email_content += junithelper.get_consolidated_html(xml_release, xml_debug)
    html_email_content += '\n<br>Repository          : {}<br>\n'.format(jTARGET_REPO_HTTP)
    html_email_content += 'Commit Hash         : <a href={}/commits/branch/{}>{}</a><br>\n'.format(jTARGET_REPO_HTTP,jTARGET_BRANCH,jTARGET_COMMIT)
    html_email_content += '<a href="{}/consoleFull">Jenkins full log build #{}</a><br><br>\n\n'.format(jBUILD_URL, jBUILD_NUMBER)
    out_file = out_dir / 'email.html'
    with out_file.open("w", encoding="utf-8") as f:
        f.write(html_email_content)
    print('Chainkins successfully dump pull request email test results file {}'.format(str(out_file)))
    sys.exit(0)

if args.update_bitbucket_build_status is not None and args.update_bitbucket_build_status:
    ## Example command :
    ##   python Chainkins.py --update_bitbucket_build_status --bb_username=sdklibraries --bb_password=ppp --target_repo=https://bitbucket.org/nch-atlassian/sdklibraries --target_commit=cd64e137c4d2b58de55d625dc9285ee257aaa69e --jenkins_status=NOT_BUILT
    ## Jenkins build status   : SUCCESS    UNSTABLE  ABORTED   FAILURE    NOT_BUILT
    ## Bitbucket build status : SUCCESSFUL           STOPPED   FAILED    INPROGRESS
    ## optional parameter : --build_href=http://a/particular/url/link/for/the/build.
    if args.bb_username is None or args.bb_password is None or args.target_repo is None or args.target_commit is None or args.jenkins_status is None:
        print('Update bitbucket build status requires --bb_username --bb_password --target_repo --target_commit --jenkins_status')
        parser.print_help()
        sys.exit(2)

    ## create output directory if not exist
    jRUN_DISPLAY_URL = os.environ['RUN_DISPLAY_URL'] if 'RUN_DISPLAY_URL' in os.environ else 'Unknown URL'
    jJOB_BASE_NAME = os.environ['JOB_BASE_NAME'] if 'JOB_BASE_NAME' in os.environ else 'Unknown'
    jBUILD_NUMBER = os.environ['BUILD_NUMBER'] if 'BUILD_NUMBER' in os.environ else 'Unknown'
    jJENKINS_SLAVE_OS = os.environ['JENKINS_SLAVE_OS'] if 'JENKINS_SLAVE_OS' in os.environ else 'Unknown Build OS'
    jTARGET_BRANCH = os.environ['jTARGET_BRANCH'] if 'jTARGET_BRANCH' in os.environ else 'Unknown'
    jPR_BITBUCKET_TITLE = os.environ['jPR_BITBUCKET_TITLE'] if 'jPR_BITBUCKET_TITLE' in os.environ else 'unknown pull request'
    jBITBUCKET_PULL_REQUEST_LINK = os.environ['BITBUCKET_PULL_REQUEST_LINK'] if 'BITBUCKET_PULL_REQUEST_LINK' in os.environ else jRUN_DISPLAY_URL

    ## Unique id for a build is the commit hash+ job_base_name + os name
    long_query_key_str = '{}-{}-{}'.format(args.target_commit, jJOB_BASE_NAME, jJENKINS_SLAVE_OS)
    query_key = bitbucketapi.hash_bb_buildstatus_key(long_query_key_str)
    query_status=''
    query_name = ''
    query_href = args.build_href if args.build_href is not None else jRUN_DISPLAY_URL
    query_desc = ''
    if 'BITBUCKET_PULL_REQUEST_ID' in os.environ: ## This is a pull request build
        pr_id = os.environ['BITBUCKET_PULL_REQUEST_ID']
        query_name = '__pr#{} : {}'.format(pr_id, jPR_BITBUCKET_TITLE)
        query_desc = '{} build #{} on {}'.format(jJOB_BASE_NAME, jBUILD_NUMBER, jJENKINS_SLAVE_OS)
    else:
        query_name = '{} {}'.format(jJOB_BASE_NAME, jJENKINS_SLAVE_OS)
        query_desc = 'Build #{} branch [{}]'.format(jBUILD_NUMBER,jTARGET_BRANCH)
        if 'jBUILD_TRIGGER' in os.environ and 'Bitbucket' not in os.environ['jBUILD_TRIGGER']:## build on main repo that's triggered manually
            query_desc +=' - triggered by {}'.format(os.environ['jBUILD_TRIGGER'])

    query_status = bitbucketapi.get_bitbucket_status(args.jenkins_status) ## get bitbucket status from jenkins status

    ## get_bitbucket_buildstatus_query(http_repo, fullcommithash, query_key, bb_status, bb_name, bb_href, bb_desc):
    query_url, query_data = bitbucketapi.get_bitbucket_buildstatus_query(args.target_repo, args.target_commit, query_key, query_status, query_name, query_href, query_desc)

    response = requests.post(query_url, auth=HTTPBasicAuth(args.bb_username, args.bb_password), headers={'Content-Type': 'application/json'}, data=query_data, verify=False)
    response_str = response.content.decode('utf-8')
    #print('query_url  [{}]'.format(query_url))
    #print('query_data [{}]'.format(query_data))
    #print('response str\n{}'.format(response_str))
    print('\n{}\n'.format(response_str))
    sys.exit(0)