#!/usr/bin/env python3

import os, sys
import pathlib
import argparse

"""
Python script helping to import JUnit xml test results, merge them and print better views
It require to install the package 'junitparser'
Usage :
    JUnitViewer --result_dir="/absolute/path/to/pymodules/dir" # (use unix path)
To send and html email :
    JUnitViewer --result_dir="/absolute/path/to/pymodules/dir" --mail_sender=my_email@nchain.com --mail_pass=my_email_password
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

def get_junitxml(result_dir_path):
    xml_file_list = sorted(result_dir_path.glob('*test*.xml'))
    #print('\n'.join(xml_file_list))
    junit_xml = junitparser.JUnitXml()
    for xml_file in xml_file_list:
        test_suite = junitparser.JUnitXml.fromfile(xml_file)
        junit_xml.add_testsuite(test_suite)
    return junit_xml

def get_html_table(junitxml):
    max_nb_col = 4
    html_table_str = '<table cellpadding="10">\n'
    col_id=0
    for test_suite in junitxml:
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

def get_html_body_email(email_sender, email_receivers, test_result_html_table):
    jenkins_env_vars = {}  # http://my_jenkins_host:8080/env-vars.html/
    ## grap Jenkins Environment variables
    ## TODO get more interesting Jenkins build status variable https://stackoverflow.com/questions/22264431/jenkins-job-build-status
    jenkins_env_vars['JOB_NAME'] = os.environ['JOB_NAME'] if 'JOB_NAME' in os.environ else 'jJOB_NAME'
    jenkins_env_vars['JOB_URL'] = os.environ['JOB_URL'] if 'JOB_URL' in os.environ else 'jJOB_URL'
    jenkins_env_vars['BUILD_NUMBER'] = os.environ['BUILD_NUMBER'] if 'BUILD_NUMBER' in os.environ else 'jBUILD_NUMBER'
    jenkins_env_vars['BUILD_ID'] = os.environ['BUILD_ID'] if 'BUILD_ID' in os.environ else 'jBUILD_ID'
    jenkins_env_vars['BUILD_URL'] = os.environ['BUILD_URL'] if 'BUILD_URL' in os.environ else 'jBUILD_URL'
    jenkins_env_vars['GIT_COMMIT'] = os.environ['GIT_COMMIT'] if 'GIT_COMMIT' in os.environ else 'jGIT_COMMIT'
    jenkins_env_vars['GIT_COMMITTER_NAME'] = os.environ['GIT_COMMITTER_NAME'] if 'GIT_COMMITTER_NAME' in os.environ else 'jGIT_COMMITTER_NAME'
    jenkins_env_vars['CHANGE_ID'] = os.environ['CHANGE_ID'] if 'CHANGE_ID' in os.environ else 'jCHANGE_ID'  ## expected to be the PR ID
    jenkins_env_vars['CHANGE_URL'] = os.environ['CHANGE_URL'] if 'CHANGE_URL' in os.environ else 'jCHANGE_URL'  ## expected to be PR diff
    jenkins_env_vars['CHANGE_AUTHOR_DISPLAY_NAME'] = os.environ['CHANGE_AUTHOR_DISPLAY_NAME'] if 'CHANGE_AUTHOR_DISPLAY_NAME' in os.environ else 'jCHANGE_AUTHOR_DISPLAY_NAME'  ## expected to be PR diff

    msg = email.message.Message()
    msg['Subject'] = 'SDKLibraries build [{}] [{}]'.format(jenkins_env_vars['BUILD_NUMBER'],jenkins_env_vars['BUILD_ID'])
    msg['From'] = email_sender
    msg['To'] = ', '.join(email_receivers)
    msg.add_header('Content-Type','text/html')

    html_body_str = '<body>\n\n'
    html_body_str += '<b><a href="{}"><b>SDKLibraries test result for BUILD_NUMBER[{}] BUILD_ID[{}]</b></a><br><br><br>\n'.format(jenkins_env_vars['BUILD_URL'],jenkins_env_vars['BUILD_NUMBER'],jenkins_env_vars['BUILD_ID'])

    html_body_str += test_result_html_table + '\n\n'

    html_body_str += '\n\n<br><br> -----TODO Use all Jenkins Variable<br><br>\n'

    for jVarName, jVarValue in jenkins_env_vars.items():
        html_body_str += '{}:[{}]<br>\n'.format(jVarName, jVarValue)

    html_body_str += '</body>\n'
    msg.set_payload(html_body_str)
    return msg


parser = argparse.ArgumentParser(description='JUnitViewer')
parser.add_argument('--result_dir', metavar='-d', help='Directory containing all JUnit xml test results')
parser.add_argument('--mail_sender', metavar='-s', help='Sender email (outlook)')
parser.add_argument('--mail_pass', metavar='-p', help='Sender email password')

args = parser.parse_args()
if not args.result_dir:
    print("Directory containing JUnit xml files is expected")
    parser.print_help()
    sys.exit(2)

test_result_dir = pathlib.Path(args.result_dir)

xml = get_junitxml(test_result_dir)
out_xml = test_result_dir/'test_all_junit.xml'
xml.write(out_xml)

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
html_table_str = get_html_table(xml)
#print(html_table_str)## Debug log

recipients=['j.murphy@nchain.com', 'c.nguyen@nchain.com', 'c.battini@nchain.com', 'd.edunfunke@nchain.com', 'p.foster@nchain.com', 'r.balagourouche@nchain.com', 'm.rae@nchain.com', 'm.jenner@nchain.com']
SERVER = "smtp.office365.com"

server = smtplib.SMTP(SERVER, 587)
server.starttls()
server.login(args.mail_sender, args.mail_pass)

msg = get_html_body_email(args.mail_sender, recipients, html_table_str)
#print(msg.as_string())## Debug log
server.sendmail(args.mail_sender, recipients, msg.as_string())
server.quit()