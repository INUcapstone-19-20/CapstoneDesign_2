import sys, math, random, threading
import time, serial
import PyQt5.QtGui
from PyQt5.QtWidgets import *
from PyQt5.QtGui import *
from PyQt5.QtCore import *
from PyQt5 import uic
from functools import partial
import communication


# Single Mode
set_count = 10
set_time = 40
current_count = 0
current_time = 0

# Battle Mode
first_turn = True
red_turn = 0
blue_turn = 0

class SerThread(QThread):
    clickChanged = pyqtSignal(int)

    def __init__(self, who):
        QThread.__init__(self)
        self.onClick = 0
        self.who = who
        self.stop_flag = False

    def stop(self):
        self.stop_flag = True

    def run(self): 
        while True:
            if self.stop_flag: break

            self.onClick = communication.click_FromArduino()
            if self.onClick == 1:
                self.clickChanged.emit(communication.count_turn)
            elif self.onClick == 99:
                self.clickChanged.emit(99)
                break
            elif self.onClick == -99:
                self.clickChanged.emit(-99)
                break


class DiceThread(QThread):
    cntChanged = pyqtSignal(int)
    result = pyqtSignal(int)

    def __init__(self, limit = 30, timing = 0.03, noPass = 10):
        QThread.__init__(self)
        self.limit = limit
        self.timing = timing
        self.noPass = noPass

    def run(self):      
        cnt = 0
        while cnt < self.limit:
            if self.noPass == 10:
                self.cntChanged.emit(cnt % 4)
            else:
                self.cntChanged.emit(cnt % self.noPass)
            cnt += 1
            time.sleep(self.timing)

        rand = random.randint(0, self.noPass - 1)
        if self.noPass == 10:
            if rand >= 7: rand = 3
            elif rand >= 4: rand = 2
            elif rand >= 1: rand = 1
        self.result.emit(rand)
            

# Screen change
def changeScreen(before, screen_number):
    if(screen_number == 1): before.main = Start()
    elif(screen_number == 2): before.main = ModeSelect()
    elif(screen_number == 3): before.main = SingleMode()
    elif(screen_number == 4): before.main = Single_Setting()
    elif(screen_number == 6): before.main = Single_Start()
    elif(screen_number == 8): before.main = Single_Win()
    elif(screen_number == 9): before.main = Single_Loose()
    elif(screen_number == 10): before.main = Replay_Game()
    elif(screen_number == 11): before.main = BattleMode()
    elif(screen_number == 15): before.main = Redturn()
    elif(screen_number == 19): before.main = Blueturn()
    elif(screen_number == 20): before.main = Result()
    elif(screen_number == 23): before.main = Blue_Loose()
    elif(screen_number == 24): before.main = Red_Loose()
    
    before.main.showFullScreen()
    # before.main.show()
    before.close()


class Start(QMainWindow):
    def __init__(self):
        super().__init__()
        uic.loadUi("ui/start.ui", self)
        self.dot = DiceThread(24, 0.6)
        self.dot.cntChanged.connect(self.splash)
        self.dot.finished.connect(partial(changeScreen, self, 2))
        self.dot.start()

        self.timer = DiceThread(limit=25, noPass=4)
        self.timer.finished.connect(partial(communication.mode_toArduino, "Loding"))
        self.timer.start()

        self.btn_start.setStyleSheet('image:url(res/bomb3.png); border:0px;')

    def splash(self, value):
        self.text = "loading"
        for i in range(0, value):
            self.text += "."

        self.lb_start.setText(self.text)

        
class ModeSelect(QMainWindow):
    def __init__(self):
        super().__init__()
        uic.loadUi("ui/modeselect.ui", self)
        communication.set_Mine()
        # Button connect
        self.btn_single.clicked.connect(partial(changeScreen, self, 3))
        self.btn_battle.clicked.connect(partial(changeScreen, self, 11))

class SingleMode(QMainWindow):
    def __init__(self):
        super().__init__()
        uic.loadUi("ui/singlemode.ui", self)
        communication.mode_toArduino("Single")
        # communication.set_Mine()

        self.qtimer = QTimer(self)
        self.qtimer.setInterval(100)
        self.qtimer.setSingleShot(True)
        self.qtimer.timeout.connect(communication.set_Mine)
        self.qtimer.start()

        
        self.btn_singlestart.clicked.connect(partial(changeScreen, self, 6))
        self.btn_singlesetting.clicked.connect(partial(changeScreen, self, 4)) 


