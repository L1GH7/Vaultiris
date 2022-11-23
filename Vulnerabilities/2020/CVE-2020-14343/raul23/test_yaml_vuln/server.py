import os
import re
import shlex
import subprocess
import sys

import yaml

from test_yaml_vuln.utils import (
    ArgumentParser, COLORS, HOST, MyFormatter, ValidHostPort, error,
    get_default_message, get_important_msg, green_msg, parse_hostport)

VERSION = '0.1a'
YAML_TO_CONDA_ENVS = {3.13: 'yaml313',
                      5.1: 'yaml51',
                      5.3: 'yaml53',
                      5.4: 'yaml54'}

YAML_BUILDS = {3.13: 'py37h1de35cc_0',
               5.1: 'py37h1de35cc_0',
               5.3: 'py37h1de35cc_0',
               5.4: 'py37h9ed2024_1'}


def input_validation(data):
    if not re.fullmatch(b"^[\n -\-/-\]a-}]*$", data,
                        flags=re.MULTILINE):
        return 1
    else:
        return 0


try:
    from flask import Flask, request

    app = Flask(__name__)
    app.secret_key = os.urandom(16)

    @app.route('/', methods=["POST"])
    def pwnme():
        file_content = request.files['file'].read()
        if input_validation(file_content):
            return "Nice try!\n", 400
        print('Loading yaml file...')
        yaml.load(file_content)
        # print(f'File content: {file_content}')
        return 'yaml file loaded\n'
except ImportError:
    from http.server import BaseHTTPRequestHandler, HTTPServer
    import cgi

    # TODO: return error message when there is an exception
    # Any error or yaml.constructor.ConstructorError?
    # e.g. `server -y 5.4` and `client --dp 3`
    # Error message like in flask:
    """
    <!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 3.2 Final//EN">
    <title>500 Internal Server Error</title>
    <h1>Internal Server Error</h1>
    <p>The server encountered an internal error and was unable to complete your request. Either the server is 
    overloaded or there is an error in the application.</p>
    """
    # Right now, client receives: curl: (52) Empty reply from server
    class MyServer(BaseHTTPRequestHandler):
        def _set_headers(self):
            self.send_response(200)
            self.send_header('Content-type', 'text/html')
            self.end_headers()

        def do_HEAD(self):
            self._set_headers()

        def do_POST(self):
            form = cgi.FieldStorage(
                fp=self.rfile,
                headers=self.headers,
                environ={'REQUEST_METHOD': 'POST'}
            )
            file_content = form.getvalue('file')
            if input_validation(file_content):
                self.send_response(400)
                self.send_header('Content-type', 'text/html')
                self.end_headers()
                self.wfile.write(b'Nice try!\n')
            else:
                print('Loading yaml file...')
                yaml.load(file_content)
                self._set_headers()
                self.wfile.write(b'yaml file loaded\n')

    class App:
        def __init__(self):
            pass

        @staticmethod
        def run(host, port):
            httpd = HTTPServer((host, port), MyServer)
            print(f' * Running on http://{host}:{port}/ (Press CTRL+C to quit)')
            httpd.serve_forever()

    app = App()


def run_conda_cmd(which_cmd, install_flask=False):
    def check_result_install(result, app_name):
        stdout = result.stdout.decode()
        # TODO: can also use `conda list --name env_name`
        if result.returncode == 0 and \
                stdout.find('Executing transaction: ...working... done') or \
                stdout.find('Successfully installed'):
            print(green_msg('Installed!'))
            return 0
        else:
            print(f"{error()} Couldn't install {app_name}\n"
                  f'{result.stderr.decode().strip()}')
            print('=====================================================')
            return 1

    result = subprocess.run(shlex.split('conda env list'), capture_output=True)
    env_list = result.stdout.decode()
    for yaml_version, env_name in YAML_TO_CONDA_ENVS.items():
        if env_list.find(f'\n{env_name} ') == -1:
            if which_cmd == 'create':
                print(f"Creating conda environment {COLORS['YELLOW']}"
                      f"'{env_name}'{COLORS['NC']} ...")
                conda_cmd = f'conda create -y --name {env_name} python=3.7'
                result = subprocess.run(shlex.split(conda_cmd),
                                        capture_output=True)
                if result.returncode == 0 and \
                        result.stdout.decode().find(f'conda activate {env_name}'):
                    print(green_msg('Created!'))
                else:
                    print(f"{error()} Couldn't create conda env '{env_name}'\n"
                          f"\n{result.stderr.decode().strip()}")
                    print('=====================================================\n')
                    continue
                print(f'Installing pyyaml-CVE-2020-14343 ...')
                result = subprocess.run(shlex.split('which conda'),
                                        capture_output=True)
                conda_cmd = f'conda run -n {env_name} ' \
                            'pip install git+https://github.com/raul23/pyyaml-CVE-2020-14343#egg=pyyaml-CVE-2020-14343'
                result = subprocess.run(shlex.split(conda_cmd),
                                        capture_output=True)
                if check_result_install(result, f'pyyaml-CVE-2020-14343'):
                    print("Stopping rest of installation for this conda env "
                          f"'{env_name}'")
                    continue
                print(f'Installing pyyaml {yaml_version} ...')
                build = YAML_BUILDS[yaml_version]
                conda_cmd = f'conda install -y --name {env_name} ' \
                            f'pyyaml={yaml_version}={build}'
                result = subprocess.run(shlex.split(conda_cmd),
                                        capture_output=True)
                if check_result_install(result, f'pyyaml {yaml_version}'):
                    print("Stopping rest of installation for this conda env "
                          f"'{env_name}'")
                    continue
                if install_flask:
                    print(f'Installing flask ...')
                    conda_cmd = f'conda install -y --name {env_name} flask'
                    result = subprocess.run(shlex.split(conda_cmd),
                                            capture_output=True)
                    check_result_install(result, 'flask')
                print()
        else:
            if which_cmd == 'remove':
                print(f"Removing conda environment {COLORS['YELLOW']}"
                      f"'{env_name}'{COLORS['NC']} ...")
                conda_cmd = f'conda env remove --name {env_name}'
                result = subprocess.run(shlex.split(conda_cmd),
                                        capture_output=True)
                result_msg = result.stderr.decode().strip()
                if result.returncode == 0 and \
                        'Remove all packages in environment' in result_msg and \
                        env_name in result_msg:
                    print(green_msg('Removed!'))
                else:
                    print(f"{error()} Couldn't remove conda env '{env_name}': "
                          f"{result_msg}")
                print()


