import os
import sys

from test_yaml_vuln.utils import (
    ArgumentParser, COLORS, HOST, MyFormatter, ValidHostPort, color_msg as c,
    get_default_message, get_important_msg)

VERSION = '0.1a'
PAYLOADS = [f'payload0{i}.yaml' for i in range(1, 4)]
_CUSTOM_OPTION = ('--cp', '--custom-payload')
_DEFAULT_OPTION = ('--dp', '--default-payload')


def get_usage():
    return f"{COLORS['BLUE']}{os.path.basename(__file__).split('.py')[0]} " \
           f"[OPTIONS]{COLORS['NC']}"


def setup_argparser():
    width = os.get_terminal_size().columns - 5
    parser = ArgumentParser(
        description=f'''
Script for sending yaml files to your local server in order to test if the server is 
affected by the CVE-2020-14343 PyYAML vulnerability which allows a maliciously crafted 
yaml file to execute remote code on a server that might be vulnerable.

{get_important_msg()}''',
        usage=get_usage(),
        add_help=False,
        # ArgumentDefaultsHelpFormatter
        # HelpFormatter
        # RawDescriptionHelpFormatter
        formatter_class=lambda prog: MyFormatter(
            prog, max_help_position=50, width=width))
    # ===============
    # General options
    # ===============
    general_group = parser.add_argument_group(
        f"{COLORS['YELLOW']}General options{COLORS['NC']}")
    general_group.add_argument('-h', '--help', action='help',
                               help='Show this help message and exit.')
    general_group.add_argument(
        '-v', '--version', action='version',
        version=f'%(prog)s v{VERSION}',
        help="Show program's version number and exit.")
    # ============
    # Send payload
    # ============
    send_group = parser.add_argument_group(
        f"{COLORS['YELLOW']}Send payload{COLORS['NC']}")
    send_group.add_argument(
        '--host', default='0.0.0.0:8080', action=ValidHostPort,
        help='Host and port of the server to connect, e.g. 0.0.0.0:8080'
             + get_default_message(HOST))
    parser_mutual_group = send_group.add_mutually_exclusive_group()
    parser_mutual_group.add_argument(
        _DEFAULT_OPTION[0], _DEFAULT_OPTION[1], dest='default',
        choices=[i + 1 for i in range(len(PAYLOADS))], type=int,
        help='ID of the default payload to send to the server. Check the '
             f'corresponding yaml file of the payload ID: {PAYLOADS}')
    parser_mutual_group.add_argument(
        _CUSTOM_OPTION[0], _CUSTOM_OPTION[1], dest='custom', metavar='FILE',
        help='Custom payload (yaml file) to send to the server.')
    return parser


def print_err(msg):
    print(f'usage: {get_usage()}')
    print(c(f'\nerror: {msg}', 'r'))


def main():
    parser = setup_argparser()
    args = parser.parse_args()
    args.hostport = args.host if args.host.count(':') else args.hostport
    if args.default:
        from test_yaml_vuln import __path__
        payload = os.path.join(__path__[0], PAYLOADS[args.default - 1])
    else:
        payload = args.custom
    if payload is None:
        print_err(f'No payload was given either with the '
                  f'{_CUSTOM_OPTION[0]}/{_CUSTOM_OPTION[1]} '
                  f'or {_DEFAULT_OPTION[0]}/{_DEFAULT_OPTION[1]} argument')
        sys.exit(2)
    os.system(f'curl -F file=@{payload} http://{args.hostport}')


if __name__ == '__main__':
    main()
