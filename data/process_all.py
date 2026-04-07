#!/usr/bin/env python3
"""
Script unificado para processar todos os arquivos .txt de dados PMU
e gerar os CSVs finais limpos em data/csv_final/.

Pipeline (por arquivo .txt):
  1. Extrai linhas CSV entre marcadores START_OF_CSV_DATA / END_OF_CSV_DATA
  2. Remove a coluna CORE_ID
  3. Salva o CSV limpo em data/csv_final/

Uso:
  python3 process_all.py                  # processa todos os .txt na pasta data/
  python3 process_all.py data1.txt        # processa apenas o arquivo especificado
"""

import sys
import os
import glob


# Colunas finais do CSV limpo (sem CORE_ID)
FINAL_COLUMNS = ["TIMESTAMP", "CPU_CYCLES", "INSTRUCTIONS", "CACHE_MISSES", "BRANCH_MISSES", "L2_CACHE_ACCESS", "LABEL"]
HEADER_WITH_CORE = "CORE_ID,TIMESTAMP,CPU_CYCLES,INSTRUCTIONS,CACHE_MISSES,BRANCH_MISSES,L2_CACHE_ACCESS,LABEL"
VALID_CORES = {'1', '2', '3'}


def process_txt_to_clean_csv(input_file, output_file):
    """
    Lê um arquivo .txt bruto com dados PMU e gera diretamente o CSV final limpo.
    Combina a extração (extract_csv) e a limpeza (clean_data) em uma única passada.
    """
    print(f"\n{'='*60}")
    print(f"  Processando: {os.path.basename(input_file)}")
    print(f"  Saída:       {os.path.basename(output_file)}")
    print(f"{'='*60}")

    in_csv = False
    lines_written = 0
    label_counts = {}

    with open(input_file, 'r', errors='replace') as fin, open(output_file, 'w') as fout:
        # Escreve o cabeçalho final (sem CORE_ID)
        fout.write(','.join(FINAL_COLUMNS) + '\n')

        for line in fin:
            stripped = line.strip()

            # Detecta início/fim de bloco CSV
            if stripped == "START_OF_CSV_DATA":
                in_csv = True
                continue
            elif stripped == "END_OF_CSV_DATA":
                in_csv = False
                continue

            if not in_csv:
                continue

            # Ignora cabeçalhos repetidos e linhas vazias
            if stripped.startswith("CORE_ID") or stripped == "":
                continue

            parts = stripped.split(',')

            # Valida: 7 colunas e CORE_ID válido (1, 2 ou 3)
            if len(parts) != 8 or parts[0] not in VALID_CORES:
                continue

            # Remove CORE_ID (índice 0), mantém as demais colunas
            # Ordem: TIMESTAMP, CPU_CYCLES, INSTRUCTIONS, CACHE_MISSES, BRANCH_MISSES, L2_CACHE_ACCESS, LABEL
            clean_row = parts[1:]  # remove o primeiro elemento (CORE_ID)
            fout.write(','.join(clean_row) + '\n')
            lines_written += 1

            # Contagem de labels para diagnóstico
            label = parts[7].strip()
            label_counts[label] = label_counts.get(label, 0) + 1

    print(f"  ✓ {lines_written} linhas escritas")
    if label_counts:
        print(f"  Distribuição de labels:")
        for label, cnt in sorted(label_counts.items()):
            print(f"    Label {label}: {cnt} amostras")

    return lines_written


def main():
    # Diretório base onde ficam os dados
    script_dir = os.path.dirname(os.path.abspath(__file__))

    # Pasta de saída para os CSVs finais
    output_dir = os.path.join(script_dir, 'csv_final')
    os.makedirs(output_dir, exist_ok=True)

    # Determina quais arquivos processar
    if len(sys.argv) > 1:
        # Argumentos fornecidos: processa apenas os arquivos especificados
        txt_files = []
        for arg in sys.argv[1:]:
            path = arg if os.path.isabs(arg) else os.path.join(script_dir, arg)
            if os.path.exists(path):
                txt_files.append(path)
            else:
                print(f"⚠ Arquivo não encontrado: {arg}")
    else:
        # Sem argumentos: processa todos os .txt na pasta data/
        txt_files = sorted(glob.glob(os.path.join(script_dir, '*.txt')))

    if not txt_files:
        print("Nenhum arquivo .txt encontrado para processar.")
        sys.exit(1)

    print(f"\n🔄 Processando {len(txt_files)} arquivo(s) .txt → csv_final/\n")

    total_lines = 0
    processed = 0

    for txt_path in txt_files:
        basename = os.path.basename(txt_path)
        # data1.txt → data1_clean.csv
        csv_name = os.path.splitext(basename)[0] + '_clean.csv'
        csv_path = os.path.join(output_dir, csv_name)

        lines = process_txt_to_clean_csv(txt_path, csv_path)
        total_lines += lines
        processed += 1

    print(f"\n{'='*60}")
    print(f"  ✅ Concluído! {processed} arquivo(s) processado(s)")
    print(f"  📊 Total de linhas: {total_lines}")
    print(f"  📁 CSVs salvos em: {output_dir}/")
    print(f"{'='*60}\n")


if __name__ == '__main__':
    main()
