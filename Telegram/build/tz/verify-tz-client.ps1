param(
    [string]$RepositoryRoot = (Resolve-Path "$PSScriptRoot\..\..\..").Path
)

$ErrorActionPreference = 'Stop'
$companyName = -join [char[]](0x5929, 0x6CFD, 0x96C6, 0x56E2)
$identityOrPasswordError = -join [char[]](0x8D26, 0x53F7, 0x6216, 0x5BC6, 0x7801, 0x9519, 0x8BEF)

function Read-RepositoryFile([string]$RelativePath) {
    return [IO.File]::ReadAllText((Join-Path $RepositoryRoot $RelativePath))
}

function Assert-Contains(
    [string]$RelativePath,
    [string]$Expected
) {
    $content = Read-RepositoryFile $RelativePath
    if (-not $content.Contains($Expected)) {
        throw "$RelativePath does not contain expected text: $Expected"
    }
}

function Assert-NotContains(
    [string]$RelativePath,
    [string]$Unexpected
) {
    $content = Read-RepositoryFile $RelativePath
    if ($content.Contains($Unexpected)) {
        throw "$RelativePath contains forbidden text: $Unexpected"
    }
}

Assert-Contains 'Telegram\SourceFiles\intro\intro_signup.cpp' 'Tz::SignupNamesAccepted(firstName, lastName)'
Assert-Contains 'Telegram\SourceFiles\intro\intro_phone.cpp' 'goNext<PasswordLoginWidget>()'
Assert-NotContains 'Telegram\SourceFiles\intro\intro_phone.cpp' 'goNext<CodeWidget>()'
Assert-Contains 'Telegram\SourceFiles\intro\intro_phone.cpp' 'MTPauth_SignIn('
Assert-Contains 'Telegram\SourceFiles\intro\intro_phone.cpp' 'MTP_string(password)'
Assert-Contains 'Telegram\SourceFiles\intro\intro_phone.cpp' 'PASSWORD_HASH_INVALID'
Assert-Contains 'Telegram\SourceFiles\intro\intro_phone.cpp' '_sentPhone = phoneDigits;'
Assert-Contains 'Telegram\SourceFiles\intro\intro_phone.cpp' 'getData()->phone = _sentPhone;'
Assert-Contains 'Telegram\SourceFiles\intro\intro_phone.cpp' 'const auto phoneHash = Tz::ConsumeLoginChallenge(getData()->phoneHash);'
Assert-Contains 'Telegram\SourceFiles\intro\intro_phone.cpp' 'refreshChallengeAndSubmit(password);'
Assert-NotContains 'Telegram\SourceFiles\intro\intro_phone.cpp' 'MTPauth_ResendCode'
Assert-NotContains 'Telegram\SourceFiles\intro\intro_phone.cpp' 'MTPauth_CheckPassword'
Assert-Contains 'Telegram\SourceFiles\tz\tz_client_contract.h' 'return !firstName.trimmed().isEmpty();'
Assert-Contains 'Telegram\SourceFiles\tz\tz_client_contract.h' 'kLoginPasswordMinimumCodePoints = 8'
Assert-Contains 'Telegram\SourceFiles\tz\tz_login_contract.h' 'NormalizeLoginPhone(QStringView phone)'
Assert-Contains 'Telegram\SourceFiles\tz\tz_login_contract.h' 'ConsumeLoginChallenge(QByteArray &challenge)'
Assert-Contains 'Telegram\SourceFiles\tz\tz_client_contract.h' ('u"' + $identityOrPasswordError + '"')
Assert-Contains 'Telegram\SourceFiles\tz\tz_client_contract.h' 'u"tz://join?invite="'
Assert-Contains 'Telegram\SourceFiles\tz\tz_client_contract.h' 'u"https://tg.tianze8.cc/+"'
Assert-Contains 'Telegram\SourceFiles\tz\tz_client_contract.h' 'kDefaultLanguageId = u"zh-hans"'
Assert-Contains 'Telegram\SourceFiles\tz\tz_client_contract.h' 'kCompatibleLegacyStorageVersion = 6008004'
Assert-Contains 'Telegram\SourceFiles\lang\lang_keys.cpp' 'QStringView(Tz::kDefaultLanguageId).toString()'
Assert-Contains 'Telegram\SourceFiles\lang\lang_instance.cpp' ': _id(DefaultLanguageId())'
Assert-Contains 'Telegram\SourceFiles\lang\lang_instance.cpp' '_pluralId(DefaultLanguageId())'
Assert-Contains 'Telegram\SourceFiles\lang\lang_cloud_manager.cpp' '_langpack.version(Pack::Current) == 0'
Assert-Contains 'Telegram\SourceFiles\storage\localstorage.cpp' 'Lang::GetInstance().fillFromSerialized(data, langpack.version)'
Assert-Contains 'Telegram\SourceFiles\lang\lang_instance.cpp' 'Tz::VisibleBrandText(GetInstance().getNonDefaultValue(key))'
Assert-Contains 'Telegram\SourceFiles\lang\lang_instance.cpp' 'Tz::VisibleBrandText(GetInstance().getValue(key))'
Assert-Contains 'Telegram\SourceFiles\tz\tz_client_contract.h' 'kInternalPublicUrl = u"https://tg.tianze8.cc/"'
Assert-Contains 'Telegram\SourceFiles\tz\tz_client_contract.h' 'VisibleBrandText(QString text)'

