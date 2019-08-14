#!/usr/bin/env python3

import os, sys
import argparse
import pathlib
import jenkinsapi
import bitbucketapi
import junithelper

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

parser.add_argument(     '--consolidate_junit'            , dest='consolidate_junit'            , action='store_true' , help='Consolidate all xml junit test results. Requires --indir_debug --indir_release --outdir')
parser.add_argument(     '--consolidate_html'             , dest='consolidate_html'             , action='store_true' , help='Consolidate all xml junit test results into html content. Requires --indir_debug --indir_release --outdir')
parser.add_argument(     '--dump_pr_email_html'           , dest='dump_pr_email_html'           , action='store_true' , help='Write all xml junit test results into html email. Requires --indir_debug --indir_release --outdir')
parser.add_argument(     '--indir_debug'   , help='Directory containing all JUnit xml test results for debug build')
parser.add_argument(     '--indir_release' , help='Directory containing all JUnit xml test results for release build')
parser.add_argument(     '--outdir'        , help='Output directory where <all_test_release.xml,all_test_debug.xml> or  all_test.html will be store')

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
    xml_debug = junithelper.get_consolidated_junitxml(test_result_dir_debug)
    xml_debug.write(out_dir / 'all_test_debug.xml')
    ## aggregate all test results in release mode
    test_result_dir_release = pathlib.Path(args.indir_release)
    xml_release = junithelper.get_consolidated_junitxml(test_result_dir_release)
    xml_release.write(out_dir / 'all_test_release.xml')
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
    xml_debug = junithelper.get_consolidated_junitxml(test_result_dir_debug)
    ## aggregate all test results in release mode
    test_result_dir_release = pathlib.Path(args.indir_release)
    xml_release = junithelper.get_consolidated_junitxml(test_result_dir_release)
    test_result_html_content = junithelper.get_consolidated_html(xml_release, xml_debug)
    out_file = out_dir / 'all_test.html'
    with out_file.open("w", encoding="utf-8") as f:
        f.write(test_result_html_content)
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

    ### Get information from BITBUCKET_PAYLOAD environment variable
    pr_author_key_path_str = 'pullrequest:author:display_name'
    pr_author_name = bitbucketapi.get_BITBUCKET_PAYLOAD_info(pr_author_key_path_str)
    source_repo_ssh = bitbucketapi.get_BITBUCKET_PR_source_ssh()
    pr_commit_hash_key_path_str = 'pullrequest:source:commit:hash'
    pr_tip_commit_hash = bitbucketapi.get_BITBUCKET_PAYLOAD_info(pr_commit_hash_key_path_str)

    ## Get other environment variables
    jBUILD_URL = os.environ['BUILD_URL'] if 'BUILD_URL' in os.environ else 'jBUILD_URL'
    jBUILD_NUMBER = os.environ['BUILD_NUMBER'] if 'BUILD_NUMBER' in os.environ else 'jBUILD_NUMBER'
    jJENKINS_SLAVE_OS = os.environ['JENKINS_SLAVE_OS'] if 'JENKINS_SLAVE_OS' in os.environ else 'jJENKINS_SLAVE_OS'
    jBITBUCKET_PULL_REQUEST_LINK = os.environ['BITBUCKET_PULL_REQUEST_LINK'] if 'BITBUCKET_PULL_REQUEST_LINK' in os.environ else 'jBITBUCKET_PULL_REQUEST_LINK'
    jBITBUCKET_SOURCE_BRANCH = os.environ['BITBUCKET_SOURCE_BRANCH'] if 'BITBUCKET_SOURCE_BRANCH' in os.environ else 'jBITBUCKET_SOURCE_BRANCH'

    ### Building email content ################################################
    html_email_content=''
    html_email_content += 'Pull Request author : <b>{}</b><br>\n'.format(pr_author_name)
    html_email_content += 'Commit Hash         : [{}]<br><br>\n\n'.format(pr_tip_commit_hash)
    html_email_content += 'Jenkins Log <a href="{}/consoleFull">Build #{}</a> on {}<br>'.format(jBUILD_URL, jBUILD_NUMBER, jJENKINS_SLAVE_OS)
    html_email_content += '<a href={}">Bitbucket</a> Repository <i>{}</i>   b[<b>{}</b>]<br><br><br>'.format(jBITBUCKET_PULL_REQUEST_LINK, source_repo_ssh, jBITBUCKET_SOURCE_BRANCH)

    ## aggregate all test results in debug mode
    test_result_dir_debug = pathlib.Path(args.indir_debug)
    xml_debug = junithelper.get_consolidated_junitxml(test_result_dir_debug)
    ## aggregate all test results in release mode
    test_result_dir_release = pathlib.Path(args.indir_release)
    xml_release = junithelper.get_consolidated_junitxml(test_result_dir_release)
    html_email_content += junithelper.get_consolidated_html(xml_release, xml_debug)
    out_file = out_dir / 'email.html'
    with out_file.open("w", encoding="utf-8") as f:
        f.write(html_email_content)
    sys.exit(0)