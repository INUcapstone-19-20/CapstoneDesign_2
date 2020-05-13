import sys, math, random, threading
import PyQt5.QtGui
from PyQt5.QtWidgets import QApplication, QMainWindow, QWidget
from PyQt5.QtGui import QIcon
from PyQt5.QtCore import Qt
from PyQt5 import uic
from functools import partial


# Single Mode
count = 10
time = 40

# Battle Mode
# temp = 1
# cycle = 0

red_turn = 0
blue_turn = 0

#serial port
# ser = serial.Serial(
#     port='/dev/ttyACM0',
#     baudrate=115200
# )

# connect UI
screen1 = uic.loadUiType("ui/start.ui")[0]
screen2 = uic.loadUiType("ui/modeselect.ui")[0]
screen3 = uic.loadUiType("ui/singlemode.ui")[0]
screen4 = uic.loadUiType("ui/single_setting.ui")[0]
screen8 = uic.loadUiType("ui/single_win.ui")[0]
screen9 = uic.loadUiType("ui/single_loose.ui")[0]
screen10 = uic.loadUiType("ui/Replay_Game.ui")[0]
screen11 = uic.loadUiType("ui/battlemode.ui")[0]
screen15 = uic.loadUiType("ui/red.ui")[0]
screen19 = uic.loadUiType("ui/blue.ui")[0]
screen20 = uic.loadUiType("ui/Result.ui")[0]
screen21 = uic.loadUiType("ui/single_default.ui")[0]
screen23 = uic.loadUiType("ui/blue_loose.ui")[0]
screen24 = uic.loadUiType("ui/red_loose.ui")[0]

# Screen change
def ChangeScreen(before, screen_number):
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

    before.main.showFullScreen()
    # before.main.show()
    before.close()

class Start(QMainWindow, screen1):
    def __init__(self):
        super().__init__()
        self.setupUi(self)

        self.btn_bomb.setStyleSheet('image:url(res/bomb3.png); border:0px;')
        
class ModeSelect(QMainWindow, screen2):
    def __init__(self):
        super().__init__()
        self.setupUi(self)
        # Button connect
        self.btn_single.clicked.connect(partial(ChangeScreen, self, 3))
        self.btn_battle.clicked.connect(partial(ChangeScreen, self, 11))


class SingleMode(QMainWindow, screen3):
    def __init__(self):
        super().__init__()
        self.setupUi(self)
        
        self.btn_singlesetting.clicked.connect(partial(ChangeScreen, self, 4))
        self.btn_singlestart.clicked.connect(partial(ChangeScreen, self, 21))


class Single_Setting(QMainWindow, screen4):
    temp_count = count
    temp_time = time

    def __init__(self):
        super().__init__()
        self.setupUi(self)
        self.setLabel()

        # Button Function
        self.btn_countup.clicked.connect(self.countUp)
        self.btn_countdown.clicked.connect(self.countDown)
        self.btn_timerup.clicked.connect(self.timeUp)
        self.btn_timerdown.clicked.connect(self.timeDown)
        self.btn_settingsave.clicked.connect(self.settingSave)
        self.btn_settingback.clicked.connect(partial(ChangeScreen, self, 3))

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
        if count > 0: self.temp_count -= 1
        self.setLabel()

    def timeUp(self):
        self.temp_time += 5
        self.setLabel()

    def timeDown(self):
        if time > 0: self.temp_time -= 5
        self.setLabel()
        
    def settingSave(self):
        global count, time
        count = self.temp_count
        time = self.temp_time

class Single_Win(QMainWindow, screen8):
    def __init__(self):
        super().__init__()
        self.setupUi(self)

        self.btn_restart.clicked.connect(partial(ChangeScreen, self, 10))
        self.btn_menu.clicked.connect(partial(ChangeScreen, self, 2))

class Single_Loose(QMainWindow, screen9):
    def __init__(self):
        super().__init__()
        self.setupUi(self)

        self.btn_restart2.clicked.connect(partial(ChangeScreen, self, 10))
        self.btn_menu2.clicked.connect(partial(ChangeScreen, self, 2))
        

