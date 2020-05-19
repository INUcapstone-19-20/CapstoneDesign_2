import sys, math, random, threading
import time, serial
import PyQt5.QtGui
from PyQt5.QtWidgets import *
from PyQt5.QtGui import *
from PyQt5.QtCore import *
from PyQt5 import uic
from functools import partial
import Mine_setUp


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

class SerThread(QThread):
    clickChanged = pyqtSignal(int)

    def __init__(self):
        QThread.__init__(self)
        self.onClick = 0

    def run(self): 
        while True:
            self.onClick = Mine_setUp.click_FromArduino()
            if self.onClick == 1:
                self.clickChanged.emit(Mine_setUp.count_turn)
            elif self.onClick == 99:
                self.clickChanged.emit(99)

# class ClickThread(QThread):
#     def __init__(self, button, file):
#         self.button = button
#         self.file = file

#     def run(self):
#         global click
#         if click:
#             eye = str(Mine_setUp.count_turn)
#             stylesheet = 'image:url(res/' + self.file + eye + 'png); border:0px;'
#             self.color.setStyleSheet(stylesheet)

class DiceThread(QThread):
    cntChanged = pyqtSignal(int)
    result = pyqtSignal(int)

    def __init__(self, limit = 30):
        QThread.__init__(self)
        self.limit = limit
        # self.cntChanged = pyqtSignal(int)
        # self.result = pyqtSignal(int)

    def run(self):
        print("Start DiceThread(" + str(self.limit) + ")")        
        cnt = 0
        while cnt < self.limit:
            self.cntChanged.emit(cnt % 4)
            cnt += 1
            time.sleep(0.03)

        print("Finish DiceThread(" + str(self.limit) + ")")
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
    elif(screen_number == 11): before.main = BattleMode()
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
        self.btn_countup.clicked.connect(self.countUp)
        self.btn_countdown.clicked.connect(self.countDown)
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
        Mine_setUp.set_Mine()

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
        global blue_turn, red_turn
        self.timer = DiceThread(60)
        
        if blue_turn == red_turn:
            self.check_blue = False
            self.check_red = False
            self.btn_bluedice.setEnabled(True)
            self.btn_reddice.setEnabled(True)
            self.btn_bluedice.setStyleSheet('image:url(res/bluedice_default.png); border:0px;')
            self.btn_reddice.setStyleSheet('image:url(res/reddice_default.png); border:0px;')
        elif blue_turn > red_turn:
            Mine_setUp.turn_ToArduino("B")
            self.btn_bluedice.move(165, 110)
            self.btn_reddice.hide()
            self.timer.finished.connect(self.killThread)
            self.timer.start()
            changeScreen(self,19)
            
        elif red_turn > blue_turn:
            Mine_setUp.turn_ToArduino("R")
            self.btn_reddice.move(165, 110)
            self.btn_bluedice.hide()
            self.timer.finished.connect(partial(changeScreen, self, 15))
            self.timer.start()
            # changeScreen(self,15)
    
    def killThread(self):
        self.timer.quit()

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
            self.timer = DiceThread(50)
            self.timer.finished.connect(self.compareDice)
            self.timer.start()

    def throwBlue(self):
        self.th = DiceThread()
        self.th.cntChanged.connect(self.setBlue)
        self.th.result.connect(self.finishBlue)
        self.btn_bluedice.setEnabled(False)
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
            self.timer = DiceThread(50)
            self.timer.finished.connect(self.compareDice)
            self.timer.start()

    def throwRed(self):
        self.th = DiceThread()
        self.th.cntChanged.connect(self.setRed)
        self.th.result.connect(self.finishRed)
        self.btn_reddice.setEnabled(False)
        self.th.start()

            

class Redturn(QMainWindow):
    def __init__(self):
        super().__init__()
        uic.loadUi("ui/redturn.ui", self)

        self.serth = SerThread()
        self.serth.clickChanged.connect(self.buttonclicked)
        self.serth.start()
        self.eye = 0
        
        self.btn_redturn.clicked.connect(self.throwRed)
        self.btn_redturn.setStyleSheet('image:url(res/reddice_default.png); border:0px;')

    def buttonclicked(self,value):
        self.filename = 'image:url(res/reddice_'
        if self.eye == 1: self.filename += 'one'
        elif self.eye == 2: self.filename += 'two'
        elif self.eye == 3: self.filename += 'three'
        self.filename += str(value) + '.png); border:0px;'
        print("filename : ", self.filename)
        self.btn_redturn.setStyleSheet(self.filename)

            
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
        self.setRed(value)
        self.eye = value
        if self.eye == 0:
            self.timer = DiceThread(80)
            self.timer.finished.connect(partial(changeScreen, self, 19))
            self.timer.start()
        elif self.eye > 0:
            Mine_setUp.count_turn = self.eye
            print("countturn : ",Mine_setUp.count_turn)
            
            # serth = SerThread()
            # serth.clickChanged.connect(self.buttonclicked)
            # serth.start()
            

    def throwRed(self):
        self.th = DiceThread()
        self.th.cntChanged.connect(self.setRed)
        self.th.result.connect(self.finishRed)
        self.th.start()


class Blueturn(QMainWindow):
    def __init__(self):
        super().__init__()
        uic.loadUi("ui/blueturn.ui", self)

        self.eye = 0
        self.serth = SerThread()
        self.serth.clickChanged.connect(self.buttonclicked)
        self.serth.start()

        self.btn_blueturn.clicked.connect(self.throwBlue)
        self.btn_blueturn.setStyleSheet('image:url(res/bluedice_default.png); border:0px;')

    def buttonclicked(self,value):
        self.filename = 'image:url(res/bluedice_'
        if self.eye == 1: self.filename += 'one'
        elif self.eye == 2: self.filename += 'two'
        elif self.eye == 3: self.filename += 'three'
        self.filename += str(value) + '.png); border:0px;'
        print("filename : ", self.filename)
        self.btn_blueturn.setStyleSheet(self.filename)

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
        self.setBlue(value)
        self.eye = value

        if self.eye == 0:
            self.timer = DiceThread(50)
            self.timer.finished.connect(partial(changeScreen, self, 15))
            self.timer.start()
        elif self.eye > 0:
            Mine_setUp.count_turn = self.eye
            print("countturn : ", Mine_setUp.count_turn)
        # else:
            # Mine_setUp.turn_ToArduino("B")
        # self.check_blue = True
        # if self.check_blue & self.check_red:
        #     self.th = DiceThread()
        #     self.th.finished.connect(self.compareDice)
        #     self.th.start()

    def throwBlue(self):
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

# def connectDice():
#     serth = DiceThread(10)
#     # serth.clickChanged.connect(func)
#     serth.start()

if __name__ == '__main__':
    app = QApplication(sys.argv)
    # app.setOverrideCursor(Qt.BlankCursor)

    # connectDice()
    # serth = SerThread()
    # serth.start()

    # GUI 시작
    ex = ModeSelect()
    # ex.showFullScreen()
    ex.show()
    sys.exit(app.exec_())
