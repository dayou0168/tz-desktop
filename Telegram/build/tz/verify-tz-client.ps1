param(
    [string]$RepositoryRoot = (Resolve-Path "$PSScriptRoot\..\..\..").Path
)

$ErrorActionPreference = 'Stop'
$companyName = -join [char[]](0x5929, 0x6CFD, 0x96C6, 0x56E2)

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
Assert-Contains 'Telegram\SourceFiles\tz\tz_client_contract.h' 'return !firstName.trimmed().isEmpty();'
Assert-Contains 'Telegram\SourceFiles\tz\tz_client_contract.h' 'kDefaultLanguageId = u"zh-hans"'
Assert-Contains 'Telegram\SourceFiles\tz\tz_client_contract.h' 'kCompatibleLegacyStorageVersion = 6008004'
Assert-Contains 'Telegram\SourceFiles\lang\lang_keys.cpp' 'QStringView(Tz::kDefaultLanguageId).toString()'
Assert-Contains 'Telegram\SourceFiles\lang\lang_instance.cpp' ': _id(DefaultLanguageId())'
Assert-Contains 'Telegram\SourceFiles\lang\lang_instance.cpp' '_pluralId(DefaultLanguageId())'
Assert-Contains 'Telegram\SourceFiles\lang\lang_cloud_manager.cpp' '_langpack.version(Pack::Current) == 0'
Assert-Contains 'Telegram\SourceFiles\storage\localstorage.cpp' 'Lang::GetInstance().fillFromSerialized(data, langpack.version)'

Assert-Contains 'Telegram\SourceFiles\core\version.h' 'AppName = "TZ"_cs'
Assert-Contains 'Telegram\SourceFiles\core\version.h' 'AppFile = "TZ"_cs'
Assert-Contains 'Telegram\SourceFiles\core\version.h' 'AppVersionStr = "1.0.1"'
Assert-Contains 'Telegram\Resources\winrc\Telegram.rc' ('VALUE "CompanyName", "' + $companyName + '"')
Assert-Contains 'Telegram\Resources\winrc\Telegram.rc' 'VALUE "ProductName", "TZ"'
Assert-Contains 'Telegram\Resources\winrc\Updater.rc' 'VALUE "FileDescription", "TZ Updater"'
Assert-Contains 'Telegram\build\setup.iss' '#define MyAppExeName "TZ.exe"'
Assert-Contains 'Telegram\build\setup.iss' ('#define MyAppPublisher "' + $companyName + '"')
Assert-Contains 'Telegram\SourceFiles\platform\win\windows_app_user_model_id.cpp' 'L"tianze.TZ"'
Assert-Contains 'Telegram\SourceFiles\window\main_window.cpp' 'AppName.utf16()'

Assert-Contains 'Telegram\SourceFiles\mtproto\mtproto_dc_options.cpp' '{ 2, "47.79.233.204", 2398 }'
Assert-NotContains 'Telegram\SourceFiles\mtproto\mtproto_dc_options.cpp' '149.154.'
Assert-NotContains 'Telegram\SourceFiles\mtproto\mtproto_dc_options.cpp' '95.161.76.100'

Write-Output 'TZ client focused source contract: PASS'
