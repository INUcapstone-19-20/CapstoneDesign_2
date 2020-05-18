import sys, math, random, threading
import time, serial
import PyQt5.QtGui
from PyQt5.QtWidgets import *
from PyQt5.QtGui import *
from PyQt5.QtCore import *
from PyQt5 import uic
from functools import partial
import Mine_setUp


ser = serial.Serial(
    port='/dev/cu.usbmodem14201',
    baudrate=115200,
    timeout = 1
)

countTurn = 0

def Decode(A):
    A = A.decode()
    A = str(A)
    return A

def serRead():
    if ser.readable():
        global countTurn
        try:    
            LINE = ser.readline()
            code = Decode(LINE)
            print("code : ", code, end='\n')
            if "Click" in code :
                countTurn -= 1
                print("!!!!!!!!!!!!!!!!!!!!")
            print("countTurn : ",countTurn)
        except serial.serialutil.SerialException:
            time.sleep(1)
    else :
        print("읽기 실패 from_serRead_")

# while (True):
#     serRead()


class SerThread(QThread):
    
    def run(self):        
        while True:
            serRead()
            # time.sleep(0.001)

# Single Mode
count = 10
timer = 40

# Battle Mode
red_turn = 0
blue_turn = 0

#serial port
# ser = serial.Serial(
#     port='/dev/ttyACM0',
#     baudrate=115200
# )

class DiceThread(QThread):
    cntChanged = pyqtSignal(int)
    result = pyqtSignal(int)
    def run(self):        
        cnt = 0
        while cnt < 30:
            self.cntChanged.emit(cnt % 4)
            cnt += 1
            time.sleep(0.03)

        rand = random.randint(0,3)
        self.result.emit(rand)
            

# Screen change
def changeScreen(before, screen_number):
    if(screen_number == 1): before.main = Start()
    elif(screen_number == 2): before.main = ModeSelect()
    elif(screen_number == 3): before.main = SingleMode()
    elif(screen_number == 4): before.main = Single_Setting()
    elif(screen_number == 8): before.main = Single_Win()
    elif(screen_number == 9): before.main = Single_Loose()
    elif(screen_number == 10): before.main = Replay_Game()
    elif(screen_number == 11):
        before.main = BattleMode()
        Mine_setUp.Mine_ToArduino("009","012")
        Mine_setUp.Turn_ToArduino("B")
    elif(screen_number == 15): before.main = Redturn()
    elif(screen_number == 19): before.main = Blueturn()
    elif(screen_number == 20): before.main = Result()
    elif(screen_number == 21): before.main = Single_Default()
    elif(screen_number == 23): before.main = Blue_Loose()
    elif(screen_number == 24): before.main = Red_Loose()
    
    # before.main.showFullScreen()
    before.main.show()
    before.close()


class Start(QMainWindow):
    def __init__(self):
        super().__init__()
        uic.loadUi("ui/start.ui", self)
        self.btn_bomb.setStyleSheet('image:url(res/bomb3.png); border:0px;')
        
class ModeSelect(QMainWindow):
    def __init__(self):
        super().__init__()
        uic.loadUi("ui/modeselect.ui", self)
        # Button connect
        self.btn_single.clicked.connect(partial(changeScreen, self, 3))
        self.btn_battle.clicked.connect(partial(changeScreen, self, 11))

class SingleMode(QMainWindow):
    def __init__(self):
        super().__init__()
        uic.loadUi("ui/singlemode.ui", self)
        
        self.btn_singlesetting.clicked.connect(partial(changeScreen, self, 4))
        self.btn_singlestart.clicked.connect(partial(changeScreen, self, 21))


class Single_Setting(QMainWindow):
    def __init__(self):
        super().__init__()
        uic.loadUi("ui/single_setting.ui", self)
        self.temp_count = count
        self.temp_time = timer
        self.setLabel()

        # Button Function
        # self.btn_countup.clicked.connect(self.countUp)
        # self.btn_countdown.clicked.connect(self.countDown)
        self.btn_countup.clicked.connect(self.tmp1)
        self.btn_countdown.clicked.connect(self.tmp2)
        self.btn_timerup.clicked.connect(self.timeUp)
        self.btn_timerdown.clicked.connect(self.timeDown)
        self.btn_settingsave.clicked.connect(self.settingSave)
        self.btn_settingback.clicked.connect(partial(changeScreen, self, 3))

        # Button image
        self.btn_settingback.setStyleSheet('image:url(res/btn_back1.png); border:0px;')
        self.btn_countup.setStyleSheet('image:url(res/btn_up.png); border:0px;')
        self.btn_countdown.setStyleSheet('image:url(res/btn_down.png); border:0px;')
        self.btn_timerup.setStyleSheet('image:url(res/btn_up.png); border:0px;')
        self.btn_timerdown.setStyleSheet('image:url(res/btn_down.png); border:0px;')
        
        
    def setLabel(self):
        time_minute = math.floor(self.temp_time / 60)
        time_second = self.temp_time % 60
        str_minute = ""
        str_second = ""

        if time_minute < 10: str_minute = "0" + str(time_minute)
        else : str_minute = str(time_minute)
        if time_second < 10: str_second = "0" + str(time_second)
        else : str_second = str(time_second)

        self.lb_count.setText(str(self.temp_count))
        self.lb_time.setText(str_minute + ":" + str_second)

    def tmp1(self):
        Mine_setUp.Mine_ToArduino("009","012")

    def tmp2(self):
        Mine_setUp.Turn_ToArduino("B")

    def countUp(self):
        self.temp_count += 1
        self.setLabel()

    def countDown(self):
        if self.temp_count > 0: self.temp_count -= 1
        self.setLabel()

    def timeUp(self):
        self.temp_time += 5
        self.setLabel()

    def timeDown(self):
        if self.temp_time > 0: self.temp_time -= 5
        self.setLabel()
        
    def settingSave(self):
        global count, timer
        count = self.temp_count
        timer = self.temp_time

