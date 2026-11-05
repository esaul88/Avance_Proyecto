import tkinter
import serial

txtcolor = "#eab4b3"
interfaz = tkinter.Tk(screenName="CONTROL INTERFACE")
interfaz.geometry("900x600")
interfaz.resizable(width=False, height=False)
interfaz.configure(bg="#928d8d")

def marcos():
    global marco1, marco2, marco3    
    marco1 = tkinter.LabelFrame(interfaz, text="Control PANEL", bg="#040000", fg="white", font=("Arial", 11, "bold"), padx=20, pady=20)
    marco1.grid(row=1, column=0, padx=25, pady=10)    
    
    marco2 = tkinter.LabelFrame(interfaz, text="STATUS", bg="#040000", fg="white", font=("Arial", 11, "bold"), padx=20, pady=20)
    marco2.grid(row=1, column=1, padx=25, pady=10)
    
    marco3 = tkinter.LabelFrame(interfaz, text="SWITCH", bg="#040000", fg="white", font=("Arial", 11, "bold"), padx=20, pady=20)
    marco3.grid(row=2, padx=25, pady=10)

def labels():
    label1 = tkinter.Label(interfaz, text="IR NEC Protocolol", font=("Arial", 18, "bold"), bg="#020000", fg="white")
    label1.grid(row=0, column=0, columnspan=3, pady=25)

    label2 = tkinter.Label(marco1, text="Enviar comando", bg="#040000", fg="white", font=("Arial", 10))
    label2.grid(row=0, column=0, padx=10, pady=(5, 2))

    label3 = tkinter.Label(marco2, text="Info recibida", bg="#040000", fg="white", font=("Arial", 10))
    label3.grid(row=0, column=0, padx=10, pady =(5))

    label4 = tkinter.Label(marco3, text="RECEIVE -- SEND", bg="#040000", fg="white", font=("Arial", 10))
    label4.grid(row=0, column=0, padx=10, pady =(5))

def commands():
    txt1 = tkinter.Entry(marco1, width=12, font=("Arial", 12), bg=txtcolor)
    txt1.grid(row=1, column=0, padx=15, pady=5)
    # txt2 = tkinter.Entry(marco1, width=12, font=("Arial", 12), bg=txtcolor)
    # txt2.grid(row=1, column=1, padx=15, pady=5)

def buttons():
    button1 = tkinter.Button(marco1, text="1", width=10, height=3)
    button1.grid(row=2, column=0, padx=10, pady=15)
    button2 = tkinter.Button(marco3, text="2", width=10, height=3)
    button2.grid(row=2, column=0, padx=10, pady=15)
    button3 = tkinter.Button(marco3, text="3", width=10, height=3)
    button3.grid(row=2, column=1, padx=10, pady=15)

marcos() 
labels() 
commands() 
buttons()  

interfaz.mainloop()