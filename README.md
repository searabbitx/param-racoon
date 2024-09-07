# 🦝param-racoon🦝
A tool similar to [param-miner](https://github.com/PortSwigger/param-miner) but worse and written in C++ for no good reason. But! You don't need Burp to use it. 

![image](https://user-images.githubusercontent.com/46087838/164746647-32d6186c-03f2-4cf0-8573-4f1052a64506.png)


# usage
```
Usage:
  param-racoon [options] url

Allowed options:
  -h [ --help ]         produce help message
  -t [ --threads ] arg  set number of threads (default: 10)
  -w [ --wordlist ] arg set path to the wordlist file
  -H [ --header ] arg   set a header
  -a [ --agent ] arg    set user agent
  -c [ --cookies ] arg  set cookies (in "NAME1=VAL1; NAME2=VAL2" format)
  -X [ --method ] arg   set request method (default: GET)
  -d [ --data ] arg     set data to be sent in request body
  -m [ --match ] arg    match a string in response to recognize valid params
  -f [ --filter ] arg   filter out responses with a string to recognize valid params
  -D [ --min-diff ] arg minimal difference in response length that is required 
                        to consider response 'different' (default: 1)
  -p [ --proxy ] arg    use the specified proxy (format: [protocol://]host[:port])
  -r [ --request ] arg  read a raw http request from the specified file
  -q [ --quiet ]        do not print the banner and configuration table
  --url arg             set target url
```

### Examples:

Basic usage (testing params based on a set of default heuristics):

```bash
param-racoon \
    -t 5 \
    -w 'wordlist/params.txt' \
    -a 'Racoonium/5.0 (X11; Racoonix x86_64)'
    -H 'X-Auth-Token: foo' \
    -H 'X-Requested-With: XMLHttpRequest' \
    -c 'PHPSESSID=dc12e575ca; LANG=en' \
    'http://example.com/some/endpoint'
```

Looking for a specific string in the response to recognize valid params:

```bash
param-racoon -w 'wordlist/params.txt' -m 'Success!' 'http://example.com/some/endpoint'
```

Looking for a specific string in the response to filter out invalid params:

```bash
param-racoon -w 'wordlist/params.txt' -f 'Error!' 'http://example.com/some/endpoint'
```

Making a POST request (note: params passed with `-d` will not be altered and will be sent with each request):
```
param-racoon -w 'wordlist/params.txt' -X POST -d 'name=foo&id=100' 'http://example.com/some/endpoint'
```

**Note**: If you use `-m FOO` or `-f FOO` switch, the only test that will be performed is checking if `FOO` is present/absent in the response for each param.

Ignoring variance in responses less than 5 bytes:
```
param-racoon -w 'wordlist/params.txt' --min-diff 5 'http://example.com/some/endpoint'
```

Reading a raw http request from a file:
```
param-racoon -w 'wordlist/params.txt' -r get_request.txt 'http://example.com/'
```
**Note**: The path in the request line read from a file overrides the path part of url passed via command line

# building

To build source simply run:

```bash
make
```

for a version with debug symbols use:

```bash
make build DEBUG=true
```

To check code formatting:

```bash
make check-format
```

To fix formatting:

```bash
make format
```

To run clang-tidy checks:

```
make tidy
```

To fix them:

```
make tidy-fix
```

To run test cases:
```
test/php/run.sh  # run the test webserver in a separate window
make run-tests   # build sources and run all tests
```

# requirements

You will need `clang-tidy` and `clang-format` to run checks, `php >= 7.4` to run the webserver for test cases, `libcurl` and `boost` to build.
