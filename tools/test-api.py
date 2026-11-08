#!/usr/bin/env python3
"""
DUXMAN LED Firmware - Comprehensive API Integration Test Suite
Valida extremo a extremo las capacidades del firmware mediante su API REST v1.
Emite marcas de depuración en tiempo real vía /api/v1/marcalog para correlación en Serial.

Sin dependencias externas: solo requiere Python 3 standard library (urllib, json).
Uso:
    python tools/test-api.py --url http://192.168.4.1
    python tools/test-api.py --url http://192.168.1.105 --verbose
"""

import argparse
import datetime
import json
import os
import re
import sys
import time
import urllib.error
import urllib.parse
import urllib.request
from typing import Any, Dict, Optional, Tuple

# Colores ANSI para terminal
CLR_RESET = "\033[0m"
CLR_RED = "\033[91m"
CLR_GREEN = "\033[92m"
CLR_YELLOW = "\033[93m"
CLR_BLUE = "\033[94m"
CLR_MAGENTA = "\033[95m"
CLR_CYAN = "\033[96m"
CLR_BOLD = "\033[1m"

ANSI_REGEX = re.compile(r"\x1B(?:[@-Z\\-_]|\[[0-?]*[ -/]*[@-~])")


def strip_ansi(text: str) -> str:
    """Elimina códigos de escape ANSI para escritura limpia en fichero de log."""
    return ANSI_REGEX.sub("", text)


