#!/usr/bin/env python3

import json
import urllib.request
import datetime
import urllib.parse


from conf import BRANCHES, PROTOCOLS


class Mirror():
    """
    Handle all mirror's properties
    """
    def __init__(self, mirror, country):
        self.state_file = None
        self.mirror = mirror
        self.country = country
        self.mirror_url = mirror.replace("$branch/$repo/$arch", "")
        self.protocol = str()
        self.last_sync = str()
        self.branches = dict()

    def get_state_file(self):
        """Fetch state file"""
        try:
            with urllib.request.urlopen(self.mirror_url + "state") as state_file:
                self.state_file = state_file.read().decode("utf-8")
        except urllib.error.URLError:
            print("\t\tCan't read state file.")

    def read_state_file(self, hashes):
        """Read infos from state file"""
        pos = self.state_file.find("date=")
        if pos >= 0:
            mirror_date = datetime.datetime.strptime(self.state_file[pos+5:pos+24], "%Y-%m-%dT%H:%M:%S")
        total_seconds = (datetime.datetime.now() - mirror_date).total_seconds()
        total_minutes = total_seconds // 60
        elapsed_hours = total_minutes // 60
        elapsed_minutes = total_minutes % 60
        # format last sync in format hh:mm
        self.last_sync = "{}:{}".format(str(int(elapsed_hours)).zfill(2), str(int(elapsed_minutes)).zfill(2))
        protocol = urllib.parse.urlsplit(self.mirror_url)[0]
        if protocol in PROTOCOLS:
            self.protocol = protocol
        for branch in BRANCHES:
            try:
                with urllib.request.urlopen(self.mirror_url + branch + "/state") as response:
                    content = response.read().decode("utf-8")
                    pos = content.find("state=")
                    if pos >= 0:
                        branch_hash = content[pos+6:pos+46]
                        self.branches[branch] = bool(branch_hash == hashes[branch])
            except urllib.error.URLError:
                    print("\t\tCan't read hash from state file.")
