#!/usr/bin/env python3

import json

from conf import BRANCHES, VERSION


class Builder():
    """
    Handle generation of output files
    """
    def __init__(self, states):
        self.states = states


    def write_json_output(self):
        """Generate JSON output"""
        try:
            with open("status.json", "w") as json_output:
                json.dump(self.states, json_output)
                print("JSON output saved in status.json")
        except OSError:
            print("Can't write JSON output")


    def write_html_output(self):
        """Generate HTML output"""
        try:
            with open("data/header.html", "r") as header_file:
                header = header_file.read()
            with open("data/footer.html", "r") as footer_file:
                footer = footer_file.read()
        except OSError:
            print("Can't read HTML template files")
        try:
            with open("index.html", "w") as index_file:
                header = header.replace("VERSION", VERSION)
                html_output = header
                for state in self.states:
                    if state["branches"]["stable"] and state["branches"]["testing"] and state["branches"]["unstable"]:
                        color = "table-success"
                    elif state["branches"]["stable"] or state["branches"]["testing"] or state["branches"]["unstable"]:
                        color = "table-warning"
                    else:
                        color = "table-danger"
                    html_output += "<tr class=\"" + color + "\">"
                    html_output += "<td><a href=\"" + state["url"] + "\">" + state["url"] + "</a></td>"
                    html_output += "<td>" + state['country'] + "</td>"
                    html_output += "<td>" + state["protocol"] + "</td>"
                    html_output += "<td>" + state["last_sync"] + "</td>"
                    for branch in BRANCHES:
                        if state["branches"][branch]:
                            html_output +="<td><i class=\"fa fa-check\" aria-hidden=\"true\"></i></td>";
                        else:
                            html_output += "<td><i class=\"fa fa-times\" aria-hidden=\"true\"></i></td>";
                    html_output += "</tr>"
                html_output += footer
                index_file.write(html_output)
                print("HTML output saved in index.html")
        except OSError:
            print("Can't write HTML output")
