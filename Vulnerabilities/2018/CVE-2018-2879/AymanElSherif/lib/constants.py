import collections

MessageInfo = collections.namedtuple('MessageInfo', ['oam_url', 'message', 'encquery', 'extra_params'])


class Constants:
    def __init__(self):
        pass

    LOGGER_NAME = 'Main_Logger'
    DEFAULT_CIPHER_BLOCK_SIZE = 16
    BANNER_FILE_NAME = 'banner'
    PO_CLASS_NAME = "PadBuster"
    DEFAULT_WAIT_TIME = 3
    NETWORK_TIMEOUT = 10
    MAX_RETRIES = 5
    DEFAULT_PADDING_FAIL_MSG = "System error. Please re-try your action. " \
                               "If you continue to get this error, please contact the Administrator"
    DEFAULT_PADDING_SUCCESS_MSG = "encreply"
