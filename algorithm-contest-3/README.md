2021.3.26

此文件夹包含第三届数计学院算法大赛相关的文件。

| 文件夹/文件 | 用途           |
| ----------- | -------------- |
| code        | 参考答案代码   |
| test-data   | 测试数据生成   |
| output.zip  | 最终的测试数据 |

# Update

1. Update `gen.bash` script so that it can handle more than 9 pieces of test data. Because of `sed` does not support **lazy match** (aka. non-greedy match) so I turn to `perl`.

2. There seems to be a bug in integer sanitizer in clang, which cause the program (exactly problem 2 solution) outputs different result. So I turn off it. But I do not file a bug to llvm for laziness!
