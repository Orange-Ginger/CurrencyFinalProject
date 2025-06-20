### This is my final project from the OOP course

### Build instructions

1. Create (if needed) and navigate to build directory:

```bash
mkdir build
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

4. After building, run the tests from build directory:

```bash
ctest
```

### Run the Converter

**Note:**  
Make sure the CSV files (`crypto_exchange_rates.csv`, `fiat_exchange_rates.csv`, `magic_exchange_rates.csv`) are in the same directory as the executable (`build/Debug`).

1. After building, go to the executable directory:

```bash
cd .\build\Debug
```

2. Run the executable:

```bash
.\currency_converter.exe
```