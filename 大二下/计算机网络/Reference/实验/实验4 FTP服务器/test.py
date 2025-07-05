import os
from ftplib import FTP
import tkinter as tk
from tkinter import messagebox
from tkinter import filedialog

class FTPClientGUI:
    def __init__(self, master):
        self.master = master
        master.title("FTP Client")
        master.geometry("400x300")

        # FTP服务器地址、用户名和密码输入框
        tk.Label(master, text="FTP Server:").grid(row=0, column=0)
        self.server_entry = tk.Entry(master)
        self.server_entry.grid(row=0, column=1)
        tk.Label(master, text="Username:").grid(row=1, column=0)
        self.username_entry = tk.Entry(master)
        self.username_entry.grid(row=1, column=1)
        tk.Label(master, text="Password:").grid(row=2, column=0)
        self.password_entry = tk.Entry(master, show="*")
        self.password_entry.grid(row=2, column=1)

        # 登录按钮
        self.login_button = tk.Button(master, text="Login", command=self.login)
        self.login_button.grid(row=3, column=0, columnspan=2)

        # 文件列表框
        self.file_listbox = tk.Listbox(master)
        self.file_listbox.grid(row=4, column=0, columnspan=2, sticky="nsew")
        scroll = tk.Scrollbar(master, command=self.file_listbox.yview)
        scroll.grid(row=4, column=2, sticky="nsew")
        self.file_listbox.config(yscrollcommand=scroll.set)

        # 上传、下载、删除按钮
        self.upload_button = tk.Button(master, text="Upload", command=self.upload)
        self.upload_button.grid(row=5, column=0)
        self.download_button = tk.Button(master, text="Download", command=self.download)
        self.download_button.grid(row=5, column=1)
        self.delete_button = tk.Button(master, text="Delete", command=self.delete)
        self.delete_button.grid(row=6, column=0, columnspan=2)

    def login(self):
        try:
            # 连接FTP服务器并登录
            self.ftp = FTP(self.server_entry.get())
            self.ftp.login(user=self.username_entry.get(), passwd=self.password_entry.get())

            # 列出FTP服务器上的文件列表
            self.ftp.cwd('/')
            self.files = self.ftp.nlst()
            self.file_listbox.delete(0, tk.END)
            for file in self.files:
                self.file_listbox.insert(tk.END, file)

            messagebox.showinfo("FTP Client", "Login success!")
        except Exception as e:
            messagebox.showerror("FTP Client", "Login failed: " + str(e))

    def upload(self):
        try:
            # 选择本地文件并上传到FTP服务器
            filename = filedialog.askopenfilename()
            with open(filename, 'rb') as f:
                self.ftp.storbinary('STOR ' + os.path.basename(filename), f)
            self.files.append(os.path.basename(filename))
            self.file_listbox.insert(tk.END, os.path.basename(filename))
            messagebox.showinfo("FTP Client", "Upload success!")
        except Exception as e:
            messagebox.showerror("FTP Client", "Upload failed: " + str(e))

    def download(self):
        try:
            # 选择FTP服务器上的文件并下载到本地
            filename = self.file_listbox.get(tk.ACTIVE)
            with open(filename, 'wb') as f:
                self.ftp.retrbinary('RETR ' + filename, f.write)
            messagebox.showinfo("FTP Client", "Download success!")
        except Exception as e:
            messagebox.showerror("FTP Client", "Download failed: " + str(e))

    def delete(self):
        try:
            # 选择FTP服务器上的文件并删除
            filename = self.file_listbox.get(tk.ACTIVE)
            self.ftp.delete(filename)
            self.files.remove(filename)
            self.file_listbox.delete(tk.ACTIVE)
            messagebox.showinfo("FTP Client", "Delete success!")
        except Exception as e:
            messagebox.showerror("FTP Client", "Delete failed: " + str(e))

root = tk.Tk()
app = FTPClientGUI(root)
root.mainloop()
