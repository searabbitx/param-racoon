# param-racoon

![image](https://user-images.githubusercontent.com/46087838/161920268-46b1a2da-d89f-4bb4-8f22-da4570de3dd6.png)

A tool similar to [param-miner](https://github.com/PortSwigger/param-miner) but worse and written in C++ for no good reason. But! You don't need Burp to use it :). 

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