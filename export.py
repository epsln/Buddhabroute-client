import numpy as np
import pandas as pd
from pandas.errors import ParserError, EmptyDataError

import configparser
from os import listdir, mkdir, getcwd, remove
from os.path import isfile, join, expanduser
import uuid
import requests
import base64
import hashlib
import json

js = {}

config = configparser.ConfigParser()
config.read('config.ini')

data = {}
m = hashlib.sha256()
m.update(str(uuid.UUID(int = uuid.getnode())).encode())
data['uuid'] = m.hexdigest()
data['maxiter'] = config['IMAGE']['maxiter']
data['shape'] = (config['IMAGE']['resx'], config['IMAGE']['resy'])
data['nickname'] = config['EXPORT']['nickname']


full_input_path = expanduser(config['EXPORT']['input_dir'])
filename_list = [join(full_input_path, f) for f in listdir(full_input_path)
                if isfile(join(full_input_path, f)) and f.endswith(".csv")
                and f != "checkpoint_buffer.csv"]

histogram = np.zeros((int(config['IMAGE']['resx']), int(config['IMAGE']['resy'])))

for input_file in filename_list:
    try:
        histo_df = pd.read_csv(input_file)
    except ParserError:
        remove(input_file)
        continue
    except UnicodeDecodeError:
        remove(input_file)
        continue
    except EmptyDataError:
        remove(input_file)
        continue

    if histo_df.shape != (int(config['IMAGE']['resx']), int(config['IMAGE']['resy'])):
        remove(input_file)
        continue

    histogram = np.add(histogram, histo_df.values)

data['histogram'] = base64.b64encode(histogram.tobytes()).decode('utf-8')
headers = {'content-type': 'application/json', 'Accept-Charset': 'UTF-8'}
r = requests.post(config['EXPORT']['url'], json=data, headers=headers)
