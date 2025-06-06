### This is my final project from the OOP course

### Build instructions

1. Create (if needed) and navigate to build directory:

```bash
mkdir -p build
cd build
```

2. Generate files with CMake:

```bash
cmake ..
```

3. Build the project:

```bash
cmake --build .
```

4. After building, run the tests:

```bash
.\test\currency_test.exe --gtest_color=yes
```