class Single_Setting(QMainWindow):
    def __init__(self):
        super().__init__()
        uic.loadUi("ui/single_setting.ui", self)
        self.temp_count = set_count
        self.temp_time = set_time
        self.setLabel()

        # Button Function
        self.btn_countup.clicked.connect(self.countUp)
        self.btn_countdown.clicked.connect(self.countDown)
        self.btn_timerup.clicked.connect(self.timeUp)
        self.btn_timerdown.clicked.connect(self.timeDown)
        self.btn_settingsave.clicked.connect(self.settingSave)
        self.btn_settingback.clicked.connect(partial(changeScreen, self, 3))

        # Popup Message
        self.lb_popup.setStyleSheet('image:url(res/saved.png);')
        self.lb_popup.setVisible(False)
        self.effect = DiceThread(300, 0.005, 300)
        self.effect.cntChanged.connect(self.savePopup)

        # Button Image
        self.btn_settingback.setStyleSheet('image:url(res/btn_back1.png); border:0px;')
        self.btn_countup.setStyleSheet('image:url(res/btn_up.png); border:0px;')
        self.btn_countdown.setStyleSheet('image:url(res/btn_down.png); border:0px;')
        self.btn_timerup.setStyleSheet('image:url(res/btn_up.png); border:0px;')
        self.btn_timerdown.setStyleSheet('image:url(res/btn_down.png); border:0px;')

    def savePopup(self, value):
        if value < 100: opacity = value * 0.01
        elif value < 200: opacity = 1.0
        else: opacity = 0.99 - (value - 200) * 0.01

        opacity_effect = QGraphicsOpacityEffect(self.lb_popup)
        opacity_effect.setOpacity(opacity)
        self.lb_popup.setGraphicsEffect(opacity_effect)
        if value == 299: self.lb_popup.setVisible(False)
        else: self.lb_popup.show()

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
        if self.temp_count > 1: self.temp_count -= 1
        self.setLabel()

    def timeUp(self):
        self.temp_time += 5
        self.setLabel()

    def timeDown(self):
        if self.temp_time > 5: self.temp_time -= 5
        self.setLabel()
        
    def settingSave(self):
        global set_count, set_time
        set_count = self.temp_count
        set_time = self.temp_time
        self.effect.start()

class Single_Start(QMainWindow):
    def __init__(self):
        super().__init__()
        uic.loadUi("ui/single_start.ui", self)
        communication.turn_ToArduino("Solo")

        self.reset()
        self.setLabel()
        
        self.qtimer = QTimer(self)
        self.qtimer.setInterval(1000)
        self.qtimer.timeout.connect(self.ticktock)

        self.serth = SerThread("single")
        self.serth.clickChanged.connect(self.buttonClicked)
        self.serth.start()

        # Popup Message
        self.lb_popup.setStyleSheet('image:url(res/warning.png);')
        self.lb_popup.setVisible(False)
        self.effect = DiceThread(2000, 0.005, 2000)
        self.effect.cntChanged.connect(self.warnPopup)
        

    def warnPopup(self, value):
        level = value % 250
        if level < 100: opacity = level * 0.01
        elif level < 200: opacity = 0.99 - (level - 100) * 0.01
        else: opacity = 0.0

        opacity_effect = QGraphicsOpacityEffect(self.lb_popup)
        opacity_effect.setOpacity(opacity)
        self.lb_popup.setGraphicsEffect(opacity_effect)
        if value == 1999: self.lb_popup.setVisible(False)
        else: self.lb_popup.show()

    def reset(self):
        global current_count, current_time
        current_count = 0
        current_time = 0

        self.limit_time = set_time
        self.limit_count = set_count
        self.firstClick = False
        self.warning = 0

    def singleFail(self):
        communication.fail_ToArduino()
        self.serth.stop()
        self.serth.exit()
        self.qtimer.stop()
        changeScreen(self, 9)

    def buttonClicked(self, value):
        if not self.firstClick:
            self.firstClick = True
            self.qtimer.start()
            self.ticktock()

        global current_count
        current_count += 1
        self.limit_count -= 1
        self.setLabel() 
        
        if value == 99:
            self.serth.stop()
            self.serth.exit()
            self.qtimer.stop()
            changeScreen(self, 8)
        elif(self.limit_count == 0):
            self.singleFail()
        elif(self.limit_count <= 3):
            self.lb_count.setStyleSheet('color: rgb(255, 0, 0);')


    def setLabel(self):
        time_minute = math.floor(self.limit_time / 60)
        time_second = self.limit_time % 60
        str_minute = ""
        str_second = ""

        if time_minute < 10: str_minute = "0" + str(time_minute)
        else: str_minute = str(time_minute)
        if time_second < 10: str_second = "0" + str(time_second)
        else: str_second = str(time_second)

        self.lb_count.setText(str(self.limit_count))
        self.lb_time.setText(str_minute + ":" + str_second)


    def ticktock(self):
        if self.limit_time == 0: 
            self.singleFail()
        elif self.warning != 2 & (self.limit_time <= 6):
            self.warning = 2
            communication.Warn_ToArduino(150)
        elif self.warning != 1 & (self.limit_time <= 11):
            self.warning = 1
            self.effect.start()
            self.lb_time.setStyleSheet('color: rgb(255, 0, 0);')
            communication.Warn_ToArduino(300)

        global current_time
        current_time += 1
        self.limit_time -= 1
        self.setLabel()
    

