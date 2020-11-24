import tkinter as tk
from tkinter import Message, Variable, filedialog, messagebox, Text
import os, re

# CREATE WINDOW
window = tk.Tk()
window.title('DES-RSA')
window.geometry('500x600')

# GET MODE
mode = tk.StringVar()
tk.Label(window, bg='yellow', width=20, text='Encode or Deocde ?').pack()
tk.Radiobutton(window, text='Encode', variable=mode, value='e', command=None).pack()
tk.Radiobutton(window, text='Decode', variable=mode, value='d', command=None).pack()
tk.Label(window, bg=None, width=20, text='').pack()

# GET FILES
in_path = []

def add_files():
    in_path.clear()
    files = filedialog.askopenfilenames()
    for f in files:
        in_path.append(f)
    tk.Label(window, bg=None, width=100, text=in_path[0] + '...').pack()

tk.Label(window, bg='yellow', width=20, text='Select a file').pack()
tk.Button(window, text='open file', font=('Arial', 12), width=10, height=1,  command=add_files).pack()
tk.Label(window, bg=None, width=20, text='').pack()

# GET KEYS
des_key = tk.StringVar()
rsa_key = tk.StringVar()
rsa_mod = tk.StringVar()
rsa_init = tk.IntVar()
tk.Label(window, bg='yellow', width=20, text='Input your key (DES)').pack()
tk.Entry(window, show=None, font=('Arial', 14), textvariable=des_key).pack()
#Is INIT ?
tk.Label(window, bg='yellow', width=40, text='If checked, please ignore the input box below').pack()
tk.Checkbutton(window, text='Init RSA',variable=rsa_init, onvalue=1, offvalue=0, command=None).pack()
tk.Label(window, bg='yellow', width=20, text='Input your key (RSA)').pack()
tk.Entry(window, show=None, font=('Arial', 14), textvariable=rsa_key).pack()
tk.Label(window, bg='yellow', width=20, text='Input your module (RSA)').pack()
tk.Entry(window, show=None, font=('Arial', 14), textvariable=rsa_mod).pack()
tk.Label(window, bg=None, width=20, text='').pack()


# RUN
def run():
    if len(in_path) == 0:
        print(tk.messagebox.showerror(title='ERROR',message='No input file!'))
        return
    if len(mode.get()) == 0:
        print(tk.messagebox.showerror(title='ERROR',message='Encode or Deocde ?'))
        return
    if len(des_key.get()) == 0:
        print(tk.messagebox.showerror(title='ERROR',message='No DES key!'))
    check_rsa = len(rsa_key.get()) != 0 and len(rsa_mod.get()) != 0
    if check_rsa == 0 and rsa_init.get() == 0:
         print(tk.messagebox.showerror(title='ERROR',message=
            'No RSA key or module！(input your key and module or choose init a rsa key pair'))
    index_dir = len(os.listdir(".\\result")) + 1
    save_dir = ""
    if mode.get() == 'e':
        save_dir = "result\\result{}(encode)".format(index_dir)
    else:
        save_dir = "result\\result{}(decode)".format(index_dir)
    os.system("mkdir {}\\des-out".format(save_dir))
    """
    For each file: $ .\\des\\des.exe  <in_file>  <e>   0  + <key> = .\\result\\result_i\\des-out\\filename
    For des key:   $ .\\rsa\\rsa.exe -d <data(dec)> --init -o .\\result\\result_i\\rsa-out.txt
    ,or
    For each file: $ .\\des\\des.exe  <in_file>  <e>   0  + <key> = .\\result\\result_i\\des-out\\filename
    For des key:   $ .\\rsa\\rsa.exe -d <data(dec)> -p <PK> -m <Module>  -o .\\result\\result_i\\rsa-out.txt
    ,or
    For each file: $ .\\des\\des.exe  <in_file>  <e>   0  + <key> = .\\result\\result_i\\des-out\\filename
    For des key:   $ .\\rsa\\rsa.exe -d <data(dec)> -s <SK> -m <Module>  -o .\\result\\result_i\\rsa-out.txt
    """
    # Run RSA
    print("Running RSA ....")
    if check_rsa and mode.get() == 'e':
        os.system(".\\rsa\\rsa.exe -d {} -p {} -m {}  -o {}\\rsa-out.txt".format(des_key.get(), rsa_key.get(), rsa_mod.get(), save_dir))
    elif check_rsa and mode.get() == 'd':
        os.system(".\\rsa\\rsa.exe -d {} -s {} -m {}  -o {}\\rsa-out.txt".format(des_key.get(), rsa_key.get(), rsa_mod.get(), save_dir))
    elif rsa_init.get() == 1:
        os.system(".\\rsa\\rsa.exe -d {} --init  -o {}\\rsa-out.txt".format(des_key.get(), save_dir))
    # GET str_key
    fopen = open("{}\\rsa-out.txt".format(save_dir), 'r')
    fileread = fopen.read()
    fopen.close()
    str_key = ""
    if mode.get() == 'd':
        str_key = re.search(r'Result:[0-9]+',fileread).group()[7:]
    else:
        str_key = des_key.get()

    # Run DES

    print("Running DES ....")
    for path_i in in_path:
        fn = path_i.split('/')[-1]
        os.system(".\\des\\des.exe {} {} 0 + {} = {}\\des-out\\{}".format(path_i, mode.get(), str_key, save_dir, fn))
    
    os.system("move des-log.txt {}\\".format(save_dir))
    print(tk.messagebox.showinfo(title='INFO',message='Finished,and log had been saved in des-log.txt'))
    in_path.clear()


tk.Button(window, text='run', font=('Arial', 12), width=10, height=1, command=run).pack()
tk.Label(window, bg=None, width=20, text='').pack()

window.mainloop()