#!/usr/bin/env python3

import json
import urllib.request
import datetime

from conf import BRANCHES


class Mirror():
    """Handle all mirror's properties"""

    def __init__(self, mirror, country, protocols):
        self.state_file = None
        self.mirror = mirror
        self.country = country
        self.protocols = protocols
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
            mirror_date = datetime.datetime.strptime(self.state_file[pos+5:pos+24],
                                                     "%Y-%m-%dT%H:%M:%S")
        seconds = (datetime.datetime.utcnow() - mirror_date).total_seconds()
        minutes = seconds // 60
        elapsed_hours = minutes // 60
        elapsed_minutes = minutes % 60
        self.last_sync = "{}:{}".format(str(int(elapsed_hours)).zfill(2),
                                      str(int(elapsed_minutes)).zfill(2))
        protocol = urllib.parse.urlsplit(self.mirror)[0]
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
