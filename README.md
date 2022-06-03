# Build project

```
rm -r build && cmake -B build . && cmake --build build
```

# Run server

```
./build/test-timeserver 127.0.0.1 8081
```

# Run tests

```
./build/tests/test-timeserver-tests
```
