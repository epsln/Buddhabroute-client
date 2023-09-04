import numpy as np
import pandas as pd
from pandas.errors import ParserError, EmptyDataError

import configparser
from os import listdir, mkdir, getcwd, remove
from os.path import isfile, join, expanduser
from pathlib import Path
import time
import uuid
import requests
import pdb
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
    logger.debug(f'{data}')


    full_input_path = expanduser(config['EXPORT']['input_dir'])
    filename_list = [join(full_input_path, f) for f in listdir(full_input_path)
                    if isfile(join(full_input_path, f)) and f.endswith(".csv")
                    and f != "checkpoint_buffer.csv"]
    logger.debug(f'{filename_list}')

    histogram = np.zeros((int(config['IMAGE']['resx']), int(config['IMAGE']['resy'])))

    for input_file in filename_list:
        try:
            histo_df = pd.read_csv(input_file)
            logger.debug(f'reading {input_file}')
        except ParserError:
            logger.info(f'{input_file} failed to parse, removing')
            remove(input_file)
            continue
        except UnicodeDecodeError:
            logger.info(f'{input_file} failed to decode, removing')
            remove(input_file)
            continue
        except EmptyDataError:
            logger.info(f'{input_file} is empty, removing')
            remove(input_file)
            continue

        if histo_df.shape != (int(config['IMAGE']['resx']), int(config['IMAGE']['resy'])):
            logger.info(f'{input_file} is not the correct dimension, removing')
            remove(input_file)
            continue

        histogram = np.add(histogram, histo_df.values)
        remove(input_file)

    url = f"{config['EXPORT']['url']}:{config['EXPORT']['port']}{config['EXPORT']['route']}"
    logger.debug(f'{url}')

    headers = {'content-type': 'application/json', 'Accept-Charset': 'UTF-8'}
    logger.debug(f'{headers}')
    # with compression to save bandwidth
    data['histogram'] = base64.b64encode(
        zlib.compress(
            histogram.tobytes()
        )
    ).decode('utf-8')

    pdb.set_trace()
    r = requests.post(url, json=data, headers=headers)
