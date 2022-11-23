import argparse
import urllib.parse

COLORS = {
    'GREEN': '\033[0;36m',  # 32
    'RED': '\033[0;31m',
    'YELLOW': '\033[0;33m',  # 32
    'BLUE': '\033[0;34m',  #
    'VIOLET': '\033[0;35m',  #
    'BOLD': '\033[1m',
    'NC': '\033[0m',
}

_COLOR_TO_CODE = {
    'g': COLORS['GREEN'],
    'r': COLORS['RED'],
    'y': COLORS['YELLOW'],
    'b': COLORS['BLUE'],
    'v': COLORS['VIOLET'],
    'bold': COLORS['BOLD']
}

# =====================
# Default option values
# =====================
HOST = '0.0.0.0:8080'


class ArgumentParser(argparse.ArgumentParser):

    def error(self, message):
        # self.print_help(sys.stderr)
        # self.print_usage(sys.stderr)
        print(self.format_usage().splitlines()[0])
        self.exit(2, color_msg(f'\nerror: {message}\n', 'r'))


# Ref.: https://stackoverflow.com/a/32891625/14664104
class MyFormatter(argparse.RawDescriptionHelpFormatter):
    """
    Corrected _max_action_length for the indenting of subactions
    """

    def add_argument(self, action):
        if action.help is not argparse.SUPPRESS:

            # find all invocations
            get_invocation = self._format_action_invocation
            invocations = [get_invocation(action)]
            current_indent = self._current_indent
            for subaction in self._iter_indented_subactions(action):
                # compensate for the indent that will be added
                indent_chg = self._current_indent - current_indent
                added_indent = 'x' * indent_chg
                invocations.append(added_indent + get_invocation(subaction))
            # print('inv', invocations)

            # update the maximum item length
            invocation_length = max([len(s) for s in invocations])
            action_length = invocation_length + self._current_indent
            self._action_max_length = max(self._action_max_length,
                                          action_length)

            # add the item to the list
            self._add_item(self._format_action, [action])

    # Ref.: https://stackoverflow.com/a/23941599/14664104
    def _format_action_invocation(self, action):
        if not action.option_strings:
            metavar, = self._metavar_formatter(action, action.dest)(1)
            return metavar
        else:
            parts = []
            # if the Optional doesn't take a value, format is:
            #    -s, --long
            if action.nargs == 0:
                parts.extend(action.option_strings)

            # if the Optional takes a value, format is:
            #    -s ARGS, --long ARGS
            # change to
            #    -s, --long ARGS
            else:
                default = action.dest.upper()
                args_string = self._format_args(action, default)
                for option_string in action.option_strings:
                    # parts.append('%s %s' % (option_string, args_string))
                    parts.append('%s' % option_string)
                parts[-1] += ' %s'%args_string
            return ', '.join(parts)


class ValidHostPort(argparse.Action):
    def __init__(self, option_strings, dest, nargs=None, **kwargs):
        if nargs is not None:
            raise ValueError("nargs not allowed")
        super().__init__(option_strings, dest, **kwargs)

    def __call__(self, parser, namespace, values, option_string=None):
        host, port = parse_hostport(values)
        if host and port:
            namespace.host = host
            namespace.port = port
            namespace.hostport = f'{host}:{port}'
        else:
            msg = f'Invalid host-port format: {values}. Valid format is ' \
                  'host:port, e.g. 0.0.0.0:8080'
            raise argparse.ArgumentTypeError(msg)


def color_msg(msg, color='y', bold=False):
    color = color.lower()
    colors = list(_COLOR_TO_CODE.keys())
    assert color in colors, f'Wrong color: {color}. Only these colors are ' \
                            f'supported: {colors}'
    if bold:
        msg = f"{COLORS['BOLD']}{msg}{COLORS['NC']}"
    return f"{_COLOR_TO_CODE[color]}{msg}{COLORS['NC']}"


def error():
    return f"{COLORS['RED']}ERROR{COLORS['NC']}"


def get_default_message(default_value):
    return f" ({COLORS['GREEN']}default: {default_value}{COLORS['NC']})"


def get_important_msg():
    return f'''
{COLORS['RED']}IMPORTANT:{COLORS['NC']} this code is for educational and informational purposes only. The
author, raul23, assumes no responsibility for the use of this code or any information 
contained therein. The user is solely responsible for any action he/she takes with this 
code and information contained in it.'''


def green_msg(msg):
    return f"{COLORS['GREEN']}{msg}{COLORS['NC']}"


# Ref.: https://stackoverflow.com/a/53172593/14664104
# NOTE: problematic "if it's important to detect and reject invalid host:port
# specifications"
def parse_hostport(hp):
    # urlparse() and urlsplit() insists on absolute URLs starting with "//"
    result = urllib.parse.urlsplit('//' + hp)
    return result.hostname, result.port