foreach ($langPath in @(
    'Telegram\Resources\langs\lang.strings',
    'Telegram\Resources\langs\cloud_lang.strings'
)) {
    $visibleValues = (Read-RepositoryFile $langPath) -split "`r?`n" |
        Where-Object { $_ -match '^\s*"[^"]+"\s*=\s*"(.*)";\s*$' } |
        ForEach-Object { $Matches[1] }
    if (($visibleValues -join "`n") -match '(?i)telegram') {
        $nonPlaceholderValues = $visibleValues -replace '(?i)\{telegram\}', ''
        if (($nonPlaceholderValues -join "`n") -match '(?i)telegram') {
            throw "$langPath contains a visible Telegram brand value"
        }
    }
    if (($visibleValues -join "`n").Contains("{$companyName}")) {
        throw "$langPath contains a translated placeholder name"
    }
}

Assert-Contains 'Telegram\SourceFiles\intro\intro_start.cpp' 'QStringView(Tz::kCompanyName).toString()'
Assert-Contains 'Telegram\SourceFiles\boxes\about_box.cpp' 'QStringView(Tz::kCompanyName).toString()'
Assert-Contains 'Telegram\SourceFiles\settings\sections\settings_notifications.cpp' 'QStringView(Tz::kCompanyName).toString()'
Assert-Contains 'Telegram\SourceFiles\core\crash_report_window.cpp' 'Tz::kInternalPublicUrl'
Assert-Contains 'Telegram\SourceFiles\core\mime_type.cpp' 'Tz::kCompanyName'
Assert-Contains 'Telegram\SourceFiles\export\output\export_output_html.cpp' 'Tz::VisibleBrandUtf8'
Assert-Contains 'Telegram\SourceFiles\export\output\export_output_json.cpp' 'Tz::VisibleBrandUtf8'
Assert-Contains 'Telegram\SourceFiles\mtproto\mtproto_config.h' 'https://tg.tianze8.cc/'

Assert-Contains 'Telegram\SourceFiles\core\version.h' 'AppName = "TZ"_cs'
Assert-Contains 'Telegram\SourceFiles\core\version.h' 'AppFile = "TZ"_cs'
Assert-Contains 'Telegram\build\version' 'AppVersion         1000002'
Assert-Contains 'Telegram\build\version' 'AppVersionStr      1.0.2'
Assert-Contains 'Telegram\SourceFiles\core\version.h' 'AppVersion = 1000002'
Assert-Contains 'Telegram\SourceFiles\core\version.h' 'AppVersionStr = "1.0.2"'
Assert-Contains 'Telegram\SourceFiles\tz\tz_client_contract.h' 'kVersion = u"1.0.2"'
Assert-Contains 'Telegram\Resources\winrc\Telegram.rc' 'FILEVERSION 1,0,2,0'
Assert-Contains 'Telegram\Resources\winrc\Telegram.rc' 'VALUE "FileVersion", "1.0.2.0"'
Assert-Contains 'Telegram\Resources\winrc\Updater.rc' 'FILEVERSION 1,0,2,0'
Assert-Contains 'Telegram\Resources\winrc\Updater.rc' 'VALUE "FileVersion", "1.0.2.0"'
Assert-Contains 'Telegram\Resources\uwp\AppX\AppxManifest.xml' 'Version="1.0.2.0"'
Assert-Contains '.github\workflows\tz-windows-release.yml' "TZ_VERSION: '1.0.2'"
Assert-Contains '.github\workflows\tz-windows-release.yml' 'name: TZ-1.0.2-windows-x64-build'
Assert-Contains '.github\workflows\tz-windows-release.yml' 'artifacts/release/TZ-1.0.2-Setup.exe'
Assert-Contains '.github\workflows\tz-windows-release.yml' 'artifacts/release/TZ-1.0.2-windows-x64.zip'
Assert-Contains '.github\workflows\tz-windows-release.yml' 'artifacts/release/TZ-1.0.2-symbols.zip'
Assert-Contains '.github\workflows\tz-windows-release.yml' '"TZ-$env:TZ_VERSION-Setup.exe",'
Assert-Contains '.github\workflows\tz-windows-release.yml' '"TZ-$env:TZ_VERSION-windows-x64.zip",'
Assert-Contains '.github\workflows\tz-windows-release.yml' '"TZ-$env:TZ_VERSION-symbols.zip"'
Assert-Contains '.github\workflows\tz-windows-release.yml' "(Join-Path `$artifacts 'SHA256SUMS.txt')"
Assert-NotContains '.github\workflows\tz-windows-release.yml' '1.0.1'
Assert-Contains 'Telegram\cmake\tz_client_tests.cmake' 'group_creation_contract_tests'
Assert-Contains 'Telegram\cmake\tz_client_tests.cmake' 'send_as_policy_contract_tests'
Assert-Contains 'Telegram\cmake\tz_client_tests.cmake' 'tz_internal_url_contract_tests'
Assert-Contains '.github\workflows\tz-windows-release.yml' '--target tz_contract_tests'
Assert-Contains '.github\workflows\tz-windows-release.yml' "'tz_internal_url_contract_tests'"
Assert-Contains '.github\workflows\tz-windows-release.yml' "local-name()='MultiProcessorCompilation'"
Assert-Contains '.github\workflows\tz-windows-release.yml' "`$node.InnerText = 'false'"
Assert-Contains '.github\workflows\tz-windows-release.yml' 'cmake --build "%BUILD_ROOT%\tdesktop\out" --config Release --target tz_contract_tests -- /m:1 /nr:false /p:BuildInParallel=false /p:CL_MPCount=1 /p:UseMultiToolTask=true /p:EnforceProcessCountAcrossBuilds=true'
Assert-Contains '.github\workflows\tz-windows-release.yml' 'MSBuild.exe "%BUILD_ROOT%\tdesktop\out\Telegram.sln" /m:1 /t:Telegram /p:Configuration=Release /p:Platform=x64 /nr:false /p:BuildInParallel=false /p:CL_MPCount=1 /p:UseMultiToolTask=true /p:EnforceProcessCountAcrossBuilds=true'
Assert-NotContains '.github\workflows\tz-windows-release.yml' '/MP1'
Assert-Contains 'Telegram\build\prepare\prepare.py' "modifiedEnv['PREPARE_SCRIPT_DIR'] = scriptPath"
Assert-Contains 'Telegram\build\prepare\prepare.py' 'python "%PREPARE_SCRIPT_DIR%\\ensure_qt_plugin_dirs.py" . qtimageformats qtsvg'

