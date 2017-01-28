#!/usr/bin/env python3

import json
import urllib.request
import time

from conf import MIRRORS_URL, BRANCHES, SLEEP_TIME
from mirror import Mirror
from builder import Builder


class StatusChecker():
    """
    Launch of actions
    """
    def __init__(self):
        self.mirrors = list()
        self.hashes = dict()
        self.states = list()

    def get_mirrors(self):
        """Get list of mirrors"""
        try:
            with open(MIRRORS_URL) as mirrors_file:
                self.mirrors = json.load(mirrors_file)
        except urllib.error.URLError:
            print("Error: can't read list of mirrors.")


    def get_hashes(self):
        """Get last hashes"""
        for branch in BRANCHES:
            try:
                with open("/repo/repo/" + branch + "/state", "r") as branch_state:
                    content = branch_state.read()
                    pos = content.find("state=")
                    if pos >= 0:
                        self.hashes[branch] = content[pos+6:pos+46]
            except OSError:
                pass
        if len(self.hashes) < len(BRANCHES):
            self.hashes = {
                "stable": 0,
                "testing": 0,
                "unstable": 0
            }
            print("Error: can't fetch last hashes.")


    def check_mirrors(self):
        """Check each mirror"""
        for country in self.mirrors:
            print("{}:".format(country))
            for mirror in self.mirrors[country]:
                print("\t{}".format(mirror))
                mirror = Mirror(mirror, country)
                mirror.get_state_file()
                if mirror.state_file:
                    mirror.read_state_file(self.hashes)
                    self.states.append({
                        "url": mirror.mirror_url,
                        "protocol": mirror.protocol,
                        "country": mirror.country,
                        "last_sync": mirror.last_sync,
                        "branches": mirror.branches
                    })

if __name__ == "__main__":
    print("--------------------------------")
    print("----Manjaro mirror's tracker----")
    print("--------------------------------\n")
    status_checker = StatusChecker()
    while True:
        status_checker.get_mirrors()
        status_checker.get_hashes()
        status_checker.check_mirrors()
        builder = Builder(status_checker.states)
        builder.write_json_output()
        builder.write_html_output()
        print("Next check in {} min".format(SLEEP_TIME))
        time.sleep(SLEEP_TIME * 60)