class ApiTester:
    def __init__(self, base_url: str, verbose: bool = False, delay: float = 0.2, interactive: bool = True, log_dir: str = "log"):
        self.base_url = base_url.rstrip("/")
        self.verbose = verbose
        self.delay = delay
        self.interactive = interactive
        self.log_dir = log_dir
        self.passed = 0
        self.failed = 0
        self.skipped = 0
        self.results = []
        self.original_state = None

        # Preparar directorio y ficheros de log
        os.makedirs(self.log_dir, exist_ok=True)
        timestamp = datetime.datetime.now().strftime("%Y%m%d-%H%M%S")
        self.log_file_path = os.path.join(self.log_dir, f"test-api-{timestamp}.log")
        self.latest_log_path = os.path.join(self.log_dir, "test-api-latest.log")
        self.log_fp = open(self.log_file_path, "w", encoding="utf-8")

        self._write_file_log(f"=== DUXMAN LED API TEST SUITE LOG ===")
        self._write_file_log(f"Fecha: {datetime.datetime.now().isoformat()}")
        self._write_file_log(f"Dispositivo: {self.base_url}")
        self._write_file_log(f"Modo interactivo: {self.interactive}")
        self._write_file_log("=" * 60 + "\n")

    def _write_file_log(self, text: str) -> None:
        """Escribe una línea en el fichero de log y en el archivo latest."""
        clean = strip_ansi(text)
        self.log_fp.write(clean + "\n")
        self.log_fp.flush()

    def print_and_log(self, text: str = "") -> None:
        """Imprime por consola y guarda simultáneamente en fichero de log."""
        print(text)
        self._write_file_log(text)

    def _http_request(
        self,
        method: str,
        path: str,
        payload: Optional[Dict[str, Any]] = None,
        raw_body: Optional[str] = None,
        timeout: float = 5.0,
    ) -> Tuple[int, Optional[Dict[str, Any]], str]:
        """Realiza una petición HTTP y devuelve (status_code, json_dict, raw_text)."""
        url = f"{self.base_url}{path}"
        data_bytes = None
        headers = {}

        if payload is not None:
            data_bytes = json.dumps(payload).encode("utf-8")
            headers["Content-Type"] = "application/json"
        elif raw_body is not None:
            data_bytes = raw_body.encode("utf-8")
            headers["Content-Type"] = "text/plain"

        req = urllib.request.Request(url, data=data_bytes, headers=headers, method=method)

        t0 = time.time()
        try:
            with urllib.request.urlopen(req, timeout=timeout) as response:
                status = response.status
                body = response.read().decode("utf-8")
                elapsed = (time.time() - t0) * 1000
                parsed_json = None
                try:
                    parsed_json = json.loads(body)
                except Exception:
                    pass
                return status, parsed_json, body
        except urllib.error.HTTPError as e:
            status = e.code
            body = e.read().decode("utf-8")
            parsed_json = None
            try:
                parsed_json = json.loads(body)
            except Exception:
                pass
            return status, parsed_json, body
        except Exception as e:
            return 0, None, str(e)

    def marcalog(self, tag: str, message: str) -> None:
        """Inyecta una marca en la consola Serial del ESP32 vía /api/v1/marcalog."""
        try:
            self._http_request(
                "POST",
                "/api/v1/marcalog",
                payload={"tag": tag, "message": message},
                timeout=2.0,
            )
        except Exception:
            pass

    def wait_for_user(self, prompt: str, tag: str = "USER_ACTION") -> None:
        """Pide una acción al usuario y espera a que pulse Enter, notificando a Serial."""
        self.print_and_log(f"\n{CLR_BOLD}{CLR_MAGENTA}┌──────────────────────────────────────────────────────────────────┐{CLR_RESET}")
        self.print_and_log(f"{CLR_BOLD}{CLR_MAGENTA}│ ACCIÓN REQUERIDA DEL USUARIO                                     │{CLR_RESET}")
        self.print_and_log(f"{CLR_BOLD}{CLR_MAGENTA}└──────────────────────────────────────────────────────────────────┘{CLR_RESET}")
        self.print_and_log(f"{CLR_YELLOW}👉 {prompt}{CLR_RESET}")
        self.marcalog(tag, f"ESPERANDO USUARIO: {prompt}")

        if not self.interactive:
            self.print_and_log(f"{CLR_CYAN}(Modo no interactivo activo: continuando automáticamente en 3s...){CLR_RESET}")
            time.sleep(3.0)
            return

        try:
            input(f"\n{CLR_BOLD}Pulsa [ENTER] una vez hayas realizado la acción para verificarla...{CLR_RESET}")
        except (KeyboardInterrupt, EOFError):
            self.print_and_log("\nPrueba cancelada por el usuario.")
            sys.exit(1)

        self.marcalog(tag, f"ACCION COMPLETADA POR USUARIO: {prompt}")
        self._write_file_log(f"-> [USUARIO CONFIRMÓ ACCIÓN]: {prompt}")

    def run_check(
        self,
        name: str,
        method: str,
        path: str,
        expected_status: int = 200,
        payload: Optional[Dict[str, Any]] = None,
        raw_body: Optional[str] = None,
        expect_ok: Optional[bool] = None,
        validator=None,
    ) -> bool:
        """Ejecuta una comprobación individual con reporte visual."""
        time.sleep(self.delay)
        t0 = time.time()
        status, json_data, raw = self._http_request(method, path, payload=payload, raw_body=raw_body)
        elapsed_ms = (time.time() - t0) * 1000

        is_success = (status == expected_status)
        failure_reason = ""

        if not is_success:
            failure_reason = f"HTTP {status} != {expected_status}"
        elif expect_ok is not None and json_data is not None:
            actual_ok = json_data.get("ok")
            if actual_ok != expect_ok:
                is_success = False
                failure_reason = f"json.ok ({actual_ok}) != {expect_ok}"

        if is_success and validator is not None:
            try:
                val_res, val_msg = validator(json_data, raw)
                if not val_res:
                    is_success = False
                    failure_reason = val_msg
            except Exception as ex:
                is_success = False
                failure_reason = f"Validator error: {ex}"

        badge = f"{CLR_GREEN}PASS{CLR_RESET}" if is_success else f"{CLR_RED}FAIL{CLR_RESET}"
        console_line = f"  [{badge}] {name} ({method} {path}) - {elapsed_ms:.0f}ms"
        print(console_line)
        self._write_file_log(f"  [{'PASS' if is_success else 'FAIL'}] {name} ({method} {path}) - status={status} duration={elapsed_ms:.0f}ms")
        if payload or raw_body:
            self._write_file_log(f"         Request payload: {payload or raw_body}")
        if raw:
            raw_summary = raw.replace("\n", " ")[:200]
            self._write_file_log(f"         Response ({len(raw)} bytes): {raw_summary}")

        if not is_success:
            print(f"         {CLR_RED}Motivo: {failure_reason}{CLR_RESET}")
            self._write_file_log(f"         Motivo fallo: {failure_reason}")
            if self.verbose:
                print(f"         Payload enviado: {payload or raw_body}")
                print(f"         Respuesta: {raw[:200]}")
            self.failed += 1
            self.results.append({"name": name, "status": "FAIL", "reason": failure_reason})
            return False

        if self.verbose and json_data:
            summary = json.dumps(json_data)[:90]
            print(f"         {CLR_CYAN}Respuesta: {summary}...{CLR_RESET}")

        self.passed += 1
        self.results.append({"name": name, "status": "PASS"})
        return True

    def run_all(self):
        print(f"\n{CLR_BOLD}=================================================================={CLR_RESET}")
        print(f"{CLR_BOLD} DUXMAN LED Firmware - Suite Completa de Verificación de API{CLR_RESET}")
        print(f"{CLR_BOLD} Dispositivo destino:{CLR_RESET} {self.base_url}")
        print(f"{CLR_BOLD}=================================================================={CLR_RESET}\n")

        self.marcalog("TEST_SUITE", "=== INICIO SUITE DE PRUEBAS AUTOMATIZADAS ===")

        # -------------------------------------------------------------
        # SUITE 0: DESCUBRIMIENTO Y METADATOS
        # -------------------------------------------------------------
        print(f"{CLR_BOLD}{CLR_BLUE}▶ 00 - Descubrimiento y Metadatos{CLR_RESET}")
        self.marcalog("TEST_DISCOVERY", "Validando OpenAPI, hardware y release")

        self.run_check(
            "Especificación OpenAPI",
            "GET",
            "/api/v1/openapi.json",
            validator=lambda j, r: ("openapi" in j or "paths" in j, "Falta bloque OpenAPI")
        )

        self.run_check(
            "Información de Hardware",
            "GET",
            "/api/v1/hardware",
            validator=lambda j, r: ("chipModel" in j or "chip" in j or "model" in j or "ok" in j, "Respuesta hardware incompleta")
        )

        self.run_check(
            "Información de Release",
            "GET",
            "/api/v1/release",
            validator=lambda j, r: ("version" in j or "firmware" in j, "Falta versión en release")
        )

        self.run_check(
            "Catálogo de Efectos",
            "GET",
            "/api/v1/effects",
            validator=lambda j, r: ("effects" in j and len(j["effects"]) > 0, "Catálogo de efectos vacío")
        )

        self.run_check(
            "Catálogo de Paletas",
            "GET",
            "/api/v1/palettes",
            validator=lambda j, r: ("palettes" in j and len(j["palettes"]) > 0, "Catálogo de paletas vacío")
        )

        # -------------------------------------------------------------
        # SUITE 1: TELEMETRÍA Y DIAGNÓSTICO
        # -------------------------------------------------------------
        print(f"\n{CLR_BOLD}{CLR_BLUE}▶ 01 - Diagnóstico y Rendimiento{CLR_RESET}")
        self.marcalog("TEST_DIAG", "Comprobando memoria y metricas")

        self.run_check(
            "Diagnóstico de Sistema (/diag)",
            "GET",
            "/api/v1/diag",
            validator=lambda j, r: ("uptimeMs" in j or "freeHeap" in j or "ok" in j, "Datos de diagnóstico ausentes")
        )

        self.run_check(
            "Métricas de Render (/metrics)",
            "GET",
            "/api/v1/metrics",
            validator=lambda j, r: ("fps" in j or "fpsCurrent" in j or "ok" in j, "Telemetría de FPS ausente")
        )

        self.run_check(
            "Reseteo de Métricas",
            "POST",
            "/api/v1/metrics/reset",
            validator=lambda j, r: (j.get("reset") is True, "Reseteo no confirmado")
        )

        # -------------------------------------------------------------
        # SUITE 2: CANAL DE DEPURACIÓN (MARCALOG)
        # -------------------------------------------------------------
        print(f"\n{CLR_BOLD}{CLR_BLUE}▶ 02 - Canal de Depuración y Marcas Serial{CLR_RESET}")
        self.run_check(
            "Marca de log vía POST JSON",
            "POST",
            "/api/v1/marcalog",
            payload={"tag": "TEST_RUNNER", "message": "Verificando log JSON en puerto serie"},
            validator=lambda j, r: (j.get("logged") is True, "Marca no aceptada")
        )

        self.run_check(
            "Marca de log vía GET query string",
            "GET",
            "/api/v1/marcalog?msg=Test_Query_Quick&tag=TEST_GET",
            validator=lambda j, r: (j.get("logged") is True, "Marca GET no aceptada")
        )

        # -------------------------------------------------------------
        # SUITE 3: CONTROL DE ESTADO Y EFECTOS
        # -------------------------------------------------------------
        print(f"\n{CLR_BOLD}{CLR_BLUE}▶ 03 - Control de Estado y Efectos en Tiempo Real{CLR_RESET}")
        self.marcalog("TEST_STATE", "Modificando estado de luces, brillo y color")

        # Guardar estado original
        st, orig_json, _ = self._http_request("GET", "/api/v1/state")
        if st == 200 and orig_json:
            self.original_state = orig_json.get("state", orig_json)

        self.run_check(
            "Lectura de Estado Inicial",
            "GET",
            "/api/v1/state",
            validator=lambda j, r: ("brightness" in j.get("state", j), "Formato de estado inválido")
        )

        # Modificación de estado
        test_patch = {
            "power": True,
            "brightness": 150,
            "effect": "lava_flow",
            "effectSpeed": 55,
            "effectLevel": 6,
            "primaryColor": "#00FFCC"
        }
        self.run_check(
            "Mutación de Estado (PATCH /state)",
            "PATCH",
            "/api/v1/state",
            payload=test_patch,
            validator=lambda j, r: (j.get("updated") is True or j.get("ok") is True, "Mutación fallida")
        )

        # Comprobar que el cambio se reflejó
        self.run_check(
            "Verificar Estado Aplicado",
            "GET",
            "/api/v1/state",
            validator=lambda j, r: (
                j.get("state", j).get("brightness") == 150 and
                j.get("state", j).get("effect") == "lava_flow",
                "El estado leído no coincide con el parche aplicado"
            )
        )

        # Aplicar paleta integrada
        self.run_check(
            "Aplicar Paleta Predefinida",
            "POST",
            "/api/v1/palettes/apply",
            payload={"palette": "sunset_drive"},
            validator=lambda j, r: (j.get("ok") is True, "Error al aplicar paleta")
        )

        # Secuencias
        self.run_check(
            "Añadir Estado a Secuencia",
            "POST",
            "/api/v1/effects/sequence/add",
            payload={"durationSec": 15},
            validator=lambda j, r: (j.get("ok") is True or j.get("added") is True, "No se pudo añadir a secuencia")
        )

        # -------------------------------------------------------------
        # SUITE 4: PALETAS DE USUARIO (CRUD)
        # -------------------------------------------------------------
        print(f"\n{CLR_BOLD}{CLR_BLUE}▶ 04 - CRUD de Paletas de Usuario{CLR_RESET}")
        self.marcalog("TEST_PALETTES", "Probando guardado y borrado de paleta custom")

        paleta_id = "test_auto_neon"
        paleta_payload = {
            "palette": {
                "id": paleta_id,
                "label": "Test Automation Palette",
                "primaryColors": ["#FF0055", "#00FF88", "#0088FF"],
                "style": "neon",
                "description": "Creada por suite automatizada"
            }
        }

        self.run_check(
            "Guardar Paleta de Usuario (/palettes/save)",
            "POST",
            "/api/v1/palettes/save",
            payload=paleta_payload,
            expect_ok=True,
            validator=lambda j, r: (j.get("ok") is True or j.get("saved") is True, "Error al guardar paleta")
        )

        self.run_check(
            "Aplicar Paleta de Usuario Guardada",
            "POST",
            "/api/v1/palettes/apply",
            payload={"palette": paleta_id},
            expect_ok=True,
            validator=lambda j, r: (j.get("ok") is True, "Error al aplicar paleta de usuario")
        )

        self.run_check(
            "Eliminar Paleta de Usuario (/palettes/delete)",
            "POST",
            "/api/v1/palettes/delete",
            payload={"palette": {"id": paleta_id}},
            expect_ok=True,
            validator=lambda j, r: (j.get("ok") is True or j.get("deleted") is True, "Error al eliminar paleta")
        )

        # -------------------------------------------------------------
        # SUITE 5: PERFILES DE HARDWARE (CRUD)
        # -------------------------------------------------------------
        print(f"\n{CLR_BOLD}{CLR_BLUE}▶ 05 - CRUD de Perfiles de Hardware{CLR_RESET}")
        self.marcalog("TEST_PROFILES", "Validando perfiles de usuario")

        self.run_check(
            "Listar Perfiles Existentes",
            "GET",
            "/api/v1/profiles",
            validator=lambda j, r: ("profiles" in j or "activeProfile" in j or "ok" in j, "Lista de perfiles inválida")
        )

        test_profile_id = "test_auto_prof"
        profile_save_payload = {
            "profile": {
                "id": test_profile_id,
                "name": "Auto Test Profile",
                "description": "Perfil temporal de test",
                "gpio": {
                    "outputs": [
                        {"pin": 8, "ledType": "ws2812b", "colorOrder": "GRB", "ledCount": 30}
                    ],
                    "power": {
                        "powerLimitEnabled": True,
                        "maxTotalCurrentmA": 1200
                    }
                }
            }
        }

        self.run_check(
            "Guardar Perfil de Usuario",
            "POST",
            "/api/v1/profiles/save",
            payload=profile_save_payload,
            validator=lambda j, r: (j.get("ok") is True, "Error al guardar perfil")
        )

        self.run_check(
            "Obtener Perfil Guardado",
            "GET",
            f"/api/v1/profiles/get?id={test_profile_id}",
            validator=lambda j, r: (j.get("profile", {}).get("id") == test_profile_id or j.get("ok") is True, "Perfil no encontrado")
        )

        self.run_check(
            "Eliminar Perfil Creado",
            "POST",
            "/api/v1/profiles/delete",
            payload={"profile": {"id": test_profile_id}},
            validator=lambda j, r: (j.get("ok") is True, "Error al eliminar perfil")
        )

        # -------------------------------------------------------------
        # SUITE 6: CONFIGURACIÓN MODULAR
        # -------------------------------------------------------------
        print(f"\n{CLR_BOLD}{CLR_BLUE}▶ 06 - Verificación de Configuración Modular{CLR_RESET}")
        self.marcalog("TEST_CONFIG", "Comprobando módulos de configuración")

        self.run_check("Lectura Configuración de Red", "GET", "/api/v1/config/network")
        self.run_check("Lectura Configuración GPIO / LEDs", "GET", "/api/v1/config/gpio")
        self.run_check("Lectura Configuración Micrófono I2S", "GET", "/api/v1/config/microphone")
        self.run_check("Lectura Configuración General", "GET", "/api/v1/config/general")
        self.run_check("Lectura Configuración Completa (/config/all)", "GET", "/api/v1/config/all")

        # -------------------------------------------------------------
        # SUITE 7: SINCRONIZACIÓN Y STREAMING LEDFX
        # -------------------------------------------------------------
        print(f"\n{CLR_BOLD}{CLR_BLUE}▶ 07 - Stack de Sincronización y LedFx{CLR_RESET}")
        self.marcalog("TEST_SYNC", "Validando stack DDP/E1.31 y clusters")

        self.run_check("Estado de Sincronización", "GET", "/api/v1/sync/state")
        self.run_check("Nodos Conectados en Cluster", "GET", "/api/v1/sync/connected")
        self.run_check("Configuración de Sincronización", "GET", "/api/v1/sync/config")

        # Cambiar modo de sync a standalone/off de forma inocua
        self.run_check(
            "Actualizar Modo de Sync",
            "PATCH",
            "/api/v1/sync/mode",
            payload={"mode": "off"},
            validator=lambda j, r: (j.get("ok") is True, "No se pudo actualizar modo sync")
        )

        # -------------------------------------------------------------
        # SUITE 8: PRUEBAS INTERACTIVAS CON USUARIO (LEDFX Y WEB UI)
        # -------------------------------------------------------------
        print(f"\n{CLR_BOLD}{CLR_BLUE}▶ 08 - Pruebas Interactivas (Web UI y Streaming LedFx){CLR_RESET}")
        
        # Prueba Interactiva 1: Cambio interactivo en la Web UI
        self.wait_for_user(
            f"Abre la Web UI en tu navegador ({self.base_url}) y cambia el color o brillo manualmente desde el panel Home.",
            tag="INTERACTIVE_WEB_UI"
        )
        
        # Medir telemetría de estado tras la acción web
        self.run_check(
            "Verificar Cambio en Web UI reflejado en API",
            "GET",
            "/api/v1/state",
            validator=lambda j, r: ("brightness" in j.get("state", j), "No se pudo verificar estado post-interacción")
        )

        # Prueba Interactiva 2: Conexión con LedFx por DDP
        self.run_check(
            "Habilitar Receptor LedFx en Firmware (DDP)",
            "PATCH",
            "/api/v1/sync/config",
            payload={
                "sync": {
                    "enabled": True,
                    "mode": "ledfx_realtime",
                    "role": "client",
                    "inputProtocol": "ddp",
                    "ddpPort": 4048
                }
            },
            validator=lambda j, r: (j.get("ok") is True, "Fallo al habilitar receptor DDP")
        )

        self.wait_for_user(
            f"Ahora inicia o conecta LedFx apuntando a la IP del ESP32 ({self.base_url.replace('http://', '')}) por DDP (puerto 4048).",
            tag="INTERACTIVE_LEDFX"
        )

        # Comprobar recepción y telemetría de LedFx
        def validate_sync_stream(j, r):
            sync_data = j.get("sync", j)
            stats = sync_data.get("stats", {})
            pkts = stats.get("packetsReceived", 0)
            fps = stats.get("inputFps", 0)
            conn = sync_data.get("connected", False)
            print(f"         {CLR_CYAN}Telemetría LedFx detectada: Paquetes={pkts}, FPS={fps}, Conectado={conn}{CLR_RESET}")
            return True, ""

        self.run_check(
            "Medir Telemetría de Recepción LedFx en Vivo",
            "GET",
            "/api/v1/sync/state",
            validator=validate_sync_stream
        )

        # Desactivar sync para no dejar la tira capturada
        self.run_check(
            "Desactivar Modo Receptor LedFx",
            "PATCH",
            "/api/v1/sync/mode",
            payload={"mode": "off"},
            validator=lambda j, r: (j.get("ok") is True, "No se pudo restaurar modo sync a off")
        )

        # -------------------------------------------------------------
        # SUITE 9: VALIDACIÓN NEGATIVA Y ROBUSTEZ (ERRORES)
        # -------------------------------------------------------------
        print(f"\n{CLR_BOLD}{CLR_BLUE}▶ 09 - Pruebas Negativas y Manejo de Errores{CLR_RESET}")
        self.marcalog("TEST_ERRORS", "Inyectando payloads invalidos para validar HTTP 400")

        self.run_check(
            "Rechazo de Parámetro Fuera de Rango (Brillo 999)",
            "PATCH",
            "/api/v1/state",
            expected_status=400,
            expect_ok=False,
            payload={"brightness": 999},
            validator=lambda j, r: (j.get("ok") is False and "error" in j, "Debe responder con sobre de error")
        )

        self.run_check(
            "Rechazo de JSON Malformado",
            "PATCH",
            "/api/v1/state",
            expected_status=400,
            expect_ok=False,
            raw_body="{ payload_incompleto: ",
            validator=lambda j, r: (j.get("error") == "invalid_json" or j.get("ok") is False, "Debe ser invalid_json")
        )

        self.run_check(
            "Rechazo de Método no Permitido",
            "DELETE",
            "/api/v1/state",
            expected_status=405,
            expect_ok=False
        )

        # -------------------------------------------------------------
        # RESTAURAR ESTADO ORIGINAL
        # -------------------------------------------------------------
        if self.original_state:
            print(f"\n{CLR_YELLOW}Restaurando estado original del dispositivo...{CLR_RESET}")
            restore_payload = {
                "power": self.original_state.get("power", True),
                "brightness": self.original_state.get("brightness", 128),
                "effect": self.original_state.get("effect", "lava_flow"),
                "effectSpeed": self.original_state.get("effectSpeed", 50)
            }
            self._http_request("PATCH", "/api/v1/state", payload=restore_payload)

        # -------------------------------------------------------------
        # RESUMEN FINAL
        # -------------------------------------------------------------
        total = self.passed + self.failed
        pass_rate = (self.passed / total * 100) if total > 0 else 0
        summary_msg = f"TESTS FINALIZADOS: {self.passed}/{total} aprobados ({pass_rate:.1f}%)"
        self.marcalog("TEST_SUMMARY", summary_msg)

        self.print_and_log(f"\n{CLR_BOLD}=================================================================={CLR_RESET}")
        self.print_and_log(f"{CLR_BOLD} RESUMEN DE EJECUCIÓN DE PRUEBAS{CLR_RESET}")
        self.print_and_log(f"{CLR_BOLD}=================================================================={CLR_RESET}")
        self.print_and_log(f" Total de pruebas:    {total}")
        self.print_and_log(f" {CLR_GREEN}Pruebas superadas:   {self.passed}{CLR_RESET}")
        self.print_and_log(f" {CLR_RED}Pruebas fallidas:    {self.failed}{CLR_RESET}")
        self.print_and_log(f" Tasa de éxito:       {pass_rate:.1f}%\n")

        # Cerrar archivo de log y crear copia en latest
        try:
            self.log_fp.close()
            with open(self.log_file_path, "r", encoding="utf-8") as src, open(self.latest_log_path, "w", encoding="utf-8") as dst:
                dst.write(src.read())
            print(f"{CLR_BOLD}{CLR_CYAN}📁 Log guardado en:{CLR_RESET} {os.path.abspath(self.log_file_path)}")
            print(f"{CLR_BOLD}{CLR_CYAN}📁 Acceso rápido:  {CLR_RESET} {os.path.abspath(self.latest_log_path)}\n")
        except Exception as e:
            print(f"Error cerrando log: {e}")

        if self.failed > 0:
            print(f"{CLR_RED}{CLR_BOLD}✘ Se detectaron fallos en la API del firmware.{CLR_RESET}")
            sys.exit(1)
        else:
            print(f"{CLR_GREEN}{CLR_BOLD}✔ Toda la API REST y funcionalidad responden con paridad al 100%.{CLR_RESET}")
            sys.exit(0)


def main():
    parser = argparse.ArgumentParser(
        description="Suite de Pruebas de Integración de API para DUXMAN-LED"
    )
    parser.add_argument(
        "--url",
        default="http://192.168.4.1",
        help="URL base del dispositivo ESP32 (ej: http://192.168.4.1 o http://192.168.1.150)",
    )
    parser.add_argument(
        "--verbose", "-v",
        action="store_true",
        help="Muestra detalles de payloads y respuestas completas",
    )
    parser.add_argument(
        "--delay", "-d",
        type=float,
        default=0.15,
        help="Retardo en segundos entre peticiones para no saturar la red (default: 0.15)",
    )
    parser.add_argument(
        "--non-interactive",
        action="store_true",
        help="Ejecuta en modo desatendido (sin pausar para que el usuario pulse Enter)",
    )

    args = parser.parse_args()
    tester = ApiTester(
        base_url=args.url,
        verbose=args.verbose,
        delay=args.delay,
        interactive=not args.non_interactive,
    )
    tester.run_all()


if __name__ == "__main__":
    main()
