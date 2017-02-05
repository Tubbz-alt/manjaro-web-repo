#!/usr/bin/env python3

import json
import urllib.request
import datetime
import urllib.parse

from conf import BRANCHES, PROTOCOLS


class Mirror():
    """Handle all mirror's properties"""

    def __init__(self, mirror, country):
        self.state_file = None
        self.mirror = mirror
        self.country = country
        self.protocol = str()
        self.last_sync = str()
        self.branches = list()

    def get_state_file(self):
        """Fetch state file"""
        try:
            with urllib.request.urlopen(self.mirror + "state") as state_file:
                self.state_file = state_file.read().decode("utf-8")
                return True
        except urllib.error.URLError:
            print("\t\tCan't read state file.")
            return False

    def read_state_file(self, hashes):
        """Read infos from state file"""
        pos = self.state_file.find("date=")
        if pos >= 0:
            mirror_date = datetime.datetime.strptime(self.state_file[pos+5:pos+24], "%Y-%m-%dT%H:%M:%S")
        diff = int((datetime.datetime.now() - mirror_date).total_seconds())
        self.last_sync = str(datetime.timedelta(seconds=diff))
        protocol = urllib.parse.urlsplit(self.mirror)[0]
        if protocol in PROTOCOLS:
            self.protocol = protocol
        for i, branch in enumerate(BRANCHES):
            try:
                with urllib.request.urlopen(self.mirror + branch + "/state") as response:
                    content = response.read().decode("utf-8")
                    pos = content.find("state=")
                    if pos >= 0:
                        branch_hash = content[pos+6:pos+46]
                        self.branches.append(bool(branch_hash == hashes[i]))
            except urllib.error.URLError:
                print("\t\tCan't read hash from state file.")
        if not self.branches:
            self.branches = None
