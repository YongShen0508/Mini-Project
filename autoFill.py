import openpyxl
from openpyxl.utils import get_column_letter

def initialize_worksheet(wb, sheet_name):
    ws = wb.active
    if ws.title == "Sheet":
        ws.title = sheet_name
    else:
        ws = wb.create_sheet(title=sheet_name)

    # Set the header row
    ws['B2'] = "Generation / Experiments"
    for i in range(10):
        ws.cell(row=2, column=3+i, value=i+1)

    # Fill B3:B2002 with 1 to 2000
    for i in range(2000):
        ws.cell(row=3+i, column=2, value=i+1)

    # Fill other specified cells
    ws['B2003'] = "Min"
    ws['B2004'] = "Max"
    ws['B2006'] = "Values"
    for i in range(30):
        ws.cell(row=2007+i, column=2, value=i+1)
    ws['B2038'] = "Time"
    
    return ws

def process_file(ws, file_name, column):
    try:
        with open(file_name, 'r') as file:
            current_row = 3

            # Read fitness values
            for line in file:
                line = line.strip()
                if not line:
                    break
                ws.cell(row=current_row, column=column, value=float(line))
                current_row += 1

            # Write MIN and MAX formulas
            col_letter = get_column_letter(column)
            ws.cell(row=2003, column=column, value=f"=MIN({col_letter}3:{col_letter}2002)")
            ws.cell(row=2004, column=column, value=f"=MAX({col_letter}3:{col_letter}2002)")

            # Skip blank lines to reach chromosome bits
            while True:
                line = file.readline().strip()
                if line:
                    break

            # Read chromosome bits
            current_row = 2007
            while True:
                ws.cell(row=current_row, column=column, value=float(line))
                current_row += 1
                line = file.readline().strip()
                if not line:
                    break

            # Skip blank lines to reach the running time
            while True:
                line = file.readline().strip()
                if line:
                    ws.cell(row=2038, column=column, value=float(line))
                    break

    except FileNotFoundError:
        print(f"Error: {file_name} not found. Please check if the file is located in the correct directory.")
    except Exception as e:
        print(f"An error occurred while processing {file_name}: {e}")

def main(workbook_filename, sheet_info):
    # Create a new Excel workbook
    wb = openpyxl.Workbook()
    total_files_processed = 0

    for sheet_name, text_file_prefix, num_files in sheet_info:
        ws = initialize_worksheet(wb, sheet_name)
        print(f"Starting to process files for {sheet_name}")
        for i in range(1, num_files + 1):
            file_number = total_files_processed + i
            file_name = f"{text_file_prefix}{file_number}.txt"
            column = 3 + ((i - 1) % 10)
            print(f"Processing {file_name} into column {column} of {sheet_name}")
            process_file(ws, file_name, column)
        total_files_processed += num_files

    # Save the workbook
    wb.save(workbook_filename)

workbook_filename = "GA_Results.xlsx"
sheet_info = [
    ("GA01", "GAResult", 10),
    ("GA02", "GAResult", 10),
    ("GA03", "GAResult", 10),
    ("GA04", "GAResult", 10),
    ("GA05", "GAResult", 10),
    ("GA06", "GAResult", 10),
    ("GA07", "GAResult", 10),
    ("GA08", "GAResult", 10),
    ("GA09", "GAResult", 10),
    ("GA10", "GAResult", 10),
]

if __name__ == "__main__":
    main(workbook_filename, sheet_info)