class Single_Win(QMainWindow):
    def __init__(self):
        super().__init__()
        uic.loadUi("ui/single_win.ui", self)
        communication.turn_ToArduino("Lock")

        self.setLabel()

        self.btn_restart.clicked.connect(self.restartClick)
        self.btn_menu.clicked.connect(partial(changeScreen, self, 2))

    def restartClick(self):
        self.next = Replay_Game("Single")
        self.next.showFullScreen()
        # self.next.show()
        self.close()

    def setLabel(self):
        global current_count, current_time
        time_minute = math.floor(current_time / 60)
        time_second = current_time % 60
        str_minute = ""
        str_second = ""

        if time_minute < 10: str_minute = "0" + str(time_minute)
        else: str_minute = str(time_minute)
        if time_second < 10: str_second = "0" + str(time_second)
        else: str_second = str(time_second)

        self.lb_count.setText(str(current_count))
        self.lb_time.setText(str_minute + ":" + str_second)


class Single_Loose(QMainWindow):
    def __init__(self):
        super().__init__()
        uic.loadUi("ui/single_loose.ui", self)
        communication.turn_ToArduino("Lock")

        self.setLabel()

        self.btn_restart2.clicked.connect(self.restartClick)
        self.btn_menu2.clicked.connect(partial(changeScreen, self, 2))

    def restartClick(self):
        self.next = Replay_Game("Single")
        self.next.showFullScreen()
        # self.next.show()
        self.close()

    def setLabel(self):
        global current_count, current_time
        time_minute = math.floor(current_time / 60)
        time_second = current_time % 60
        str_minute = ""
        str_second = ""

        if time_minute < 10: str_minute = "0" + str(time_minute)
        else: str_minute = str(time_minute)
        if time_second < 10: str_second = "0" + str(time_second)
        else: str_second = str(time_second)

        self.lb_count.setText(str(current_count))
        self.lb_time.setText(str_minute + ":" + str_second)
        
        

class Replay_Game(QMainWindow):
    def __init__(self, mode):
        super().__init__()
        uic.loadUi("ui/Replay_Game.ui", self)
        communication.set_Mine()
        self.mode = mode
        
        self.qtimer = QTimer(self)
        self.qtimer.setInterval(2000)
        self.qtimer.setSingleShot(True)
        if mode == "Single":
            self.qtimer.timeout.connect(partial(changeScreen, self, 6))
        elif mode == "Battle":
            self.qtimer.timeout.connect(partial(changeScreen, self, 11))
        self.qtimer.start()

        self.btn_reset.setStyleSheet('image:url(res/replay.png); border:0px;')
        self.btn_reset.clicked.connect(partial(changeScreen, self, 2))

    def timeout(self):
        self.qtimer.stop()
        changeScreen(self, 6)


