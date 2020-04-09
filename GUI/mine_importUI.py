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
screen11 = uic.loadUiType("ui/battlemode.ui")[0]


# Screen change
def ChangeScreen(before, screen_number):
    if(screen_number == 2): before.main = ModeSelect()
    elif(screen_number == 3): before.main = SingleMode()
    elif(screen_number == 4): before.main = Single_Setting()
    elif(screen_number == 11): before.main = BattleMode()

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


class BattleMode(QMainWindow, screen11):
    def __init__(self):
        super().__init__()
        self.setupUi(self)

        self.btn_bluedice.setStyleSheet('image:url(res/bluedice_default.png); border:0px;')
        self.btn_reddice.setStyleSheet('image:url(res/reddice_default.png); border:0px;')


if __name__ == '__main__':
    app = QApplication(sys.argv)
    ex = ModeSelect()
    ex.showFullScreen()
    sys.exit(app.exec_())