def run_server(host, port):
    conda_env = sys.executable.split('/')[-3]
    print(f' * Conda env: {conda_env}')
    print(f' * PyYAML version: {yaml.__version__}')
    app.run(host=host, port=port)


def setup_argparser():
    width = os.get_terminal_size().columns - 5
    parser = ArgumentParser(
        description=f'''
Script for running a web server for testing the CVE-2020-14343 PyYAML vulnerability. The
listening server waits for a client to send it a yaml file that exploits the
vulnerability in order to make the server execute a remote code. The script can also create
conda environments with different PyYAML versions: {list(YAML_TO_CONDA_ENVS.keys())}.
{COLORS['BOLD']}NOTE:{COLORS['NC']} PyYAML with versions < 5.4 are affected with CVE-2020-14343.

{get_important_msg()}''',
        usage=f"{COLORS['BLUE']}%(prog)s [OPTIONS]{COLORS['NC']}",
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
    # =========================
    # Create conda environments
    # =========================
    create_group = parser.add_argument_group(
        f"{COLORS['YELLOW']}Create conda environments{COLORS['NC']}")
    create_group.add_argument(
        '-c', '--create-envs', action="store_true",
        help='Create all conda environments for testing with the '
             'CVE-2020-14343 PyYAML vulnerability. One conda environment for '
             'each of the following PyYAML version: '
             f'{list(YAML_TO_CONDA_ENVS.keys())}')
    create_group.add_argument(
        '-f', '--install-flask', action="store_true",
        help='Install flask for each created conda environment.')
    # =========================
    # Remove conda environments
    # =========================
    remove_group = parser.add_argument_group(
        f"{COLORS['YELLOW']}Remove conda environments{COLORS['NC']}")
    remove_group.add_argument(
        '-r', '--remove-envs', action="store_true",
        help='Delete all conda environments used for testing with the '
             'CVE-2020-14343 PyYAML vulnerability.')
    # ==============
    # Run web server
    # ==============
    run_group = parser.add_argument_group(
        f"{COLORS['YELLOW']}Run web server{COLORS['NC']}")
    run_group.add_argument(
        '--host', default='0.0.0.0:8080', action=ValidHostPort,
        help='Host and port for the server to run, e.g. 0.0.0.0:8080'
             + get_default_message(HOST))
    run_group.add_argument(
        '-y', '--yaml-version', choices=YAML_TO_CONDA_ENVS.keys(), type=float,
        help='Use a particular PyYAML version for the server to run.')
    return parser


def main():
    parser = setup_argparser()
    args = parser.parse_args()
    if args.host.count(':'):
        args.hostport = args.host
        args.host, args.port = parse_hostport(args.host)
    if args.create_envs:
        run_conda_cmd('create', args.install_flask)
    elif args.remove_envs:
        run_conda_cmd('remove')
    elif args.yaml_version:
        result = subprocess.run(shlex.split('which conda'), capture_output=True)
        conda_path = result.stdout.decode().split('/conda')[0]
        try:
            subprocess.run(
                f'source "{conda_path}/etc/profile.d/conda.sh" && conda '
                'deactivate && conda activate '
                f'{YAML_TO_CONDA_ENVS[args.yaml_version]} && server '
                f'--host {args.hostport}', shell=True)
        except KeyboardInterrupt:
            pass
        """
        # https://stackoverflow.com/a/65289264/14664104
        os.system(f'conda run -n {YAML_TO_CONDA_ENVS[args.yaml_version]} '
                  'python server.py')
        """
    else:
        try:
            run_server(args.host, args.port)
        except KeyboardInterrupt:
            pass


if __name__ == '__main__':
    try:
        main()
    except (KeyboardInterrupt, OSError) as e:
        # OSError: [Errno 48] Address already in use
        # if Ctrl+C too quick before "Running on ..."
        pass
