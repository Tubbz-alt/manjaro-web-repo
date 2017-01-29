#!/usr/bin/env python3

import json
import datetime
import os

from conf import BRANCHES, VERSION
from helpers import close


class Builder():
    """
    Handle generation of output files
    """
    def __init__(self, states):
        self.states = states
        self.output_folder = "docs/"
        self.check_folder()

    def check_folder(self):
        try:
            if not os.path.isdir(self.output_folder):
                os.makedirs(self.output_folder)
        except OSError:
            print("Error: can't create output folder")
            close()


    def write_json_output(self):
        """Generate JSON output"""
        try:
            with open(self.output_folder + "status.json", "w") as json_output:
                json.dump(self.states, json_output, sort_keys=True)
                print("JSON output saved in {}status.json".format(self.output_folder))
        except OSError:
            print("Error: can't write JSON output")
            close()


    def write_html_output(self):
        """Generate HTML output"""
        try:
            with open("data/header.html", "r") as header_file:
                header = header_file.read()
            with open("data/footer.html", "r") as footer_file:
                footer = footer_file.read()
        except OSError:
            print("Error: can't read HTML template files")
            close()
        try:
            with open(self.output_folder + "index.html", "w") as index_file:
                header = header.replace("VERSION", "v{}".format(VERSION))
                header = header.replace("DATE", datetime.datetime.now().strftime("%Y-%m-%d %H:%M"))
                html_output = header
                for state in self.states:
                    if all(state["branches"]):
                        color = "table-success"
                    elif any(state["branches"]):
                        color = "table-warning"
                    else:
                        color = "table-danger"
                    html_output += "<tr class=\"{}\">".format(color)
                    html_output += "<td><a href=\"{url}\">{url}</a></td>".format(url=state["url"])
                    html_output += "<td>{}</td>".format(state["country"])
                    html_output += "<td>{}</td>".format(state["protocol"])
                    html_output += "<td>{}</td>".format(state["last_sync"])
                    for i, branch in enumerate(BRANCHES):
                        icon = '<td><i class="fa fa-{}" aria-hidden="true" data-toggle="tooltip" data-placement="top" title="{}"></i></td>'
                        if state["branches"][i]:
                            html_output += icon.format("check", "Up to date")
                        else:
                            html_output += icon.format("times", "Out of date")
                    html_output += "</tr>"
                html_output += footer
                index_file.write(html_output)
                print("HTML output saved in {}index.html".format(self.output_folder))
        except OSError:
            print("Error: can't write HTML output")
            close()
