import sys
import PyQt5.QtGui
from PyQt5.QtWidgets import QApplication, QMainWindow, QWidget
from PyQt5.QtGui import QIcon
from PyQt5 import uic


#connect UI
screen2 = uic.loadUiType("ui/modeselect.ui")[0]
screen3 = uic.loadUiType("ui/singlemode.ui")[0]
screen4 = uic.loadUiType("ui/single_setting.ui")[0]
screen11 = uic.loadUiType("ui/battlemode.ui")[0]


class ModeSelect(QMainWindow, screen2):
    def __init__(self):
        super().__init__()
        self.setupUi(self)
        # Button function
        self.btn_single.clicked.connect(self.click_single)
        self.btn_battle.clicked.connect(self.click_battle)

    def click_single(self):
        self.main = SingleMode()
        self.main.showFullScreen()
        self.close()

    def click_battle(self):
        self.main = BattleMode()
        self.main.showFullScreen()
        self.close()

class SingleMode(QMainWindow, screen3):
    def __init__(self):
        super().__init__()
        self.setupUi(self)

        self.btn_singlesetting.clicked.connect(self.click_setting)

    def click_setting(self):
        self.main = Single_Setting()
        self.main.showFullScreen()
        self.close()

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

        self.btn_settingback.clicked.connect(self.click_back)

    def click_back(self):
        self.main = ModeSelect()
        self.main.showFullScreen()
        self.close()

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