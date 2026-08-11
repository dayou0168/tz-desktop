# TZ Desktop 1.0.5

**简体中文** | [English](README.en.md)

TZ Desktop 是 **TZ / 天泽集团** 品牌的 Windows x64 私有即时通信客户端。本仓库基于上游 Telegram Desktop 定制，客户端相关的服务端与管理端源码见 [tz-server](https://github.com/dayou0168/tz-server)（访问权限以该仓库设置为准）。

## Windows x64 下载

当前公开的 Windows 二进制版本为 [TZ 1.0.3 LTO](https://github.com/dayou0168/tz-desktop/releases/tag/1.0.3)：

| 文件 | 用途 |
| --- | --- |
| [TZ-1.0.3-Setup.exe](https://github.com/dayou0168/tz-desktop/releases/download/1.0.3/TZ-1.0.3-Setup.exe) | Windows 安装程序，用于安装 TZ Desktop。 |
| [TZ-1.0.3-windows-x64.zip](https://github.com/dayou0168/tz-desktop/releases/download/1.0.3/TZ-1.0.3-windows-x64.zip) | 便携包；解压后直接运行其中的 `TZ.exe`，无需安装。 |
| [TZ-1.0.3-symbols.zip](https://github.com/dayou0168/tz-desktop/releases/download/1.0.3/TZ-1.0.3-symbols.zip) | 调试符号，供开发和故障分析使用；普通运行不需要。 |
| [SHA256SUMS.txt](https://github.com/dayou0168/tz-desktop/releases/download/1.0.3/SHA256SUMS.txt) | 上述三个发布文件的 SHA-256 校验值，用于核对下载完整性。 |

下载后建议先用 `SHA256SUMS.txt` 核对文件。Setup 和便携包是两种运行方式，不需要同时下载；symbols 不是应用运行组件。

## 当前状态与安全提示

- 1.0.5 仅发布源码，不包含 Windows 二进制；LTO 安装包和便携包后续在高内存 Windows 电脑上单独编译。上方下载区继续指向现有公开二进制版 1.0.3。
- 1.0.5 在“设置 → 隐私与安全”中新增独立的 TZ 登录密码修改入口，并保持与两步验证密码完全分离。
- 截至 2026-08-10，当前 Release 中的 `TZ-1.0.3-Setup.exe` 和便携包内 `TZ.exe` **均未进行 Authenticode 数字签名**。Windows 可能显示未知发布者或安全提示；请确认下载地址来自本仓库，并在决定运行前核对 SHA-256。
- 1.0.3 LTO 的发布文件已经公开，但安装、启动与运行、Windows 双客户端交互以及端到端（E2E）场景仍需在实际环境中验收。
- Release、源码存在或校验值一致只能证明对应文件与版本可追溯，不能替代实际环境中的功能和安全验收。

## 从源码构建

仓库保留的 [Windows 上游通用构建文档](docs/building-win.md) 仅供依赖环境参考；它仍包含上游 Telegram Desktop 的示例仓库地址，不可直接照抄来构建 TZ。

在高配 Windows 电脑上构建 1.0.5 正式 LTO 产物时，请直接使用 [TZ Desktop 1.0.5 Windows LTO 构建交接](docs/TZ_WINDOWS_1.0.5_LTO_CODEX.zh-CN.md)。该文档包含可直接粘贴给 Codex 的指令、固定提交、完整构建顺序、预期产物和实机验收清单。

## 上游、许可证与第三方组件

TZ Desktop 是基于 [Telegram Desktop](https://github.com/telegramdesktop/tdesktop) 的衍生项目。Telegram Desktop 及其原始代码归相应上游作者和贡献者所有；本项目不是 Telegram 官方客户端，也不应被理解为由 Telegram 官方发布或背书。

本仓库代码按照 [GNU GPL v3（含 OpenSSL 链接例外）](LICENSE) 提供。第三方组件仍分别适用其各自许可证和声明；可参阅英文页保留的[第三方组件列表](README.en.md#third-party)、[`LEGAL`](LEGAL) 以及 [`Telegram/ThirdParty`](Telegram/ThirdParty) 中随源码提供的许可文件。
