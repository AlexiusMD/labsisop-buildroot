#!/usr/bin/env python3

import json
import time
import os
from http.server import BaseHTTPRequestHandler, HTTPServer
from datetime import datetime

# Todos os tipos de whitespace a serem removidos de strings
whitespace: str = "\n\t\r\f\v "

# --- Alunos devem implementar as funções abaixo --- #

def get_info_file_dict(resource_name: str) -> dict[str, str]:
    info_dict: dict[str, str] = {}
    with open(f"/proc/{resource_name}", 'r') as info_file:
        for line in info_file:
            strip_line = line.strip(whitespace)
            if strip_line != "":
                split_line = strip_line.split(':')
                info_dict[split_line[0].strip(whitespace)] = split_line[1].strip(whitespace) if split_line[1] != "" else ""
            else:
                continue

    return info_dict     

def get_datetime() -> str:
    with open("/sys/class/rtc/rtc0/since_epoch") as rtc_file:
        curr_time = float(rtc_file.read())
        return datetime.fromtimestamp(curr_time).isoformat()


def get_uptime() -> str:
    with open('/proc/uptime', 'r') as uptime_file:
        uptime_info = uptime_file.read()
        split_uptime = uptime_info.split()
        return split_uptime[0]

def get_cpu_info():
    cpu_info: dict[str,str] = get_info_file_dict('cpuinfo')
    return {
        "model": cpu_info.get("model name"),
        "speed_mhz": cpu_info.get("cpu MHz"),
        "usage_percent": 0.0 # Vamo pedir pro sor
    }

def get_memory_info() -> dict[str,str]:
    mem_info: dict[str,str] = get_info_file_dict('meminfo')
    mem_total = mem_info.get('MemTotal')
    mem_free = mem_info.get('MemFree')

    # Pega parte numérica das quantais de memória em KB
    mem_total = int(mem_total.split()[0])
    mem_free = int(mem_free.split()[0])

    # Calcula quantia usada (total - livre)
    mem_used = mem_total - mem_free

    # Dividido por mil pois kb / 1000 = mb
    return {
        "total_mb": f"{mem_total / 1000} mb",
        "used_mb": f"{mem_used / 1000} mb"
    }

def get_os_version() -> str:
    with open("/proc/version", "r") as version_file:
        return version_file.read()

def get_process_list() -> list[dict]:
    proc_dir = os.listdir('/proc')
    proc_list: list[dict] = []
    for file in proc_dir:
        if file.isdigit():
            with open(f"/proc/{file}/comm", "r") as comm_file:
                name = comm_file.read().strip(whitespace)
            proc_list.append({"pid": int(file), "name": name})
        
    return proc_list  # lista de { "pid": int, "name": str }

def get_disks() -> list[dict]:
    disk_list: list[dict] = []
    with open("/proc/partitions", "r") as file:
        lines = file.readlines()[2:] # Pula 2 primeiras linhas de cabeçalho
        for line in lines:
            split_line = line.split()
            if len(split_line) == 4:
                size_kb = int(split_line[2])
                device = split_line[3]
                disk_list.append({
                    "device": device,
                    "size_mb": size_kb / 1000
                })
    return disk_list  # lista de { "device": str, "size_mb": int }

def get_usb_devices():
    return []  # lista de { "port": str, "description": str }

def get_network_adapters() -> dict[str,str]:
    adapters: dict[str, str] = []

    with open("/proc/net/dev") as interface_list:
        lines = interface_list.readlines()[2:]
        for line in lines:
           adapters.append({
                "interface": line.split()[0].strip(whitespace),
                "ip_address": "" # we dont got that yet
            })
    
    return adapters # lista de { "interface": str, "ip_address": str }

# --- Servidor HTTP --- #

class StatusHandler(BaseHTTPRequestHandler):
    def do_GET(self):
        if self.path != "/status":
            self.send_response(404)
            self.end_headers()
            self.wfile.write(b"Not Found")
            return

        response = {
            "datetime": get_datetime(),
            "uptime_seconds": get_uptime(),
            "cpu": get_cpu_info(),
            "memory": get_memory_info(),
            "os_version": get_os_version(),
            "processes": get_process_list(),
            "disks": get_disks(),
            "usb_devices": get_usb_devices(),
            "network_adapters": get_network_adapters()
        }

        data = json.dumps(response, indent=2).encode()
        self.send_response(200)
        self.send_header("Content-Type", "application/json")
        self.send_header("Content-Length", str(len(data)))
        self.end_headers()
        self.wfile.write(data)

def run_server(port=8080):
    print(f"Servidor disponível em http://0.0.0.0:{port}/status")
    server = HTTPServer(("0.0.0.0", port), StatusHandler)
    server.serve_forever()

if __name__ == "__main__":
    run_server()