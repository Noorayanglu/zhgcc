# -*- coding: utf-8 -*-
"""
Created on Sun Jan 21 14:56:59 2024

@author: husts
"""

import requests
import json

# ZHES233230009
#src_server = 'http://175.27.239.14:20063'
# ZHES233230010
#src_server = 'http://175.27.239.14:20065'
# ZHES233230011
#src_server = 'http://175.27.239.14:20069'
# ZHES233230012
src_server = 'http://175.27.239.14:20067'
src_req_url = '%s/api/settings/get'%(src_server)
src_r = requests.get(src_req_url, headers={'Accept': 'application/json'})
print(src_r.text)