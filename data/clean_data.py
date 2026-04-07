import csv
import sys
import os

def clean_csv(input_file, output_file):
    print(f"Lendo de {input_file} e salvando dados processados em {output_file}...")
    
    with open(input_file, 'r', newline='') as infile, open(output_file, 'w', newline='') as outfile:
        reader = csv.reader(infile)
        writer = csv.writer(outfile)
        
        header = next(reader)
        
        # Colunas finais: removemos CORE_ID e mantemos o LABEL original do cenário
        expected_columns = ["TIMESTAMP", "CPU_CYCLES", "INSTRUCTIONS", "CACHE_MISSES", "BRANCH_MISSES", "LABEL"]
        writer.writerow(expected_columns)
        
        try:
            core_id_idx = header.index("CORE_ID")
            col_indices = {col: header.index(col) for col in expected_columns}
        except ValueError as e:
            print(f"Erro: Coluna não encontrada no cabeçalho: {e}")
            return
            
        count = 0
        label_counts = {}
        
        for row in reader:
            if not row:
                continue
            
            # Manter o label original que veio do pmu_monitor (definido pelo SCENARIO)
            # Cenário 1: label 0 = benchmark
            # Cenário 2: label 1 = ataque
            # Cenário 3: label 2 = benchmark, label 3 = ataque
            new_row = [row[col_indices[col]] for col in expected_columns]
            writer.writerow(new_row)
            count += 1
            
            # Contagem de labels para diagnóstico
            label = row[col_indices["LABEL"]].strip()
            label_counts[label] = label_counts.get(label, 0) + 1
            
    print(f"\nConcluído! {count} linhas processadas.")
    print(f"\nDistribuição de labels:")
    for label, cnt in sorted(label_counts.items()):
        print(f"  Label {label}: {cnt} amostras")

if __name__ == '__main__':
    script_dir = os.path.dirname(os.path.abspath(__file__))
    
    input_path = os.path.join(script_dir, 'data_final2.csv')
    output_path = os.path.join(script_dir, 'data_final_clean2.csv')
    
    if len(sys.argv) > 1:
        input_path = sys.argv[1]
    if len(sys.argv) > 2:
        output_path = sys.argv[2]
        
    if not os.path.exists(input_path):
        print(f"Erro: Arquivo '{input_path}' não encontrado.")
        sys.exit(1)
        
    clean_csv(input_path, output_path)
