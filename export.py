#!/bin/python3
import numpy as np

import configparser
from os import listdir, mkdir, getcwd, remove
from os.path import isfile, join, expanduser
from pathlib import Path
import subprocess
import time
import threading
import uuid
import requests
import base64
import hashlib
import json
import sys
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

def readBuddhabroute(config, data, output, n):
    path = getcwd()
    if output:
        process = subprocess.Popen([join(path, './buddhabroute')],stdout=subprocess.PIPE)
    else:
        process = subprocess.Popen([join(path, './buddhabroute'), '--no-output'] ,stdout=subprocess.PIPE)
    url = f"{config['EXPORT']['url']}:{config['EXPORT']['port']}{config['EXPORT']['route']}"
    while True:
        output = process.stdout.readline().decode("ascii")
        if not output:
            break
        histogram = [float(x) for x in output.replace('\n', '').split(' ')[:-1]]
        try:
            histogram = np.reshape(histogram, (int(config['COMPUTE']['resx']), int(config['COMPUTE']['resy'])))
        except ValueError:
            logger.error(f"Wrong shape in the histogram !")
            logger.error(f"Expected size {int(config['COMPUTE']['resx']), int(config['COMPUTE']['resy'])}.")
            logger.error(f"Received size {histogram.shape}.")
            sys.exit()
        # with compression to save bandwidth
        logger.debug(f"Exporting ! {n}")
        logger.debug(f"{histogram.sum()}")
        data['histogram'] = base64.b64encode(
            zlib.compress(
                histogram.tobytes()
            )
        ).decode('utf-8')

        r = requests.post(url, json=data, headers=headers)

if __name__ == '__main__':
    js = {}
    path = getcwd()

    config = configparser.ConfigParser()
    config.read(join(path, 'config.ini'))

    configure_logger(debug=config['EXPORT']['debug'])

    data = {}
    m = hashlib.sha256()
    m.update(str(uuid.UUID(int = uuid.getnode())).encode())
    data['uuid'] = m.hexdigest()
    data['maxiter'] = config['COMPUTE']['maxiter']
    data['shape'] = (config['COMPUTE']['resx'], config['COMPUTE']['resy'])
    data['function_name'] = config['COMPUTE']['function_name']
    data['nickname'] = config['EXPORT']['nickname']
    data['version'] = config['EXPORT']['version']
    headers = {'content-type': 'application/json', 'Accept-Charset': 'UTF-8'}

    histogram = np.zeros((int(config['COMPUTE']['resx']), int(config['COMPUTE']['resy'])))
    thread_list = []

    #Main thread does not stop, so we can't put it in the same list as other ones
    main_thread = threading.Thread(target = readBuddhabroute, args = (config, data, True, 0))
    main_thread.start()

    for n in range(max(int(config['EXPORT']['workers']), 0)):
        x = threading.Thread(target = readBuddhabroute, args = (config, data, False, n), daemon = True)
        time.sleep(2)
        thread_list.append(x)
        x.start()