class BattleMode(QMainWindow):
    def __init__(self):
        super().__init__()
        uic.loadUi("ui/battlemode.ui", self)
        
        global first_turn
        first_turn = True
        self.check_blue = False
        self.check_red = False
        communication.mode_toArduino("Battle")

        self.qtimer = QTimer(self)
        self.qtimer.setInterval(100)
        self.qtimer.setSingleShot(True)
        self.qtimer.timeout.connect(communication.set_Mine)
        self.qtimer.start()
        
        self.btn_bluedice.clicked.connect(self.throwBlue)
        self.btn_reddice.clicked.connect(self.throwRed)

        self.btn_bluedice.setStyleSheet('image:url(res/bluedice_default.png); border:0px;')
        self.btn_reddice.setStyleSheet('image:url(res/reddice_default.png); border:0px;')

    def blueEffect(self, value):
        self.btn_bluedice.move(61 + value, 110)
        self.btn_reddice.move(271 + 2.1 * value, 110)

    def redEffect(self, value):
        self.btn_bluedice.move(59 - 2.1 * value, 110)
        self.btn_reddice.move(269 - value, 110)

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
            self.effect = DiceThread(105, 0.01, 105)
            self.effect.cntChanged.connect(self.blueEffect)
            self.effect.start()

            self.timer.finished.connect(partial(changeScreen, self, 19))
            self.timer.start()
        elif red_turn > blue_turn:
            self.effect = DiceThread(105, 0.01, 105)
            self.effect.cntChanged.connect(self.redEffect)
            self.effect.start()

            self.timer.finished.connect(partial(changeScreen, self, 15))
            self.timer.start()
    
    def setBlue(self, value):
        if(value == 0):
            self.btn_bluedice.setStyleSheet('image:url(res/bluedice_one1.png); border:0px;')
        elif(value == 1):
            self.btn_bluedice.setStyleSheet('image:url(res/bluedice_two2.png); border:0px;')
        elif(value == 2):
            self.btn_bluedice.setStyleSheet('image:url(res/bluedice_three3.png); border:0px;')
            
    def finishBlue(self, value):
        global blue_turn
        blue_turn = value
        self.setBlue(blue_turn)
        self.check_blue = True
        if self.check_blue & self.check_red:
            self.timer = DiceThread(50)
            self.timer.finished.connect(self.compareDice)
            self.timer.start()

    def throwBlue(self):
        self.th = DiceThread(noPass=3)
        self.th.cntChanged.connect(self.setBlue)
        self.th.result.connect(self.finishBlue)
        self.btn_bluedice.setEnabled(False)
        self.th.start()

    def setRed(self, value):
        if(value == 0):
            self.btn_reddice.setStyleSheet('image:url(res/reddice_one1.png); border:0px;')
        elif(value == 1):
            self.btn_reddice.setStyleSheet('image:url(res/reddice_two2.png); border:0px;')
        elif(value == 2):
            self.btn_reddice.setStyleSheet('image:url(res/reddice_three3.png); border:0px;')

    def finishRed(self, value):
        global red_turn
        red_turn = value
        self.setRed(red_turn)
        self.check_red = True
        if self.check_blue & self.check_red:
            self.timer = DiceThread(50)
            self.timer.finished.connect(self.compareDice)
            self.timer.start()

    def throwRed(self):
        self.th = DiceThread(noPass=3)
        self.th.cntChanged.connect(self.setRed)
        self.th.result.connect(self.finishRed)
        self.btn_reddice.setEnabled(False)
        self.th.start()
            

class Redturn(QMainWindow):
    def __init__(self):
        super().__init__()
        uic.loadUi("ui/redturn.ui", self)

        self.eye = 100
        self.serth = SerThread("red")
        self.serth.clickChanged.connect(self.buttonClicked)

        global first_turn
        if first_turn:
            self.lb_redturn.setText("턴 횟수 정하기")
            first_turn = False
        else: self.lb_redturn.setText("빨강 플레이어 턴")

        self.btn_redturn.clicked.connect(self.throwRed)
        self.btn_redturn.setStyleSheet('image:url(res/reddice_default.png); border:0px;')


    def buttonClicked(self,value):
        self.filename = 'image:url(res/reddice_'
        if self.eye == 1: self.filename += 'one'
        elif self.eye == 2: self.filename += 'two'
        elif self.eye == 3: self.filename += 'three'

        if value == 99:
            self.serth.stop()
            self.serth.exit()
            changeScreen(self, 24)
        elif value == -99:
            self.serth.stop()
            self.serth.exit()
            changeScreen(self, 23)
        else:
            self.filename += str(value) + '.png); border:0px;'
            self.btn_redturn.setStyleSheet(self.filename)
            if value == 0:
                communication.turn_ToArduino("Lock")
                self.timer = DiceThread(60)
                self.timer.finished.connect(self.checkBoom)
                self.timer.start()
                

    def checkBoom(self):
        self.serth.stop()
        self.serth.exit()
        changeScreen(self,19)
            

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
        self.lb_redturn.setText("빨강 플레이어 턴")
        
        if self.eye == 0:
            communication.turn_ToArduino("Lock")
            self.timer = DiceThread(60)
            self.timer.finished.connect(partial(changeScreen, self, 19))
            self.timer.start()
        elif self.eye > 0:
            communication.count_turn = self.eye
            communication.turn_ToArduino("Red_")
            self.serth.start()
            

    def throwRed(self):
        self.th = DiceThread()
        self.th.cntChanged.connect(self.setRed)
        self.th.result.connect(self.finishRed)
        self.btn_redturn.setEnabled(False)
        self.th.start()


