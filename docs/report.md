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
      - [4.2 Bison declaration](#42-bison-declaration)
      - [4.3 Grammar rules](#43-grammar-rules)
      - [4.4 Epilogue](#44-epilogue)
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

我们首先在语法分析器的头文件里 include 了 `<string>` 和 `ast/index.hpp`，前者是所有终结符的语义值类型 `std::string`，后者声明了所有非终结符的语义值类型（AST 节点）。随后我们 include 了 `base/common.hpp`，这里主要是用到了其中对 `std::shared_ptr` 的缩写 `SPtr`。

```cpp {.line-numbers}
// src/parser.yy

%code top {
#include <memory>   // std::make_shared

#include "driver.hpp"
#include "lexer.hpp"
#include "utils/logger.hpp"

using std::make_shared;
using location_type = yy::Parser::location_type;
using symbol_type = yy::Parser::symbol_type;

static symbol_type yylex(yy::Lexer* p_lexer) {
  return p_lexer->ReadToken();
}

// A workaround to solve the undefined reference issue, as we have defined
// another yylex() which is not default.
int yyFlexLexer::yylex() {
  Logger::Warn("calling wrong yylex()");
  return EXIT_FAILURE;
}
}
```

然后，我们在语法分析器的源代码里提供了函数 `yy::Parser::yylex()` 的定义，在 Parser 调用 Lexer 读取 token 时需要使用。这里这样写的目的是为了修改 Flex 提供的函数 `yylex()` 的默认函数签名 `int yyFlexLexer::yylex()`，因为 Bison 要求 `yylex()` 函数的返回值是一个 `symbol_type` 而不是 `int`，但普通的函数重载最多只能修改传入的参数，而不能修改返回值的类型。这里仍然提供了一个 `int yyFlexLexer::yylex()` 的定义，否则编译时会报错。

此外，为了修改这个返回值类型，我们还需要在 `src/lexer.hpp` 里定义宏 `YY_DECL`：

```cpp {.line-numbers}
// src/lexer.hpp

// Override the default yylex() to return a symbol_type instead of int.
#define YY_DECL yy::Parser::symbol_type yy::Lexer::ReadToken()
```

这样 Flex 在生成 `yylex()` 的代码时，就会以这个为实际的函数签名。

流程上，Parser 在读取 token 时，会先调用自己的 `yy::Parser::yylex()`（无法修改），这个函数我们已经提供了定义，其中传入的参数是由设置 `%lex-param` 决定的，接下来我们会提到。然后这个函数会调用 `yy::Lexer::ReadToken()`，其中 `yy::Lexer` 是我们自己定义的类，代码如下所示，主要目的是为了给 `yyFlexLexer` 提供一个额外的私有成员 `Driver& drv_`，在词法分析时需要用到。这里 `yy::Lexer::ReadToken()` 我们只提供声明（通过定义宏 `YY_DECL` 的方式），而函数定义则由 Flex 自动生成。最后 `yy::Lexer::ReadToken()` 就会以 Flex 自动生成的原本 `yyFlexLexer::yylex()` 的实现为我们读取下一个 token，而原来的 `int yyFlexLexer::yylex()` 则不会再被调用（但仍需提供定义，理论上这应该是 Flex 的 bug）。当然，为此我们还需要调整词法分析器的源代码，通过 Bison 的接口令 `yy::Lexer::ReadToken()` 在实现里返回的值不是 `int` 而是 `symbol_type`，这个我们之后再讲。

```cpp {.line-numbers}
// src/lexer.hpp

namespace yy {
class Lexer : public yyFlexLexer {
 public:
  explicit Lexer(Driver* p_drv) : drv_{*p_drv} {}

  virtual Parser::symbol_type ReadToken();

 private:
  Driver& drv_;
};
}  // namespace yy
```

是不是绕了很大一个弯子？主要原因是 Bison 和 Flex 的接口很多都写死了。为了实现交互，只能用这种比较 dirty 的办法了。尽管不一定是唯一的方式，但应该是目前最好的方式。

#### 4.2 Bison declaration

接下来，我们需要对 Bison 进行一些设置，具体含义见注释。

```cpp {.line-numbers}
// src/parser.yy

%skeleton "lalr1.cc"      // 使用 LALR1 语法分析器
%require "3.8"            // 指定 Bison 版本为 3.8+（部分特性仅 3.8+ 支持）
%header "src/parser.hpp"  // 生成 Parser 时将头文件分离，并将 Parser 的头文件保存在这个路径
%locations                // 提供 location 接口，以便 Lexer 记录当前分析到的位置

%define api.parser.class {Parser}         // 指定生成 Parser 的类名为 Parser
%define api.location.file "location.hpp"  // 将 location 的头文件保存在这个路径（相对于 parser.hpp 位置）
%define api.token.constructor             // 提供 token 构造函数接口，以便 Lexer 返回 symbol_type 的 token
%define api.token.prefix {T_}             // 为 token 名增加指定前缀
%define api.token.raw                     // 因为 Lexer 返回的已经是 symbol_type 的 token 而非读取的原字符，
                                          // 这里不再需要转换
%define api.value.type variant            // 指定 token 语义值类型为 variant，用于同时支持更广泛的类型，
                                          // 参见 C++17 的 std::variant

%define parse.assert         // 通过运行时检查确保 token 被正常构造与析构，以保证上述 variant 类型被正确使用
%define parse.trace          // 启用 Debug 功能
%define parse.error verbose  // 指定报错等级，verbose 级别将对语法分析时遇到的错误进行详细报错
%define parse.lac full       // 启用 lookahead correction (LAC)，提高对语法错误的处理能力

%lex-param {yy::Lexer* p_lexer}                       // 函数 yy::Parser::yylex() 传入的参数
%parse-param {yy::Lexer* p_lexer} {Driver* p_driver}  // 函数 yy::Parser::Parser() 传入的参数
```

对这些设置的详尽解释建议直接参考官方文档。

然后，我们需要声明语法分析时遇到的所有终结符和非终结符的语义值类型。其中 `%token` 表示终结符，`%nterm` 表示非终结符。

```cpp {.line-numbers}
// src/parser.yy

%token
  // Reserved keywords
  <std::string>       AND
  <std::string>       ARRAY
  <std::string>       BEGIN
  <std::string>       BY
  <std::string>       DIV
  <std::string>       DO
  <std::string>       ELSE
  <std::string>       ELSIF
  <std::string>       END
  <std::string>       EXIT
  <std::string>       FOR
  <std::string>       IF
  <std::string>       IN
  <std::string>       IS
  <std::string>       LOOP
  <std::string>       MOD
  <std::string>       NOT
  <std::string>       OF
  <std::string>       OR
  <std::string>       OUT
  <std::string>       PROCEDURE
  <std::string>       PROGRAM
  <std::string>       READ
  <std::string>       RECORD
  <std::string>       RETURN
  <std::string>       THEN
  <std::string>       TO
  <std::string>       TYPE
  <std::string>       VAR
  <std::string>       WHILE
  <std::string>       WRITE

  // Operators
  <std::string>       ASSIGN              ":="
  <std::string>       PLUS                "+"
  <std::string>       MINUS               "-"
  <std::string>       STAR                "*"
  <std::string>       SLASH               "/"
  <std::string>       LT                  "<"
  <std::string>       LE                  "<="
  <std::string>       GT                  ">"
  <std::string>       GE                  ">="
  <std::string>       EQ                  "="
  <std::string>       NE                  "<>"

  // Delimiters
  <std::string>       COLON               ":"
  <std::string>       SEMICOLON           ";"
  <std::string>       COMMA               ","
  <std::string>       DOT                 "."
  <std::string>       LPAREN              "("
  <std::string>       RPAREN              ")"
  <std::string>       LSBRAC              "["
  <std::string>       RSBRAC              "]"
  <std::string>       LCBRAC              "{"
  <std::string>       RCBRAC              "}"
  <std::string>       LSABRAC             "[<"
  <std::string>       RSABRAC             ">]"
  <std::string>       BACKSLASH           "\\"

  // Constants
  <std::string>       INTEGER             "integer"
  <std::string>       REAL                "real"
  <std::string>       STRING              "string"

  // Identifiers
  <std::string>       ID                  "identifier"
;
```

这里 `INTEGER` 和 `REAL` 的类型分别设置为 `int32_t` 和 `double` 也是可以的，真正做语法分析时应该这样设置。这里设置成 `std::string` 单纯是为了输出语法树时能保留原来的格式，例如值为 `2.0` 的 `REAL` 应输出为 `2.0` 而不是 `2`。

```cpp {.line-numbers}
// src/parser.yy

%nterm
  // Programs
  <SPtr<Program>>             program
  <SPtr<Body>>                body

  // Declarations
  <SPtr<Decls>>               decls
  <SPtr<Decls>>               decl           // 不是 typo，就是 Decls 类型，因为一个 decl 可以包含多个
                                             // var_decls / type_decls / proc_decls
  <SPtr<VarDecls>>            var_decls
  <SPtr<VarDecl>>             var_decl
  <SPtr<TypeDecls>>           type_decls
  <SPtr<TypeDecl>>            type_decl
  <SPtr<ProcDecls>>           proc_decls
  <SPtr<ProcDecl>>            proc_decl
  <SPtr<FormalParams>>        formal_params
  <SPtr<FormalParam>>         formal_param
  <SPtr<TypeAnnot>>           type_annot
  <SPtr<Type>>                type
  <SPtr<Components>>          components
  <SPtr<Component>>           component
  <SPtr<Ids>>                 ids
  <SPtr<Id>>                  id

  // Statements
  <SPtr<Stmts>>               stmts
  <SPtr<Stmt>>                stmt
  <SPtr<ActualParams>>        actual_params
  <SPtr<ReadParams>>          read_params
  <SPtr<WriteParams>>         write_params
  <SPtr<ElifSections>>        elif_sections
  <SPtr<ElifSection>>         elif_section
  <SPtr<ElseSection>>         else_section
  <SPtr<ForStep>>             for_step

  // Expressions
  <SPtr<Exprs>>               exprs
  <SPtr<Expr>>                expr
  <SPtr<WriteExprs>>          write_exprs
  <SPtr<WriteExpr>>           write_expr
  <SPtr<AssignExprs>>         assign_exprs
  <SPtr<AssignExpr>>          assign_expr
  <SPtr<ArrayExprs>>          array_exprs
  <SPtr<ArrayExpr>>           array_expr
  <SPtr<Number>>              number
  <SPtr<Integer>>             integer
  <SPtr<Real>>                real
  <SPtr<String>>              string
  <SPtr<Lvalues>>             lvalues
  <SPtr<Lvalue>>              lvalue
  <SPtr<CompValues>>          comp_values
  <SPtr<ArrayValues>>         array_values
;
```

可以看到对所有的非终结符，我们都定义了一个 AST 节点，每个非终结符的类型就是这个节点的 `std::shared_ptr`。为什么使用 `std::shared_ptr` 而不是普通指针？因为这样就不需要手动管理内存了，否则析构时需要释放以当前节点为根的子树的所有节点，关键是报错（抛异常）时要记得释放，这点比较复杂。为什么不使用 `std::unique_ptr`？其实语义上用 `std::unique_ptr` 是对的，因为所有节点都是唯一的。事实上我一开始也是使用的 `std::unique_ptr`，但我可能能力有限，花了两整天时间都没能解决所有权转移时的各种问题，最后还是放弃了。`std::shared_ptr` 由于可以进行拷贝构造，相对要省心太多，而且增加的额外性能开销其实也可以接受。我们毕竟只是一个 toy project，还不至于需要扣 `std::unique_ptr` 和 `std::shared_ptr` 的这点性能差距。

需要注意的是，我们对终结符 `INTEGER`, `REAL`, `STRING` 还额外封装了三个对应的非终结符，这个主要是为了书写规则时的形式统一，不定义这几个非终结符也是可以的。

此外，我们还需要指定操作符的优先级和结合性。从上到下表示优先级依次提高；`%left`, `%right`, `%nonassoc` 分别表示左结合、右结合和非结合。这里我们额外定义了两个假的操作符 `POS` 和 `NEG`，分别对应作为单目运算符时的 `PLUS` 和 `MINUS`。这个假操作符占位操作将在之后通过 `%prec` 显式指定优先级时利用到。

```cpp {.line-numbers}
// src/parser.yy

%left                 OR;
%left                 AND;
%nonassoc             EQ NE;
%nonassoc             LT LE GT GE;
%left                 PLUS MINUS;
%left                 STAR SLASH DIV MOD;
%right                POS NEG NOT;
```

#### 4.3 Grammar rules

那么接下来就是定义语法分析的具体规则了，也就是定义所有非终结符的产生式。由于量比较大，但模式很多又完全一致，因此我会针对几个比较有代表性的规则进行详解。

首先讲一下 Bison 里产生规则的格式：

```cpp {.line-numbers}
result:
  rule1-components { /* action1 */ }
| rule2-components { /* action2 */ }
;
```

其中 `result` 即为产生的非终结符，`rule1-components` 和 `rule2-components` 表示两种可以产生 `result` 的终结符 / 非终结符组合方式，`action1` 和 `action2` 表示每次匹配到相应产生式时需要执行的动作（C++ 代码）。

下面我们来看一些实例。

```cpp {.line-numbers}
// src/parser.yy

%start program;

// Programs

program:
  PROGRAM IS body SEMICOLON {
    $$ = make_shared<Program>(@$, $body);
    p_driver->set_program($$);
  }
;
```

首先是我们程序的根结点 `program`。我们知道 `program` 的产生式如下 [^2]：

```cpp {.line-numbers}
program -> PROGRAM IS body ';'
```

由于我们已经定义了 `;` 的 token 为 `SEMICOLON`，在产生规则里我们可以用 `SEMICOLON` 代替。

在规则对应的动作里，我们利用 `std::make_shared` 直接构造一个指向 `Program` 类的 `std::shared_ptr`，构造函数传入的参数为 `@$` 和 `$body`，分别表示 `$` 当前的 location 和 `body` 的语义值，其中 `$` 即表示当前非终结符（也就是 `program`）。最后我们将这个指针赋值给 `$$`，也就是 `$` 的语义值。

由于 `$` 是我们的根结点 `program`，因此我们将其保存到 `Driver` 类里。将来我们通过 `Driver` 打印语法树时，这就是我们语法树的根。

之后我会在语法树实现的章节详细讲解这里发生了什么。现在我们只需要知道，每个非终结符 `x` 的语义值就是一个指向这个 AST 节点 `x` 的指针 `$x`。当构造一个节点 `x` 时，需要传入这个节点的位置 `@x` 和其子节点的语义值 `$y`，其中子节点即构成这个非终结符的所有非终结符和有值的终结符（`INTEGER`, `REAL`, `STRING`）。

举个复杂点的例子，比如 `stmt` 的产生规则就是：

```cpp {.line-numbers}
// src/parser.yy

stmt:
  lvalue ASSIGN expr SEMICOLON {
    $$ = make_shared<AssignStmt>(@$, $lvalue, $expr);
  }
| id LPAREN actual_params RPAREN SEMICOLON {
    $$ = make_shared<ProcCallStmt>(@$, $id, $actual_params);
  }
| READ LPAREN read_params RPAREN SEMICOLON {
    $$ = make_shared<ReadStmt>(@$, $read_params);
  }
| WRITE LPAREN write_params RPAREN SEMICOLON {
    $$ = make_shared<WriteStmt>(@$, $write_params);
  }
| IF expr THEN stmts elif_sections[elif] else_section[else] END SEMICOLON {
    $$ = make_shared<IfStmt>(@$, $expr, $stmts, $elif, $else);
  }
| WHILE expr DO stmts END SEMICOLON {
    $$ = make_shared<WhileStmt>(@$, $expr, $stmts);
  }
| LOOP stmts END SEMICOLON {
    $$ = make_shared<LoopStmt>(@$, $stmts);
  }
| FOR id ASSIGN expr[begin] TO expr[end] for_step[step] DO stmts END SEMICOLON {
    $$ = make_shared<ForStmt>(@$, $id, $begin, $end, $step, $stmts);
  }
| EXIT SEMICOLON {
    $$ = make_shared<ExitStmt>(@$);
  }
| RETURN SEMICOLON {
    $$ = make_shared<ReturnStmt>(@$);
  }
| RETURN expr SEMICOLON {
    $$ = make_shared<ReturnStmt>(@$, $expr);
  }
;
```

其中 `nterm[alias]` 表示为 `nterm` 取别名 `alias`。

注意到 `$$` 在不同的产生式里可以有不同的类型，这主要是利用了 C++ 的多态性。我们在实现 AST 时，利用继承实现了多态，例如此处 `AssignStmt`, `ProcCallStmt` 等等都是以 `Stmt` 为基类的派生类，因此其指针可以直接赋值给基类指针，同时不失语义（利用虚函数，基类指针可以调用派生类的函数实现）。

再看一个例子，这个是 `expr` 的产生规则：

```cpp {.line-numbers}
// src/parser.yy

expr:
  number { $$ = make_shared<NumberExpr>(@$, $1); }
| lvalue { $$ = make_shared<LvalueExpr>(@$, $1); }
| LPAREN expr RPAREN { $$ = make_shared<ParenExpr>(@$, $2); }
| PLUS expr %prec POS { $$ = make_shared<UnaryExpr>(@$, make_shared<Op>(@1, $1), $2); }
| MINUS expr %prec NEG { $$ = make_shared<UnaryExpr>(@$, make_shared<Op>(@1, $1), $2); }
| NOT expr { $$ = make_shared<UnaryExpr>(@$, make_shared<Op>(@1, $1), $2); }
| expr PLUS expr { $$ = make_shared<BinaryExpr>(@$, $1, make_shared<Op>(@2, $2), $3); }
| expr MINUS expr { $$ = make_shared<BinaryExpr>(@$, $1, make_shared<Op>(@2, $2), $3); }
| expr STAR expr { $$ = make_shared<BinaryExpr>(@$, $1, make_shared<Op>(@2, $2), $3); }
| expr SLASH expr { $$ = make_shared<BinaryExpr>(@$, $1, make_shared<Op>(@2, $2), $3); }
| expr DIV expr { $$ = make_shared<BinaryExpr>(@$, $1, make_shared<Op>(@2, $2), $3); }
| expr MOD expr { $$ = make_shared<BinaryExpr>(@$, $1, make_shared<Op>(@2, $2), $3); }
| expr OR expr { $$ = make_shared<BinaryExpr>(@$, $1, make_shared<Op>(@2, $2), $3); }
| expr AND expr { $$ = make_shared<BinaryExpr>(@$, $1, make_shared<Op>(@2, $2), $3); }
| expr LT expr { $$ = make_shared<BinaryExpr>(@$, $1, make_shared<Op>(@2, $2), $3); }
| expr LE expr { $$ = make_shared<BinaryExpr>(@$, $1, make_shared<Op>(@2, $2), $3); }
| expr GT expr { $$ = make_shared<BinaryExpr>(@$, $1, make_shared<Op>(@2, $2), $3); }
| expr GE expr { $$ = make_shared<BinaryExpr>(@$, $1, make_shared<Op>(@2, $2), $3); }
| expr EQ expr { $$ = make_shared<BinaryExpr>(@$, $1, make_shared<Op>(@2, $2), $3); }
| expr NE expr { $$ = make_shared<BinaryExpr>(@$, $1, make_shared<Op>(@2, $2), $3); }
| id LPAREN actual_params RPAREN { $$ = make_shared<ProcCallExpr>(@$, $1, $3); }
| id LCBRAC comp_values RCBRAC { $$ = make_shared<RecordConstrExpr>(@$, $1, $3); }
| id LSABRAC array_values RSABRAC { $$ = make_shared<ArrayConstrExpr>(@$, $1, $3); }
;
```

首先可以注意到，我们也可以用 `$1`, `$2` 来表示产生式右侧的第 1, 2 个符号。

这里为什么不能将 `UnaryExpr` 或 `BinaryExpr` 的产生式合并成一个 `op expr` 和 `expr op expr` 呢？是因为我们需要利用操作符 `op` 的优先级和结合性。如果我们将 `op` 提取出来，在产生式里仅留下一个非操作符 `op`，Bison 将无法利用终结符的优先级和结合性信息，从而导致错误的语法分析结果。

式中，`PLUS expr %prec POS` 即表示对此式采用 `POS` 的优先级。前面我们看到，`POS` 的优先级和 `NOT` 同级，通过这种方式我们就实现了对 `PLUS` 作为单目运算符时优先级的重定义。

除了这种单个节点的构造方式外，我们还存在另一种数组节点的构造方式。

```cpp {.line-numbers}
// src/parser.yy

elif_sections:
  %empty { $$ = make_shared<ElifSections>(@$); }
| elif_sections elif_section { $$ = $1; if ($$) $$->Insert($2); }
;
```

`elif_sections` 的产生式如下：

```cpp {.line-numbers}
elif_sections -> {elif_section}
```

表示 `elif_sections` 由多个 `elif_section` 构成。

在 Bison 的产生规则中，我们通过一种递归的方式来描述这个产生式。作为递归的退出条件，空产生式可以用 `%empty` 表示，其对应的动作是新建一个 `ElifSections` 对象。在 AST 节点的实现中，其本质是一个 `std::vector<std::shared_ptr<ElifSection>>`，也就是一个子节点指针的数组。之后每多分析到一个 `elif_section`，就往这个数组里插一个子节点指针，并更新此节点的 location。这里 `Insert()` 函数会同时完成这两件事情。

此外，我们还提供了 `InsertArray()` 函数，用于转移另一个数组节点 `y` 的所有子节点到数组节点 `x`。适用于语义上不应当将 `y` 作为 `x` 的子节点的情形。例如：

```cpp {.line-numbers}
// src/parser.yy

decls:
  %empty { $$ = make_shared<Decls>(@$); }
| decls decl { $$ = $1; if ($$) $$->InsertArray($2); }
;

decl:
  VAR var_decls { $$ = $2; if ($$) $$->set_loc(@$); }
| PROCEDURE proc_decls { $$ = $2; if ($$) $$->set_loc(@$); }
| TYPE type_decls { $$ = $2; if ($$) $$->set_loc(@$); }
;
```

这里 `$$->set_loc(@$)` 的作用即更新一下当前节点的 location。需要注意的是，这里 `decls` 和 `decl` 的语义值类型均为 `std::shared_ptr<Decls>`。

其他规则基本上就大同小异了，不在这里赘述。具体代码可以参见 [src/parser.yy](../src/parser.yy)。

#### 4.4 Epilogue

最后我们在 Epilogue 给出了函数 `yy::Parser::error()` 的实现，用于语法分析器的报错。

```cpp {.line-numbers}
// src/parser.yy

void yy::Parser::error(const location_type& loc, const std::string& msg) {
  Logger::Error(msg, &loc);
}
```

流程上，当 Lexer 或 Parser 遇到词法错误或语法错误时，就会抛出一个 `std::syntax_error` 异常。程序捕获这个异常后，就会调用函数 `yy::Parser::error()`，然后调用函数 `Logger::Error()`，并传入参数 `loc` 和 `msg`，分别对应错误发生的位置和错误信息。

函数 `Logger::Error()` 的实现很简单，其实就是对 `operator<<()` 的一个封装。

```cpp {.line-numbers}
// src/utils/logger.cpp

void Logger::Log(
    const std::string& msg, const yy::location* p_loc, std::ostream& os) {
  if (p_loc) {
    auto loc = *p_loc;
    os << loc.begin.line << ":" << loc.begin.column << "-" << loc.end.line
       << ":" << loc.end.column << ": ";
  }
  os << msg << RESET << "\n";
}

void Logger::Error(
    const std::string& msg, const yy::location* p_loc, std::ostream& os) {
  if (LOG_LEVEL <= ERROR) {
    os << RED << "[ERROR] ";
    Log(msg, p_loc, os);
  }
}
```

这里利用 ANSI 字符颜色转义序列美化了下日志输出，给报错信息加了个颜色。

```cpp {.line-numbers}
// src/base/common.cpp

// ANSI colors
#define RED "\e[0;31m"
#define RESET "\e[0;0m"
```

## 贡献者

- [**Hakula Chen**](https://github.com/hakula139)<[i@hakula.xyz](mailto:i@hakula.xyz)> - 复旦大学

## 许可协议

本项目遵循 MIT 许可协议，详情参见 [LICENSE](../LICENSE) 文件。

[^1]: [C++ Parsers (Bison 3.8.1)](https://www.gnu.org/software/bison/manual/html_node)  
[^2]: [The PCAT Programming Language Reference Manual](http://web.cecs.pdx.edu/~harry/compilers/PCATLangSpec.pdf)  
