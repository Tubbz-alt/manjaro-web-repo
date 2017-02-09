#!/usr/bin/env python3

import json
import urllib.request
import time

from collections import OrderedDict
from conf import MIRRORS_URL, BRANCHES
from mirror import Mirror
from builder import Builder
from helpers import close


class StatusChecker():
    """Launch of actions"""
    
    def __init__(self):
        self.mirrors = list()
        self.hashes = list()
        self.states = list()

    def get_mirrors(self):
        """Get list of mirrors"""
        try:
            with urllib.request.urlopen(MIRRORS_URL) as mirrors_file:
                self.mirrors = json.loads(mirrors_file.read().decode("utf-8"),
                                          object_pairs_hook=OrderedDict)
        except urllib.error.URLError:
            print("Error: can't fetch list of mirrors.")
            close()

    def get_hashes(self):
        """Get last hashes"""
        for branch in BRANCHES:
            try:
                with open("/var/repo/repo/" + branch + "/state", "r") as branch_state:
                    content = branch_state.read()
                    pos = content.find("state=")
                    if pos >= 0:
                        self.hashes.append(content[pos+6:pos+46])
            except OSError:
                pass
        if len(self.hashes) < len(BRANCHES):
            print("Error: can't fetch last hashes.")
            close()

    def check_mirrors(self):
        """Check each mirror"""
        for country in self.mirrors:
            print("{}:".format(country))
            for mirror in self.mirrors[country]:
                protocols = self.mirrors[country][mirror]["protocols"]
                mirror = Mirror(mirror, country, protocols)
                print("\t{}".format(mirror.mirror))
                if mirror.get_state_file():
                    mirror.read_state_file(self.hashes)
                    self.states.append({
                        "url": mirror.mirror,
                        "protocols": mirror.protocols,
                        "country": mirror.country,
                        "last_sync": mirror.last_sync,
                        "branches": mirror.branches
                    })


if __name__ == "__main__":
    print("--------------------------------")
    print("----Manjaro mirror's tracker----")
    print("--------------------------------")
    status_checker = StatusChecker()
    status_checker.get_mirrors()
    status_checker.get_hashes()
    status_checker.check_mirrors()
    builder = Builder(status_checker.states)
    builder.write_json_output()
    builder.write_html_output()
