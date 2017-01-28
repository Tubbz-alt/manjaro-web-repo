#!/usr/bin/env python3

import json


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
                print("JSON output save in status.json")
        except OSError:
            print("Can't write JSON output")

    def write_html_output(self):
        """Generate HTML output"""
        try:
            with open("data/header.html", "r") as header_file:
                header = header_file.read()
        except OSError:
            print("Can't read HTML header file")
        try:
            with open("data/footer.html", "r") as footer_file:
                footer = footer_file.read()
        except OSError:
            print("Can't read HTML footer file")
        try:
            with open("index.html", "w") as index_file:
                content = header
                for state in self.states:
                    if state["branches"]["stable"] and state["branches"]["testing"] and state["branches"]["unstable"]:
                        color = "table-success"
                    elif state["branches"]["stable"] or state["branches"]["testing"] or state["branches"]["unstable"]:
                        color = "table-warning"
                    else:
                        color = "table-danger"
                    content += "<tr class=\"" + color + "\">"
                    content += "<tr>"
                    content += "<td><a href=\"" + state["url"] + "\">" + state["url"] + "</a></td>"
                    content += "<td>" + state['country'] + "</td>"
                    content += "<td>" + state["protocol"] + "</td>"
                    content += "<td>" + state["last_sync"] + "</td>"
                    for branch in state["branches"]:
                        if state["branches"][branch]:
                            content +="<td><i class=\"fa fa-check\" aria-hidden=\"true\"></i></td>";
                        else:
                            content += "<td><i class=\"fa fa-times\" aria-hidden=\"true\"></i></td>";
                    content += "</tr>"
                content += footer
                index_file.write(content)
                print("HTML output save in index.html")
        except OSError:
            print("Can't write HTML output")
