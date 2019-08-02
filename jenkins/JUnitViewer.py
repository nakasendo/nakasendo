#!/usr/bin/env python3

import os, sys
import pathlib
import argparse

"""
Python script helping to import JUnit xml test results, merge them and print better views
It require to install the package 'junitparser'. It has to specify at least result_dir_debug
or result_dir_release parameter pointing to build debug/release directories
Usage:
    JUnitViewer --result_dir_debug="/absolute/path/to/build/dir/debug" --result_dir_release="/absolute/path/to/build/dir/release" # (use unix path)
To send and html email :
    JUnitViewer --result_dir_debug="/absolute/path/to/build/dir/debug" --result_dir_release="/absolute/path/to/build/dir/release" --mail_sender=my_email@nchain.com --mail_pass=my_email_password
"""

try:
    import junitparser
except ImportError :
    print("junitparser package is required to execute this script. Try to install it :")
    print("python -m -pip install junitparser")

def percentage_to_hex_color(passed_percentage):# percentage passed
    #light red                                 very red
    color_vector = ['ffff00', 'ffbf00', 'ff8000', 'ff9999', 'ff6666', 'ff0000']
    if passed_percentage >99.9:
        return '#00ff2b'# Green. absolutely everything passed

    percentage_int = int(passed_percentage)
    failed_color_index = int((100-percentage_int)/20)
    color_str = '#{}'.format(color_vector[failed_color_index])
    return color_str

def aggregate_junitxml(result_dir_path):
    xml_file_list = sorted(result_dir_path.glob('*test*.xml'))
    #print('\n'.join(xml_file_list))
    junit_xml = junitparser.JUnitXml()
    for xml_file in xml_file_list:
        test_suite = junitparser.JUnitXml.fromfile(xml_file)
        junit_xml.add_testsuite(test_suite)
    return junit_xml

def get_html_table(xml_junit):
    max_nb_col = 4
    html_table_str = '<table cellpadding="10">\n'
    col_id=0
    for test_suite in xml_junit:
        if col_id < 1:
            html_table_str += '  <tr>\n'

        test_suite_name = test_suite.name
        test_suite_time = test_suite.time
        nb_test_cases = len(test_suite)
        test_suite_failures = test_suite.failures
        test_suite_errors = test_suite.errors
        total_failed = test_suite_failures + test_suite_errors
        total_passed = nb_test_cases-total_failed
        passing_percentage = 0 if nb_test_cases == 0 else 100*((total_passed)/(nb_test_cases))
        test_suite_color_str = percentage_to_hex_color(passing_percentage)
        html_table_str += '    <td {}>\n'.format('style="background-color:{}"'.format(test_suite_color_str))
        html_table_str += '        <b>'+ test_suite_name + '</b><br>\n'
        html_table_str += '        time   : ' + str(test_suite_time) + 's<br>\n'
        html_table_str += '        passed : ' + str(total_passed) + '<br>\n'
        html_table_str += '        failed : ' + str(test_suite_failures) + '<br>\n'
        html_table_str += '        error  : ' + str(test_suite_errors) + '<br>\n'
        html_table_str += '        TOTAL  : ' + str(nb_test_cases) + '<br>\n'
        html_table_str += '    </td>\n'

        col_id=col_id+1
        if col_id > 3:
            col_id=0
            html_table_str += '  </tr>\n'

    html_table_str += '</table>\n'
    return html_table_str

def get_html_table_test_result(xml_release, xml_debug):
    html_test_str =''
    if len(xml_release) > 0:
        html_test_str+= '<b>Test results on Release</b>:<br>{}<br>\n'.format(get_html_table(xml_release))
    else:
        html_test_str+= '<font color="red"><b>Test results on Release</b>: MISSING</font><br><br>\n'

    if len(xml_debug) > 0:
        html_test_str+= '<b>Test results on Debug</b>:<br>{}<br>\n'.format(get_html_table(xml_debug))
    else:
        html_test_str+= '<font color="red"><b>Test results on Debug: MISSING</b></font><br><br>\n'

    return html_test_str