class Replay_Game(QMainWindow, screen10):
    def __init__(self):
        super().__init__()
        self.setupUi(self)

        self.btn_reset.setStyleSheet('image:url(res/replay.png); border:0px;')

        self.btn_reset.clicked.connect(partial(ChangeScreen, self, 2))


class BattleMode(QMainWindow, screen11):
    blue_turn = 0
    red_turn = 0
    check_blue = False
    check_red = False
    sequence = 1
    cycle = 0

    def __init__(self):
        super().__init__()
        self.setupUi(self)

        self.btn_bluedice.clicked.connect(self.throwBlue)

        self.btn_bluedice.setStyleSheet('image:url(res/bluedice_default.png); border:0px;')
        self.btn_reddice.setStyleSheet('image:url(res/reddice_default.png); border:0px;')

    def throwBlue(self):
        self.btn_bluedice.setStyleSheet('image:url(res/bluedice_random.gif); border:0px;')

    # def throwBlue(self):
    #     self.blue_turn = random.choice([1,2,3])
    #     print("blue : ", self.blue_turn)
    #     self.changeBlue()

    #     self.check_blue = True
    #     print("check_blue : ", self.check_blue)
    #     if(self.blue_turn & self.red_turn):
    #         timer = threading.Timer(3, self.checkBoth)
    #         timer.start()
    #         print("blue : ", self.blue_turn)

        

    # def changeBlue(self):
    #     timer = threading.Timer(0.03, self.changeBlue)
    #     timer.start()
    #     if self.sequence == 1:
    #         self.btn_bluedice.setStyleSheet('image:url(res/bluedice_one.png); border:0px;')
    #     elif self.sequence == 2:
    #         self.btn_bluedice.setStyleSheet('image:url(res/bluedice_two.png); border:0px;')
    #     elif self.sequence == 3:
    #         self.btn_bluedice.setStyleSheet('image:url(res/bluedice_three.png); border:0px;')
    #     self.sequence += 1
    #     self.cycle += 1
    #     if self.sequence > 3: self.sequence = 1
    #     if self.cycle > 25:
    #         timer.cancel()
    #         self.cycle = 1
    #         if self.blue_turn == 1:
    #             self.btn_bluedice.setStyleSheet('image:url(res/bluedice_one.png); border:0px;')
    #         elif self.blue_turn == 2:
    #             self.btn_bluedice.setStyleSheet('image:url(res/bluedice_two.png); border:0px;')
    #         elif self.blue_turn == 3:
    #             self.btn_bluedice.setStyleSheet('image:url(res/bluedice_three.png); border:0px;')

    # def throwRed(self):
    #     self.red_turn = random.choice([1,2,3])
    #     print("red : ", self.red_turn)
    #     self.changeRed()

    #     self.check_red = True
    #     print("check_red : ", self.check_red)
    #     if(self.blue_turn & self.red_turn):
    #         timer = threading.Timer(3, self.checkBoth)
    #         timer.start()
    #         print("red : ", self.red_turn)
        

    # def changeRed(self):
    #     timer = threading.Timer(0.03, self.changeRed)
    #     timer.start()
    #     if self.sequence == 1:
    #         self.btn_reddice.setStyleSheet('image:url(res/reddice_one.png); border:0px;')
    #     elif self.sequence == 2:
    #         self.btn_reddice.setStyleSheet('image:url(res/reddice_two.png); border:0px;')
    #     elif self.sequence == 3:
    #         self.btn_reddice.setStyleSheet('image:url(res/reddice_three.png); border:0px;')
    #     self.sequence += 1
    #     self.cycle += 1
    #     if self.sequence > 3: self.sequence = 1
    #     if self.cycle > 25:
    #         timer.cancel()
    #         self.cycle = 1
    #         if self.red_turn == 1:
    #             self.btn_reddice.setStyleSheet('image:url(res/reddice_one.png); border:0px;')
    #         elif self.red_turn == 2:
    #             self.btn_reddice.setStyleSheet('image:url(res/reddice_two.png); border:0px;')
    #         elif self.red_turn == 3:
    #             self.btn_reddice.setStyleSheet('image:url(res/reddice_three.png); border:0px;')

    # def checkBoth(self):
    #     print(self.blue_turn)
    #     print(self.red_turn)
    #     if self.blue_turn > self.red_turn:
    #         self.btn_bluedice.move(170,110)
    #         self.btn_reddice.hide()
    #     elif self.blue_turn < self.red_turn:
    #         self.btn_reddice.move(170,110)
    #         self.btn_bluedice.hide()
    #     else:
    #         self.btn_bluedice.setStyleSheet('image:url(res/bluedice_default.png); border:0px;')
    #         self.btn_reddice.setStyleSheet('image:url(res/reddice_default.png); border:0px;')
    #         self.check_blue = False
    #         self.check_red = False
            
            

