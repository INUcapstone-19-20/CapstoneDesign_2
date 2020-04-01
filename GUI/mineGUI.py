import tkinter as tk                
from tkinter import font  as tkfont
from tkinter import ttk

class mineGUI(tk.Tk):

    def __init__(self, *args, **kwargs):
        tk.Tk.__init__(self, *args, **kwargs)

        self.title_font = tkfont.Font(family='NanumSquareRound', size=40, weight="bold", slant="roman")
        self.common_font = tkfont.Font(family='NanumSquareRound', size=30, weight="bold", slant="roman")

        container = tk.Frame(self)
        container.pack(side="top", fill="both", expand=True)
        container.grid_rowconfigure(0, weight=1)
        container.grid_columnconfigure(0, weight=1)

        self.frames = {}
        for F in (모드선택, 싱글모드, 배틀모드):
            page_name = F.__name__
            frame = F(parent=container, controller=self)
            self.frames[page_name] = frame
            frame.grid(row=0, column=0, sticky="nsew")

        self.show_frame("모드선택")

    def show_frame(self, page_name):
        '''Show a frame for the given page name'''
        frame = self.frames[page_name]
        frame.tkraise()


class 모드선택(tk.Frame):

    def __init__(self, parent, controller):
        
        tk.Frame.__init__(self, parent)
        self.controller = controller

        # background_image = tk.PhotoImage(file='loading.png')
        # background_label = tk.Label(self, image=background_image)
        # background_label.place(relwidth=1, relheight=1)

        button1 = tk.Button(self, bg="#E0E0E0", text="싱글", font=controller.title_font, 
                            highlightcolor="#E0E0E0",command=lambda: controller.show_frame("싱글모드"))
        button2 = tk.Button(self, highlightcolor="#AEAEAE", bg="#AEAEAE", text="배틀", font=controller.title_font,
                            highlightbackground="#AEAEAE",command=lambda: controller.show_frame("배틀모드"))
        button1.place(x=0,relwidth=0.5,relheight=1)
        button2.place(relx=0.5,rely=-0.5,relwidth=0.5,relheight=2)

        label1 = tk.Label(self, text="[ 모드", font=controller.common_font, bg="#E0E0E0")
        label2 = tk.Label(self, text="선택 ]", font=controller.common_font, bg="#AEAEAE")
        label1.place(relx=0.28,y=20)
        label2.place(relx=0.51,y=20)
        
        
        
        


class 싱글모드(tk.Frame):

    def __init__(self, parent, controller):
        tk.Frame.__init__(self, parent, bg="#e0e0e0")
        self.controller = controller
        label = tk.Label(self, text="지뢰찾기", font=controller.title_font, bg="#e0e0e0")
        label.pack(pady=30,anchor="center")
        button3 = tk.Button(self, text="[ 게임 시작 ]", font=controller.common_font, bg="#e0e0e0",
                           highlightthickness = 0, bd=0, command=lambda: controller.show_frame("모드선택"))
        button4 = tk.Button(self, text="[ 게임 설정 ]", font=controller.common_font, bg="#e0e0e0",
                           highlightthickness = 0, bd=0, command=lambda: controller.show_frame("모드선택"))
        
        button3.pack(pady=20)
        button4.pack()


class 배틀모드(tk.Frame):

    def __init__(self, parent, controller):
        tk.Frame.__init__(self, parent,bg="#e0e0e0")
        img1 = tk.PhotoImage(file="res/reddice_default.png")
        img1 = img1.zoom(16)
        img1 = img1.subsample(32)

        img2 = tk.PhotoImage(file="res/bluedice_default.png")
        img2 = img2.zoom(16)
        img2 = img2.subsample(32)

        # style = ttk.Style()
        # style.theme_use()
        # style.configure('Custom.myButton', bordercolor="red")

        self.controller = controller
        label = tk.Label(self, text="[ 순서 정하기 ]",bg="#e0e0e0", font=controller.common_font)
        label.pack(side="top", fill="x", pady=20)
        red_button = tk.Button(self, text="red",highlightthickness = 0, bd = 0, image=img1,
                           command=lambda: controller.show_frame("모드선택"))
        red_button.image = img1
        red_button.place(relx=0.17, rely=0.35)

        blue_button = tk.Button(self, text="red",highlightthickness = 0, bd = 0, image=img2,
                           command=lambda: controller.show_frame("모드선택"))
        blue_button.image = img2
        blue_button.place(relx=0.57, rely=0.35)
        
        

if __name__ == "__main__":
    app = mineGUI()
    app.attributes("-fullscreen", True)
    # app.geometry("480x320")
    
    app.mainloop()
