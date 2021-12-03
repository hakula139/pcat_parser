# PCAT 语法分析

[![wakatime](https://wakatime.com/badge/user/f4a35a1f-0e29-4093-a647-e66aad164737/project/fb1abada-88bc-4df7-a81d-c7bb1af6fdfc.svg)](https://wakatime.com/badge/user/f4a35a1f-0e29-4093-a647-e66aad164737/project/fb1abada-88bc-4df7-a81d-c7bb1af6fdfc)

本项目利用 Bison 和 Flex，实现了对给定 PCAT 语言样例的语法分析，使用 C++17 编写。

## 目录

- [PCAT 语法分析](#pcat-语法分析)
  - [目录](#目录)
  - [项目报告](#项目报告)
    - [1. 项目结构总览](#1-项目结构总览)
    - [2. 运行本项目](#2-运行本项目)
      - [2.1 依赖](#21-依赖)
      - [2.2 构建与运行](#22-构建与运行)
      - [2.3 测试](#23-测试)
    - [3. Makefile 文件说明](#3-makefile-文件说明)
    - [4. Bison 使用方法](#4-bison-使用方法)
      - [4.1 Prologue](#41-prologue)
  - [贡献者](#贡献者)
  - [许可协议](#许可协议)
  
## 项目报告

### 1. 项目结构总览

- `bin/`：二进制文件的存放位置（构建时自动生成）
  - `parser`：本 PCAT 语法分析器的二进制文件
- `build/`：构建过程中产生的临时文件的存放位置（构建时自动生成）
- `docs/`：一些文档
  - `report.md`：本项目报告
- `lib/`：项目依赖
  - `bison-3.8.2.tar.gz`：有些操作系统自带的 Bison 版本较老，无法运行本项目，因此这里提供了 Bison 3.8.2 的源码供编译使用（项目提供了自动化安装脚本）
- `output/`：对测试样例的语法分析结果（运行后自动生成）
- `scripts/`：一些自动化脚本
  - `prebuild.sh`：项目依赖的自动化安装脚本，如果你的操作系统没有自带 Bison 3.8 及以上版本的话，可以执行本脚本编译安装
  - `test.sh`：自动化测试脚本，对 `tests/` 文件夹下的 PCAT 代码文件进行语法分析，并将分析结果保存在 `output/` 文件夹下
- `src/`：项目源代码
  - `ast/`：抽象语法树（Abstract Syntax Tree, AST）的节点定义
    - `body.hpp`：body
    - `body.cpp`
    - `constant.hpp`：integer, real, number, string
    - `constant.cpp`
    - `decl.hpp`：declaration
    - `decl.cpp`
    - `expr.hpp`：expression
    - `expr.cpp`
    - `identifier.hpp`：identifier
    - `index.hpp`：`#include` 了所有 AST 的头文件，便于其他文件导入
    - `lvalue.hpp`：lvalue
    - `lvalue.cpp`
    - `node.hpp`：node，所有节点的基类
    - `node.cpp`
    - `operator.hpp`：operator
    - `param.hpp`：parameters
    - `param.cpp`
    - `program.hpp`：program
    - `program.cpp`
    - `stmt.hpp`：statement
    - `stmt.cpp`
    - `type.hpp`：type
    - `type.cpp`
  - `base/`：一些通用头文件
    - `common.hpp`：一些可复用代码
    - `config.hpp`：一些项目设置
  - `utils/`：一些工具类函数
    - `logger.hpp`：日志记录器，也用于语法分析时的报错
    - `logger.cpp`
  - `driver.hpp`：语法分析器
  - `driver.cpp`
  - `lexer.hpp`：词法分析器的头文件
  - `lexer.lex`：词法分析器的 Lex 文件，用于 Flex 生成词法分析器的源文件
  - `lexer.cpp`：词法分析器的源文件（构建时自动生成）
  - `location.hpp`：语法分析器 location 的头文件，用于标识当前分析到的位置（构建时自动生成）
  - `main.cpp`：主程序入口
  - `parser.yy`：语法分析器的 Yacc 文件，用于 Bison 生成语法分析器的头文件、源文件和 `location.hpp`
  - `parser.hpp`：语法分析器的头文件（构建时自动生成）
  - `parser.cpp`：语法分析器的源文件（构建时自动生成）
- `tests/`：用于测试的 PCAT 代码文件的存放位置
- `.clang-format`：用于 Clang-Format 进行代码格式化的配置文件
- `Makefile`：用于 Make 构建本项目
- `README.md`

### 2. 运行本项目

#### 2.1 依赖

在构建本项目前，你需要安装以下依赖。

- [GNU make](https://www.gnu.org/software/make) 4.0 或以上
- [GCC](https://gcc.gnu.org/releases.html) 9.0 或以上（以支持 C++17 特性）
- [Flex](https://github.com/westes/flex) 2.6.4（其他版本未测试）
- [Bison](https://www.gnu.org/software/bison) 3.8
  - 更低版本确定不可用；暂时没有更高版本，但根据文档，Bison 的 C++ API 接口随时会改，因此未来如果 API 有 breaking change 则同样不兼容。
  - 本项目在 `lib` 目录下提供了 Bison 3.8.2 的源码供编译使用，可以使用我们提供的自动化安装脚本 `scripts/prebuild.sh` 进行安装。执行前你可能需要先通过 `chmod +x scripts/prebuild.sh` 赋予执行权限。

#### 2.2 构建与运行

本项目使用 Makefile 组织，因此在根目录执行 `make` 即可构建本项目。对于 Windows，可以使用 [MinGW-w64](http://mingw-w64.org/doku.php/download)，然后执行 `mingw32-make`，也可以使用 WSL（推荐）。

运行项目时，执行 `make INPUT=<path>` 以读取文件，其中 `<path>` 即为指定的文件路径。例如：

```bash
make INPUT="tests/case_1.pcat"
```

输出文件将默认保存在 `output` 目录下。

#### 2.3 测试

本项目自带了 14 个 PCAT 语言测试样例，位于 `tests` 目录下。测试时依次通过上述命令处理即可，或者也可以使用我们提供的自动化测试脚本 `scripts/test.sh`。

例如对于这样的输入（`tests/case_1.pcat`）：

```text {.line-numbers}
PROGRAM IS
    VAR i, j : INTEGER := 1; 
    VAR x : REAL := 2.0;
    VAR y : REAL := 3.0;
BEGIN 
    WRITE ("i = ", i, ", j = ", j);
    WRITE ("x = ", x, ", y = ", y);
END;
```

就将得到这样的输出：

```text {.line-numbers}
tests/case_1.pcat

program <1:1-8:5>
  body <1:11-8:4>
    declaration list <1:11-4:25>
      variable declaration <2:9-2:29>
        identifier list <2:9-2:13>
          identifier <2:9-2:10> i
          identifier <2:12-2:13> j
        type annotation <2:14-2:23>
          identifier type <2:16-2:23>
            identifier <2:16-2:23> INTEGER
        number expression <2:27-2:28> 1
          integer <2:27-2:28> 1
      variable declaration <3:9-3:25>
        identifier list <3:9-3:10>
          identifier <3:9-3:10> x
        type annotation <3:11-3:17>
          identifier type <3:13-3:17>
            identifier <3:13-3:17> REAL
        number expression <3:21-3:24> 2.0
          real <3:21-3:24> 2.0
      variable declaration <4:9-4:25>
        identifier list <4:9-4:10>
          identifier <4:9-4:10> y
        type annotation <4:11-4:17>
          identifier type <4:13-4:17>
            identifier <4:13-4:17> REAL
        number expression <4:21-4:24> 3.0
          real <4:21-4:24> 3.0
    statement list <5:6-7:36>
      write statement <6:5-6:36>
        write parameter list <6:12-6:34>
          write expression <6:12-6:18> "i = "
            string <6:12-6:18> "i = "
          write expression <6:20-6:21> i
            lvalue expression <6:20-6:21> i
              lvalue <6:20-6:21> i
          write expression <6:23-6:31> ", j = "
            string <6:23-6:31> ", j = "
          write expression <6:33-6:34> j
            lvalue expression <6:33-6:34> j
              lvalue <6:33-6:34> j
      write statement <7:5-7:36>
        write parameter list <7:12-7:34>
          write expression <7:12-7:18> "x = "
            string <7:12-7:18> "x = "
          write expression <7:20-7:21> x
            lvalue expression <7:20-7:21> x
              lvalue <7:20-7:21> x
          write expression <7:23-7:31> ", y = "
            string <7:23-7:31> ", y = "
          write expression <7:33-7:34> y
            lvalue expression <7:33-7:34> y
              lvalue <7:33-7:34> y
```

### 3. Makefile 文件说明

项目的构建顺序如下：

首先，通过此 recipe 利用 Flex 生成词法分析器的源文件 `lexer.cpp`。

```makefile {.line-numbers}
SRC_DIR   := src

LEX_IN    := $(SRC_DIR)/lexer.lex
LEX_SRC   := $(SRC_DIR)/lexer.cpp
LEX       := flex

$(LEX_SRC): $(LEX_IN)
	@echo + $@
	@$(LEX) -o $@ $<
```

随后，通过此 recipe 利用 Bison 生成语法分析器的头文件 `parser.hpp`、源文件 `parser.cpp` 和 `location.hpp`。

```makefile {.line-numbers}
YACC_IN   := $(SRC_DIR)/parser.yy
YACC_SRC  := $(SRC_DIR)/parser.cpp
YACC_H    := $(SRC_DIR)/parser.hpp $(SRC_DIR)/location.hpp
YACC      := bison

$(YACC_SRC): $(YACC_IN)
	@echo + $@
	@$(YACC) -o $@ -d $<

$(YACC_H): $(YACC_SRC)
```

然后，通过此 recipe 利用 g++ 生成各源代码文件的目标文件（`.o`）。这里我们通过 `-MMD` 参数，在编译时生成相应的依赖文件（`.d`），用于 Make 理解文件之间的依赖关系，避免手写繁琐的依赖规则。

```makefile {.line-numbers}
BUILD_DIR := build

CXX       := g++
CXXFLAGS  := -g -Wall -O3 -std=c++17 -MMD
MKDIR     := mkdir -p

$(BUILD_DIR)/%.cpp.o: %.cpp
	@echo + $@
	@$(MKDIR) $(dir $@)
	@$(CXX) $(CXXFLAGS) -c -o $@ $<
```

最后，通过此 recipe 利用 g++ 生成最终的二进制文件 `bin/parser`。这里我们利用 `$(shell find $(SRC_DIR) -name *.cpp)` 语句自动递归查找 `src` 文件夹里的所有 `.cpp` 文件，避免在 Makefile 里硬编码所有的文件名。

```makefile {.line-numbers}
TARGET    := parser
BIN_DIR   := bin

SRCS      := $(YACC_SRC) $(LEX_SRC) $(shell find $(SRC_DIR) -name *.cpp)
OBJS      := $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS      := $(OBJS:.o=.d)

$(BIN_DIR)/$(TARGET): $(OBJS)
	@echo + $@
	@$(MKDIR) $(dir $@)
	@$(CXX) $(CXXFLAGS) -o $@ $^

# ...

-include $(DEPS)
```

运行项目时，通过默认 recipe（顺序位于第一个）执行二进制文件 `bin/parser`。这里变量 `INPUT` 需要由用户输入，作为读取的文件路径。

```makefile {.line-numbers}
OUT_DIR   := output

start: $(BIN_DIR)/$(TARGET)
	@$(MKDIR) $(OUT_DIR)
	@$< $(INPUT)
```

清理项目时，执行 `make clean` 即可通过此 recipe 清除所有构建过程中生成的文件。

```makefile {.line-numbers}
RM        := rm -rf

clean:
	@$(RM) $(BIN_DIR) $(BUILD_DIR) $(LEX_SRC) $(YACC_SRC) $(YACC_H)
```

### 4. Bison 使用方法

以下主要参考 Bison 的官方文档。[^1]

Bison 文件的结构和 Flex 文件类似，总体分为四个部分：

- Prologue：定义了一些 Bison 头文件和源文件中需要使用的函数和变量，一般通过 `#include` 外部宏的方式实现
- Bison declarations：声明了所有终结符和非终结符以及它们语义值的类型，同时指定了一些操作符的优先级；此外这里也包含了 Bison 的各种设置
- Grammar rules：定义了语法分析的具体规则，即每个非终结符有哪些产生式，匹配到相应产生式时需要执行什么动作等等
- Epilogue：用户可以在这里定义一些辅助函数，此部分代码会被直接复制到 `src/parser.cpp` 里

```cpp {.line-numbers}
%{
// Prologue
%}

// Bison declarations

%%
// Grammar rules
%%

// Epilogue
```

其中，Prologue 部分也可以有其他一些替代形式。本项目中利用到的为如下两种：

```cpp {.line-numbers}
%code requires {
// Prologue
}
```

此部分代码将被直接复制到 `src/parser.hpp` 的头部，往往包含在语法分析器的头文件里就需要用到的声明，例如所有终结符和非终结符的类型声明。

```cpp {.line-numbers}
%code top {
// Prologue
}
```

此部分代码将被直接复制到 `src/parser.cpp` 的头部，包含语法分析器的实现里需要用到的声明。

下面我们展开讲讲本项目中这四个部分的具体内容。

#### 4.1 Prologue

```cpp {.line-numbers}
// src/parser.yy

%code requires {
#include <string>

#include "ast/index.hpp"
#include "base/common.hpp"

class Driver;
namespace yy {
  class Lexer;
}
}
```

我们首先在语法分析器的头文件里 include 了 `<string>` 和 `ast/index.hpp`，前者是所有终结符的语义值类型 `std::string`，后者声明了所有非终结符的语义值类型（AST 节点）。随后我们 include 了 `base/common.hpp`，这里主要是用到了其中对 `std::shared_ptr` 的缩写 `SPtr`，之后我会解释为什么我使用 `std::shared_ptr` 而不是 `std::unique_ptr` 或者普通指针，这里有很多大坑。

## 贡献者

- [**Hakula Chen**](https://github.com/hakula139)<[i@hakula.xyz](mailto:i@hakula.xyz)> - 复旦大学

## 许可协议

本项目遵循 MIT 许可协议，详情参见 [LICENSE](../LICENSE) 文件。

[^1]: [C++ Parsers (Bison 3.8.1)](https://www.gnu.org/software/bison/manual/html_node)  
[^2]: [The PCAT Programming Language Reference Manual](http://web.cecs.pdx.edu/~harry/compilers/PCATLangSpec.pdf)  
