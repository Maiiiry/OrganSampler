# Collects everything a bug report needs into one zip on the Desktop:
#   %APPDATA%\OrganSampler\CrashLogs  (activity.log, crash reports)
#   settings.json, selftest.txt        (the current settings, the last self-test)
#   system.txt                         (Windows version, CPU, RAM, sound devices, OrganSampler versions found)
# No sample data, no voicing, no combinations. Drag the zip into the "Logs"
# field of the bug report at https://github.com/zurek-jiri/OrganSampler/issues/new/choose
#
# Usage: right-click > Run with PowerShell, or:  powershell -ExecutionPolicy Bypass -File collect_logs.ps1
param([string]$Output = "")

$ErrorActionPreference = "Continue"
$appData = Join-Path $env:APPDATA "OrganSampler"
if (-not (Test-Path $appData)) { Write-Host "No OrganSampler data folder at $appData - has the program been started on this computer?"; exit 1 }
$stamp = Get-Date -Format "yyyy-MM-dd_HH-mm"
if ($Output -eq "") { $Output = Join-Path ([Environment]::GetFolderPath("Desktop")) "OrganSampler-logs-$stamp.zip" }
$work = Join-Path $env:TEMP "OrganSampler-logs-$stamp"
if (Test-Path $work) { Remove-Item $work -Recurse -Force }
New-Item -ItemType Directory -Path $work | Out-Null

foreach ($name in "CrashLogs") {
    $src = Join-Path $appData $name
    if (Test-Path $src) { Copy-Item $src (Join-Path $work $name) -Recurse -Force }
}
foreach ($name in "settings.json", "selftest.txt") {
    $src = Join-Path $appData $name
    if (Test-Path $src) { Copy-Item $src $work -Force }
}

# The computer, for the report (nothing personal: no user names, no serial numbers).
$lines = @()
$lines += "collected: $(Get-Date -Format s)"
try { $os = Get-CimInstance Win32_OperatingSystem; $lines += "os: $($os.Caption) $($os.Version) ($($os.OSArchitecture))" } catch {}
try { $cpu = Get-CimInstance Win32_Processor | Select-Object -First 1; $lines += "cpu: $($cpu.Name.Trim()), $($cpu.NumberOfCores) cores / $($cpu.NumberOfLogicalProcessors) threads" } catch {}
try { $ram = [math]::Round((Get-CimInstance Win32_ComputerSystem).TotalPhysicalMemory / 1GB, 1); $lines += "ram: $ram GB" } catch {}
try { $lines += "display scaling: " + ((Get-ItemProperty "HKCU:\Control Panel\Desktop\WindowMetrics" -ErrorAction SilentlyContinue).AppliedDPI / 96 * 100) + " %" } catch {}
try { $lines += "sound devices:"; Get-CimInstance Win32_SoundDevice | ForEach-Object { $lines += "  " + $_.Name + " (" + $_.Status + ")" } } catch {}
try {
    $lines += "audio endpoints (render):"
    Get-CimInstance -Namespace root\cimv2 -ClassName Win32_PnPEntity -Filter "PNPClass='AudioEndpoint'" | ForEach-Object { $lines += "  " + $_.Name }
} catch {}
try {
    $lines += "OrganSampler executables found:"
    foreach ($dir in @("C:\OrganSampler", [Environment]::GetFolderPath("Desktop"), [Environment]::GetFolderPath("UserProfile") + "\Downloads")) {
        Get-ChildItem $dir -Filter "OrganSampler*.exe" -ErrorAction SilentlyContinue | ForEach-Object {
            $v = $_.VersionInfo.ProductVersion; $lines += "  $($_.FullName)  version $v  ($($_.LastWriteTime))"
        }
    }
} catch {}
$lines | Set-Content (Join-Path $work "system.txt") -Encoding utf8

if (Test-Path $Output) { Remove-Item $Output -Force }
Compress-Archive -Path (Join-Path $work "*") -DestinationPath $Output -CompressionLevel Optimal
Remove-Item $work -Recurse -Force
$size = [math]::Round((Get-Item $Output).Length / 1KB)
Write-Host ""
Write-Host "Log bundle written: $Output ($size KB)"
Write-Host "Attach it to the bug report: https://github.com/zurek-jiri/OrganSampler/issues/new/choose"
Write-Host ""
try { Start-Process explorer.exe "/select,`"$Output`"" } catch {}
