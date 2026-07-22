# TZ Desktop 1.0.3

**简体中文** | [English](README.en.md)

TZ Desktop 是 **TZ / 天泽集团** 品牌的 Windows x64 私有即时通信客户端。本仓库基于上游 Telegram Desktop 定制，客户端相关的服务端与管理端源码见 [tz-server](https://github.com/dayou0168/tz-server)（访问权限以该仓库设置为准）。

## Windows x64 下载

当前公开版本为 [TZ 1.0.2 LTO](https://github.com/dayou0168/tz-desktop/releases/tag/1.0.2)：

| 文件 | 用途 |
| --- | --- |
| [TZ-1.0.2-Setup.exe](https://github.com/dayou0168/tz-desktop/releases/download/1.0.2/TZ-1.0.2-Setup.exe) | Windows 安装程序，用于安装 TZ Desktop。 |
| [TZ-1.0.2-windows-x64.zip](https://github.com/dayou0168/tz-desktop/releases/download/1.0.2/TZ-1.0.2-windows-x64.zip) | 便携包；解压后直接运行其中的 `TZ.exe`，无需安装。 |
| [TZ-1.0.2-symbols.zip](https://github.com/dayou0168/tz-desktop/releases/download/1.0.2/TZ-1.0.2-symbols.zip) | 调试符号，供开发和故障分析使用；普通运行不需要。 |
| [SHA256SUMS.txt](https://github.com/dayou0168/tz-desktop/releases/download/1.0.2/SHA256SUMS.txt) | 上述三个发布文件的 SHA-256 校验值，用于核对下载完整性。 |

下载后建议先用 `SHA256SUMS.txt` 核对文件。Setup 和便携包是两种运行方式，不需要同时下载；symbols 不是应用运行组件。

## 当前状态与安全提示

- 当前源码正在准备未来的 TZ 1.0.3 LTO；尚未执行正式 Release 构建、打包、Windows 实机验收或发布。上方下载区继续指向现有公开版 1.0.2。
- 截至 2026-07-22，当前 Release 中的 `TZ-1.0.2-Setup.exe` 和便携包内 `TZ.exe` **均未进行 Authenticode 数字签名**。Windows 可能显示未知发布者或安全提示；请确认下载地址来自本仓库，并在决定运行前核对 SHA-256。
- 1.0.2 LTO 的发布文件已经公开，但安装、启动与运行、Windows 双客户端交互以及端到端（E2E）场景尚未完成验收。本仓库不宣称这些项目已经通过。
- Release、源码存在或校验值一致只能证明对应文件与版本可追溯，不能替代实际环境中的功能和安全验收。

## 从源码构建

Windows 源码构建请从仓库现有的 [Windows 构建文档](docs/building-win.md) 开始，并使用其引用的 [`Telegram/build/prepare/win.bat`](Telegram/build/prepare/win.bat) 准备依赖。构建环境、依赖布局和命令以该文档及脚本的当前内容为准。

## 上游、许可证与第三方组件

TZ Desktop 是基于 [Telegram Desktop](https://github.com/telegramdesktop/tdesktop) 的衍生项目。Telegram Desktop 及其原始代码归相应上游作者和贡献者所有；本项目不是 Telegram 官方客户端，也不应被理解为由 Telegram 官方发布或背书。

本仓库代码按照 [GNU GPL v3（含 OpenSSL 链接例外）](LICENSE) 提供。第三方组件仍分别适用其各自许可证和声明；可参阅英文页保留的[第三方组件列表](README.en.md#third-party)、[`LEGAL`](LEGAL) 以及 [`Telegram/ThirdParty`](Telegram/ThirdParty) 中随源码提供的许可文件。