& python (Join-Path $RepositoryRoot 'Telegram\build\prepare\test_ensure_qt_plugin_dirs.py')
if ($LASTEXITCODE -ne 0) {
    throw "Qt plugin directory guard tests failed with exit code $LASTEXITCODE."
}
Assert-Contains 'Telegram\CMakeLists.txt' '${src_loc}/window/window_peer_menu.cpp'
Assert-Contains 'Telegram\CMakeLists.txt' '${src_loc}/window/window_session_controller.cpp'
Assert-Contains 'Telegram\CMakeLists.txt' '$<$<CONFIG:Release>:/Ob1>'
Assert-NotContains 'Telegram\CMakeLists.txt' '/MP1'
Assert-Contains 'Telegram\Resources\winrc\Telegram.rc' ('VALUE "CompanyName", "' + $companyName + '"')
Assert-Contains 'Telegram\Resources\winrc\Telegram.rc' 'VALUE "ProductName", "TZ"'
Assert-Contains 'Telegram\Resources\winrc\Updater.rc' 'VALUE "FileDescription", "TZ Updater"'
Assert-Contains 'Telegram\build\setup.iss' '#define MyAppExeName "TZ.exe"'
Assert-Contains 'Telegram\build\setup.iss' ('#define MyAppPublisher "' + $companyName + '"')
Assert-Contains 'Telegram\SourceFiles\platform\win\windows_app_user_model_id.cpp' 'L"tianze.TZ"'
Assert-Contains 'Telegram\SourceFiles\window\main_window.cpp' 'AppName.utf16()'
Assert-Contains 'Telegram\SourceFiles\core\application.cpp' '.protocol = u"tz"_q'
Assert-NotContains 'Telegram\SourceFiles\core\application.cpp' '.protocol = u"tg"_q'
Assert-Contains 'Telegram\SourceFiles\platform\win\specific_win.cpp' 'Core::Application::UnregisterUrlScheme()'
Assert-Contains 'Telegram\SourceFiles\core\local_url_handlers.cpp' 'tg://join?invite='
Assert-Contains 'Telegram\SourceFiles\dialogs\dialogs_widget.cpp' 'application/x-telegram-dialog'
Assert-Contains 'Telegram\SourceFiles\payments\ui\payments_panel.cpp' 'TelegramWebviewProxy'
Assert-Contains 'Telegram\SourceFiles\lang\lang_instance.cpp' '"tdesktop"_cs'
Assert-Contains 'Telegram\SourceFiles\core\crash_report_window.cpp' '.telegramcrash'

Assert-Contains 'Telegram\SourceFiles\mtproto\mtproto_dc_options.cpp' '{ 2, "47.79.233.204", 2398 }'
Assert-NotContains 'Telegram\SourceFiles\mtproto\mtproto_dc_options.cpp' '149.154.'
Assert-NotContains 'Telegram\SourceFiles\mtproto\mtproto_dc_options.cpp' '95.161.76.100'

Write-Output 'TZ client focused source contract: PASS'
