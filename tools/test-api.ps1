<#
.SYNOPSIS
    Suite de Pruebas de Integración de API para DUXMAN-LED (PowerShell)
.DESCRIPTION
    Ejecuta pruebas automatizadas de extremo a extremo contra la API REST v1 del firmware.
    Emite marcas de depuración en la consola Serial del ESP32 vía /api/v1/marcalog.
.PARAMETER BaseUrl
    URL base del dispositivo (por defecto: http://192.168.4.1)
.PARAMETER VerboseOutput
    Muestra los payloads JSON y respuestas completas
.EXAMPLE
    .\tools\test-api.ps1 -BaseUrl "http://192.168.4.1"
    .\tools\test-api.ps1 -BaseUrl "http://192.168.1.150" -VerboseOutput
#>

param (
    [string]$BaseUrl = "http://192.168.4.1",
    [switch]$VerboseOutput = $false,
    [switch]$NonInteractive = $false,
    [string]$LogDir = "log",
    [int]$DelayMs = 150
)

$BaseUrl = $BaseUrl.TrimEnd('/')
$passed = 0
$failed = 0

if (-not (Test-Path $LogDir)) {
    New-Item -ItemType Directory -Path $LogDir -Force | Out-Null
}

$timestamp = Get-Date -Format "yyyyMMdd-HHmmss"
$logFilePath = Join-Path $LogDir "test-api-ps-$timestamp.log"
$latestLogPath = Join-Path $LogDir "test-api-latest.log"

function Write-TestLog {
    param([string]$text)
    Add-Content -Path $logFilePath -Value $text -Encoding UTF8
}

Write-TestLog "=== DUXMAN LED API TEST SUITE LOG (PowerShell) ==="
Write-TestLog "Fecha: $(Get-Date -Format s)"
Write-TestLog "Dispositivo: $BaseUrl"
Write-TestLog "Modo no interactivo: $NonInteractive"
Write-TestLog ("=" * 60)`n

function Send-MarcaLog {
    param([string]$tag, [string]$msg)
    try {
        $body = @{ tag = $tag; message = $msg } | ConvertTo-Json -Compress
        Invoke-RestMethod -Uri "$BaseUrl/api/v1/marcalog" -Method Post -Body $body -ContentType "application/json" -TimeoutSec 2 | Out-Null
    } catch {}
}

function Wait-UserAction {
    param([string]$prompt, [string]$tag = "USER_ACTION")
    Write-Host "`n┌──────────────────────────────────────────────────────────────────┐" -ForegroundColor Magenta
    Write-Host "│ ACCIÓN REQUERIDA DEL USUARIO                                     │" -ForegroundColor Magenta
    Write-Host "└──────────────────────────────────────────────────────────────────┘" -ForegroundColor Magenta
    Write-Host "👉 $prompt" -ForegroundColor Yellow

    Send-MarcaLog $tag "ESPERANDO USUARIO: $prompt"

    if ($NonInteractive) {
        Write-Host "(Modo no interactivo: continuando automáticamente en 3s...)" -ForegroundColor Cyan
        Start-Sleep -Seconds 3
        return
    }

    Write-Host "Pulsa [ENTER] una vez hayas realizado la acción para verificarla..." -ForegroundColor White -NoNewline
    [void][System.Console]::ReadLine()
    Send-MarcaLog $tag "ACCION COMPLETADA POR USUARIO: $prompt"
    Write-TestLog "-> [USUARIO ACCION COMPLETADA]: $prompt"
}

function Invoke-ApiTest {
    param(
        [string]$Name,
        [string]$Method,
        [string]$Path,
        [int]$ExpectedStatus = 200,
        [object]$Body = $null,
        [scriptblock]$Validator = $null
    )

    Start-Sleep -Milliseconds $DelayMs
    $sw = [System.Diagnostics.Stopwatch]::StartNew()
    $status = 0
    $resp = $null
    $success = $false
    $reason = ""

    try {
        $params = @{
            Uri = "$BaseUrl$Path"
            Method = $Method
            TimeoutSec = 5
        }
        if ($Body) {
            $params["Body"] = ($Body | ConvertTo-Json -Compress -Depth 10)
            $params["ContentType"] = "application/json"
        }

        $resp = Invoke-RestMethod @params
        $status = 200
        $success = ($ExpectedStatus -eq 200)
    } catch [System.Net.WebException] {
        if ($_.Response) {
            $status = [int]$_.Response.StatusCode
        }
        if ($status -eq $ExpectedStatus) {
            $success = $true
        } else {
            $reason = "HTTP $status != $ExpectedStatus"
        }
    } catch {
        $reason = $_.Exception.Message
    }
    $sw.Stop()

    if ($success -and $Validator -and $resp) {
        $vResult = & $Validator $resp
        if (-not $vResult) {
            $success = $false
            $reason = "Validacion fallida de estructura"
        }
    }

    if ($success) {
        Write-Host "  [PASS] $Name ($Method $Path) - $($sw.ElapsedMilliseconds)ms" -ForegroundColor Green
        Write-TestLog "  [PASS] $Name ($Method $Path) - status=$status duration=$($sw.ElapsedMilliseconds)ms"
        $script:passed++
    } else {
        Write-Host "  [FAIL] $Name ($Method $Path) - $reason" -ForegroundColor Red
        Write-TestLog "  [FAIL] $Name ($Method $Path) - status=$status duration=$($sw.ElapsedMilliseconds)ms - reason=$reason"
        $script:failed++
    }
}

