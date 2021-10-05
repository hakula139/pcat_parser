# PCAT 词法分析

[![wakatime](https://wakatime.com/badge/user/f4a35a1f-0e29-4093-a647-e66aad164737/project/b645c02d-da46-4ea1-a962-5891666415f5.svg)](https://wakatime.com/badge/user/f4a35a1f-0e29-4093-a647-e66aad164737/project/b645c02d-da46-4ea1-a962-5891666415f5)

本项目利用 Flex (fast lexical analyzer generator)，实现了对给定 PCAT 语言样例的词法分析。

## 目录

- [PCAT 词法分析](#pcat-词法分析)
  - [目录](#目录)
  - [项目报告](#项目报告)
    - [1. Flex 使用方法](#1-flex-使用方法)
      - [1.1 定义区](#11-定义区)
        - [1.1.1 正则表达式原理](#111-正则表达式原理)
      - [1.2 规则区](#12-规则区)
      - [1.3 用户代码区](#13-用户代码区)
        - [1.3.1 判断 token 所在行列号](#131-判断-token-所在行列号)
        - [1.3.2 判断 token 的类型](#132-判断-token-的类型)
        - [1.3.3 实现报错功能](#133-实现报错功能)
          - [1.3.3.1 整数错误](#1331-整数错误)
          - [1.3.3.2 字符串错误](#1332-字符串错误)
          - [1.3.3.3 标识符错误](#1333-标识符错误)
          - [1.3.3.4 注释错误](#1334-注释错误)
    - [2. 运行本项目](#2-运行本项目)
      - [2.1 依赖](#21-依赖)
      - [2.2 构建与运行](#22-构建与运行)
      - [2.3 测试](#23-测试)
  - [贡献者](#贡献者)
  - [许可协议](#许可协议)
  
## 项目报告

### 1. Flex 使用方法

一个 Flex 词法分析程序分为三个部分 [^1]：

- 定义区 (definitions)：包含所有 token 和初始状态 (start condition) 的声明，以及一些 Flex 的设置
- 规则区 (rules)：定义了 Flex 在扫描时遇到相应 pattern 时需要采取的操作
- 用户代码区 (user code)：（可选）用户可以在这里定义一些 Flex 扫描时需要用到的辅助函数，此部分代码会被直接复制到 Flex 自动生成的 C/C++ 代码里（本项目中为 `src/lexer.cpp`）

三个部分之间使用一个单行的 `%%` 分隔。

```cpp {.line-numbers}
// Definitions
%%
// Rules
%%
// User code
```

下面我们展开讲讲本项目中这三个部分是如何实现的。

#### 1.1 定义区

```cpp {.line-numbers}
// src/lexer.lex

%{
#include "lexer.hpp"
%}
```

我们首先 include 了一个 `lexer.hpp`，其中以枚举的形式定义了所有我们需要用到的 token 类型。这样我们之后就可以在不失可读性的前提下，通过 `switch` 语句对每个 token 类型进行相应的处理。这里被 `%{` 和 `%}` 包围的内容也会被直接复制到 `src/lexer.cpp` 里。

```cpp {.line-numbers}
// src/lexer.hpp

#ifndef SRC_LEXER_HPP_
#define SRC_LEXER_HPP_

enum Tokens {
  T_EOF = 0,
  T_WS,
  T_NEWLINE,
  T_INTEGER,
  T_REAL,
  T_STRING,
  T_RESERVED,
  T_IDENTIFIER,
  T_OPERATOR,
  T_DELIMITER,
  T_COMMENTS_BEGIN,
  T_COMMENTS,
  T_COMMENTS_END,
};

enum Errors {
  E_UNTERM_STRING = 10000,
  E_UNTERM_COMMENTS,
  E_UNKNOWN_CHAR,
};

#endif  // SRC_LEXER_HPP_
```

随后，我们对 Flex 进行了一些设置。

```cpp {.line-numbers}
// src/lexer.lex

%option c++
%option noyywrap
%option yylineno
```

这几个设置分别表示：

- `c++`：使用 C++ API；这里主要是因为我个人不太喜欢 C 和 C++ 的语法混用，所以就尽可能还是想使用纯 C++ 实现，不使用 C++ API 当然也没有任何问题
- `noyywrap`：在遇到 EOF (end-of-file) 时，不调用函数 `yywrap()`，而是直接终止程序；因为我们这里只打算一次处理一个源文件，所以遇到 EOF 后简单退出就可以了
- `yylineno`：让 Flex 在扫描时自行维护当前所在的行号，保存在变量 `yylineno` 中（C++ API 下需使用函数 `lineno()` 读取）；当然我们也可以手动实现，但如果 Flex 本来就提供了这个接口，那么使用官方的实现通常是比较稳妥的选择

然后，我们声明了一个初始状态 `COMMENT`。

```cpp {.line-numbers}
// src/lexer.lex

%x      COMMENT
```

声明这个初始状态的目的将在之后的规则区部分阐述。

最后，我们声明了一些名称 (name)，用来对一些正则表达式片段以 `${name}` 的形式进行复用。

```cpp {.line-numbers}
// src/lexer.lex

WS                    ([ \t]+)
NEWLINE               (\r?\n)

DIGIT                 [0-9]
INTEGER               ({DIGIT}+)
REAL                  ({DIGIT}+"."{DIGIT}*)
LETTER                [A-Za-z]
STRING                (\"[^\n"]*\")
UNTERM_STRING         (\"[^\n"]*)

RESERVED              (AND|ARRAY|BEGIN|BY|DIV|DO|ELSE|ELSIF|END|EXIT|FOR|IF|IN|IS|LOOP|MOD|NOT|OF|OR|OUT|PROCEDURE|PROGRAM|READ|RECORD|RETURN|THEN|TO|TYPE|VAR|WHILE|WRITE)
IDENTIFIER            ({LETTER}({LETTER}|{DIGIT})*)
OPERATOR              (":="|"+"|"-"|"*"|"/"|"<"|"<="|">"|">="|"="|"<>")
DELIMITER             (":"|";"|","|"."|"("|")"|"["|"]"|"{"|"}"|"[<"|">]"|"\\")

COMMENTS_BEGIN        "(*"
COMMENTS_END          "*)"
```

虽说是正则表达式，其实只是 Flex 实现的简化版正则表达式，像贪婪模式 `?` 之类的高级标识符都是不支持的。这也导致了我们很难简单通过正则表达式来匹配一个多行注释，关于这点之后我们会展开来讲。

以上就是定义区的全部内容。

##### 1.1.1 正则表达式原理

下面讲一下上述各个正则表达式的原理。[^2]

- `WS`：空白字符；`[ \t]+` 表示匹配 1 个或多个空格或 tab (`\t`)
- `NEWLINE`：换行符；`\r?\n` 表示匹配 LF (`\n`) 或 CRLF (`\r\n`) 两种格式的换行符

- `DIGIT`：数字；`[0-9]` 表示匹配 $0\sim 9$ 中的任一数字
- `INTEGER`：整数，注意 PCAT 语法下将 `INTEGER` 规定为不超过 $2^{31} - 1$ 的**非负**整数；`{DIGIT}+` 表示匹配 1 个或多个数字（也就是先前定义的 `DIGIT`）；由于 `INTEGER` 一定非负，因此不需要检查开头是否有负号，这里我们将整数是否越界的报错判断逻辑留到了用户代码部分
- `REAL`：浮点数（实数），注意 PCAT 语法下将 `REAL` 规定为没有精度限制的**非负**实数；`{DIGIT}+"."{DIGIT}*` 表示匹配一个包含小数点 `.` 的浮点数，其中小数点前为 1 个或多个数字，小数点后为 0 个或多个数字
- `LETTER`：字母；`[A-Za-z]` 表示匹配任一大写或小写字母
- `STRING`：（单行）字符串；`\"[^\n"]*\")` 表示匹配一个开头和结尾均为引号 `"` 的字符串，引号之间包含 0 个或多个非换行字符 (`\n`) 的任意字符
- `UNTERM_STRING`：（单行）未闭合的字符串；实际上就是缺少了右引号的 `STRING`，用于后续报错功能的实现

- `RESERVED`：保留字；原表达式即匹配其中出现的关键字之一
- `IDENTIFIER`：标识符；`{LETTER}({LETTER}|{DIGIT})*` 表示匹配一个由字母或数字组成的标识符，其中第一个字符必须为字母
- `OPERATOR`：运算符；原表达式即匹配其中出现的运算符之一
- `DELIMITER`：分隔符；原表达式即匹配其中出现的分隔符之一

- `COMMENTS_BEGIN`：（多行）注释开始；即 `(*`
- `COMMENTS_END`：（多行）注释结束；即 `*)`

#### 1.2 规则区

然后就是 Flex 程序中的关键部分——规则区。这里我们利用先前在 `src/lexer.hpp` 中定义好的 token 类型，将扫描过程中读取到的 token 进行分类。

```cpp {.line-numbers}
// src/lexer.lex

<INITIAL><<EOF>>              return T_EOF;
<INITIAL>{WS}                 return T_WS;
<INITIAL>{NEWLINE}            return T_NEWLINE;

<INITIAL>{INTEGER}            return T_INTEGER;
<INITIAL>{REAL}               return T_REAL;
<INITIAL>{STRING}             return T_STRING;
<INITIAL>{UNTERM_STRING}      return E_UNTERM_STRING;

<INITIAL>{RESERVED}           return T_RESERVED;
<INITIAL>{IDENTIFIER}         return T_IDENTIFIER;
<INITIAL>{OPERATOR}           return T_OPERATOR;
<INITIAL>{DELIMITER}          return T_DELIMITER;

<INITIAL>{COMMENTS_BEGIN}     BEGIN(COMMENT); return T_COMMENTS_BEGIN;
<COMMENT>{COMMENTS_END}       BEGIN(INITIAL); return T_COMMENTS_END;
<COMMENT>(.|{NEWLINE})        return T_COMMENTS;
<COMMENT><<EOF>>              return E_UNTERM_COMMENTS;

<INITIAL>.                    return E_UNKNOWN_CHAR;
```

这部分中，每一行就代表一个规则。左侧为规则的 pattern，右侧为匹配到此 pattern 时需要采取的操作。

规则中 pattern 的格式为 `<起始状态>正则表达式`。起始状态部分可以缺省，表示无需考虑状态，始终匹配。这里 `<INITIAL>` 表示默认的起始状态，`<COMMENT>` 则为我们之前声明的注释状态。正则表达式部分就用到了我们前面在定义区里定义的名称，这里可以直接使用。

可以看到，当我们匹配到相应的 pattern 时，通常我们会直接 `return` 一个 token 类型，这就是分类的过程。用户程序中，每次我们调用函数 `yylex()`，即可从源文件中读入一个单词，并根据这些规则进行匹配，最后得到相应的 token 类型。

这里比较特殊的是对注释的处理。最开始我是使用的纯正则表达式进行匹配，这个当然可以做，表达式应该是这样：`"(*"([^*]|(\*[^)]))*"*)"`。写成这样是因为 Flex 的正则表达式不支持贪婪模式，因此普通的匹配会直接匹配到程序的第一个 `(*` 和最后一个 `*)`，然后中间就全部被当成注释了。为了避免这个问题，需要手动设定第一次匹配到 `*)` 就退出。

但问题出在对未闭合多行注释错误的处理，至少我是没能写出好的正则表达式实现。虽然可以通过测试样例，但对于我自行设计的一些更复杂的样例，就不太能很好地匹配了。因此我最终放弃了纯正则表达式的方案。

现在的方案是这样：`<INITIAL>` 状态下匹配到 `(*`，就进入 `<COMMENT>` 状态；`<COMMENT>` 状态下匹配到 `*)`，就回到 `<INITIAL>` 状态；`<COMMENT>` 状态下匹配到非 EOF 的任意字符，都保持当前状态；`<COMMENT>` 状态下匹配到 EOF，就返回未闭合注释错误。对于注释的内容，我们在用户代码中手动维护，这个之后再详细阐述。这样我们就优雅地实现了注释的匹配，可读性也比纯正则表达式要好很多。

最后，我们使用 `<INITIAL>.` 规则进行兜底，用来处理源文件中出现的未知字符。

#### 1.3 用户代码区

为了代码结构清晰，我们将用户代码单独分出来，作为一个独立的文件 `src/main.cpp`。一些细枝末节（例如文件 I/O 的实现）这里就不赘述了，具体可以直接查阅源代码，我们这里只讲关键的 token 所在行列号和类型的判断，以及报错功能的实现。

##### 1.3.1 判断 token 所在行列号

核心思路实际上就是维护 Flex 当前扫描到的位置。由于 Flex 会自行维护当前所在的行号，因此我们的重点在于维护当前所在的列号。当然，对于多行注释，我们也需要手动维护注释起始位置的行号。

```cpp {.line-numbers}
// src/main.cpp

std::tuple<int, int> UpdatePosition(const yyFlexLexer& lexer) {
  static int cur_row = 1;     // self-maintained current row number
  static int cur_column = 1;  // self-maintained current column number

  auto start_row = cur_row;
  auto start_column = cur_column;
  std::string token = lexer.YYText();
  auto size = lexer.YYLeng();

  if (lexer.lineno() == start_row) {
    cur_column += size;
  } else {
    cur_row = lexer.lineno();
    auto endl_pos = token.find_last_of('\n');
    cur_column = endl_pos == std::string::npos ? size : size - endl_pos;
  }

  return {start_row, start_column};
}
```

具体来说，我们的思路是利用 Flex 当前匹配到的单词长度 `yyleng`（C++ API 下需使用函数 `YYLeng()` 读取）作为当前所在列关于上次扫描时的偏移量，然后根据上次扫描时所在的列号，计算出本次扫描时所在的列号。因此，我们在扫描时也需要读入空白字符，不能直接简单跳过了。当读取到换行符时，行号更新（加 1），列号归 0。

这里为了程序的可维护性，考虑到多行注释可能存在的不同实现，我们保持了对一次性读入多行注释这种情况的支持。这种情况下，读入的文本 `yytext` 中可能包含换行符，因此不能简单通过增加一个 `yyleng` 就得到当前列号。这里如果我们发现当前所在行（由 Flex 自动维护的 `yylineno`）与上次扫描时不同，说明读入的文本中存在换行符。那么我们就找到换行符最后一次出现的位置，计算这个位置到文本末尾的距离，即可得到实际的列号。

这样，我们就得到 token 所在的行列号了。需要注意的是，我们多行注释目前的实现会实时更新当前所在的位置，但我们需要的其实是注释的起始位置，也就是 `(*` 的位置。当然我们也不能不让它更新位置了，不然之后读入的 token 就会出现行列号错误的问题。我们这里采用的方案是，简单备份一下 `(*` 所在的位置，之后遇到 `*)` 时，输出这个备份的起始位置就可以了。

##### 1.3.2 判断 token 的类型

这个很简单，直接调用函数 `yylex()` 得到当前 token 的类型，然后用一个 `switch` 语句还原成一个字符串就可以了。例如：

```cpp {.line-numbers}
// src/main.cpp

auto t = lexer.yylex();

auto [type, error_msg] =
    [&token](int t) -> std::tuple<const char*, const char*> {
  switch (t) {
    case T_REAL: return {"real", NULL};
    case T_OPERATOR: return {"operator", NULL};
    case T_DELIMITER: return {"delimiter", NULL};
    // ...
    default: return {"error", "UnknownError"};
  }
}(t);
```

这里变量 `type` 保存的就是输出的 token 类型。

##### 1.3.3 实现报错功能

根据 PCAT 语言的规范 [^2]，存在以下几种可能的词法错误：

- 整数大小超过 $2^{31} - 1$
- 字符串常量的长度（不含引号）超过 $255$
- 字符串中包含非法字符 tab
- 标识符长度超过 $255$
- 未闭合的字符串（没有右引号）
- 未闭合的注释（没有 `*)`）
- 出现未知字符

以下我们分别讲讲这些报错功能是如何实现的。

###### 1.3.3.1 整数错误

整数的越界错误我们是留到用户代码区再处理的，因为这样比较方便。具体来说，当 token 类型是整数时，我们先检查它的位数（十进制）是否超过 10。因为如果位数超过 10 了，就一定大于 $2147483647$ 了。这样做是为了防止类型转换时越界。对于 10 位以内的整数，我们就可以放心地利用函数 `std::stoull()` 转换为 64 位整型 `uint64_t` 了，然后简单地和 `INT32_MAX` 比一下大小即可。

```cpp {.line-numbers}
// src/main.cpp

case T_INTEGER: {
  if (token.size() > 10 || std::stoull(token) > INT32_MAX) {
    return {"error", "RangeError: out of range"};
  }
  return {"integer", NULL};
}
```

###### 1.3.3.2 字符串错误

字符串的错误分为 3 种。对于字符串值相关的 2 种错误，我们可以简单地留到用户代码区解决。具体来说，先判断一下总长度（含引号）是否超过 $257$，然后查找一下字符串中是否包含 tab 就可以了。

```cpp {.line-numbers}
// src/main.cpp

case T_STRING: {
  if (token.size() > 257) {
    return {"error", "ValueError: string literal is too long"};
  } else if (token.find('\t') != std::string::npos) {
    return {"error", "ValueError: invalid character found in string"};
  }
  return {"string", NULL};
}
```

而对于字符串未闭合的错误，由于未闭合的字符串从定义上来说就不是字符串了，因此需要在规则区解决。之前我们在 token 分类时有一个对应未闭合字符串的 `E_UNTERM_STRING` 类别，就是用于这种情况的报错实现。

```cpp {.line-numbers}
// src/main.cpp

case E_UNTERM_STRING:
  return {"error", "SyntaxError: unterminated string literal"};
```

###### 1.3.3.3 标识符错误

标识符过长的报错处理和字符串是一样的，这里不再赘述。

```cpp {.line-numbers}
// src/main.cpp

case T_IDENTIFIER: {
  if (token.size() > 255) {
    return {"error", "CompileError: identifier is too long"};
  }
  return {"identifier", NULL};
}
```

出现未知字符或许也可以算标识符错误吧，也放在这里。处理方式之前已经提到了，就是在规则区利用一个兜底规则，将所有未成功匹配的字符判定为未知字符。

```cpp {.line-numbers}
// src/main.cpp

case E_UNKNOWN_CHAR:
  return {"error", "CompileError: unknown character"};
```

###### 1.3.3.4 注释错误

注释的错误是最难处理的，因为我们还需要手动维护注释的内容（如果希望在报错信息中展示的话）。为什么不能简单地使用函数 `yymore()` 呢？其实普通注释是可以的，但对于未闭合的注释，Flex 将不可避免地读入 EOF。而当 Flex 读到 EOF 时，会直接清空当前的 `yytext` 缓冲区，而这个问题是很难规避的 [^3]，于是注释的内容就保存不下来了。比较好的解决方案是手动维护。

先看代码（只保留了注释处理相关的部分）：

```cpp {.line-numbers}
// src/main.cpp

static std::string buf;  // self-maintained buffer for comments
static int start_row_bak = 1;
static int start_column_bak = 1;

auto t = lexer.yylex();
auto [start_row, start_column] = UpdatePosition(lexer);
std::string token = lexer.YYText();

switch (t) {
  // ...

  case T_COMMENTS_BEGIN: {
    start_row_bak = start_row;
    start_column_bak = start_column;
    // fallthrough
  }
  case T_COMMENTS: {
    buf += token;
    break;
  }
  case E_UNTERM_COMMENTS:
  case T_COMMENTS_END: {
    // Use the actual starting position of T_COMMENTS_BEGIN.
    start_row = start_row_bak;
    start_column = start_column_bak;
    token = buf + token;
    buf.clear();
    // fallthrough
  }

  default: {
    auto [type, error_msg] =
        [&token](int t) -> std::tuple<const char*, const char*> {
      switch (t) {
        case T_COMMENTS_END: return {"comments", NULL};
        case E_UNTERM_COMMENTS:
          return {"error", "SyntaxError: unterminated comments"};
        // ...
      }
    }(t);

    // Output to file
  }
}
```

具体来说：

- 当遇到注释开头 `T_COMMENTS_BEGIN` 时，备份注释开头所在的行列号，将本次读入的文本（也就是 `(*`）附加到缓冲区 `buf` 末尾
- 当遇到注释文本 `T_COMMENTS` 时，将读入的文本附加到 `buf` 末尾
- 当遇到注释结尾 `T_COMMENTS_END` 或遇到 EOF（也就是注释未闭合的情况 `E_UNTERM_COMMENTS`）时，还原注释开头所在的行列号，将读入的文本（也就是 `*)`）附加到 `buf` 末尾，并将 `buf` 的内容（也就是注释的完整内容）作为 `token`，然后清空 `buf`，进入下一阶段
  - 对于 `T_COMMENTS_END` 的情况，说明注释格式正常，正常输出
  - 对于 `E_UNTERM_COMMENTS` 的情况，说明注释未闭合，进行报错

至此，所有词法错误的报错功能就都实现完了。

### 2. 运行本项目

#### 2.1 依赖

在构建本项目前，你需要安装以下依赖。

- [GNU make](https://www.gnu.org/software/make) 4.0 或以上
- [GCC](https://gcc.gnu.org/releases.html) 9.0 或以上（以支持 C++17 特性）
- [Flex](https://github.com/westes/flex) 2.6.4（其他版本未测试）

#### 2.2 构建与运行

本项目使用 Makefile 组织，因此在根目录执行 `make` 即可构建并运行本项目。对于 Windows，可以使用 [MinGW-w64](http://mingw-w64.org/doku.php/download)，然后执行 `mingw32-make`，也可以使用 WSL（推荐）。

这里执行 `make` 默认是进入命令行交互模式，按 `CTRL + D` 退出。如果需要从文件中读入，则需要执行 `make INPUT=<path>`，其中 `<path>` 即为指定的文件路径。例如：

```bash
make INPUT="tests/case_1.pcat"
```

输出文件将默认保存在 `output` 目录下。

#### 2.3 测试

本项目自带了 14 个 PCAT 语言测试样例，位于 `tests` 目录下。测试时依次通过上述命令处理即可，或者你当然也可以简单写一个 bash 脚本。

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
ROW   COL   TYPE                TOKEN / ERROR MESSAGE
--------------------------------------------------------------------------------
1     1     reserved keyword    PROGRAM
1     9     reserved keyword    IS
2     5     reserved keyword    VAR
2     9     identifier          i
2     10    delimiter           ,
2     12    identifier          j
2     14    delimiter           :
2     16    identifier          INTEGER
2     24    operator            :=
2     27    integer             1
2     28    delimiter           ;
3     5     reserved keyword    VAR
3     9     identifier          x
3     11    delimiter           :
3     13    identifier          REAL
3     18    operator            :=
3     21    real                2.0
3     24    delimiter           ;
4     5     reserved keyword    VAR
4     9     identifier          y
4     11    delimiter           :
4     13    identifier          REAL
4     18    operator            :=
4     21    real                3.0
4     24    delimiter           ;
5     1     reserved keyword    BEGIN
6     5     reserved keyword    WRITE
6     11    delimiter           (
6     12    string              "i = "
6     18    delimiter           ,
6     20    identifier          i
6     21    delimiter           ,
6     23    string              ", j = "
6     31    delimiter           ,
6     33    identifier          j
6     34    delimiter           )
6     35    delimiter           ;
7     5     reserved keyword    WRITE
7     11    delimiter           (
7     12    string              "x = "
7     18    delimiter           ,
7     20    identifier          x
7     21    delimiter           ,
7     23    string              ", y = "
7     31    delimiter           ,
7     33    identifier          y
7     34    delimiter           )
7     35    delimiter           ;
8     1     reserved keyword    END
8     4     delimiter           ;
--------------------------------------------------------------------------------

Total: 50 tokens, 0 errors
```

## 贡献者

- [**Hakula Chen**](https://github.com/hakula139)<[i@hakula.xyz](mailto:i@hakula.xyz)> - 复旦大学

## 许可协议

本项目遵循 MIT 许可协议，详情参见 [LICENSE](../LICENSE) 文件。

[^1]: [Flex - a scanner generator - Format](https://ftp.gnu.org/old-gnu/Manuals/flex-2.5.4/html_node/flex_6.html)  
[^2]: [The PCAT Programming Language Reference Manual](http://web.cecs.pdx.edu/~harry/compilers/PCATLangSpec.pdf)  
[^3]: [Using flex, how can I keep yytext contents when EOF is reached and input is provided via YY_INPUT? - Stack Overflow](https://stackoverflow.com/questions/14418560/using-flex-how-can-i-keep-yytext-contents-when-eof-is-reached-and-input-is-prov)  
