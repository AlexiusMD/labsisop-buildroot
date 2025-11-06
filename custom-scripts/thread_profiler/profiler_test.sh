#!/bin/sh

# ==============================================================================
# SCRIPT DE TESTE DE ESCALONAMENTO COM TRACE-CMD
#
# AVISO: Este script DEVE ser executado como root (ex: sudo ./run_tests.sh)
#        para que o trace-cmd e as políticas de tempo real funcionem.
#
# Pré-requisitos:
# 1. O 'trace-cmd' deve estar instalado.
# 2. O 'thread_profiler' deve estar compilado e em um diretório do PATH.
# ==============================================================================

# Parâmetros de teste (ajuste se necessário)
NUM_THREADS=2
BUFFER_KB=15

echo "Iniciando captura de traces... Isso pode demorar alguns segundos."
echo "------------------------------------------------------------"

# --- Grupo 1: Linha de Base (CFS - SCHED_OTHER) ---
echo "[Grupo 1] Teste 1.1: base_other_x2.dat"
trace-cmd record -e sched:sched_switch -o base_other_x2.dat \
    thread_profiler $NUM_THREADS $BUFFER_KB SCHED_OTHER 0 SCHED_OTHER 0

# --- Grupo 2: Comparação de Políticas CFS (OTHER vs. BATCH vs. IDLE) ---
echo "[Grupo 2] Teste 2.1: cfs_batch_vs_other.dat"
trace-cmd record -e sched:sched_switch -o cfs_batch_vs_other.dat \
    thread_profiler $NUM_THREADS $BUFFER_KB SCHED_BATCH 0 SCHED_OTHER 0

echo "[Grupo 2] Teste 2.2: cfs_idle_vs_other.dat"
trace-cmd record -e sched:sched_switch -o cfs_idle_vs_other.dat \
    thread_profiler $NUM_THREADS $BUFFER_KB SCHED_IDLE 0 SCHED_OTHER 0

echo "[Grupo 2] Teste 2.3: cfs_batch_x2.dat"
trace-cmd record -e sched:sched_switch -o cfs_batch_x2.dat \
    thread_profiler $NUM_THREADS $BUFFER_KB SCHED_BATCH 0 SCHED_BATCH 0

# --- Grupo 3: Variação de Prioridade CFS (Valores nice) ---
echo "[Grupo 3] Teste 3.1: cfs_nice_p19.dat"
trace-cmd record -e sched:sched_switch -o cfs_nice_p19.dat \
    nice -n 19 thread_profiler $NUM_THREADS $BUFFER_KB SCHED_OTHER 0 SCHED_OTHER 0

echo "[Grupo 3] Teste 3.2: cfs_nice_n20.dat"
trace-cmd record -e sched:sched_switch -o cfs_nice_n20.dat \
    nice -n -20 thread_profiler $NUM_THREADS $BUFFER_KB SCHED_OTHER 0 SCHED_OTHER 0

# --- Grupo 4: Políticas de Tempo Real (FIFO vs. RR) ---
echo "[Grupo 4] Teste 4.1: rt_fifo_50_vs_49.dat"
trace-cmd record -e sched:sched_switch -o rt_fifo_50_vs_49.dat \
    thread_profiler $NUM_THREADS $BUFFER_KB SCHED_FIFO 50 SCHED_FIFO 49

echo "[Grupo 4] Teste 4.2: rt_fifo_50_vs_50.dat"
trace-cmd record -e sched:sched_switch -o rt_fifo_50_vs_50.dat \
    thread_profiler $NUM_THREADS $BUFFER_KB SCHED_FIFO 50 SCHED_FIFO 50

echo "[Grupo 4] Teste 4.3: rt_rr_50_vs_49.dat"
trace-cmd record -e sched:sched_switch -o rt_rr_50_vs_49.dat \
    thread_profiler $NUM_THREADS $BUFFER_KB SCHED_RR 50 SCHED_RR 49

echo "[Grupo 4] Teste 4.4: rt_rr_50_vs_50.dat"
trace-cmd record -e sched:sched_switch -o rt_rr_50_vs_50.dat \
    thread_profiler $NUM_THREADS $BUFFER_KB SCHED_RR 50 SCHED_RR 50

echo "[Grupo 4] Teste 4.5: rt_fifo_50_vs_rr_50.dat"
trace-cmd record -e sched:sched_switch -o rt_fifo_50_vs_rr_50.dat \
    thread_profiler $NUM_THREADS $BUFFER_KB SCHED_FIFO 50 SCHED_RR 50

# --- Grupo 5: Comparação Mista (RT vs. CFS) ---
echo "[Grupo 5] Teste 5.1: rt_fifo_1_vs_other_0.dat"
trace-cmd record -e sched:sched_switch -o rt_fifo_1_vs_other_0.dat \
    thread_profiler $NUM_THREADS $BUFFER_KB SCHED_FIFO 1 SCHED_OTHER 0

echo "[Grupo 5] Teste 5.2: rt_fifo_1_vs_other_n20.dat"
trace-cmd record -e sched:sched_switch -o rt_fifo_1_vs_other_n20.dat \
    nice -n -20 thread_profiler $NUM_THREADS $BUFFER_KB SCHED_FIFO 1 SCHED_OTHER 0

echo "[Grupo 5] Teste 5.3: rt_fifo_1_vs_idle_0.dat"
trace-cmd record -e sched:sched_switch -o rt_fifo_1_vs_idle_0.dat \
    thread_profiler $NUM_THREADS $BUFFER_KB SCHED_FIFO 1 SCHED_IDLE 0

# --- Grupo 6: Testes com SCHED_LOW_IDLE ---
echo "[Grupo 6] Teste 6.1: cfs_lowidle_vs_other.dat"
trace-cmd record -e sched:sched_switch -o cfs_lowidle_vs_other.dat \
    thread_profiler $NUM_THREADS $BUFFER_KB SCHED_LOW_IDLE 0 SCHED_OTHER 0

echo "[Grupo 6] Teste 6.2: cfs_lowidle_vs_idle.dat"
trace-cmd record -e sched:sched_switch -o cfs_lowidle_vs_idle.dat \
    thread_profiler $NUM_THREADS $BUFFER_KB SCHED_LOW_IDLE 0 SCHED_IDLE 0

echo "[Grupo 6] Teste 6.3: rt_fifo_1_vs_lowidle.dat"
trace-cmd record -e sched:sched_switch -o rt_fifo_1_vs_lowidle.dat \
    thread_profiler $NUM_THREADS $BUFFER_KB SCHED_FIFO 1 SCHED_LOW_IDLE 0

echo "[Grupo 6] Teste 6.4: rt_lowidle_x2.dat"
trace-cmd record -e sched:sched_switch -o rt_lowidle_x2.dat \
    thread_profiler $NUM_THREADS $BUFFER_KB SCHED_LOW_IDLE 0 SCHED_LOW_IDLE 0

echo "------------------------------------------------------------"
echo "✅ Captura de todos os traces concluída."
echo "Todos os arquivos .dat foram gerados."