class Blueturn(QMainWindow):
    def __init__(self):
        super().__init__()
        uic.loadUi("ui/blueturn.ui", self)

        self.eye = 100
        self.serth = SerThread("blue")
        self.serth.clickChanged.connect(self.buttonClicked)

        global first_turn
        if first_turn:
            self.lb_blueturn.setText("턴 횟수 정하기")
            first_turn = False
        else: self.lb_blueturn.setText("파랑 플레이어 턴")

        self.btn_blueturn.clicked.connect(self.throwBlue)
        self.btn_blueturn.setStyleSheet('image:url(res/bluedice_default.png); border:0px;')

    def buttonClicked(self,value):
        self.filename = 'image:url(res/bluedice_'
        if self.eye == 1: self.filename += 'one'
        elif self.eye == 2: self.filename += 'two'
        elif self.eye == 3: self.filename += 'three'

        if value == 99:
            self.serth.stop()
            self.serth.exit()
            changeScreen(self,24)
        elif value == -99:
            self.serth.stop()
            self.serth.exit()
            changeScreen(self,23)
        else:
            self.filename += str(value) + '.png); border:0px;'
            
            self.btn_blueturn.setStyleSheet(self.filename)
            if value == 0:
                communication.turn_ToArduino("Lock")
                self.timer = DiceThread(60)
                self.timer.finished.connect(self.checkBoom)
                self.timer.start()

    def checkBoom(self):
        self.serth.stop()
        self.serth.exit()
        changeScreen(self,15)


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
        self.lb_blueturn.setText("파랑 플레이어 턴")
        
        if self.eye == 0:
            communication.turn_ToArduino("Lock")
            self.timer = DiceThread(60)
            self.timer.finished.connect(partial(changeScreen, self, 15))
            self.timer.start()
        elif self.eye > 0:
            communication.count_turn = self.eye
            communication.turn_ToArduino("Blue")
            self.serth.start()


    def throwBlue(self):
        self.th = DiceThread()
        self.th.cntChanged.connect(self.setBlue)
        self.th.result.connect(self.finishBlue)
        self.btn_blueturn.setEnabled(False)
        self.th.start()


class Result(QMainWindow):
    def __init__(self,winner):
        super().__init__()
        uic.loadUi("ui/Result.ui", self)

        self.winner = winner
        if self.winner == "Red":
            # self.lb_winner.setText("[빨강 플레이어 승리!]")
            self.lb_winner.setText("빨강 플레이어 승리!")
            self.lb_image.setStyleSheet('image:url(res/win_red.png); border:0px;')
        elif self.winner == "Blue":
            self.lb_winner.setText("파랑 플레이어 승리!")
            self.lb_image.setStyleSheet('image:url(res/win_blue.png); border:0px;')


        self.btn_restart.clicked.connect(self.restartClick)
        self.btn_new.clicked.connect(partial(changeScreen, self, 2))

    def restartClick(self):
        self.next = Replay_Game("Battle")
        self.next.showFullScreen()
        # self.next.show()
        self.close()


class Blue_Loose(QMainWindow):
    def __init__(self):
        super().__init__()
        uic.loadUi("ui/blue_loose.ui", self)

        self.btn_bbom.setStyleSheet('image:url(res/blueexplosion.png); border:0px;')
        # self.btn_bbom.clicked.connect(self.gotoResult)ㅎ
        self.qtimer = QTimer(self)
        self.qtimer.setInterval(5000)
        self.qtimer.setSingleShot(True)
        self.qtimer.timeout.connect(self.gotoResult)
        self.qtimer.start()

    def gotoResult(self):
        self.next = Result("Red")
        self.next.showFullScreen()
        # self.next.show()
        self.close()

class Red_Loose(QMainWindow):
    def __init__(self):
        super().__init__()
        uic.loadUi("ui/red_loose.ui", self)

        self.btn_rbom.setStyleSheet('image:url(res/redexplosion.png); border:0px;')
        # self.btn_rbom.clicked.connect(self.gotoResult)
        self.qtimer = QTimer(self)
        self.qtimer.setInterval(5000)
        self.qtimer.setSingleShot(True)
        self.qtimer.timeout.connect(self.gotoResult)
        self.qtimer.start()

    def gotoResult(self):
        self.next = Result("Blue")
        self.next.showFullScreen()
        # self.next.show()
        self.close()


if __name__ == '__main__':
    app = QApplication(sys.argv)
    app.setOverrideCursor(Qt.BlankCursor)

    # GUI 시작
    ex = Start()
    # ex = Result("Blue")

    ex.showFullScreen()
    # ex.show()
    sys.exit(app.exec_())
