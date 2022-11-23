import sys


class Util:
    def __init__(self):
        pass

    @staticmethod
    def has_padding_error(response, success_msg, fail_msg, logger):
        padding_error = False
        if str.isdigit(success_msg):
            response_in_status_code = True
        else:
            response_in_status_code = False

        if response_in_status_code:
            if success_msg == str(response.status_code):
                padding_error = False
            elif fail_msg == str(response.status_code):
                padding_error = True
            else:
                logger.error("Unexpected reply from OAM. Please examine response")
                sys.exit(-1)
        else:
            if success_msg in response.text:
                padding_error = False
            elif fail_msg in response.text:
                padding_error = True
            else:
                logger.error("Unexpected reply from OAM. Please examine response")
                sys.exit(-1)

        return padding_error
