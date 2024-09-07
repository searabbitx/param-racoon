# 🦝param-racoon🦝
A tool similar to [param-miner](https://github.com/PortSwigger/param-miner) but worse and written in C++ for no good reason. But! You don't need Burp to use it. 

![image](https://user-images.githubusercontent.com/46087838/161920268-46b1a2da-d89f-4bb4-8f22-da4570de3dd6.png)

# usage
```
Usage:
  param-racoon [options] url

Allowed options:
  -h [ --help ]         produce help message
  -t [ --threads ] arg  set number of threads (default: 10)
  -w [ --wordlist ] arg set path to the wordlist file
  -H [ --header ] arg   set a header
  --url arg             set target url
```

Example:
```bash
param-racoon -t 5 -w 'wordlist/params.txt' -H 'X-Auth-Token: foo' -H 'X-Requested-With: XMLHttpRequest' 'http://example.com/some/endpoint'
```

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

You will need `clang-tidy` and `clang-format` to run checks, `php >= 7.4` to run the webserver for test cases, libcurl and boost to build.
