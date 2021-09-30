# PCAT 词法分析

本项目利用 Flex (fast lexical analyzer generator)，实现了对给定 PCAT 语言样例的词法分析。

## 目录

- [PCAT 词法分析](#pcat-词法分析)
  - [目录](#目录)
  - [项目报告](#项目报告)
    - [Flex 使用方法](#flex-使用方法)
  - [贡献者](#贡献者)
  - [许可协议](#许可协议)
  
## 项目报告

### Flex 使用方法

一个 Flex 程序分为三个部分：

- 定义区（Declarations）：包含所有 token 的声明，以及一些 Flex 的设置

```cpp {.line-numbers}
%{
// Declarations
%}

%%
//
%%

//
```

## 贡献者

- [**Hakula Chen**](https://github.com/hakula139)<[i@hakula.xyz](mailto:i@hakula.xyz)> - 复旦大学

## 许可协议

本项目遵循 MIT 许可协议，详情参见 [LICENSE](../LICENSE) 文件。
