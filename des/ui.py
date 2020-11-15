import tkinter as tk
from tkinter import Message, Variable, filedialog, messagebox, Text
import os
from typing import Text

# CREATE WINDOW
window = tk.Tk()
window.title('DES')
window.geometry('500x600')

# GET MODE
mode = tk.StringVar()
tk.Label(window, bg='yellow', width=20, text='Encode or Deocde ?').pack()
tk.Radiobutton(window, text='Encode', variable=mode, value='e', command=None).pack()
tk.Radiobutton(window, text='Decode', variable=mode, value='d', command=None).pack()
tk.Label(window, bg=None, width=20, text='').pack()
# GET ALGO
algo = tk.StringVar()
tk.Label(window, bg='yellow', width=20, text='Select a algorithm').pack()
tk.Radiobutton(window, text='DES', variable=algo, value='0', command=None).pack()
tk.Radiobutton(window, text='DES3EEE', variable=algo, value='1', command=None).pack()
tk.Radiobutton(window, text='DES3EDE', variable=algo, value='2', command=None).pack()
tk.Radiobutton(window, text='DES2EEE', variable=algo, value='3', command=None).pack()
tk.Radiobutton(window, text='DES2EDE', variable=algo, value='4', command=None).pack()
tk.Label(window, bg=None, width=20, text='').pack()

#OPEN FILES
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

#GET KEYS
key1 = tk.StringVar()
key2 = tk.StringVar()
key3 = tk.StringVar()
tk.Label(window, bg='yellow', width=20, text='Input your key').pack()
tk.Entry(window, show=None, font=('Arial', 14), textvariable=key1).pack()
tk.Entry(window, show=None, font=('Arial', 14), textvariable=key2).pack()
tk.Entry(window, show=None, font=('Arial', 14), textvariable=key3).pack()
tk.Label(window, bg=None, width=20, text='').pack()
#RUN
def run():
    if len(in_path) == 0:
        print(tk.messagebox.showerror(title='ERROR',message='No input file!'))
        return
    if len(mode.get()) == 0:
        print(tk.messagebox.showerror(title='ERROR',message='Encode or Deocde ?'))
        return
    if len(algo.get()) == 0:
        print(tk.messagebox.showerror(title='ERROR',message='Please select a algorithm'))
        return
    if len(key1.get()) == 0:
        print(tk.messagebox.showwarning(title='WARNING',message='No input keys, and random alphanumeric key(s) may be generated.\nSee log.txt for details'))
    keys = ''
    if(len(key1.get())!=0):
        keys = keys + ' + ' + key1.get()
    if(len(key2.get())!=0):
        keys = keys + ' + ' + key2.get()
    if(len(key3.get())!=0):
        keys = keys + ' + ' + key3.get()
    # $ des.exe  <in_path>  [mode] [algo] + [key1] + [key2] + [key3]
    for path_i in in_path:
        os.system('des.exe'+' '+path_i+' '+mode.get()+' '+algo.get()+' '+keys)
    print(tk.messagebox.showinfo(title='INFO',message='Finished,and log had been saved in log.txt'))
    in_path.clear()


tk.Button(window, text='run', font=('Arial', 12), width=10, height=1, command=run).pack()
tk.Label(window, bg=None, width=20, text='').pack()

window.mainloop()