class Redturn(QMainWindow, screen15):
    red_turn = 0
    check_red = False
    sequence = 1
    cycle = 0
    def __init__(self):
        super().__init__()
        self.setupUi(self)

        self.btn_red.setStyleSheet('image:url(res/reddice_default.png); border:0px;')
        self.btn_red.clicked.connect(self.turnChange)

    def turnChange(self):
        ChangeScreen(self, 19)

        self.btn_redturn.clicked.connect(self.throwRed)
    
    def throwRed(self):
        self.red_turn = random.choice([1,2,3])
        print("red : ", self.red_turn)
        self.changeRed()

        self.check_red = True
        print("check_red : ", self.check_red)
        if(self.blue_turn & self.red_turn):
            timer = threading.Timer(3, self.checkBoth)
            timer.start()
            print("red : ", self.red_turn)
        

    def changeRed(self):
        timer = threading.Timer(0.03, self.changeRed)
        timer.start()
        if self.sequence == 1:
            self.btn_reddice.setStyleSheet('image:url(res/reddice_one.png); border:0px;')
        elif self.sequence == 2:
            self.btn_reddice.setStyleSheet('image:url(res/reddice_two.png); border:0px;')
        elif self.sequence == 3:
            self.btn_reddice.setStyleSheet('image:url(res/reddice_three.png); border:0px;')
        self.sequence += 1
        self.cycle += 1
        if self.sequence > 3: self.sequence = 1
        if self.cycle > 25:
            timer.cancel()
            self.cycle = 1
            if self.red_turn == 1:
                self.btn_reddice.setStyleSheet('image:url(res/reddice_one.png); border:0px;')
            elif self.red_turn == 2:
                self.btn_reddice.setStyleSheet('image:url(res/reddice_two.png); border:0px;')
            elif self.red_turn == 3:
                self.btn_reddice.setStyleSheet('image:url(res/reddice_three.png); border:0px;')




class Blueturn(QMainWindow, screen19):
    def __init__(self):
        super().__init__()
        self.setupUi(self)

        self.btn_blue.setStyleSheet('image:url(res/bluedice_default.png); border:0px;')
        self.btn_blue.clicked.connect(self.turnChange)


class Result(QMainWindow, screen20):
    def __init__(self):
        super().__init__()
        self.setupUi(self)

        self.btn_restart.clicked.connect(partial(ChangeScreen, self, 11))
        self.btn_new.clicked.connect(partial(ChangeScreen, self, 2))

class Single_Default(QMainWindow, screen21):
    def __init__(self):
        super().__init__()
        self.setupUi(self)

        time_minute = math.floor(time / 60)
        time_second = time % 60
        str_minute = ""
        str_second = ""

        if time_minute < 10: str_minute = "0" + str(time_minute)
        else : str_minute = str(time_minute)
        if time_second < 10: str_second = "0" + str(time_second)
        else : str_second = str(time_second)

        self.lb_count.setText(str(count))
        self.lb_time.setText(str_minute + ":" + str_second)

class Blue_Loose(QMainWindow, screen23):
    def __init__(self):
        super().__init__()
        self.setupUi(self)

        self.btn_bbom.clicked.connect(partial(ChangeScreen, self, 20))

class Red_Loose(QMainWindow, screen24):
    def __init__(self):
        super().__init__()
        self.setupUi(self)

        self.btn_rbom.clicked.connect(partial(ChangeScreen, self, 20))


if __name__ == '__main__':
    app = QApplication(sys.argv)
    # app.setOverrideCursor(Qt.BlankCursor)
    ex = ModeSelect()
    ex.showFullScreen()
    # ex.show()
    sys.exit(app.exec_())
