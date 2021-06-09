# ImpalaJIT
A lightweight JIT compiler for flexible data access in simulation applications

### Dependencies
* LLVM - build
* Z3 - run-time

### Building
1. mkdir build
2. cd build
3. cmake .. -DCMAKE_INSTALL_PREFIX=<path>
4. make -j

### Installing
* make install

### Generating Parser, Scanner and ASM
This step is only necessary if you have modified one of the following files: <br />
[parser.yy](compiler/parser.yy) <br />
[scanner.ll](compiler/scanner.ll) <br />
[assembly.dasc](code-gen/assembly/assembly.dasc) <br /><br />
Prerequisites for the <b>generate</b> task are: <br />
Bison (Tested with version 3.0.4): https://www.gnu.org/software/bison/ <br />
Flex (Tested with version 2.6.0): https://github.com/westes/flex <br />

* make generate

### License
ImpalaJIT is release under the MIT License (see [COPYING](COPYING))
