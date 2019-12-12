import pandas as pd

def csv2html(infile, sep):
    data = pd.read_csv(infile,delimiter=sep)
    data.drop(data.filter(regex="Unnamed"),axis=1, inplace=True)
    df = pd.DataFrame(data)
    table = df.to_html()     # html-formatted string
    return table

infile = 'out_original_matrix'
table = csv2html(infile, ' ')

# just checking...
print(table)
file = open("table.html", "w")
file.write(table)
file.close()

