import tkinter
from ftplib import FTP
from tkinter import messagebox


class FTPWindows:
    def __init__(self, root):
        self.ftp = None
        self.root = root
        self.root.title("叶茂林的FTP")
        self.root.geometry("350x220")

        tkinter.Label(self.root, text="服务器：").grid(row=0, column=5)
        self.severEntry = tkinter.Entry(self.root)
        self.severEntry.grid(row=0, column=6, columnspan=25)

        tkinter.Label(self.root, text="用户名：").grid(row=1, column=5)
        self.userEntry = tkinter.Entry(self.root)
        self.userEntry.grid(row=1, column=6, columnspan=25)

        tkinter.Label(self.root, text="密码：").grid(row=2, column=5)
        self.passwordEntry = tkinter.Entry(self.root)
        self.passwordEntry.grid(row=2, column=6, columnspan=25)

        self.loginButton = tkinter.Button(self.root, text="登陆", command=self.login)
        self.loginButton.grid(row=3, column=18)

        self.fileListbox = tkinter.Listbox(self.root)
        self.fileListbox.grid(row=0, column=45, rowspan=3, columnspan=30)

        self.loadButton = tkinter.Button(self.root, text="下载", command=self.download)
        self.loadButton.grid(row=3, column=60)

    def login(self):
        self.ftp = FTP(self.severEntry.get())
        self.ftp.login(user=self.userEntry.get(), passwd=self.passwordEntry.get())
        messagebox.showinfo(self.userEntry.get() + "登陆成功")

        self.ftp.cwd('/')
        self.files = self.ftp.nlst()
        for file in self.files:
            self.fileListbox.insert(tkinter.END, file)

    def download(self):
        filename = self.fileListbox.get(tkinter.ACTIVE)
        with open(filename, 'wb') as file:
            self.ftp.retrbinary('RETR' + filename, file.write())
            messagebox.showinfo(filename + "下载成功")


root = tkinter.Tk()
FTPWindows(root)
root.mainloop()