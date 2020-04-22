import sys
import PyQt5.QtGui
from PyQt5.QtWidgets import QApplication, QMainWindow, QWidget
from PyQt5.QtGui import QIcon
from PyQt5 import uic
from functools import partial


#connect UI
screen2 = uic.loadUiType("ui/modeselect.ui")[0]
screen3 = uic.loadUiType("ui/singlemode.ui")[0]
screen4 = uic.loadUiType("ui/single_setting.ui")[0]
screen10 = uic.loadUiType("ui/Replay_Game.ui")[0]
screen11 = uic.loadUiType("ui/battlemode.ui")[0]
screen15 = uic.loadUiType("ui/redturn.ui")[0]
screen19 = uic.loadUiType("ui/blueturn.ui")[0]
screen20 = uic.loadUiType("ui/Result.ui")[0]
screen21 = uic.loadUiType("ui/single_default.ui")[0]
screen23 = uic.loadUiType("ui/blue_loose.ui")[0]
screen24 = uic.loadUiType("ui/red_loose.ui")[0]

# Screen change
def ChangeScreen(before, screen_number):
    if(screen_number == 2): before.main = ModeSelect()
    elif(screen_number == 3): before.main = SingleMode()
    elif(screen_number == 4): before.main = Single_Setting()
    elif(screen_number == 10): before.main = Replay_Game()
    elif(screen_number == 11): before.main = BattleMode()
    elif(screen_number == 15): before.main = Redturn()
    elif(screen_number == 19): before.main = Blueturn()
    elif(screen_number == 20): before.main = Result()
    elif(screen_number == 21): before.main = Single_Default()
    elif(screen_number == 23): before.main = Blue_Loose()
    elif(screen_number == 24): before.main = Red_Loose()

    before.main.showFullScreen()
    before.close()

        
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
    def __init__(self):
        super().__init__()
        self.setupUi(self)

        # Button image
        self.btn_settingback.setStyleSheet('image:url(res/btn_back1.png); border:0px;')
        self.btn_countup.setStyleSheet('image:url(res/btn_up.png); border:0px;')
        self.btn_countdown.setStyleSheet('image:url(res/btn_down.png); border:0px;')
        self.btn_timerup.setStyleSheet('image:url(res/btn_up.png); border:0px;')
        self.btn_timerdown.setStyleSheet('image:url(res/btn_down.png); border:0px;')
        
        self.btn_settingback.clicked.connect(partial(ChangeScreen, self, 3))


class Replay_Game(QMainWindow, screen10):
    def _init_(self):
        super()._init_()
        self.setupUi(self)

        self.btn_reset.setStyleSheet('image:url(res/replay.png); border:0px;')

        self.btn_reset.clicked.connect(partial(ChangeScreen, self, 2))


class BattleMode(QMainWindow, screen11):
    def __init__(self):
        super().__init__()
        self.setupUi(self)

        self.btn_bluedice.setStyleSheet('image:url(res/bluedice_default.png); border:0px;')
        self.btn_reddice.setStyleSheet('image:url(res/reddice_default.png); border:0px;')


class Redturn(QMainWindow, screen15):
    def __init__(self):
        super().__init__()
        self.setupUi(self)

        self.btn_redturn.setStyleSheet('image:url(res/reddice_default.png); boarder:0px')


class Blueturn(QMainWindow, screen19):
    def __init__(self):
        super().__init__()
        self.setupUi(self)

        self.btn_blueturn.setStyleSheet('image:url(res/bluedice_default.png); boarder:0px')


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
    ex = ModeSelect()
    ex.showFullScreen()
    sys.exit(app.exec_())