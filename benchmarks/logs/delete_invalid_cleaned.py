import os
from pathlib import Path

# --- SETTINGS ---
# SAFETY SWITCH: Set to False to actually delete files.
# When True, it only prints which files would be deleted.
DRY_RUN = False

# A valid CSV needs a header AND at least one row of data.
# So, any file with less than 2 lines is considered invalid.
MIN_VALID_LINES = 3
# -----------------

def check_and_delete_invalid_csv(file_path: Path):
    """
    Checks if a cleaned CSV file is invalid (i.e., only has a header) and deletes it.
    """
    try:
        with open(file_path, 'r', encoding='utf-8') as f:
            lines = f.readlines()
            # Filter out empty lines to get an accurate count
            non_empty_lines = [line for line in lines if line.strip()]
            
    except Exception as e:
        print(f"⚠️  Could not read file {file_path.name}: {e}")
        return

    # Check if the file has fewer lines than the minimum required
    if len(non_empty_lines) < MIN_VALID_LINES:
        print(f"🔎 Found invalid CSV: '{file_path.name}' (has {len(non_empty_lines)} line(s))")
        
        if DRY_RUN:
            print(f"   └── (Dry Run) Would delete this file.")
        else:
            try:
                file_path.unlink() # This is the command that deletes the file
                print(f"   └── 🗑️  DELETED successfully.")
            except Exception as e:
                print(f"   └── ❌ FAILED to delete file: {e}")

if __name__ == "__main__":
    source_directory = Path('cleaned_logs')
    
    if not source_directory.is_dir():
        print(f"❌ Error: Directory not found at '{source_directory.resolve()}'")
        print("Please make sure you are running this script from the 'bench_logs' directory.")
    else:
        if DRY_RUN:
            print("--- 🧐 Running in DRY RUN mode. No files will be deleted. ---")
        else:
            print("--- 🔥 Running in DELETE mode. Invalid files will be permanently removed. ---")
        
        print(f"\nScanning for invalid CSVs in: {source_directory.resolve()}\n")

        for file_path in source_directory.iterdir():
            if file_path.is_file() and file_path.suffix == '.csv':
                # This line is now corrected
                check_and_delete_invalid_csv(file_path)

        print("\n🚀 Scan complete.")
