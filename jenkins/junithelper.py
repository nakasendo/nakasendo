#!/usr/bin/env python3

import os, sys
import pathlib
import argparse

"""
Python module helping combine JUnit xml test results, merge them and print to a unified xml
It can also help to print html result
"""

try:
    import junitparser
except ImportError :
    print("junitparser package is required to execute this script. Try to install it :")
    print("python -m -pip install junitparser")

def _percentage_to_hex_color(passed_percentage):# percentage passed
    #light red                                 very red
    color_vector = ['ffff00', 'ffbf00', 'ff8000', 'ff9999', 'ff6666', 'ff0000']
    if passed_percentage >99.9:
        return '#00ff2b'# Green. absolutely everything passed
    percentage_int = int(passed_percentage)
    failed_color_index = int((100-percentage_int)/20)
    color_str = '#{}'.format(color_vector[failed_color_index])
    return color_str

def _append_testname_postfix(junit_xml, test_name_postfix):
    for child in junit_xml:
        if type(child) is junitparser.TestSuite:
            _append_testname_postfix(child, test_name_postfix)
        if type(child) is junitparser.TestCase:
            child.name += test_name_postfix
            child.classname += test_name_postfix

## Test reported by mocha has 'testsuites' tag which junitparser in python don't like it. Change testsuites to testsuite make it work
def _fix_testsuites_if_exist(xml_file):
    has_testsuites=False
    xml_content=''
    with open(xml_file) as f:
        xml_content = f.read()
        if 'testsuites' in xml_content:
            has_testsuites=True
            xml_content=xml_content.replace('testsuites', 'testsuite')
    if has_testsuites:
        with open(xml_file, "w") as f:
            f.write(xml_content)

def get_consolidated_junitxml(result_dir_path, test_name_postfix=''):
    xml_file_list = sorted(result_dir_path.glob('*test*.xml'))
    #print('\n'.join(xml_file_list))
    junit_xml = junitparser.JUnitXml()
    for xml_file in xml_file_list:
        _fix_testsuites_if_exist(xml_file)# Mocha junit reporter has testsuites which is incompatible with junitparser
        test_results = junitparser.JUnitXml.fromfile(xml_file)
        is_test_suites = False ## case where in the xml file is a test_suites (multiple test_suite)
        for suite in test_results.testsuites():
            junit_xml.add_testsuite(suite)
            is_test_suites=True
        if not is_test_suites:
            junit_xml.add_testsuite(test_results)

    if test_name_postfix is not None:
        _append_testname_postfix(junit_xml, test_name_postfix)
    return junit_xml

def _get_html_table(xml_junit):
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
        passing_percentage = 100 if nb_test_cases == 0 else 100*((total_passed)/(nb_test_cases))
        test_suite_color_str = _percentage_to_hex_color(passing_percentage)
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

def get_consolidated_html(xml_release, xml_debug):
    html_test_str =''
    if len(xml_release) > 0:
        html_test_str+= '<b>Test results on Release</b>:<br>{}<br>\n'.format(_get_html_table(xml_release))
    else:
        html_test_str+= '<font color="red"><b>Test results on Release</b>: MISSING</font><br><br>\n'

    if len(xml_debug) > 0:
        html_test_str+= '<b>Test results on Debug</b>:<br>{}<br>\n'.format(_get_html_table(xml_debug))
    else:
        html_test_str+= '<font color="red"><b>Test results on Debug: MISSING</b></font><br><br>\n'

    return html_test_str