class Single_Win(QMainWindow):
    def __init__(self):
        super().__init__()
        uic.loadUi("ui/single_win.ui", self)

        self.btn_restart.clicked.connect(partial(changeScreen, self, 10))
        self.btn_menu.clicked.connect(partial(changeScreen, self, 2))

class Single_Loose(QMainWindow):
    def __init__(self):
        super().__init__()
        uic.loadUi("ui/single_loose.ui", self)

        self.btn_restart2.clicked.connect(partial(changeScreen, self, 10))
        self.btn_menu2.clicked.connect(partial(changeScreen, self, 2))
        

class Replay_Game(QMainWindow):
    def __init__(self):
        super().__init__()
        uic.loadUi("ui/Replay_Game.ui", self)

        self.btn_reset.setStyleSheet('image:url(res/replay.png); border:0px;')
        self.btn_reset.clicked.connect(partial(changeScreen, self, 2))


class BattleMode(QMainWindow):
    def __init__(self):
        super().__init__()
        uic.loadUi("ui/battlemode.ui", self)
        
        self.check_blue = False
        self.check_red = False

        # self.playLabel = QLabel(self)
        # self.playLabel.resize(120, 120)
        # self.playLabel.move(75, 125)

        # movie = QMovie("res/bluedice_random(pass.gif")
        # self.playLabel.setMovie(movie)
        # movie.start()
        # self.playLabel.setScaledContents(True)
        # self.playLabel.hide()
        
        self.btn_bluedice.clicked.connect(self.throwBlue)
        self.btn_reddice.clicked.connect(self.throwRed)

        self.btn_bluedice.setStyleSheet('image:url(res/bluedice_default.png); border:0px;')
        self.btn_reddice.setStyleSheet('image:url(res/reddice_default.png); border:0px;')

    def compareDice(self):
        print("compare!")
        global blue_turn, red_turn
        self.th = DiceThread()
        
        if blue_turn == red_turn:
            self.check_blue = False
            self.check_red = False
            self.btn_bluedice.setStyleSheet('image:url(res/bluedice_default.png); border:0px;')
            self.btn_reddice.setStyleSheet('image:url(res/reddice_default.png); border:0px;')
        elif blue_turn > red_turn:
            self.btn_bluedice.move(165, 110)
            self.btn_reddice.hide()
            self.th.finished.connect(partial(changeScreen, self, 19))
            self.th.start()
        elif red_turn > blue_turn:
            self.btn_reddice.move(165, 110)
            self.btn_bluedice.hide()
            self.th.finished.connect(partial(changeScreen, self, 15))
            self.th.start()

    def setBlue(self, value):
        if(value == 0):
            self.btn_bluedice.setStyleSheet('image:url(res/bluedice_pass.png); border:0px;')
        elif(value == 1):
            self.btn_bluedice.setStyleSheet('image:url(res/bluedice_one1.png); border:0px;')
        elif(value == 2):
            self.btn_bluedice.setStyleSheet('image:url(res/bluedice_two2.png); border:0px;')
        elif(value == 3):
            self.btn_bluedice.setStyleSheet('image:url(res/bluedice_three3.png); border:0px;')
            
    def finishBlue(self, value):
        global blue_turn
        blue_turn = value
        self.setBlue(blue_turn)
        print(blue_turn)
        self.check_blue = True
        if self.check_blue & self.check_red:
            self.th = DiceThread()
            self.th.finished.connect(self.compareDice)
            self.th.start()

    def throwBlue(self):
        global blue_turn
        self.th = DiceThread()
        self.th.cntChanged.connect(self.setBlue)
        self.th.result.connect(self.finishBlue)
        self.th.start()

    def setRed(self, value):
        if(value == 0):
            self.btn_reddice.setStyleSheet('image:url(res/reddice_pass.png); border:0px;')
        elif(value == 1):
            self.btn_reddice.setStyleSheet('image:url(res/reddice_one1.png); border:0px;')
        elif(value == 2):
            self.btn_reddice.setStyleSheet('image:url(res/reddice_two2.png); border:0px;')
        elif(value == 3):
            self.btn_reddice.setStyleSheet('image:url(res/reddice_three3.png); border:0px;')

    def finishRed(self, value):
        global red_turn
        red_turn = value
        self.setRed(red_turn)
        print(red_turn)
        self.check_red = True
        if self.check_blue & self.check_red:
            self.th = DiceThread()
            self.th.finished.connect(self.compareDice)
            self.th.start()

    def throwRed(self):
        self.th = DiceThread()
        self.th.cntChanged.connect(self.setRed)
        self.th.result.connect(self.finishRed)
        self.th.start()

            

