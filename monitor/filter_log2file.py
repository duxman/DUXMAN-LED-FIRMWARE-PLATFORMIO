# Custom PlatformIO monitor filter to log serial output into project 'log/' folder
import io
import os
import shutil
from datetime import datetime

from platformio.device.monitor.filters.base import DeviceMonitorFilterBase


class LogToFile(DeviceMonitorFilterBase):
    NAME = "log2file"

    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self._log_fp = None
        self._latest_path = None

    def __call__(self):
        log_dir = "log"
        if not os.path.isdir(log_dir):
            os.makedirs(log_dir, exist_ok=True)

        timestamp = datetime.now().strftime("%Y%m%d-%H%M%S")
        log_file_name = os.path.join(log_dir, f"device-monitor-{timestamp}.log")
        self._latest_path = os.path.join(log_dir, "device-monitor-latest.log")

        print(f"--- [DUXMAN-LOG] Guardando salida del Monitor Serie en: {os.path.abspath(log_file_name)}")
        self._log_fp = io.open(log_file_name, "w", encoding="utf-8")
        return self

    def __del__(self):
        if self._log_fp:
            try:
                self._log_fp.close()
            except Exception:
                pass

    def rx(self, text):
        if self._log_fp:
            self._log_fp.write(text)
            self._log_fp.flush()
            if self._latest_path:
                try:
                    with open(self._latest_path, "a", encoding="utf-8") as lf:
                        lf.write(text)
                except Exception:
                    pass
        return text
