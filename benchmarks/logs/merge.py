from pathlib import Path

csv_files = sorted(Path(".").glob("*.log"), key=lambda f: f.name)

with open("merged_output.csv", "w", encoding="utf-8") as outfile:
    for csv_file in csv_files:
        with open(csv_file, "r", encoding="utf-8") as infile:
            for line in infile:
                outfile.write(line.rstrip("\n") + "\n")
        # outfile.write(f"(# {csv_file.stem} above)\n")
        # outfile.write("\n")  # Blank line between tables