class Redturn(QMainWindow):
    # red_turn = 0
    # check_red = False
    # sequence = 1
    # cycle = 0

    def __init__(self):
        super().__init__()
        uic.loadUi("ui/redturn.ui", self)

        self.btn_redturn.clicked.connect(self.throwRed)
        self.btn_redturn.setStyleSheet('image:url(res/reddice_default.png); border:0px;')

    def setRed(self, value):
        if(value == 0):
            self.btn_redturn.setStyleSheet('image:url(res/reddice_pass.png); border:0px;')
        elif(value == 1):
            self.btn_redturn.setStyleSheet('image:url(res/reddice_one1.png); border:0px;')
        elif(value == 2):
            self.btn_redturn.setStyleSheet('image:url(res/reddice_two2.png); border:0px;')
        elif(value == 3):
            self.btn_redturn.setStyleSheet('image:url(res/reddice_three3.png); border:0px;')

    def finishRed(self, value):
        global red_turn
        red_turn = value
        self.setRed(red_turn)

        if red_turn == 0:
            changeScreen(self, 19)
        else:
            Mine_setUp.Turn_ToArduino("R")

        # self.check_red = True
        # if self.check_blue & self.check_red:
        #     self.th = DiceThread()
        #     self.th.finished.connect(self.compareDice)
        #     self.th.start()

    def throwRed(self):
        self.th = DiceThread()
        self.th.cntChanged.connect(self.setRed)
        self.th.result.connect(self.finishRed)
        self.th.start()

class Blueturn(QMainWindow):
    def __init__(self):
        super().__init__()
        uic.loadUi("ui/blueturn.ui", self)

        self.btn_blueturn.clicked.connect(self.throwBlue)
        self.btn_blueturn.setStyleSheet('image:url(res/bluedice_default.png); border:0px;')

    def setBlue(self, value):
        if(value == 0):
            self.btn_blueturn.setStyleSheet('image:url(res/bluedice_pass.png); border:0px;')
        elif(value == 1):
            self.btn_blueturn.setStyleSheet('image:url(res/bluedice_one1.png); border:0px;')
        elif(value == 2):
            self.btn_blueturn.setStyleSheet('image:url(res/bluedice_two2.png); border:0px;')
        elif(value == 3):
            self.btn_blueturn.setStyleSheet('image:url(res/bluedice_three3.png); border:0px;')
            
    def finishBlue(self, value):
        global blue_turn
        blue_turn = value
        self.setBlue(blue_turn)
        
        if blue_turn == 0:
            changeScreen(self, 15)
        else:
            Mine_setUp.Turn_ToArduino("B")
        # self.check_blue = True
        # if self.check_blue & self.check_red:
        #     self.th = DiceThread()
        #     self.th.finished.connect(self.compareDice)
        #     self.th.start()

    def throwBlue(self):
        global blue_turn
        self.th = DiceThread()
        self.th.cntChanged.connect(self.setBlue)
        self.th.result.connect(self.finishBlue)
        self.th.start()


class Result(QMainWindow):
    def __init__(self):
        super().__init__()
        uic.loadUi("ui/Result.ui", self)

        self.btn_restart.clicked.connect(partial(changeScreen, self, 11))
        self.btn_new.clicked.connect(partial(changeScreen, self, 2))

class Single_Default(QMainWindow):
    def __init__(self):
        super().__init__()
        uic.loadUi("ui/single_default.ui", self)

        time_minute = math.floor(timer / 60)
        time_second = timer % 60
        str_minute = ""
        str_second = ""

        if time_minute < 10: str_minute = "0" + str(time_minute)
        else : str_minute = str(time_minute)
        if time_second < 10: str_second = "0" + str(time_second)
        else : str_second = str(time_second)

        self.lb_count.setText(str(count))
        self.lb_time.setText(str_minute + ":" + str_second)

class Blue_Loose(QMainWindow):
    def __init__(self):
        super().__init__()
        uic.loadUi("ui/blue_loose.ui", self)

        self.btn_bbom.clicked.connect(partial(changeScreen, self, 20))

class Red_Loose(QMainWindow):
    def __init__(self):
        super().__init__()
        screen24 = uic.loadUi("ui/red_loose.ui", self)

        self.btn_rbom.clicked.connect(partial(changeScreen, self, 20))



if __name__ == '__main__':
    app = QApplication(sys.argv)
    # app.setOverrideCursor(Qt.BlankCursor)

    serth = SerThread()
    serth.start()

    ex = ModeSelect()
    # ex.showFullScreen()
    ex.show()
    sys.exit(app.exec_())
