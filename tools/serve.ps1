# BOMBANANA IRL - local game server
# Serves sim/ over HTTP so other devices on the same Wi-Fi can play.
# Run it with serve.bat (double-click) - no install needed.

$ErrorActionPreference = 'Stop'
$root = Split-Path -Parent $PSScriptRoot   # tools\ -> repo root
$port = 8080

function Find-Python {
    $candidates = @(
        (Join-Path $env:USERPROFILE '.platformio\penv\Scripts\python.exe'),
        'py',
        'python'
    )
    foreach ($c in $candidates) {
        if ($c -ne 'py' -and $c -ne 'python' -and -not (Test-Path $c)) { continue }
        try {
            $out = & $c -c "import sys; print(sys.version_info[0])" 2>$null
            if ($LASTEXITCODE -eq 0 -and "$out".Trim() -eq '3') { return $c }
        } catch { }
    }
    return $null
}

function Get-LanIp {
    try {
        $cfg = Get-NetIPConfiguration | Where-Object {
            $_.IPv4DefaultGateway -ne $null -and
            $_.NetAdapter.Status -eq 'Up' -and
            $_.IPv4Address.IPAddress -notlike '169.254.*' -and
            $_.IPv4Address.IPAddress -ne '127.0.0.1'
        } | Select-Object -First 1
        if ($cfg) { return $cfg.IPv4Address.IPAddress }
    } catch { }
    return $null
}

$py = Find-Python
if (-not $py) {
    Write-Host ''
    Write-Host '  Python 3 was not found.' -ForegroundColor Red
    Write-Host '  Install Python 3 (python.org) or open sim\index.html directly.'
    Write-Host ''
    exit 1
}

$ip  = Get-LanIp
$url = 'http://localhost:{0}/' -f $port

Write-Host ''
Write-Host '  BOMBANANA IRL - local game server' -ForegroundColor Green
Write-Host '  ---------------------------------'
Write-Host ('  This PC   : {0}' -f $url) -ForegroundColor Cyan
if ($ip) {
    Write-Host ('  Wi-Fi     : http://{0}:{1}/' -f $ip, $port) -ForegroundColor Cyan
    Write-Host '              (open this on a phone connected to the same Wi-Fi)'
} else {
    Write-Host '  Wi-Fi     : no LAN address found - are you connected?' -ForegroundColor Yellow
}
Write-Host ('  Folder    : {0}\sim' -f $root)
Write-Host '  Stop      : Ctrl+C'
Write-Host ''
Write-Host '  If Windows asks about the firewall, allow it on Private networks.'
Write-Host ''

# Open the browser once the server is up (2 s delay, separate process).
if ($env:BOMBANANA_NO_BROWSER -ne '1') {
    Start-Job -ScriptBlock {
        param($u)
        Start-Sleep -Seconds 2
        Start-Process $u
    } -ArgumentList $url | Out-Null
}

& $py -m http.server $port --bind 0.0.0.0 --directory (Join-Path $root 'sim')
