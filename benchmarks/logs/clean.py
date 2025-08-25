import os
from pathlib import Path

def clean_log_file(input_path: Path, output_path: Path):
    """
    Reads a log file, extracts the CSV data, and saves it to a new file.
    This version validates that data rows look like actual data.
    """
    try:
        with open(input_path, 'r', encoding='utf-8') as f:
            lines = f.readlines()
    except Exception as e:
        print(f"❌ Error reading file {input_path}: {e}")
        return

    cleaned_data = []
    header_found = False
    lines_to_skip = 2 # The two info lines after the header

    for line in lines:
        if header_found:
            # Skip the two unwanted info lines
            if lines_to_skip > 0:
                lines_to_skip -= 1
                continue
            
            # Stop if we hit the end marker
            if line.startswith('end-->'):
                break

            # --- THE FIX IS HERE ---
            # A valid data row must contain commas. If not, the data section
            # has ended, and we should stop processing this file.
            if ',' not in line:
                break
            # --- END OF FIX ---

            # If the line passes the check, add it to our list
            cleaned_data.append(line)

        elif line.startswith('budget_function,PMU_counter_used_budget'):
            header_found = True
            cleaned_data.append(line)

    if not header_found:
        print(f"⚠️  Skipping invalid file (header not found): {input_path.name}")
        return

    if cleaned_data:
        try:
            # Only write the header if no valid data rows were found
            # This creates the 1-line CSV that the deletion script can find.
            with open(output_path, 'w', encoding='utf-8', newline='') as f:
                f.writelines(cleaned_data)
            print(f"✅ Successfully cleaned '{input_path.name}' -> '{output_path.name}'")
        except Exception as e:
            print(f"❌ Error writing to file {output_path}: {e}")

if __name__ == "__main__":
    source_directory = Path('.')
    output_directory = source_directory / "cleaned_logs"
    output_directory.mkdir(exist_ok=True)

    print(f"Scanning for log files in: {source_directory.resolve()}")
    print(f"Cleaned files will be saved in: {output_directory.resolve()}\n")

    for file_path in source_directory.iterdir():
        if file_path.is_file() and file_path.suffix in ['.txt', '.log', '']:
            if file_path.name not in ['clean.py', 'delete_invalid_cleaned.py']:
                output_file_path = output_directory / f"{file_path.stem}.csv"
                clean_log_file(file_path, output_file_path)

    print("\n🚀 All files processed.")
