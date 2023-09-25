import numpy as np
import pandas as pd
from pandas.errors import ParserError, EmptyDataError

import configparser
from os import listdir, mkdir, getcwd, remove
from os.path import isfile, join, expanduser
from pathlib import Path
import subprocess
import time
import uuid
import requests
import base64
import hashlib
import json
import logging
import zlib

logger = logging.getLogger("__name__")

def configure_logger(debug, dryrun=False, log_prefix=None):
    """Configure the logger."""
    log_dir = Path.home() / f"{Path(__file__).stem}_LOG"
    log_dir.mkdir(exist_ok=True)
    log_file = log_dir / f"{time.strftime('%Y%m%d')}.log"
    log_format = ( 
        f"%(asctime)s{' - DRYRUN' if dryrun else ''} - %(levelname)s - {log_prefix if log_prefix else ''}%(message)s"
    )   
    logging.basicConfig(
        format=log_format,
        datefmt="%Y-%m-%d %H:%M:%S",
        level=logging.INFO,
        handlers=[logging.StreamHandler(), logging.FileHandler(log_file)],
        force=True,
    )   
    if debug:
        logger.setLevel(logging.DEBUG)
    logging.getLogger("sh").setLevel(logging.WARNING)

if __name__ == '__main__':
    js = {}

    config = configparser.ConfigParser()
    config.read('config.ini')

    configure_logger(debug=config['EXPORT']['debug'])

    data = {}
    m = hashlib.sha256()
    m.update(str(uuid.UUID(int = uuid.getnode())).encode())
    data['uuid'] = m.hexdigest()
    data['maxiter'] = config['IMAGE']['maxiter']
    data['shape'] = (config['IMAGE']['resx'], config['IMAGE']['resy'])
    data['nickname'] = config['EXPORT']['nickname']
    data['version'] = config['EXPORT']['version']
    url = f"{config['EXPORT']['url']}:{config['EXPORT']['port']}{config['EXPORT']['route']}"
    headers = {'content-type': 'application/json', 'Accept-Charset': 'UTF-8'}
    logger.debug(f'Sending {data} to {url}')

    histogram = np.zeros((int(config['IMAGE']['resx']), int(config['IMAGE']['resy'])))
    process_list = []

    for n in range(int(config['IMAGE']['workers'])):
        process_list.append(subprocess.Popen(['./buddhabroute-dev'],stdout=subprocess.PIPE))

    while True:
        for n, process in enumerate(process_list): 
            output = process.stdout.readline().decode("ascii")
            histogram = [float(x) for x in output.replace('\n', '').split(' ')[:-1]]
            try:
                histogram = np.reshape(histogram, (int(config['IMAGE']['resx']), int(config['IMAGE']['resy'])))
            except ValueError:
                logger.error(f"Wrong shape in the histogram ! Check size. Crashing.") 
                sys.exit()
            logger.debug(f'proc {n}: {histogram.shape}')


            # with compression to save bandwidth
            data['histogram'] = base64.b64encode(
                zlib.compress(
                    histogram.tobytes()
                )
            ).decode('utf-8')

            r = requests.post(url, json=data, headers=headers)