Write-Host "==================================================================" -ForegroundColor Cyan
Write-Host " DUXMAN LED Firmware - Suite de Pruebas de API (PowerShell)" -ForegroundColor Cyan
Write-Host " Destino: $BaseUrl" -ForegroundColor Cyan
Write-Host "=================================================================="

Send-MarcaLog "TEST_POWERSHELL" "Iniciando suite de verificacion por PowerShell"

Write-Host "`n▶ 00 - Descubrimiento y Metadatos" -ForegroundColor Yellow
Invoke-ApiTest "OpenAPI Schema" "Get" "/api/v1/openapi.json"
Invoke-ApiTest "Hardware Info" "Get" "/api/v1/hardware"
Invoke-ApiTest "Release Info" "Get" "/api/v1/release"
Invoke-ApiTest "Catalogo Efectos" "Get" "/api/v1/effects"
Invoke-ApiTest "Catalogo Paletas" "Get" "/api/v1/palettes"

Write-Host "`n▶ 01 - Diagnostico y Marcas de Depuracion" -ForegroundColor Yellow
Invoke-ApiTest "Diagnostico (/diag)" "Get" "/api/v1/diag"
Invoke-ApiTest "Metricas de Render" "Get" "/api/v1/metrics"
Invoke-ApiTest "Marca de Log (POST JSON)" "Post" "/api/v1/marcalog" -Body @{ tag = "PS_TEST"; message = "Probando log desde PowerShell" }
Invoke-ApiTest "Marca de Log (GET)" "Get" "/api/v1/marcalog?msg=Probando_Query_PS"

Write-Host "`n▶ 02 - Control de Estado y Efectos" -ForegroundColor Yellow
Invoke-ApiTest "Lectura de Estado" "Get" "/api/v1/state"
Invoke-ApiTest "Modificar Estado (PATCH)" "Patch" "/api/v1/state" -Body @{ power = $true; brightness = 140; effect = "lava_flow"; effectSpeed = 60 }
Invoke-ApiTest "Aplicar Paleta" "Post" "/api/v1/palettes/apply" -Body @{ palette = "sunset_drive" }

Write-Host "`n▶ 03 - Configuracion Modular" -ForegroundColor Yellow
Invoke-ApiTest "Config Red" "Get" "/api/v1/config/network"
Invoke-ApiTest "Config GPIO" "Get" "/api/v1/config/gpio"
Invoke-ApiTest "Config Microfono" "Get" "/api/v1/config/microphone"
Invoke-ApiTest "Config General" "Get" "/api/v1/config/general"
Invoke-ApiTest "Config Sincronizacion" "Get" "/api/v1/sync/config"

Write-Host "`n▶ 04 - Pruebas Interactivas (Web UI y LedFx)" -ForegroundColor Yellow
Wait-UserAction "Abre la Web UI ($BaseUrl) y cambia el color o brillo manualmente desde el panel." "INTERACTIVE_WEB_UI"
Invoke-ApiTest "Verificar Cambio en Web UI" "Get" "/api/v1/state"

Invoke-ApiTest "Activar Receptor LedFx (DDP)" "Patch" "/api/v1/sync/config" -Body @{
    sync = @{
        enabled = $true
        mode = "ledfx_realtime"
        role = "client"
        inputProtocol = "ddp"
        ddpPort = 4048
    }
}
$cleanIp = $BaseUrl.Replace('http://', '').Replace('https://', '').Split(':')[0]
Wait-UserAction "Inicia o conecta LedFx apuntando a la IP $cleanIp por DDP (puerto 4048)." "INTERACTIVE_LEDFX"
Invoke-ApiTest "Verificar Telemetría LedFx" "Get" "/api/v1/sync/state"
Invoke-ApiTest "Desactivar Receptor LedFx" "Patch" "/api/v1/sync/mode" -Body @{ mode = "off" }

Write-Host "`n▶ 05 - Manejo de Errores" -ForegroundColor Yellow
Invoke-ApiTest "Parametro Invalido (Brillo 999 -> 400)" "Patch" "/api/v1/state" -ExpectedStatus 400 -Body @{ brightness = 999 }

$total = $passed + $failed
$rate = if ($total -gt 0) { [math]::Round(($passed / $total) * 100, 1) } else { 0 }

Send-MarcaLog "TEST_COMPLETE" "Suite finalizada: $passed/$total superadas ($rate%)"

$summaryLine1 = "=================================================================="
$summaryLine2 = " RESULTADOS: $passed/$total pruebas superadas ($rate%)"
$summaryLine3 = "=================================================================="

Write-Host "`n$summaryLine1" -ForegroundColor Cyan
Write-Host $summaryLine2 -ForegroundColor $(if ($failed -eq 0) { "Green" } else { "Red" })
Write-Host $summaryLine3 -ForegroundColor Cyan

Write-TestLog "`n$summaryLine1"
Write-TestLog $summaryLine2
Write-TestLog $summaryLine3

try {
    Copy-Item -Path $logFilePath -Destination $latestLogPath -Force
    Write-Host "📁 Log guardado en: $logFilePath" -ForegroundColor Cyan
    Write-Host "📁 Acceso rápido:   $latestLogPath`n" -ForegroundColor Cyan
} catch {}