def get_html_body_email(email_sender, email_receivers, test_result_html_table):
    jenkins_env_vars = {}  # http://my_jenkins_host:8080/env-vars.html/
    ## grap Jenkins Environment variables
    ## TODO get more interesting Jenkins build status variable https://stackoverflow.com/questions/22264431/jenkins-job-build-status
    jenkins_env_vars['JENKINS_SLAVE_OS'] = os.environ['JENKINS_SLAVE_OS'] if 'JENKINS_SLAVE_OS' in os.environ else 'jJENKINS_SLAVE_OS'
    jenkins_env_vars['BUILD_URL'] = os.environ['BUILD_URL'] if 'BUILD_URL' in os.environ else 'jBUILD_URL'
    jenkins_env_vars['BUILD_NUMBER'] = os.environ['BUILD_NUMBER'] if 'BUILD_NUMBER' in os.environ else 'jBUILD_NUMBER'
    jenkins_env_vars['BITBUCKET_PULL_REQUEST_ID'] = os.environ['BITBUCKET_PULL_REQUEST_ID'] if 'BITBUCKET_PULL_REQUEST_ID' in os.environ else 'jBITBUCKET_PULL_REQUEST_ID'
    jenkins_env_vars['BITBUCKET_PULL_REQUEST_LINK'] = os.environ['BITBUCKET_PULL_REQUEST_LINK'] if 'BITBUCKET_PULL_REQUEST_LINK' in os.environ else 'jBITBUCKET_PULL_REQUEST_LINK'
    jenkins_env_vars['BUILD_URL'] = os.environ['BUILD_URL'] if 'BUILD_URL' in os.environ else 'jBUILD_URL'
    jenkins_env_vars['BITBUCKET_PR_ACTOR'] = os.environ['BITBUCKET_PR_ACTOR'] if 'BITBUCKET_PR_ACTOR' in os.environ else 'jBITBUCKET_PR_ACTOR'
    jenkins_env_vars['BITBUCKET_PR_SOURCE_REPO'] = os.environ['BITBUCKET_PR_SOURCE_REPO'] if 'BITBUCKET_PR_SOURCE_REPO' in os.environ else 'jBITBUCKET_PR_SOURCE_REPO'
    jenkins_env_vars['BITBUCKET_PR_TITLE'] = os.environ['BITBUCKET_PR_TITLE'] if 'BITBUCKET_PR_TITLE' in os.environ else 'jBITBUCKET_PR_TITLE'#pullrequest:title
    jenkins_env_vars['BITBUCKET_SOURCE_BRANCH'] = os.environ['BITBUCKET_SOURCE_BRANCH'] if 'BITBUCKET_SOURCE_BRANCH' in os.environ else 'jBITBUCKET_SOURCE_BRANCH'

    msg = email.message.Message()
    msg['Subject'] = '{} PR #{} : "{}"'.format(jenkins_env_vars['JENKINS_SLAVE_OS'], jenkins_env_vars['BITBUCKET_PULL_REQUEST_ID'], jenkins_env_vars['BITBUCKET_PR_TITLE'])
    msg['From'] = email_sender
    msg['To'] = ', '.join(email_receivers)
    msg.add_header('Content-Type','text/html')

    html_body_str = '<body>\n\n'
    html_body_str += 'Pull Request author : <b>{}</b><br><br>\n'.format(jenkins_env_vars['BITBUCKET_PR_ACTOR']) # pullrequest:author:display_name
    html_body_str += '<a href="{}/consoleFull">Log Build #{} on {}</a><br>\n'.format(jenkins_env_vars['BUILD_URL'], jenkins_env_vars['BUILD_NUMBER'], jenkins_env_vars['JENKINS_SLAVE_OS'])
    html_body_str += '<a href="{}">Repository <i>{}</i>   b[<b>{}</b>]</a><br><br><br>\n'.format(jenkins_env_vars['BITBUCKET_PULL_REQUEST_LINK'], jenkins_env_vars['BITBUCKET_PR_SOURCE_REPO'], jenkins_env_vars['BITBUCKET_SOURCE_BRANCH'])

    html_body_str += test_result_html_table + '\n\n'

    html_body_str += '</body>\n'
    msg.set_payload(html_body_str)
    return msg

parser = argparse.ArgumentParser(description='JUnitViewer')
parser.add_argument('--result_dir_debug', metavar='-d', help='Directory containing all JUnit xml test results for debug build')
parser.add_argument('--result_dir_release', metavar='-r', help='Directory containing all JUnit xml test results for release build')
parser.add_argument('--mail_sender', metavar='-s', help='Sender email (outlook)')
parser.add_argument('--mail_pass', metavar='-p', help='Sender email password')

args = parser.parse_args()
if not args.result_dir_debug and not args.result_dir_release:
    print("Directory containing JUnit xml files is expected")
    parser.print_help()
    sys.exit(2)

test_result_dir_debug = pathlib.Path(args.result_dir_debug) if args.result_dir_debug is not None else None
test_result_dir_release = pathlib.Path(args.result_dir_release) if args.result_dir_release is not None else None

xml_debug = aggregate_junitxml(test_result_dir_debug) if test_result_dir_debug is not None else junitparser.JUnitXml()
if test_result_dir_debug is not None:
    xml_debug.write(test_result_dir_debug/'test_all_junit.xml')

xml_release = aggregate_junitxml(test_result_dir_release) if test_result_dir_release is not None else junitparser.JUnitXml()
if test_result_dir_release is not None:
    xml_release.write(test_result_dir_release/'test_all_junit.xml')

###################################################################
## SEND EMAIL
has_sender=True
if not (args.mail_sender and args.mail_pass):
    has_sender=False
if not has_sender:
    print("Email will not be send. Require email and password")
    parser.print_help()
    sys.exit()

import email.message
import smtplib

## Build the html body for the email
html_table_str = get_html_table_test_result(xml_release, xml_debug)
#print(html_table_str)## Debug log

recipients=['j.murphy@nchain.com', 'c.nguyen@nchain.com', 'j.wilden@nchain.com', 'c.battini@nchain.com', 'd.edunfunke@nchain.com', 'r.balagourouche@nchain.com', 'm.rae@nchain.com', 'p.foster@nchain.com']

SERVER = "smtp.office365.com"
server = smtplib.SMTP(SERVER, 587)
server.starttls()
server.login(args.mail_sender, args.mail_pass)

msg = get_html_body_email(args.mail_sender, recipients, html_table_str)
#print(msg.as_string())## Debug log
server.sendmail(args.mail_sender, recipients, msg.as_string())
server.quit()

#https://stackoverflow.com/questions/44324335/how-to-get-environment-variable-in-jenkins-email-notification