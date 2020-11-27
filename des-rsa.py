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

def Encode(des_key, rsa_pk, rsa_module, rsa_init, check_rsa, save_dir, in_path):

    if check_rsa:
        os.system(".\\rsa\\rsa.exe -d {} -p {} -m {}  -o {}\\rsa-out.txt".format(des_key, rsa_pk, rsa_module, save_dir))
    elif rsa_init == 1:
        os.system(".\\rsa\\rsa.exe -d {} --init  -o {}\\rsa-out.txt".format(des_key, save_dir))

    header = ""
    with open("{}\\rsa-out.txt".format(save_dir), 'r') as f:
        fr = f.read()
        r_s  = re.search(r'Result:[0-9]+',fr).group()[7:]
        if  rsa_init == 1:
            r_m = re.search(r'Module:[0-9]+',fr).group()[7:]
        else:
            r_m = rsa_module
        header = (40 - len(r_s))*'0' +  r_s + (40 - len(r_m))*'0' + r_m;
    for path_i in in_path:
        fn = path_i.split('/')[-1]
        os.system(".\\des\\des.exe {} {} 0 --dec {} --head {} = {}\\out\\{}".format(path_i, mode.get(), des_key, header, save_dir, fn))

def Decode(rsa_sk, save_dir, file):
    rsa_input = ""
    rsa_module = ""
    des_key = ""
    fn = file.split('/')[-1]
    temp_file = ".\\{}\\temp\\{}".format(save_dir, fn)
    with open(file, "rb") as f:
        content = f.read()
        rsa_input = str(content[:40], encoding="utf-8")
        rsa_module = str(content[40:80], encoding="utf-8")
        with open(temp_file, "wb+") as f:
            f.write(content[80:])
    os.system(".\\rsa\\rsa.exe -d {} -s {} -m {}  -o {}\\rsa-out.txt".format(rsa_input, rsa_sk, rsa_module, save_dir))
    with open("{}\\rsa-out.txt".format(save_dir), 'r') as f:
        fr = f.read()
        des_key = re.search(r'Result:[0-9]+',fr).group()[7:]
    os.system(".\\des\\des.exe {} d 0 --dec {} = .\\{}\\out\\{}".format(temp_file, des_key, save_dir, fn))



# RUN
def run():
    if len(in_path) == 0:
        print(tk.messagebox.showerror(title='ERROR',message='No input file!'))
        return
    if len(mode.get()) == 0:
        print(tk.messagebox.showerror(title='ERROR',message='Encode or Deocde ?'))
        return
    if len(des_key.get()) == 0 and mode.get() == 'e':
        print(tk.messagebox.showerror(title='ERROR',message='No DES key!'))
    check_rsa = (len(rsa_key.get()) != 0 and mode.get() == 'd') or \
        (len(rsa_key.get()) != 0 and len(rsa_mod.get()) != 0 and mode.get() == 'e')
    if check_rsa == 0 and rsa_init.get() == 0:
         print(tk.messagebox.showerror(title='ERROR',message=
            'No RSA key or module！(input your key and module or choose init a rsa key pair'))
    index_dir = len(os.listdir(".\\result")) + 1
    save_dir = "result\\result{}".format(index_dir)
    os.system("mkdir {}\\out".format(save_dir))

    if mode.get() == 'e':
        Encode(des_key.get(), rsa_key.get(), rsa_mod.get(), rsa_init.get(), check_rsa, save_dir, in_path)
    else:
        os.system('mkdir .\\{}\\temp'.format(save_dir))
        for file in in_path:
            Decode(rsa_key.get(), save_dir, file)
    
    os.system("move des-log.txt {}\\".format(save_dir))
    print(tk.messagebox.showinfo(title='INFO',message='Finished,and log had been saved in des-log.txt'))
    in_path.clear()


tk.Button(window, text='run', font=('Arial', 12), width=10, height=1, command=run).pack()
tk.Label(window, bg=None, width=20, text='').pack()

window.mainloop()