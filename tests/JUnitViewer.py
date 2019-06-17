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

def percentage_to_hex_color(percentage):# percentage passed
    #green                                red
    color_vector = ['00ff2b', '00ffde', '0077ff', 'ffab00', 'ff6600', 'ff0000']
    percentage_int = int(percentage)
    color_index = int((100-percentage_int)/20)
    color_str = '#{}'.format(color_vector[color_index])
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
    html_table_str = '<h2>Test results [Build Mode]</h2>\n'
    html_table_str += '<table cellpadding="10">\n'
    col_id=0
    for test_suite in junitxml:
        if col_id < 1:
            html_table_str += '  <tr>\n'

        test_suite_name = test_suite.name
        test_suite_time = test_suite.time
        test_suite_tests = test_suite.tests
        test_suite_failures = test_suite.failures
        test_suite_errors = test_suite.errors
        total_failed = test_suite_failures + test_suite_errors
        total_passed = test_suite_tests-total_failed
        passing_percentage = 100*((total_passed+1)/(test_suite_tests+1))
        test_suite_color_str = percentage_to_hex_color(passing_percentage)
        html_table_str += '    <td {}>\n'.format('style="background-color:{}"'.format(test_suite_color_str))
        html_table_str += '        <b>'+ test_suite_name + '</b><br/>\n'
        html_table_str += '        time   : ' + str(test_suite_time) + 's<br/>\n'
        html_table_str += '        passed : ' + str(total_passed) + '<br/>\n'
        html_table_str += '        failed : ' + str(test_suite_failures) + '<br/>\n'
        html_table_str += '        error  : ' + str(test_suite_errors) + '<br/>\n'
        html_table_str += '        TOTAL  : ' + str(test_suite_tests) + '<br/>\n'
        html_table_str += '    </td>\n'

        col_id=col_id+1
        if col_id > 3:
            col_id=0
            html_table_str += '  </tr>\n'

    html_table_str += '</table>\n'
    return html_table_str

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
#recipients=['c.nguyen@nchain.com', 'chithanhnguyen.math@gmail.com']
recipients=['j.murphy@nchain.com', 'c.nguyen@nchain.com', 'c.battini@nchain.com', 'd.edunfunke@nchain.com', 'p.foster@nchain.com', 'r.balagourouche@nchain.com', 'm.rae@nchain.com', 'm.jenner@nchain.com']
SERVER = "smtp.office365.com"

server = smtplib.SMTP(SERVER, 587)
server.starttls()
server.login(args.mail_sender, args.mail_pass)

html_table_str = get_html_table(xml)
#print(html_table_str)
html_body_str = '<body>\n\n{}\n\n</body>\n'.format(html_table_str)
msg = email.message.Message()
msg['Subject'] = 'SDKLibraries test results [BuildName] [Build ID] [Build DateTime]'
msg['From'] = args.mail_sender
msg['To'] = ', '.join(recipients)
msg.add_header('Content-Type','text/html')
msg.set_payload(html_body_str)

server.sendmail(msg['From'], recipients, msg.as_string())
server.quit()