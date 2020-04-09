from PyQt5 import QtCore
from PyQt5 import QtGui
from PyQt5 import uic
from PyQt5.QtWidgets import QMainWindow,QStackedWidget,QWidget, QHBoxLayout, QPushButton, QLabel, QApplication


class MainWindow(QMainWindow):
    def __init__(self, parent=None):
        super(MainWindow, self).__init__(parent)
        self.central_widget = QStackedWidget()
        self.setCentralWidget(self.central_widget)
        login_widget = LoginWidget(self)
        login_widget.btn_single.clicked.connect(self.click_single)
        login_widget.btn_battle.clicked.connect(self.click_battle)
        self.central_widget.addWidget(login_widget)

    def click_single(self):
        logged_in_widget = SingleMode(self)
        self.central_widget.addWidget(logged_in_widget)
        self.central_widget.setCurrentWidget(logged_in_widget)

    def click_battle(self):
        logged_in_widget = BattleMode(self)
        self.central_widget.addWidget(logged_in_widget)
        self.central_widget.setCurrentWidget(logged_in_widget)


class LoginWidget(QWidget):
    def __init__(self, parent=None):
        super(LoginWidget, self).__init__(parent)
        # self.ui = uic.loadUi("mineGUI.ui")
        # self.ui.show()

        layout = QHBoxLayout()
        self.setFixedWidth(480)
        self.setFixedHeight(320)

        self.btn_single = QPushButton(self)
        self.btn_single.setText("싱글")
        self.btn_single.setFixedHeight(300)
        

        self.btn_battle = QPushButton(self)
        self.btn_battle.setText("배틀")
        self.btn_battle.setFixedHeight(300)

        
        layout.addWidget(self.btn_single)
        layout.addWidget(self.btn_battle)
        self.setLayout(layout)
        # you might want to do self.button.click.connect(self.parent().login) here


class SingleMode(QWidget):
    def __init__(self, parent=None):
        super(SingleMode, self).__init__(parent)
        layout = QHBoxLayout()

        self.label = QLabel(self)
        self.label.setText("지뢰찾기")
        self.label.setFont(QtGui.QFont("NanumSquareRound", 28))
        self.label.setGeometry(QtCore.QRect(150, 0, 200, 100))

        # layout.addWidget(self.label)
        self.setLayout(layout)


class BattleMode(QWidget):
    def __init__(self, parent=None):
        super(BattleMode, self).__init__(parent)
        layout = QHBoxLayout()

        self.lb_battle = QLabel(self)
        self.lb_battle.setText("순서 정하기")
        self.lb_battle.setFont(QtGui.QFont("NanumSquareRound", 28))
        self.lb_battle.setGeometry(QtCore.QRect(150, 0, 200, 100))

        # layout.addWidget(self.label)
        self.setLayout(layout)



if __name__ == '__main__':
    app = QApplication([])
    window = MainWindow()
    window.show()
    app.exec_()
