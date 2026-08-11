# TZ Desktop 1.0.5 Windows LTO 构建交接

本文是高配 Windows 电脑使用 Codex 构建 TZ Desktop 1.0.5 的当前权威入口。目标是生成 Windows x64 LTO 安装包、便携包、调试符号和 SHA-256 清单；开始前不需要修改客户端源码。

## 固定构建边界

- 仓库：`https://github.com/dayou0168/tz-desktop.git`
- tag：`1.0.5`
- 版本：`1.0.5` / `1000005`
- 架构：Windows x64
- 配置：Release + LTO
- 服务入口：`tztg.tianze8.cc:2398`
- MTProto DC：DC2
- 公开链接域名：`tg.tianze8.cc`
- 默认语言：简体中文

不得在“为了编译”时更新 Telegram Desktop 基线、递归子模块、TL 层、DC、RSA 公钥、域名、应用标识或版本号。1.0.5 已通过源码契约门禁，构建电脑的任务是复现并验收该固定源码。

## 建议电脑环境

- 64 位 Windows 11。
- 建议至少 64 GB 内存；LTO 链接阶段内存占用较高，保持 Windows 页面文件为“系统管理”。
- 建议至少 200 GB 可用磁盘空间。
- Visual Studio 2022 或仓库构建脚本可识别的更新版本，安装“使用 C++ 的桌面开发”、MSVC x64 工具和 Windows SDK。
- Python 3.10、Git、7-Zip、Inno Setup 6，并确保可在终端中调用。

## 首次克隆

在空间充足的短路径中执行，例如 `D:\TBuild`：

```powershell
Set-Location D:\TBuild
git clone --recursive --branch 1.0.5 https://github.com/dayou0168/tz-desktop.git
Set-Location .\tz-desktop
git rev-parse HEAD
git describe --tags --exact-match
git submodule status --recursive
```

`git describe --tags --exact-match` 必须输出 `1.0.5`。任何子模块状态行以 `-` 开头时，先执行：

```powershell
git submodule update --init --recursive
```

## 交给 Codex 的最短指令

在高配电脑上用 Codex 打开 `D:\TBuild\tz-desktop`，粘贴以下内容：

```text
阅读仓库根目录 AGENTS.md、REVIEW.md，以及 docs/TZ_WINDOWS_1.0.5_LTO_CODEX.zh-CN.md。
这是固定 tag 1.0.5 的 Windows x64 正式 LTO 构建，不升级源码、子模块、TL 层、DC、RSA、域名或版本号。
先运行 Telegram/build/tz/verify-tz-client.ps1，再严格复用 .github/workflows/tz-windows-release.yml 中 build-windows-x64 和 package-windows-x64 的命令：准备依赖、以 DESKTOP_APP_ENABLE_LTO=ON 配置、串行构建并运行全部 TZ contract tests、串行构建 Telegram、打包安装程序/便携包/symbols，并生成 SHA256SUMS.txt。
只在发现真实构建阻塞时修改代码；修改前先说明根因。不要把普通依赖下载时间或 LTO 编译时间当成失败。构建完成后核对版本、端点、DC2、RSA、文件列表和 SHA-256，并给出产物的绝对路径。
```

## Codex 应执行的构建顺序

仓库的 [Windows Release workflow](../.github/workflows/tz-windows-release.yml) 是命令级权威来源，顺序不得省略：

1. 运行 `Telegram\build\tz\verify-tz-client.ps1`。
2. 运行 `Telegram\build\prepare\win.bat` 准备依赖。
3. 调用 Visual Studio 的 `vcvars64.bat`。
4. 在 `Telegram` 目录用下列关键参数配置：

```bat
configure.bat x64 ^
  -D TDESKTOP_API_TEST=ON ^
  -D CMAKE_CONFIGURATION_TYPES=Release ^
  -D DESKTOP_APP_SPECIAL_TARGET=win64 ^
  -D DESKTOP_APP_ENABLE_LTO=ON ^
  -D DESKTOP_APP_DISABLE_AUTOUPDATE=ON ^
  -D DESKTOP_APP_DISABLE_CRASH_REPORTS=ON
```

5. 按 workflow 禁用生成工程的并行编译，避免 LTO 内存峰值失控。
6. 串行构建 `tz_contract_tests`，并运行以下五个测试：
   - `tz_client_contract_tests.exe`
   - `group_creation_contract_tests.exe`
   - `mtproto_dc_options_contract_tests.exe`
   - `send_as_policy_contract_tests.exe`
   - `tz_internal_url_contract_tests.exe`
7. 使用 workflow 中的串行 MSBuild 参数构建 `Telegram` Release x64；不要改成高并发。
8. 使用 Inno Setup 6 和 7-Zip 生成发布文件并校验压缩包。

## 预期产物

最终发布目录应包含：

- `TZ-1.0.5-Setup.exe`
- `TZ-1.0.5-windows-x64.zip`
- `TZ-1.0.5-symbols.zip`
- `SHA256SUMS.txt`

源码 Release 不包含这些文件。只有高配电脑完成构建和实机验收后，才把它们上传到 [1.0.5 Release](https://github.com/dayou0168/tz-desktop/releases/tag/1.0.5)。

## 构建后验收

- 安装包、便携包内应用版本均为 1.0.5。
- 登录页能连接服务端，不停留在网络转圈状态。
- 正确密码可登录，错误密码被拒绝。
- “设置 → 隐私与安全 → 修改登录密码”能修改登录密码，新密码可登录且旧密码失效。
- 两个 Windows 客户端可互发消息、图片、视频、文件和分享链接。
- 断网后恢复连接，消息不丢失、不重复。
- `https://tg.tianze8.cc/+...` 和用户名链接能在客户端内正确打开。
- 用户资料页的图片、视频、文件、链接和共同群组计数可见。
- 安装、卸载、便携运行均通过；`SHA256SUMS.txt` 与三个产物一致。

若以上任何一项未通过，不要上传二进制附件，也不要把 Actions artifact 或未验收的本地产物写成正式发布成功。
