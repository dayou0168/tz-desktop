# TZ/假飞机项目跨电脑接管文档

> 状态快照日期：2026-07-20（Asia/Shanghai）  
> 适用项目：软件名 TZ，版本 1.0.1；内部中文品牌“天泽集团”；服务端为 TZ 定制 gramsrv  
> 环境性质：当前服务器与本地环境仅用于跑通、测试和验证，不属于正式上线环境。  
> 本文只记录可公开、可追溯的非秘密信息。任何密码、API ID/hash、手机号、验证码、2FA、完整邀请 hash、session、env、token、私钥都不得写入仓库。

## 1. 一页式当前状态

以下状态必须分开理解，不能互相替代。

| 分类 | 当前状态 | 可追溯证据 |
|---|---|---|
| 已实现 | 客户端仓库已形成软件名 TZ、版本 1.0.1 的品牌与 Windows Release 工作流；服务端 GitHub 源码快照包含内部账号创建、密码认证、账号视角私聊记录/导出、删除/彻底删除后端与后台 UI 等实现内容 | 客户端内容基线 `5534f32b70b5531a0463b1a0f4caaadcd7309769`；服务端 main `f97348db4947a9479fd67912cfca2dbaef6cb050`、tree `e1846a93cad91835c48dd3256e701392c68e932d` |
| 已提交未发布 | 客户端 `5534f32...` 已在 main，GitHub Actions 正在构建，但还没有可认定为正式发布的 GitHub Release | [run 29742818667](https://github.com/dayou0168/tz-desktop/actions/runs/29742818667) 的 head 为 `5534f32...`，2026-07-20 核验时状态为 `in_progress` |
| 历史测试部署 | 2026-07-19 的部署记录证明服务端快照 `f97348d` 曾部署到当前测试服务器，并完成 systemd、迁移、Admin API/UI、HTTPS 与发布包验证 | 部署记录中的 source commit/tree 与当前 `dayou0168/tz-server` main 一致；这只是历史部署证据 |
| 当前测试部署待核验 | 2026-07-20 严格 BatchMode SSH 只读复核超时，不能确认当前 systemd 进程、二进制链接和哈希仍与 2026-07-19 完全一致 | 当前 `https://tg.tianze8.cc/` 可返回 HTTP 200 与 HSTS；这不能替代 systemd/二进制核验 |
| 正在构建 | GitHub run `29742818667` 正在 `Build TZ Release x64`；本机另有 LTO 候选构建正在运行 | 两者都是运行中候选，均不是成功产物、安装包验收或 Release |
| 未验收 | Windows 双客户端可视行为、消息实时/已读/断线补差分、邀请链接回退、最终安装包和 1.0.1 Release 尚未完成验收 | 必须以实际 Windows 客户端、产物哈希和 GitHub Release 为准 |

### 当前严禁误写

- 不得把正在运行的本地 LTO 构建写成“构建完成”。
- 不得把 run `29742818667` 写成“Actions 成功”；当前只是运行中。
- 不得把 Actions artifact 写成 GitHub Release。
- 不得用服务端单元测试、源码存在或历史部署记录代替 Windows 客户端可视验收。
- 不得因 HTTPS 200 就断言 MTProto、Admin API、systemd 和当前二进制全部正常。

## 2. 仓库、版本与部署映射

### 2.1 Windows 客户端

- 仓库：[dayou0168/tz-desktop](https://github.com/dayou0168/tz-desktop)
- 默认分支：`main`
- 本文编写时业务/构建内容基线：`5534f32b70b5531a0463b1a0f4caaadcd7309769`
- 软件名：`TZ`
- 版本：`1.0.1`
- 内部中文品牌：`天泽集团`
- 上游：[telegramdesktop/tdesktop](https://github.com/telegramdesktop/tdesktop)
- Windows Actions：[TZ Windows Release](https://github.com/dayou0168/tz-desktop/actions/workflows/tz-windows-release.yml)
- Releases：[tz-desktop/releases](https://github.com/dayou0168/tz-desktop/releases)

本接管文档本身提交到 main 后，main HEAD 会前进到纯文档 commit；客户端业务/构建基线仍是 `5534f32...`，除非后续另有业务提交。

### 2.2 TZ 服务端

- 定制仓库：[dayou0168/tz-server](https://github.com/dayou0168/tz-server)（私有仓库，需要相应 GitHub 权限）
- 默认分支：`main`
- 当前 main：`f97348db4947a9479fd67912cfca2dbaef6cb050`
- 当前 main tree：`e1846a93cad91835c48dd3256e701392c68e932d`
- 仓库形态：独立仓库快照，GitHub 元数据不是 fork，`f97348d` 是无父提交的源码快照
- 原始上游：[iamxvbaba/gramsrv](https://github.com/iamxvbaba/gramsrv)
- 2026-07-20 核验到的上游 main：`2965f5d47d25245b4155f4f712bac101aadce4b9`

由于 TZ 服务端使用无父提交快照，不能从 GitHub ancestry 推导其与当前上游 main 的精确 ahead/behind 关系。升级上游前必须做 tree/patch 级审计。

### 2.3 服务端历史提交关系

以下提交来自可追溯的本机轻量 Git refs。它们不是当前 GitHub main 的祖先，因为 main 是 squash/root snapshot；但 tree 差异核对证明对应实现内容已进入 `f97348d`：

| 提交 | 含义 | 集成/部署判断 |
|---|---|---|
| `e6deb27a32dd184c9a4943ddaca0606060bfa82b` | 后台内部账号创建 UI | 内容已进入 `f97348d`；随 `f97348d` 有 2026-07-19 历史部署证据 |
| `0b513af537502f6ef902aff2814628841044dd2c` | 内部账号密码认证 | 本机历史显示其父提交为 `e6deb27`；内容已进入 `f97348d`，有同一历史部署证据 |
| `b1275fb29fd47838b94fc8422ee5d9c761cfa375` | 账号视角私聊记录与导出后端 | 独立历史分支；内容已进入 `f97348d`，有同一历史部署证据 |
| `236bfebad1ca3337c420ce2bdcba2628d14f20a4` | 账号视角私聊与导出后台 UI | 独立历史分支；内容已进入 `f97348d`，有同一历史部署证据 |
| `e2b0b0ae27f095b02faa30df5d658ad38bfc5919` | TZ 服务端 Linux Release workflow | GitHub commit 存在且是 `f97348d` 的后继，但当前 main 仍指向 `f97348d`；没有集成到当前 main、没有部署证据 |
| `fd749459f1b766c43b020e88485ead635ef3b6dd` | 本机集成分支上的同 tree Release workflow | 不在当前 GitHub main；没有部署证据 |

### 2.4 历史测试部署映射

下列内容是 2026-07-19 部署记录，不是 2026-07-20 的实时 systemd 复核结果：

- 域名：`tg.tianze8.cc`
- 历史源码工作目录：`/opt/gramsrv/src`
- 历史不可变 release 根：`/opt/gramsrv/releases/f97348d-20260719T175542Z`
- 当前测试部署链接约定：`/opt/gramsrv/bin/gramsrv-current`、`/opt/gramsrv/bin/gramsrv-admin-current`
- systemd：`gramsrv.service`、`gramsrv-admin.service`、`gramsrv-deps.service`
- MTProto：公网 TCP `2398`
- Admin API：loopback `2399`
- Admin UI：loopback `2600`，由 HTTPS 入口反向代理
- 公开链接页：历史记录为 loopback `2401`
- PostgreSQL/Redis：仅 loopback，不应暴露公网
- 历史 schema：`95|f`

历史测试部署发布包：

- 路径：`/opt/gramsrv/artifacts/tz-server-1.0.1-linux-amd64.tar.gz`
- 大小：`85,936,080` bytes
- SHA256：`cce49f32a2cf179d859232fb9a1c9bd4e56c7ec0e079e78cec6a187c9e6c80cb`
- 历史验证：当前测试服务器与本地测试环境哈希一致，包内 436 项，不含运行时 env、数据库 dump 或私钥

## 3. 当前 Windows Release 状态

截至 2026-07-20：

- 当前 run：[29742818667](https://github.com/dayou0168/tz-desktop/actions/runs/29742818667)
- workflow：`TZ Windows Release`
- head：`5534f32b70b5531a0463b1a0f4caaadcd7309769`
- 状态：`in_progress`
- 当前步骤：`Build TZ Release x64`
- 已完成到：`Configure TZ Release x64`
- GitHub Releases：0
- Git tag：0
- 本地 LTO：仅运行中候选，不是已完成产物

当前 workflow 使用 `DESKTOP_APP_ENABLE_LTO=OFF`，会上传以下 Actions artifacts，保留期 14 天：

- `TZ-1.0.1-Setup.exe`
- `TZ-1.0.1-windows-x64.zip`
- `TZ-1.0.1-symbols.zip`
- `SHA256SUMS.txt`

该 workflow 权限为 `contents: read`，只上传 Actions artifact，不会自动创建 GitHub Release。只有成功结束、下载核验、Windows 实机验收并明确发布后，才能创建 1.0.1 Release。

## 4. 未完成清单

“源码存在”与“最终完成”之间仍有以下缺口。

| 范围 | 已知状态 | 完成标准 |
|---|---|---|
| 客户端密码登录 UI | 服务端密码认证内容已进入 `f97348d`；客户端最终密码登录交互未验收 | 新装客户端以真实账号完成首次密码设置、登录、错误密码、重置后旧密码拒绝/新密码接受的可视 E2E |
| Telegram 字样品牌清理 | 已有 TZ 品牌提交；客户端可见运行时文字仍需最终替换为“天泽集团”。仓库源码中的技术标识、上游项目名和上游链接不属于运行时 UI | 对安装器、窗口、登录页、菜单、通知、协议提示、关于页等客户端可见运行时文字做全量可视审查，最终统一替换为“天泽集团” |
| 后台重复创建账号入口 | 源码快照包含相关修复内容 | 当前测试后台确认只有一个明确入口，刷新与窄屏下不重复 |
| 创建/重置密码 UI | 源码与历史 E2E 有实现证据 | 在当前测试部署的二进制和真实浏览器中重新验收创建、重置、错误提示与权限边界 |
| 账号视角私聊记录/批量导出 | 后端与 UI 内容已进入快照，历史 E2E 通过 | 用当前测试数据做分页、账号级与单会话导出、下载内容和大数据量验证 |
| 勾选删除 | 已有批量选择/删除路径 | 对非空会话、跨页选择、撤销/确认、权限和审计记录在当前测试后台验收 |
| 媒体占位 | 当前设计为文字占位，不导出媒体二进制 | 图片、视频、语音、文件、贴纸等类型都要显示稳定且不泄露媒体内容 |
| 彻底删除 | 历史空会话 delete/purge E2E 通过 | 非空真实数据下验证预览、二次确认、物理删除、幂等、审计和不可恢复边界 |
| 群邀请链接 | 静态可点击与协议未注册回退未完成验收 | Windows 已注册/未注册协议两种环境都能得到可理解、可继续操作的结果 |
| Reaction/贴纸 | 服务端 seed 导入有证据 | 两个隔离 Windows 客户端互发、接收、重启后可见，面板与消息气泡均通过可视验收 |
| 消息实时/已读 | 服务端源码宣称支持 fan-out/read boundary | Windows 双客户端验证实时送达、已读回执、多 session 不重复 echo |
| 断线补差分 | 服务端源码含 difference 路径 | 断网发送、重连、进程重启后通过 `getDifference` 补齐且不丢不重 |
| 网络目标 | HTTPS 可达，但域名经 Cloudflare 的 2398 探测不能证明 origin MTProto | 从正式 Windows 安装包连接目标服务，验证登录、收发、媒体和日志无新增协议错误 |
| 安装包与 1.0.1 发布 | Actions 和本地构建均在运行 | 产物成功、SHA256 一致、干净 Windows 安装/卸载/便携包验收、创建 tag 与 GitHub Release |
| 语音通话 | 明确冻结 | 不改 voice/TURN/SFU；待安装包稳定且用户明确重开后再排期双客户端验收 |
| 服务端未集成/未部署 | `e2b0b0a` / `fd749459` 的 Linux Release workflow 不在当前 main | 决定是否需要后再单独集成；不得把它写成已部署 |
| 当前测试服务器在线状态 | SSH 实时复核失败 | 从获授权的新电脑只读核验 systemd、二进制 symlink/hash、schema、监听和 API/UI，再更新本文 |

## 5. 服务端迁移、部署与回滚

### 5.1 部署前只读核验

1. 核对 `dayou0168/tz-server/main` commit 与准备部署的 source tree。
2. 读取当前 systemd unit、`ExecStart`、active/enabled、当前二进制 symlink 与 SHA256。
3. 核对数据库 schema、磁盘空间、PostgreSQL/Redis 状态、监听端口和 HTTPS。
4. 备份数据库与运行配置；配置只留在服务器，不打印、不上传 GitHub。
5. 确认新 release 目录及其父目录允许 service user 遍历，避免 `CHDIR Permission denied`。

### 5.2 最短部署链

1. 将固定 commit 的源码放到新的不可变 release 目录，不覆盖当前目录。
2. 先完成 Admin Web 静态资源，再构建 Admin 二进制，避免嵌入旧 `dist`。
3. 生成带版本/commit 的 `gramsrv` 与 `gramsrv-admin` 二进制并计算 SHA256。
4. 先执行受控数据库备份，再应用目标版本需要的 forward migration。
5. 原子切换 `gramsrv-current` 和 `gramsrv-admin-current` symlink。
6. `daemon-reload` 并串行重启依赖、主服务、Admin；逐个检查 active/enabled。
7. 验证 schema、2398、loopback 2399/2600、HTTPS、登录/session/logout、密码、记录/导出、删除/purge。
8. 生成发布包与 `.sha256`，当前测试服务器和接管电脑各核验一次。
9. 删除临时 helper、临时 auth/session 与中间包；保留正式 artifact、manifest、备份和回滚说明。

### 5.3 回滚原则

- 回滚优先切回上一组已验证的版本化二进制 symlink，再串行重启并核验。
- 不要默认执行 down migration；若新 schema 与旧二进制不兼容，按部署前备份恢复或采用经过审查的 forward fix。
- 回滚后必须重新核验 systemd、schema、监听、HTTPS、登录和消息，不以“进程启动”作为完成。
- 任何秘密配置仍保留在当前测试服务器配置文件中，不进入 release 包、日志摘录或 GitHub。

## 6. 官方 Reaction/贴纸 seed

当前服务端源码的配置入口为 `TELESRV_STICKER_SEED_DIR`，默认相对路径 `data/sticker-seed`。2026-07-19 的历史证据包括：

- importer 测试：74 个 reactions、9 个 sticker sets、907 个 documents、1814 个导入 blobs
- 当前测试服务器历史导入后：74 个 available reactions、9 个 sticker sets、907 个 seed documents、0 个缺失主 blob
- 导入后数据库总 blobs 为 1952，其中包含导入前已有数据
- 规范化 manifest SHA256：`656701c3361c902135c21b958c32869a82b2c90d9fc65c8d78c81a5e36d8d692`
- manifest：1825 个内容哈希，零不匹配；临时授权/session 已清理

待核验事项：

- 当前测试服务器实际使用的 seed 绝对路径
- 当前 seed 目录与数据库计数是否仍一致
- 当前 seed archive 的正式文件名、安装包 SHA256 与服务器留存位置

曾提供过一个 seed release archive 候选哈希，但在本次可用的 GitHub/历史证据中未找到可重复核验的对应文件，因此本文不把它列为有效发布哈希。

## 7. Windows Release 最短执行链

### 7.1 日常构建

- 默认 `DESKTOP_APP_ENABLE_LTO=OFF`，缩短反馈时间。
- 使用官方 `Telegram\build\prepare\win.bat` 准备 Windows 依赖。
- 可用 GitHub-hosted `windows-2022`，也可显式选择已维护的自托管 `tz-windows-x64`。
- 不为日常构建增加额外审批门禁。

### 7.2 正式里程碑

1. 锁定准备发布的 client commit 与全部 recursive gitlink。
2. 正式里程碑可改为 `DESKTOP_APP_ENABLE_LTO=ON`；记录该模式，不与日常 OFF 产物混淆。
3. 使用 `prepare\win.bat` 后配置 Release x64，构建 `TZ.exe`、安装器、便携 zip、symbols。
4. 检查安装包只含运行时必需文件；PDB 只进入单独 symbols artifact。
5. 生成 `SHA256SUMS.txt`，逐个对安装器、便携包和 symbols 核验。
6. 在干净 Windows 环境验收安装、启动、登录、双客户端消息、断线恢复、Reaction/贴纸、卸载。
7. 只有上述步骤成功后，创建 `1.0.1` tag 和 GitHub Release，上传安装器、便携包、symbols 与 SHA256。
8. Release 页面和 tag/commit 必须一致；不得拿 Actions artifact URL 充当 Release URL。

## 8. 新电脑接管

### 8.1 构建电脑准备

真正要构建客户端时才执行递归 clone：

```powershell
git clone --recursive https://github.com/dayou0168/tz-desktop.git
cd tz-desktop
git submodule status --recursive
```

服务端仓库为私有仓库，先用具有权限的 GitHub 账号认证，再单独 clone。不要把认证 token 写入 remote URL、脚本或文档。

### 8.2 打开 Codex 新任务

在新电脑的 Codex 中创建一个新的总控任务，并把工作目录指向新 clone 的协调目录。Codex 的本地任务历史、未提交文件、构建缓存、临时日志和本机 refs 不会因 GitHub clone 自动同步；GitHub 中的 commit、本文和明确上传的 artifacts 才能跨电脑继承。

### 8.3 首条提示词模板

```text
你负责 TZ/假飞机项目总控。先阅读 docs/TZ_PROJECT_HANDOFF.zh-CN.md，
再通过 GitHub API 实时核验 tz-desktop/main、tz-server/main、Actions、Releases，
并在获得当前测试服务器授权后只读核验 systemd、二进制 symlink/hash、schema、监听与 HTTPS。

总控任务保持只读，不修改业务代码；实现、构建、测试、部署分别派发到独立模块任务。
模块任务优先使用模型 5.6sol、reasoning high；若当前环境不可用，必须明确报告，
不得静默替换。不要把源码存在、commit、Actions artifact、历史部署或截图互相当成完成。
不得触碰或清理任何仍在运行的构建树。不得输出任何密码、API 凭据、手机号、验证码、
2FA、邀请 hash、session、env、token 或私钥。
```

## 9. 真相来源与禁止误判

| 真相来源 | 能证明什么 | 不能证明什么 |
|---|---|---|
| GitHub commit/tree | 某一份源码和文档已进入指定 ref；tree diff 可证明快照包含哪些内容 | 不能证明构建成功、已部署或用户可见行为正确 |
| GitHub Actions run | 指定 head 的自动化步骤状态、日志与 artifacts | `queued`/`in_progress` 不是成功；artifact 不是 Release |
| GitHub Release/tag | 某 commit 的正式发布记录、发布资产与公开 URL | 仍不能替代安装后运行验收 |
| 当前测试服务器 systemd | 当前 unit、进程、启动时间、ExecStart 和 active/enabled | 单纯 active 不能证明 API、协议、数据和客户端正常 |
| 当前测试服务器二进制/symlink/hash | 当前实际运行文件与发布包是否对应固定版本 | 不能证明数据库迁移和客户端可见功能 |
| 数据库 schema/计数 | migration 与 seed 数据当前状态 | 不能证明 Windows UI 能显示或交互 |
| HTTPS/API E2E | Web/Admin 入口和具体接口在当前环境工作 | HTTPS 200 不能证明 MTProto 2398 或 Windows 客户端 |
| Windows 双客户端可视验收 | 登录、消息、已读、断线恢复、Reaction/贴纸等真实体验 | 一次截图不能证明重启、离线恢复和长期稳定性 |

## 10. 接管后的第一轮只读命令目标

新电脑第一次接管只需收集以下结果，不要立即构建或部署：

1. `tz-desktop/main` 与 `tz-server/main` 的精确 SHA。
2. run `29742818667` 的最终状态与 conclusion。
3. Releases/tag 的实际数量、资产名和 SHA256。
4. 当前测试服务器三项 systemd unit、二进制 symlink/hash、schema 与监听。
5. 当前 seed 路径、manifest/hash、Reaction/贴纸计数。
6. 是否存在经过验收的 TZ 1.0.1 安装包。
7. 将所有“待核验”更新为带日期和证据的结果；仍无证据的继续保留为待核验。
